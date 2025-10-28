#include <iostream>
#include <vector>
#include <ctime>
#include <windows.h>
#include <cmath>
#include <conio.h>
#include <stack>
#include <algorithm>
#include <random>
#include "vetoresutils.h"
#include "procgen.h"

using namespace std;

// Função de desafio de vetores
bool DesafioVetores(mt19937 &rng) 
{
    uniform_int_distribution<int> dist(-5, 5);
    Vetor A{dist(rng), dist(rng), dist(rng)};
    Vetor B{dist(rng), dist(rng), dist(rng)};
    Vetor C{dist(rng), dist(rng), dist(rng)}; // para produto misto, se necessário

    system("cls");
    cout << "\n=== Desafio Vetores (R³) ===\n";
    cout << "A = (" << A.x << "," << A.y << "," << A.z << ")\n";
    cout << "B = (" << B.x << "," << B.y << "," << B.z << ")\n";

    uniform_int_distribution<int> opDist(0,5);
    int operacao = opDist(rng); // 0=subtração, 1=soma, 2=módulo, 3=produto escalar, 4=produto vetorial, 5=angulo

    int rx=0, ry=0, rz=0;
    double rm=0.0;
    bool correto;

    if(operacao == 1) { // soma
        cout << "Calcule A + B = (?, ?, ?)\n";
        cout << "x: "; cin >> rx;
        cout << "y: "; cin >> ry;
        cout << "z: "; cin >> rz;
        correto = (rx==A.x+B.x && ry==A.y+B.y && rz==A.z+B.z);
    } 
    else if(operacao == 0) { // subtração
        cout << "Calcule A - B = (?, ?, ?)\n";
        cout << "x: "; cin >> rx;
        cout << "y: "; cin >> ry;
        cout << "z: "; cin >> rz;
        correto = (rx==A.x-B.x && ry==A.y-B.y && rz==A.z-B.z);
    } 
    else if(operacao == 2) { // módulo
        cout << "Calcule |A + B| = ? (uma casa decimal)\n";
        cin >> rm;
        double m = Modulo(Vetor{A.x+B.x, A.y+B.y, A.z+B.z});
        correto = (fabs(rm - m)<0.1);
    } 
    else if(operacao == 3) { // produto escalar
        cout << "Calcule A · B = ?\n";
        cin >> rm;
        correto = (fabs(rm - ProdutoEscalar(A,B)) < 0.1);
    } 
    else if(operacao == 4) { // produto vetorial
        cout << "Calcule A x B = (?, ?, ?)\n";
        cout << "x: "; cin >> rx;
        cout << "y: "; cin >> ry;
        cout << "z: "; cin >> rz;
        Vetor Cvet = ProdutoVetorial(A,B);
        correto = (rx==Cvet.x && ry==Cvet.y && rz==Cvet.z);
    }
    else if(operacao == 5) { // ângulo
        cout << "Calcule o ângulo entre A e B (em graus, 1 casa decimal) = ?\n";
        cin >> rm;
        double ang = AnguloEntreVetores(A,B);
        correto = (fabs(rm - ang)<0.5);
    }

    if(correto) cout << "✅ CORRETO!\n";
    else cout << "❌ ERRADO!\n";

    Sleep(2000);
    system("cls");
    return correto;
}

// Funções do labirinto
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

vector<vector<char>> GerarLabirinto(int linhas, int colunas, mt19937 &rng) 
{
    if(linhas%2==0) linhas++;
    if(colunas%2==0) colunas++;

    vector<vector<char>> lab(linhas, vector<char>(colunas, 'X'));
    auto dentro = [&](int x,int y){ return x>0 && y>0 && x<linhas-1 && y<colunas-1; };
    int startX=1, startY=1;
    stack<pair<int,int>> pilha;
    pilha.push({startX,startY});
    lab[startX][startY]=' ';
    int dx[] = {-2,2,0,0};
    int dy[] = {0,0,-2,2};

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

    lab[1][0]='E';
    lab[linhas-2][colunas-1]='S';

    int numDesafios=max(1,(linhas*colunas)/30);
    uniform_int_distribution<int> distX(0, linhas-1);
    uniform_int_distribution<int> distY(0, colunas-1);

    for(int i=0;i<numDesafios;i++){
        int x,y;
        do{ x=distX(rng); y=distY(rng); }while(lab[x][y]!=' ');
        lab[x][y]='D';
    }

    return lab;
}