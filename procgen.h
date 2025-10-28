#ifndef PROCGEN_H
#define PROCGEN_H

#include <vector>
#include <random>

using namespace std;

bool DesafioVetores(mt19937 &rng);
void MostraLabirinto(const vector<vector<char>> &lab, int px, int py);
vector<vector<char>> GerarLabirinto(int linhas, int colunas, mt19937 &rng);

#endif