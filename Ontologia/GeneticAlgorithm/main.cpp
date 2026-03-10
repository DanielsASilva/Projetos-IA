#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include <chrono>
#include <cmath>
#include <cfloat>

// Enumerações para Curso e Tema (conforme seu modelo)
typedef enum { ENGENHARIA, CIENCIA, DADOS } Curso;
typedef enum { ARVORE_BINARIA, GRAFO, STACK, QUEUE, ARRAY } TemaEnum;

// Structs
struct Aluno {
    unsigned long matricula;
    std::string nome;
    uint64_t cpf;
    Curso curso; // Adicionado para cálculo do entregável
};

struct Tema {
    std::string nome;
    TemaEnum tipo; // Adicionado para cálculo de distância
};

struct Grupo {
    std::vector<Aluno> alunos;
    Tema tema;
};

struct Professor {
    unsigned long inscricao;
    std::string nome;
};

struct Entregavel {
    std::string nome;
    std::string conteudo;
    float nota;
    Grupo *grupo;
    Professor *professor;
};

// Constantes Globais
const int POPULACAO_SIZE = 50;
const int GERACOES = 100;
const float TAXA_MUTACAO = 0.1f;
const int QTD_GRUPOS = 5;
const int ALUNOS_POR_GRUPO = 4;

// Representa uma solução (Cromossomo)
struct Individuo {
    std::vector<int> genes; // Índice: Aluno --> Valor: Grupo
    double fitness;

    void calcular_fitness() {
        // Objetivo: Cada grupo deve ter exatamente 4 alunos.
        // Penalizamos grupos que fogem desse número.
        std::vector<int> contagem(QTD_GRUPOS, 0);
        for (int g : genes) contagem[g]++;

        double erro = 0;
        for (int c : contagem) {
            erro += std::abs(c - ALUNOS_POR_GRUPO);
        }
        // Fitness: quanto menor o erro, maior o fitness.
        this->fitness = 1.0 / (1.0 + erro);
    }
};

// Config de Randomização
std::random_device dev;
std::mt19937 rng(dev());

const std::string nomes_disponiveis[] = {
    "Andre", "Pedro", "Alex", "Joao", "Jose", "Ricardo", "Lucas",
    "Alexandro", "Mario", "Hugo", "Maria", "Ana", "Vanessa", "Lara",
    "Gabriele", "Joana", "Raquel", "Luana", "Wanda", "Rafaela"};

// Funções para o Algoritmo Genético

// Crossover: 1-point (Metade-Metade)
Individuo crossover(const Individuo& p1, const Individuo& p2) {
    Individuo filho;
    std::uniform_int_distribution<int> dist_ponto(0, p1.genes.size() - 1);
    int ponto = dist_ponto(rng);

    for (int i = 0; i < p1.genes.size(); ++i) {
        filho.genes.push_back(i < ponto ? p1.genes[i] : p2.genes[i]);
    }
    return filho;
}

// Mutação: Move um aluno de um grupo pra outro de forma aleatória
void mutacao(Individuo& ind) {
    std::uniform_real_distribution<float> dist_prob(0, 1);
    std::uniform_int_distribution<int> dist_grupo(0, QTD_GRUPOS - 1);

    if (dist_prob(rng) < TAXA_MUTACAO) {
        int gene_idx = std::uniform_int_distribution<int>(0, ind.genes.size() - 1)(rng);
        ind.genes[gene_idx] = dist_grupo(rng);
    }
}

