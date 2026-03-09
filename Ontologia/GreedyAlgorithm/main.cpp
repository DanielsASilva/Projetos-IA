#include <chrono>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <random>
#include <string>
#include <vector>

typedef struct {
  unsigned long matricula;
  std::string nome;
  uint64_t cpf;
} Aluno;

typedef struct {
  std::string nome;
} Tema;

typedef struct {
  std::vector<Aluno> alunos;
  Tema tema;
} Grupo;

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

  size_t i = 0;

  size_t name_idx = dist_nome(rng);

  return {
      .matricula = dist_matricula(rng),
      .nome = nomes_disponiveis[name_idx],
      .cpf = dist_cpf(rng),
  };
}

int main() {
  std::vector<Grupo> grupos;
  grupos.reserve(5);

  std::vector<Aluno> alunos;
  alunos.reserve(20);

  grupos.push_back(
      {.alunos = std::vector<Aluno>(), .tema = {.nome = "Árvore Binária"}});

  grupos.push_back({.alunos = std::vector<Aluno>(), .tema = {.nome = "Grafo"}});

  grupos.push_back({.alunos = std::vector<Aluno>(), .tema = {.nome = "Stack"}});

  grupos.push_back({.alunos = std::vector<Aluno>(), .tema = {.nome = "Queue"}});

  grupos.push_back({.alunos = std::vector<Aluno>(), .tema = {.nome = "Array"}});

  for (int i = 0; i < 20; i++) {
    Aluno aluno = aluno_new_random();
    alunos.push_back(aluno);
    std::cout << aluno.nome << "\n";
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
      std::cout << aluno.nome << "\n";
    }
  }

  alunos.clear();

  return EXIT_SUCCESS;
}
