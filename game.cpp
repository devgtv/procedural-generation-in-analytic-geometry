#include <iostream>
#include <vector>
#include <ctime>
#include <windows.h>
#include <cmath>
#include <conio.h>
#include <stack>
#include <algorithm>
#include <random>

using namespace std;

struct Vetor {
    int x, y, z;
};

// Função para calcular módulo de um vetor
double Modulo(const Vetor &V) {
    return sqrt(pow(V.x,2) + pow(V.y,2) + pow(V.z,2));
}

// Função para calcular produto escalar
double ProdutoEscalar(const Vetor &A, const Vetor &B) {
    return A.x*B.x + A.y*B.y + A.z*B.z;
}

// Função para calcular produto vetorial
Vetor ProdutoVetorial(const Vetor &A, const Vetor &B) {
    return Vetor{
        A.y*B.z - A.z*B.y,
        A.z*B.x - A.x*B.z,
        A.x*B.y - A.y*B.x
    };
}

// Função para calcular produto misto (A · (B × C))
double ProdutoMisto(const Vetor &A, const Vetor &B, const Vetor &C) {
    Vetor BxC = ProdutoVetorial(B,C);
    return ProdutoEscalar(A, BxC);
}

// Função para calcular ângulo entre dois vetores em graus
double AnguloEntreVetores(const Vetor &A, const Vetor &B) {
    double cosTheta = ProdutoEscalar(A,B) / (Modulo(A) * Modulo(B));
    if(cosTheta > 1) cosTheta = 1; // evita erro numérico
    if(cosTheta < -1) cosTheta = -1;
    return acos(cosTheta) * (180.0 / acos(-1)); 
}

// Função de desafio de vetores
bool DesafioVetores(mt19937 &rng) {
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
void MostraLabirinto(const vector<vector<char>> &lab, int px, int py) {
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

vector<vector<char>> GerarLabirinto(int linhas, int colunas, mt19937 &rng) {
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

int main(){
    SetConsoleOutputCP(65001);
    mt19937 rng(time(nullptr));

    int linhas=15, colunas=21;
    vector<vector<char>> lab = GerarLabirinto(linhas, colunas, rng);

    int px=1, py=0;
    int tentativas=3;
    char move;

    while(true){
        system("cls");
        cout<<"=== LABIRINTO DOS VETORES ===\n";
        cout<<"\nTentativas restantes: "<<tentativas<<endl;
        MostraLabirinto(lab,px,py);
        cout<<"Use W/A/S/D para mover.\nSeu movimento: ";
        move=_getch();

        int nx=px, ny=py;
        if(move=='W'||move=='w') nx--;
        else if(move=='S'||move=='s') nx++;
        else if(move=='A'||move=='a') ny--;
        else if(move=='D'||move=='d') ny++;
        else continue;

        if(nx<0||ny<0||nx>=lab.size()||ny>=lab[0].size()) continue;
        if(lab[nx][ny]=='X'){
            cout<<"PAREDE! Escolha outro caminho."<<endl;
            Sleep(1500);
            continue;
        }

        if(lab[nx][ny]=='D'){
            bool acerto = DesafioVetores(rng);
            if(!acerto){
                tentativas--;
                if(tentativas==0){
                    cout<<"\nVocê perdeu todas as tentativas. FIM DE JOGO.\n";
                    Sleep(3000);
                    break;
                }
                continue;
            }
            lab[nx][ny]=' ';
        }

        px=nx; 
        py=ny;

        if(lab[px][py]=='S'){
            cout<<"\n🎉 PARABÉNS! Você chegou ao final!\n";
            Sleep(3000);
            break;
        }
    }

    return 0;
}
