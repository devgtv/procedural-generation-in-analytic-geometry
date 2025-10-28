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


bool desafioVetores(mt19937 &rng) {
    // gera vetores aleatorios
    uniform_int_distribution<int> dist(-5, 5);
    Vetor A{dist(rng), dist(rng), dist(rng)};
    Vetor B{dist(rng), dist(rng), dist(rng)};

    system("cls"); // limpa tela
    cout << "\n=== Desafio Vetores (R³) ===\n";
    cout << "A = (" << A.x << "," << A.y << "," << A.z << ")\n";
    cout << "B = (" << B.x << "," << B.y << "," << B.z << ")\n";

    uniform_int_distribution<int> opDist(0, 2);
    int operacao = opDist(rng); // 0=subtração, 1=soma, 2=módulo

    int rx=0, ry=0, rz=0;
    double rm=0.0;
    bool correto;
// Escolhe operação aleatória (0 = subtração, 1 = soma, 2 = módulo)
    if(operacao == 1) {
        cout << "Calcule A + B = (?, ?, ?)\n";
        cout << "x: "; cin >> rx;
        cout << "y: "; cin >> ry;
        cout << "z: "; cin >> rz;
        correto = (rx==A.x+B.x && ry==A.y+B.y && rz==A.z+B.z);
    } else if(operacao==0) {
        cout << "Calcule A - B = (?, ?, ?)\n";
        cout << "x: "; cin >> rx;
        cout << "y: "; cin >> ry;
        cout << "z: "; cin >> rz;
        correto = (rx==A.x-B.x && ry==A.y-B.y && rz==A.z-B.z);
    } else {
        cout << "Calcule |A + B| = ?\n";
        cin >> rm;
        double m = sqrt(pow(A.x+B.x,2) + pow(A.y+B.y,2) + pow(A.z+B.z,2));
        correto = (fabs(rm - m)<0.1);
    }

    if(correto) cout << "✅ CORRETO!\n";
    else cout << "❌ ERRADO!\n";

    Sleep(2000);
    system("cls");
    return correto;
}


void mostraLabirinto(const vector<vector<char>> &lab, int px, int py) { // exibe o labirinto
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

// funcao para gerar o labirinto procedural 
vector<vector<char>> geraLabirinto(int linhas, int colunas, mt19937 &rng) {
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
    SetConsoleOutputCP(65001); // coloca no padrao utf - 8

    // Inicializa rng com tempo atual
    mt19937 rng(time(nullptr));

    int linhas=15, colunas=21;
    vector<vector<char>> lab = geraLabirinto(linhas, colunas, rng);

    int px=1, py=0;
    int tentativas=3;
    char move;

    while(true){
        system("cls"); // limpa a tela no inicio do loop
        cout<<"=== LABIRINTO DOS VETORES ===\n";
        cout<<"\nTentativas restantes: "<<tentativas<<endl;
        mostraLabirinto(lab,px,py);
        cout<<"Use W/A/S/D para mover.\nSeu movimento: ";
        move=_getch(); // le a tecla sem precisar de enter

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
            bool acerto = desafioVetores(rng);
            if(!acerto){
                tentativas--;
                if(tentativas==0){
                    cout<<"\nVocê perdeu todas as tentativas. FIM DE JOGO.\n";
                    Sleep(3000);
                    break;
                }
                continue;
            }
            lab[nx][ny]=' '; // remove o desafio se acertou 
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
