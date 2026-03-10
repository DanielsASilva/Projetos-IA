#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include <chrono>

// Structs
struct Aluno {
    unsigned long matricula;
    std::string nome;
    uint64_t cpf;
};

struct Tema {
    std::string nome;
};

struct Grupo {
    std::vector<Aluno> alunos;
    Tema tema;
};

// Global Constants
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

Aluno aluno_new_random() {
    std::uniform_int_distribution<int> dist_nome(0, 19);
    return { (unsigned long)rand()%1000, nomes_disponiveis[dist_nome(rng)], 12345678901 };
}

// Funções para o Algoritmo Genético

// Crossover 1-point (Metade-Metade)
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
    // 1. Setup inicial
    std::vector<Aluno> lista_alunos;
    for(int i=0; i<20; ++i) lista_alunos.push_back(aluno_new_random());

    std::vector<Tema> temas = {{"Arvore Binaria"}, {"Grafo"}, {"Stack"}, {"Queue"}, {"Array"}};

    // 2. Criando População Inicial Aleatória
    std::vector<Individuo> populacao(POPULACAO_SIZE);
    std::uniform_int_distribution<int> dist_grupo(0, QTD_GRUPOS - 1);

    for (auto& ind : populacao) {
        for (int i = 0; i < 20; ++i) ind.genes.push_back(dist_grupo(rng));
        ind.calcular_fitness();
    }

    // 3. Ciclo Genético Principal
    for (int g = 0; g < GERACOES; ++g) {
        std::sort(populacao.begin(), populacao.end(), [](const Individuo& a, const Individuo& b) {
            return a.fitness > b.fitness;
        });

        if (populacao[0].fitness == 1.0) break; // Solução perfeita encontrada

        std::vector<Individuo> nova_pop;

        // Seleção Elitismo: mantém os 2 melhores
        nova_pop.push_back(populacao[0]);
        nova_pop.push_back(populacao[1]);

        while (nova_pop.size() < POPULACAO_SIZE) {
            Individuo filho = crossover(populacao[0], populacao[1]); // Crossover simples com os melhores
            mutacao(filho);
            filho.calcular_fitness();
            nova_pop.push_back(filho);
        }
        populacao = nova_pop;
    }

    // 4. Resultado Final
    Individuo melhor = populacao[0];
    std::vector<Grupo> grupos_finais(QTD_GRUPOS);
    for(int i=0; i<QTD_GRUPOS; ++i) grupos_finais[i].tema = temas[i];

    for (int i = 0; i < melhor.genes.size(); ++i) {
        int grupo_id = melhor.genes[i];
        grupos_finais[grupo_id].alunos.push_back(lista_alunos[i]);
    }

    // Log dos Resultados
    for (int i = 0; i < QTD_GRUPOS; ++i) {
        std::cout << "=== Grupo " << i << " (" << grupos_finais[i].tema.nome << ") ===\n";
        for (const auto& a : grupos_finais[i].alunos) std::cout << "- " << a.nome << "\n";
    }

    return 0;
}