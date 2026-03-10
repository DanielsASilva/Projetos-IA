#include <cstdint>
#include <iostream>

typedef enum { ENGENHARIA, CIENCIA, DADOS } Curso;

typedef struct {
  unsigned long matricula;
  std::string nome;
  uint64_t cpf;
  Curso curso;
} Aluno;

int main(int argc, char *argv[]) {
  std::cout << "Hello, World!\n";
  return 0;
}
