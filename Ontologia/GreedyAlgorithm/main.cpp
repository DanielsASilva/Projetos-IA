#include <cfloat>
#include <chrono>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <random>
#include <string>
#include <sys/types.h>
#include <vector>

typedef enum { ENGENHARIA, CIENCIA, DADOS } Curso;

typedef enum { ARVORE_BINARIA, STACK, QUEUE } Tema;

typedef struct {
  unsigned long matricula;
  std::string nome;
  uint64_t cpf;
  Curso curso;
} Aluno;

typedef struct {
  unsigned long inscricao;
  std::string nome;
} Professor;

typedef struct {
  std::vector<Aluno> alunos;
  Tema tema;
} Grupo;

typedef struct {
  std::string nome;
  std::string conteudo;
  float nota;
  Grupo *grupo;
  Professor *professor;
} Entregavel;

const std::string nomes_disponiveis[] = {
    "Andre",     "Pedro", "Alex",   "Joao",  "Jose",  "Ricardo", "Lucas",
    "Alexandro", "Mario", "Hugo",   "Maria", "Ana",   "Vanessa", "Lara",
    "Gabriele",  "Joana", "Raquel", "Luana", "Wanda", "Rafaela"};

std::random_device dev;
std::mt19937
    rng(dev() ^
        (uint64_t)std::chrono::steady_clock::now().time_since_epoch().count());

Aluno aluno_new_random() {
  static std::uniform_int_distribution<std::mt19937::result_type>
      dist_matricula(0, 1000);
  static std::uniform_int_distribution<std::mt19937::result_type> dist_nome(0,
                                                                            19);
  static std::uniform_int_distribution<uint64_t> dist_cpf(10000000000,
                                                          99999999999);
  static std::uniform_int_distribution<std::mt19937::result_type> dist_materia(
      0, 2);

  size_t i = 0;

  size_t name_idx = dist_nome(rng);

  return {
      .matricula = dist_matricula(rng),
      .nome = nomes_disponiveis[name_idx],
      .cpf = dist_cpf(rng),
      .curso = static_cast<Curso>(dist_materia(rng)),
  };
}