int main() {
    // Setup inicial de Alunos e Professores
    std::vector<Aluno> lista_alunos;
    for(int i = 0; i < 20; i++) {
        lista_alunos.push_back({
            (unsigned long)202400 + i, 
            nomes_disponiveis[i], // Evitar duplicatas
            10000000000ULL + i,
            static_cast<Curso>(rand() % 3)
        });
    }

    Professor professores[] = {
        {.inscricao = 1, .nome = "Professor 1"},
        {.inscricao = 2, .nome = "Professor 2"},
        {.inscricao = 3, .nome = "Professor 3"},
    };

    std::vector<Tema> temas = {
        {"Arvore Binaria", ARVORE_BINARIA}, 
        {"Grafo", GRAFO}, 
        {"Stack", STACK}, 
        {"Queue", QUEUE}, 
        {"Array", ARRAY}
    };

    // Criando População Inicial Aleatória
    std::vector<Individuo> populacao(POPULACAO_SIZE);
    std::uniform_int_distribution<int> dist_grupo(0, QTD_GRUPOS - 1);

    for (auto& ind : populacao) {
        for (int i = 0; i < 20; ++i) ind.genes.push_back(dist_grupo(rng));
        ind.calcular_fitness();
    }

    // Loop Principal
    for (int g = 0; g < GERACOES; ++g) {
        std::sort(populacao.begin(), populacao.end(), [](const Individuo& a, const Individuo& b) {
            return a.fitness > b.fitness;
        });

        if (populacao[0].fitness == 1.0) break; 

        std::vector<Individuo> nova_pop;
        nova_pop.push_back(populacao[0]);
        nova_pop.push_back(populacao[1]);

        while (nova_pop.size() < POPULACAO_SIZE) {
            Individuo filho = crossover(populacao[0], populacao[1]); 
            mutacao(filho);
            filho.calcular_fitness();
            nova_pop.push_back(filho);
        }
        populacao = nova_pop;
    }

    // Escolhendo melhores indivíduos da população
    Individuo melhor = populacao[0];
    std::vector<Grupo> grupos_finais(QTD_GRUPOS);
    for(int i=0; i<QTD_GRUPOS; ++i) grupos_finais[i].tema = temas[i];

    for (int i = 0; i < melhor.genes.size(); ++i) {
        int grupo_id = melhor.genes[i];
        grupos_finais[grupo_id].alunos.push_back(lista_alunos[i]);
    }

    // Entregáveis
    std::vector<Entregavel> entregaveis = {
        {.nome = "Entrega 1", .conteudo = "...", .nota = 0, .grupo = &grupos_finais[0], .professor = &professores[0]},
        {.nome = "Entrega 2", .conteudo = "...", .nota = 0, .grupo = &grupos_finais[1], .professor = NULL}, // Precisa de atribuição
        {.nome = "Entrega 3", .conteudo = "...", .nota = 0, .grupo = &grupos_finais[2], .professor = &professores[1]},
        {.nome = "Entrega 4", .conteudo = "...", .nota = 0, .grupo = &grupos_finais[3], .professor = NULL}, // Precisa de atribuição
        {.nome = "Entrega 5", .conteudo = "...", .nota = 0, .grupo = &grupos_finais[4], .professor = &professores[2]}
    };

    // K-Nearest Neighbours para alocar os professores
    for (int i = 0; i < 5; i++) {
        if (entregaveis[i].professor == NULL) {
            float media_curso = 0.0;
            for (Aluno a : entregaveis[i].grupo->alunos) media_curso += static_cast<int>(a.curso);
            media_curso /= entregaveis[i].grupo->alunos.size();

            float min_distance = FLT_MAX;
            Professor *closest_professor = NULL;

            for (int j = 0; j < 5; j++) {
                if (j == i || entregaveis[j].professor == NULL) continue;

                float media_curso_vizinho = 0.0;
                for (Aluno a : entregaveis[j].grupo->alunos) media_curso_vizinho += static_cast<int>(a.curso);
                media_curso_vizinho /= entregaveis[j].grupo->alunos.size();

                float distance = std::sqrt(
                    std::pow((media_curso - media_curso_vizinho), 2) +
                    std::pow((static_cast<int>(entregaveis[i].grupo->tema.tipo) - 
                              static_cast<int>(entregaveis[j].grupo->tema.tipo)), 2)
                );

                if (distance < min_distance) {
                    min_distance = distance;
                    closest_professor = entregaveis[j].professor;
                }
            }
            entregaveis[i].professor = closest_professor;
        }
    }

    // Log dos Resultados
    for (int i = 0; i < QTD_GRUPOS; ++i) {
        std::cout << "========================================\n";
        std::cout << "Grupo " << i << " - Tema: " << grupos_finais[i].tema.nome << "\n";
        std::cout << "Professor: " << entregaveis[i].professor->nome << "\n";
        std::cout << "integrantes: \n";
        for (const auto& a : grupos_finais[i].alunos) {
            std::cout << "- " << a.nome << " (Curso: " << a.curso << ")\n";
        }
        std::cout << "========================================\n";
    }

    return 0;
}