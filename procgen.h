#ifndef PROCGEN_H
#define PROCGEN_H

#include <vector>
#include <random>

using namespace std;

// Estrutura para configurações de probabilidade dos desafios
struct ConfigRNG {
    // Pesos para cada tipo de desafio (0=Subtração, 1=Soma, 2=Módulo, 3=Produto Escalar, 4=Produto Vetorial, 5=Ângulo, 6=BOSS)
    vector<int> pesosOperacoes = {1, 1, 1, 1, 1, 1, 2}; // Padrão: BOSS tem peso 2
    // Multiplicador para quantidade de desafios no labirinto
    double multiplicadorDesafios = 1.0; // Padrão: quantidade normal
    // Tamanho do labirinto
    int linhasLabirinto = 15; // Padrão: 15 linhas
    int colunasLabirinto = 21; // Padrão: 21 colunas
};

bool DesafioVetores(mt19937 &rng, const ConfigRNG &config = ConfigRNG());
void MostraLabirinto(const vector<vector<char>> &lab, int px, int py);
vector<vector<char>> GerarLabirinto(int linhas, int colunas, mt19937 &rng, const ConfigRNG &config = ConfigRNG());

#endif