int main() {
  std::vector<Grupo> grupos;
  grupos.reserve(5);

  std::vector<Aluno> alunos;
  alunos.reserve(20);

  Professor professores[] = {
      {.inscricao = 1, .nome = "Professor 1"},
      {.inscricao = 2, .nome = "Professor 2"},
      {.inscricao = 3, .nome = "Professor 3"},
  };

  grupos.push_back(
      {.alunos = std::vector<Aluno>(), .tema = Tema::ARVORE_BINARIA});

  grupos.push_back(
      {.alunos = std::vector<Aluno>(), .tema = Tema::ARVORE_BINARIA});

  grupos.push_back({.alunos = std::vector<Aluno>(), .tema = Tema::STACK});

  grupos.push_back({.alunos = std::vector<Aluno>(), .tema = Tema::QUEUE});

  grupos.push_back({.alunos = std::vector<Aluno>(), .tema = Tema::QUEUE});

  for (int i = 0; i < 20; i++) {
    Aluno aluno = aluno_new_random();
    alunos.push_back(aluno);
  }

  while (alunos.size() > 0) {
    size_t grupo_menos_alunos_indice = 0;
    size_t menor_quantidade_alunos = 9999;

    for (size_t i = 0; i < grupos.size(); i++) {
      if (grupos[i].alunos.size() < menor_quantidade_alunos) {
        menor_quantidade_alunos = grupos[i].alunos.size();
        grupo_menos_alunos_indice = i;
      }
    }

    grupos[grupo_menos_alunos_indice].alunos.push_back(alunos[0]);
    alunos.erase(alunos.begin());
  }

  for (size_t i = 0; i < grupos.size(); i++) {
    std::cout << "========================================\n";
    std::cout << "Grupo " << i << "\n";
    std::cout << "integrantes: \n";
    for (auto aluno : grupos[i].alunos) {
      std::cout << aluno.nome << " (" << aluno.curso << ")\n";
    }
    std::cout << "========================================\n";
  }

  Entregavel entregaveis[] = {{
                                  .nome = "Estrutura de arvore binaria",
                                  .conteudo = "Conteudo do entregavel 1",
                                  .nota = 0.0,
                                  .grupo = &grupos[0],
                                  .professor = &professores[0],
                              },
                              {
                                  .nome = "Estrutura de arvore 2",
                                  .conteudo = "Conteudo do entregavel 2",
                                  .nota = 0.0,
                                  .grupo = &grupos[1],
                                  .professor = NULL,
                              },
                              {
                                  .nome = "Stack",
                                  .conteudo = "Conteudo do entregavel Stack",
                                  .nota = 0.0,
                                  .grupo = &grupos[2],
                                  .professor = &professores[1],
                              },
                              {
                                  .nome = "Estrutura de Queue 2",
                                  .conteudo = "Conteudo do entregavel 4",
                                  .nota = 0.0,
                                  .grupo = &grupos[3],
                                  .professor = NULL,
                              },
                              {
                                  .nome = "Estrutura de Queue",
                                  .conteudo = "Conteudo do entregavel 5",
                                  .nota = 0.0,
                                  .grupo = &grupos[4],
                                  .professor = &professores[2],
                              }};

  for (int i = 0; i < 5; i++) {

    if (entregaveis[i].professor == NULL) {
      // int qtd_curso[] = {0, 0, 0};
      float media_qtd_curso = 0.0;
      // const int N = sizeof(qtd_curso) / sizeof(int);

      for (Aluno aluno : entregaveis[i].grupo->alunos) {
        // if (aluno.curso == Curso::ENGENHARIA)
        //   qtd_curso[0] += 1;
        // if (aluno.curso == Curso::CIENCIA)
        //   qtd_curso[1] += 1;
        // if (aluno.curso == Curso::DADOS)
        //   qtd_curso[2] += 1;
        media_qtd_curso += static_cast<int>(aluno.curso);
      }

      media_qtd_curso /= entregaveis->grupo->alunos.size();

      // entregavel.grupo->tema;

      float min_distance = FLT_MAX;
      Professor *closest_professor = NULL;

      for (int j = 0; j < 5; j++) {
        // Don't consider the same group
        if (j == i) {
          continue;
        }

        // Don't consider groups without a professor yet
        if (entregaveis[j].professor == NULL) {
          continue;
        }

        float media_qtd_curso_vizinho = 0.0;
        // const int N = sizeof(qtd_curso) / sizeof(int);

        for (Aluno aluno : entregaveis[j].grupo->alunos) {
          // if (aluno.curso == Curso::ENGENHARIA)
          //   qtd_curso[0] += 1;
          // if (aluno.curso == Curso::CIENCIA)
          //   qtd_curso[1] += 1;
          // if (aluno.curso == Curso::DADOS)
          //   qtd_curso[2] += 1;
          media_qtd_curso_vizinho += static_cast<int>(aluno.curso);
        }

        media_qtd_curso_vizinho /= entregaveis->grupo->alunos.size();

        float distance =
            std::sqrt(std::pow((media_qtd_curso - media_qtd_curso_vizinho), 2) +
                      std::pow((static_cast<int>(entregaveis[i].grupo->tema) -
                                static_cast<int>(entregaveis[j].grupo->tema)),
                               2));
        // std::cout << distance << " - " << min_distance << "\n";
        if (distance < min_distance) {
          closest_professor =
              &professores[entregaveis[j].professor->inscricao - 1];
          min_distance = distance;
          // std::cout << entregavel.professor->nome << "\n";
        }
      }
      entregaveis[i].professor = closest_professor;

      // int index =
      //     std::distance(qtd_curso, std::max_element(qtd_curso, qtd_curso +
      //     N));

      // Curso curso = static_cast<Curso>(index);
    }
  }

  int i = 0;
  for (auto g : entregaveis) {
    std::cout << "Grupo: " << i << "\n";
    std::cout << "Tema: " << g.nome << "\n";
    std::cout << "Professor: " << g.professor->nome << "\n";
    i++;
  }

  // alunos.clear();

  return EXIT_SUCCESS;
}
