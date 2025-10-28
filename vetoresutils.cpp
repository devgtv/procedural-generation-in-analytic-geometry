#include <cmath>
#include "vetoresutils.h"

// Função para calcular módulo de um vetor
double Modulo(const Vetor &V) 
{
    return sqrt(pow(V.x,2) + pow(V.y,2) + pow(V.z,2));
}

// Função para calcular produto escalar
double ProdutoEscalar(const Vetor &A, const Vetor &B) 
{
    return A.x*B.x + A.y*B.y + A.z*B.z;
}

// Função para calcular produto vetorial
Vetor ProdutoVetorial(const Vetor &A, const Vetor &B) 
{
    return Vetor{
        A.y*B.z - A.z*B.y,
        A.z*B.x - A.x*B.z,
        A.x*B.y - A.y*B.x
    };
}

// Função para calcular produto misto (A · (B × C))
double ProdutoMisto(const Vetor &A, const Vetor &B, const Vetor &C) 
{
    Vetor BxC = ProdutoVetorial(B,C);
    return ProdutoEscalar(A, BxC);
}

// Função para calcular ângulo entre dois vetores em graus
double AnguloEntreVetores(const Vetor &A, const Vetor &B) 
{
    double cosTheta = ProdutoEscalar(A,B) / (Modulo(A) * Modulo(B));
    if(cosTheta > 1) cosTheta = 1; // evita erro numérico
    if(cosTheta < -1) cosTheta = -1;
    return acos(cosTheta) * (180.0 / acos(-1)); 
}