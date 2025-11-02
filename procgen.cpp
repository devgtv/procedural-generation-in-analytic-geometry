#include <iostream>
#include <vector>
#include <ctime>
#include <windows.h>
#include <cmath>
#include <conio.h>
#include <stack>
#include <algorithm>
#include <random>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib") // Biblioteca necessária para PlaySound()

#include "vetoresutils.h" // Funções auxiliares de vetores (ProdutoEscalar, ProdutoVetorial, Modulo, etc.)
#include "procgen.h"      // Funções auxiliares de geração procedural de labirintos

using namespace std;

// =======================
// Função de desafio de vetores
// =======================
/*
 * DesafioVetores
 * ----------------
 * Executa um desafio matemático envolvendo vetores no R³.
 * Pode incluir: soma, subtração, módulo, produto escalar, produto vetorial, ângulo
 * ou um modo especial "BOSS" (produto misto).
 *
 * Parâmetros:
 *   rng - gerador de números aleatórios (mt19937)
 *
 * Retorno:
 *   bool - true se o usuário acertou o desafio, false caso contrário
 */
bool DesafioVetores(mt19937 &rng, const ConfigRNG &config) 
{
    // Criação de vetores aleatórios A, B e C (C usado apenas no modo BOSS)
    uniform_int_distribution<int> dist(-5, 5);
    Vetor A{dist(rng), dist(rng), dist(rng)};
    Vetor B{dist(rng), dist(rng), dist(rng)};
    Vetor C{dist(rng), dist(rng), dist(rng)};

    system("cls"); // Limpa a tela
    cout << "\n=== Desafio Vetores (R³) ===\n";
    cout << "A = (" << A.x << "," << A.y << "," << A.z << ")\n";
    cout << "B = (" << B.x << "," << B.y << "," << B.z << ")\n";

    // ===============================
    // Seleção da operação usando configuração de pesos customizada
    // ===============================
    vector<int> opPesos;
    for(int i = 0; i < config.pesosOperacoes.size(); i++){
        for(int j = 0; j < config.pesosOperacoes[i]; j++){
            opPesos.push_back(i);
        }
    }
    if(opPesos.empty()) opPesos = {0}; // Fallback caso todos os pesos sejam 0
    uniform_int_distribution<int> distOp(0, opPesos.size()-1);
    int operacao = opPesos[distOp(rng)];

    // Variáveis para entrada do usuário
    int rx=0, ry=0, rz=0;  // respostas vetoriais
    double rm=0.0;          // resposta escalar
    bool correto = false;   // flag de acerto

    // ===============================
    // Desafios normais
    // ===============================
    
    if(operacao == 1) { // Soma de vetores
        cout << "Calcule A + B = (?, ?, ?)\n";
        cout << "x: "; cin >> rx;
        cout << "y: "; cin >> ry;
        cout << "z: "; cin >> rz;
        correto = (rx==A.x+B.x && ry==A.y+B.y && rz==A.z+B.z);
    } 
    else if(operacao == 0) { // Subtração de vetores
        cout << "Calcule A - B = (?, ?, ?)\n";
        cout << "x: "; cin >> rx;
        cout << "y: "; cin >> ry;
        cout << "z: "; cin >> rz;
        correto = (rx==A.x-B.x && ry==A.y-B.y && rz==A.z-B.z);
    } 
    else if(operacao == 2) { // Módulo de vetor
        cout << "Calcule |A + B| = ? (uma casa decimal)\n";
        cin >> rm;
        double m = Modulo(Vetor{A.x+B.x, A.y+B.y, A.z+B.z});
        correto = (fabs(rm - m)<0.1);
    } 
    else if(operacao == 3) { // Produto escalar
        cout << "Calcule A · B = ?\n";
        cin >> rm;
        correto = (fabs(rm - ProdutoEscalar(A,B)) < 0.1);
    } 
    else if(operacao == 4) { // Produto vetorial
        cout << "Calcule A x B = (?, ?, ?)\n";
        cout << "x: "; cin >> rx;
        cout << "y: "; cin >> ry;
        cout << "z: "; cin >> rz;
        Vetor Cvet = ProdutoVetorial(A,B);
        correto = (rx==Cvet.x && ry==Cvet.y && rz==Cvet.z);
    }
    else if(operacao == 5) { // Ângulo entre vetores
        cout << "Calcule o ângulo entre A e B (em graus, 1 casa decimal) = ?\n";
        cin >> rm;
        double ang = AnguloEntreVetores(A,B);
        correto = (fabs(rm - ang)<0.5);
    }
    // ===============================
    // Modo BOSS
    // ===============================
    else if(operacao == 6) { 
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

        // Efeito de alerta piscante
        for (int i = 0; i < 3; ++i) {
            system("cls");
            SetConsoleTextAttribute(hConsole, 12); // vermelho
            cout << "\n🔥 ALERTA! BOSS DESAFIO DETECTADO 🔥\n";
            Sleep(300);
            system("cls");
            SetConsoleTextAttribute(hConsole, 7);
            Sleep(300);
        }

        // Cor vermelha fixa e música de fundo
        system("cls");
        SetConsoleTextAttribute(hConsole, 12);
        PlaySound(TEXT("boss.wav"), NULL, SND_FILENAME | SND_ASYNC);

        cout << "\n🔥🔥🔥 BOSS DESAFIO: PRODUTO MISTO 🔥🔥🔥\n";
        cout << "A = (" << A.x << "," << A.y << "," << A.z << ")\n";
        cout << "B = (" << B.x << "," << B.y << "," << B.z << ")\n";
        cout << "C = (" << C.x << "," << C.y << "," << C.z << ")\n\n";
        cout << "Calcule o PRODUTO MISTO [A,B,C] = A · (B x C)\n";
        cout << "(Resultado é um escalar)\n\n";

        cout << "Resposta: ";
        cin >> rm;

        double produtoMisto = ProdutoEscalar(A, ProdutoVetorial(B, C));
        correto = fabs(rm - produtoMisto) < 0.1;

        if (correto) cout << "\n✅ INCRÍVEL! VOCÊ DERROTOU O BOSS!\n";
        else cout << "\n❌ O BOSS TE DERROTOU...\n";

        PlaySound(NULL, 0, 0); // para música
        SetConsoleTextAttribute(hConsole, 7);
        Sleep(4000);
        system("cls");
        return correto;
    }

    // Feedback para o usuário
    if(correto) cout << "✅ CORRETO!\n";
    else cout << "❌ ERRADO!\n";

    Sleep(2000);
    system("cls");
    return correto;
}

