#include <iostream>
#include <array>
#include <span>
#include <random>
#include <algorithm>

struct item{
    double valor;
    double peso;
};

double calculaFitness(std::span<int> itensEscolhidos, std::span<item> itens, double pesoMax){
    double peso = 0.0d;
    double valor = 0.0d;

    for(int i = 0; i < itensEscolhidos.size(); i++){
        if(itensEscolhidos[i]){
            peso += itens[i].peso;
            valor += itens[i].valor;
        }
    }

    if(peso > pesoMax)
        return 0;
    else
        return valor;
}

int main(int argc, char* agrv[]){
        const int quantidadeItens = 10;
        const int tamanhoPopulacao = quantidadeItens * 10;
        double mochilaMax = 50;
        double valorMax = 1000;

        /* Comentado para facilitar o teste
        std::cout << "Qual a capacidade máxima da mochila? ";
        std::cin >> mochilaMax;

        std::cout << "Qual o valor máximo possível para um item? ";
        std::cin >> valorMax;
        */

        std::array<item, quantidadeItens> itens;
        
        // Inicia os geradores de números aleatórios
        std::mt19937 mt{ std::random_device{}() };

        std::uniform_real_distribution randValor{0.0d, valorMax}; 
        std::uniform_real_distribution randPeso{0.0d, mochilaMax * 0.6}; 

        std::uniform_int_distribution randCromossomo{0, 1}; 
        
        // Inicia os itens com um valor e peso aleatório
        for(int i = 0; i < quantidadeItens; i++){
            itens[i].valor = randValor(mt);
            itens[i].peso  = randPeso(mt);
        }

        for(int i = 0; i < quantidadeItens; i++){
            std::cout << "Valor: " << itens[i].valor << " Peso: " << itens[i].peso << "\n";
        }
        
        // A população é formada por um array de cromossomos, nesse caso, o cromossomo é um
        // array de valores binários que representam se um item foi escolhido ou não
        std::array<std::array<int, quantidadeItens>, tamanhoPopulacao> populacao;
        std::array<double, tamanhoPopulacao> fitness;
        
        // Inicia a população escolhendo itens aleatórios
        for(int i = 0; i < tamanhoPopulacao; i++){
            for(int j = 0; j < quantidadeItens; j++){
                populacao[i][j] = randCromossomo(mt);
            }
        }
        
        // Inicializando array com indices para o torneio
        std::array<int, tamanhoPopulacao> indices;
        std::iota(indices.begin(), indices.end(), 0);
        
        std::array<std::array<int, quantidadeItens>, tamanhoPopulacao / 4> vencedoresTorneio;
        int indexTorneio;
        int geracao = 1;
        while(true){
            for(int j = 0; j < tamanhoPopulacao; j++){
                fitness[j] = calculaFitness(populacao[j], itens, mochilaMax);
            }
            
            std::cout << "Geração " << geracao << ": \n";
            for(int j = 0; j < tamanhoPopulacao; j++){
                std::cout << "Fitness " << j << ": " << fitness[j] << "\n"; 
            }
            
            if(geracao == 1)
                break;
        
            
            // O torneio seleciona um quarto da população por rodada, então o processo todo
            // deve ser executado 4 vezes para preencher a nova população
            for(int j = 0; j < 4; j++){
                // Torneio
                // Embaralha os itens para escolher cromossomos aleatórios
                std::shuffle(indices.begin(), indices.end(), mt);
                
                indexTorneio = 0;
                // Escolhe metade da população embaralhada e compara os valores de fitness
                // de 2 em 2, os que possuirem maior fitness são escolhidos
                for(int i = 0; i < (tamanhoPopulacao / 2) - 1; i += 2){
                    if(fitness[indices[i]] >= fitness[indices[i + 1]])
                        vencedoresTorneio[indexTorneio] = populacao[indices[i]];
                    else
                        vencedoresTorneio[indexTorneio] = populacao[indices[i + 1]];
                    indexTorneio++;
                }
                
                // Crossover
        
                // Mutação
            }   
            geracao++;
            
        }

        auto maxIndex = std::max_element(fitness.begin(), fitness.end());
        int index = maxIndex - fitness.begin();
        
        double pesoTotal = 0;
        double valorTotal = 0;
        std::cout << "O individuo " << index << " foi selecionado:\n";
        for(int i = 0; i < populacao[index].size(); i++){
            if(populacao[index][i]){
                std::cout << "Item " << i << " | " << "Valor: " << itens[i].valor << " Peso: " << itens[i].peso << "\n";
                valorTotal += itens[i].valor;
                pesoTotal  += itens[i].peso;
            }
        }    
        std::cout << "Valor Total: " << valorTotal << " Peso total: " << pesoTotal << std::endl; 
    return 0;
}
