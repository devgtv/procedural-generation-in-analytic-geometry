#ifndef VETORES_UTILS_H
#define VETORES_UTILS_H

struct Vetor 
{
    int x, y, z;
};

double Modulo(const Vetor &V);
double ProdutoEscalar(const Vetor &A, const Vetor &B);
Vetor ProdutoVetorial(const Vetor &A, const Vetor &B);
double ProdutoMisto(const Vetor &A, const Vetor &B, const Vetor &C);
double AnguloEntreVetores(const Vetor &A, const Vetor &B);

#endif