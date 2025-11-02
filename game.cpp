#include <iostream>
#include <ctime>
#include <windows.h>
#include <cmath>
#include <conio.h>
#include <stack>
#include <algorithm>
#include <iomanip>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
#include "vetoresutils.h"
#include "procgen.h"
#include "devtools.h"

using namespace std;

// Configuração global do RNG (pode ser ajustada no Devtools)
ConfigRNG configRNGGlobal;

void Jogar()
{
    mt19937 rng(time(nullptr));

    // Parar música do lobby e tocar música do labirinto
    PlaySound(NULL, 0, 0); // Para qualquer música anterior
    PlaySound(TEXT("maze.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);

    vector<vector<char>> lab = GerarLabirinto(configRNGGlobal.linhasLabirinto, configRNGGlobal.colunasLabirinto, rng, configRNGGlobal);

    int px=1, py=0;
    int tentativas=3;
    int pontuacao=0;
    char move;

    while(true){
        system("cls");
        cout<<"=== LABIRINTO DOS VETORES ===\n";
        cout<<"\nTentativas restantes: "<<tentativas<<endl;
        cout<<"Pontuação: "<<pontuacao<<" pontos\n";
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
            bool acerto = DesafioVetores(rng, configRNGGlobal);
            if(!acerto){
                tentativas--;
                if(tentativas==0){
                    system("cls");
                    cout<<"=== FIM DE JOGO ===\n\n";
                    cout<<"Você perdeu todas as tentativas.\n";
                    cout<<"Pontuação final: "<<pontuacao<<" pontos\n";
                    PlaySound(NULL, 0, 0); // Parar música do labirinto
                    Sleep(3000);
                    break;
                }
                continue;
            }
            pontuacao += 10; // Ganha 10 pontos por desafio acertado
            lab[nx][ny]=' ';
        }

        px=nx; 
        py=ny;

        if(lab[px][py]=='S'){
            system("cls");
            cout<<"=== PARABÉNS! ===\n\n";
            cout<<"🎉 Você chegou ao final do labirinto!\n";
            cout<<"Pontuação final: "<<pontuacao<<" pontos\n";
            PlaySound(NULL, 0, 0); // Parar música do labirinto
            Sleep(3000);
            break;
        }
    }
    
    // Parar música do labirinto ao sair
    PlaySound(NULL, 0, 0);
}


void MostrarMenu()
{
    // Tocar música do lobby
    PlaySound(TEXT("lobby.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
    
    int opcaoSelecionada = 0;
    const int TOTAL_OPCOES = 2;
    char tecla;
    
    while(true){
        system("cls");
        cout<<"=== MENU PRINCIPAL ===\n\n";
        
        if(opcaoSelecionada == 0)
            cout<<"> Jogar\n";
        else
            cout<<"  Jogar\n";
            
        if(opcaoSelecionada == 1)
            cout<<"> Devtools\n";
        else
            cout<<"  Devtools\n";
        
        cout<<"\nUse as setas (↑/↓) para navegar e ENTER para selecionar\n";
        cout<<"Ou ESC para sair\n";
        
        tecla = _getch();
        
        if(tecla == -32 || tecla == 224){ // Código de setas no Windows
            tecla = _getch();
            if(tecla == 72) // Seta para cima
                opcaoSelecionada = (opcaoSelecionada - 1 + TOTAL_OPCOES) % TOTAL_OPCOES;
            else if(tecla == 80) // Seta para baixo
                opcaoSelecionada = (opcaoSelecionada + 1) % TOTAL_OPCOES;
        }
        else if(tecla == 13 || tecla == '\r'){ // ENTER
            if(opcaoSelecionada == 0){
                PlaySound(NULL, 0, 0); // Parar música do lobby
                Jogar();
                PlaySound(TEXT("lobby.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP); // Voltar música do lobby
            }
            else if(opcaoSelecionada == 1){
                // Manter música do lobby tocando no devtools
                Devtools(configRNGGlobal);
            }
        }
        else if(tecla == 27){ // ESC
            PlaySound(NULL, 0, 0); // Parar música ao sair
            break;
        }
    }
}

int main()
{
    SetConsoleOutputCP(65001);
    MostrarMenu();
    return 0;
}
