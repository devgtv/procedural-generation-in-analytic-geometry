#include <iostream>
#include <ctime>
#include <windows.h>
#include <cmath>
#include <conio.h>
#include <stack>
#include <algorithm>
#include "vetoresutils.h"
#include "procgen.h"

using namespace std;

int main()
{
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