// =======================
// Função de exibição do labirinto
// =======================
/*
 * MostraLabirinto
 * ----------------
 * Exibe o labirinto na tela, destacando o jogador e paredes.
 *
 * Parâmetros:
 *   lab - matriz do labirinto
 *   px  - posição x do jogador
 *   py  - posição y do jogador
 */
void MostraLabirinto(const vector<vector<char>> &lab, int px, int py) 
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    for(int i=0;i<lab.size();i++){
        for(int j=0;j<lab[i].size();j++){
            if(i==px && j==py){
                SetConsoleTextAttribute(hConsole, 14); cout<<"★ "; SetConsoleTextAttribute(hConsole, 7);
            } else if(lab[i][j]=='X'){
                SetConsoleTextAttribute(hConsole, 8); cout<<"■ "; SetConsoleTextAttribute(hConsole, 7);
            } else cout<<lab[i][j]<<" ";
        }
        cout<<endl;
    }
}

// =======================
// Função de geração do labirinto
// =======================
/*
 * GerarLabirinto
 * ----------------
 * Cria um labirinto usando o algoritmo de backtracking recursivo.
 * Adiciona entradas, saídas e desafios aleatórios.
 *
 * Parâmetros:
 *   linhas - número de linhas
 *   colunas - número de colunas
 *   rng - gerador de números aleatórios (mt19937)
 *
 * Retorno:
 *   vector<vector<char>> - matriz representando o labirinto
 */
vector<vector<char>> GerarLabirinto(int linhas, int colunas, mt19937 &rng, const ConfigRNG &config) 
{
    // Ajuste para garantir número ímpar de linhas e colunas
    if(linhas%2==0) linhas++;
    if(colunas%2==0) colunas++;

    vector<vector<char>> lab(linhas, vector<char>(colunas, 'X'));

    auto dentro = [&](int x,int y){ return x>0 && y>0 && x<linhas-1 && y<colunas-1; };
    int startX=1, startY=1;
    stack<pair<int,int>> pilha;

    // Ponto inicial
    pilha.push({startX,startY});
    lab[startX][startY]=' ';
    int dx[] = {-2,2,0,0};
    int dy[] = {0,0,-2,2};

    // Algoritmo de backtracking para gerar caminhos
    while(!pilha.empty()){
        int x=pilha.top().first;
        int y=pilha.top().second;
        vector<int> dirs={0,1,2,3};
        shuffle(dirs.begin(),dirs.end(),rng);
        bool moveu=false;
        for(int d: dirs){
            int nx=x+dx[d], ny=y+dy[d];
            if(dentro(nx,ny) && lab[nx][ny]=='X'){
                lab[(x+nx)/2][(y+ny)/2]=' ';
                lab[nx][ny]=' ';
                pilha.push({nx,ny});
                moveu=true;
                break;
            }
        }
        if(!moveu) pilha.pop();
    }

    // Entrada e saída
    lab[1][0]='E';
    lab[linhas-2][colunas-1]='S';

    // Inserção de desafios aleatórios (com multiplicador configurável)
    int numDesafios = max(1, (int)((linhas*colunas)/30.0 * config.multiplicadorDesafios));
    uniform_int_distribution<int> distX(0, linhas-1);
    uniform_int_distribution<int> distY(0, colunas-1);

    for(int i=0;i<numDesafios;i++){
        int x,y;
        do{ x=distX(rng); y=distY(rng); }while(lab[x][y]!=' ');
        lab[x][y]='D';
    }

    return lab;
}
