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
#include "consoleutils.h"

using namespace std;

// Configuração global do RNG (pode ser ajustada no Devtools)
ConfigRNG configRNGGlobal;

// Função para mostrar créditos
void MostrarCreditos()
{
    LimparTela();
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    
    // Título
    SetConsoleTextAttribute(hConsole, 14); // Amarelo
    cout << "\n\n";
    cout << "    ╔════════════════════════════════════╗\n";
    cout << "    ║          JOGO DA BEBETECH          ║\n";
    cout << "    ╚════════════════════════════════════╝\n\n";
    SetConsoleTextAttribute(hConsole, 7); // Branco
    
    // Créditos
    cout << "    Programador: Gustavo Fialho\n\n";
    cout << "    Revisor matematico: Lucas\n\n";
    cout << "    Level Design: Gustavo Fialho, Henrique Augusto e Lucas\n\n";
    
    cout << "    Tester: Henrique Augusto\n\n";
    
    cout << "    Debugger: Thiago Bahia\n\n";
    cout << "    versonamento de codigo : Thiago Bahia\n\n";
    SetConsoleTextAttribute(hConsole, 10); // Verde
    cout << "    Obrigado a todos!\n\n";
    SetConsoleTextAttribute(hConsole, 7); // Branco
    
    cout << "\n    Pressione qualquer tecla para continuar...";
    _getch();
}

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
    int move; // int para suportar códigos das setas
    int pxAntiga = px, pyAntiga = py; // Guardar posição anterior
    int tentativasAntiga = tentativas, pontuacaoAntiga = pontuacao;

    // Limpar e mostrar tela inicial
    LimparTela();
    cout<<"=== LABIRINTO DOS VETORES ===\n";
    cout<<"\nTentativas restantes: "<<tentativas<<endl;
    cout<<"Pontuação: "<<pontuacao<<" pontos\n";
    
    // Calcular offset para o labirinto (linha onde começa)
    int linhaInicioLabirinto = 4; // Após título, linha vazia, tentativas e pontuação
    
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    
    // Mostrar labirinto inicial
    for(int i=0;i<lab.size();i++){
        for(int j=0;j<lab[i].size();j++){
            if(i==px && j==py){
                SetConsoleTextAttribute(hConsole, 14); cout<<"★ "; SetConsoleTextAttribute(hConsole, 7);
            } else if(lab[i][j]=='X'){
                SetConsoleTextAttribute(hConsole, 8); cout<<"■ "; SetConsoleTextAttribute(hConsole, 7);
            } else if(lab[i][j]=='D'){
                SetConsoleTextAttribute(hConsole, 12); cout<<"♦ "; SetConsoleTextAttribute(hConsole, 7);
            } else if(lab[i][j]=='S'){
                SetConsoleTextAttribute(hConsole, 10); cout<<"S "; SetConsoleTextAttribute(hConsole, 7);
            } else cout<<lab[i][j]<<" ";
        }
        cout<<endl;
    }
    cout<<"Use as setas (↑↓←→) ou W/A/S/D para mover.\nSeu movimento: ";

    while(true){
        move=_getch();

        int nx=px, ny=py;
        bool movimentoValido = false;
        
        // Verificar se é uma seta do teclado (código 224 ou -32 seguido do código da seta)
        if(move == -32 || move == 224){
            move = _getch();
            if(move == 72) { nx--; movimentoValido = true; }      // Seta para cima (↑)
            else if(move == 80) { nx++; movimentoValido = true; } // Seta para baixo (↓)
            else if(move == 75) { ny--; movimentoValido = true; } // Seta para esquerda (←)
            else if(move == 77) { ny++; movimentoValido = true; } // Seta para direita (→)
        }
        // Verificar se é WASD
        else if(move=='W'||move=='w') { nx--; movimentoValido = true; }
        else if(move=='S'||move=='s') { nx++; movimentoValido = true; }
        else if(move=='A'||move=='a') { ny--; movimentoValido = true; }
        else if(move=='D'||move=='d') { ny++; movimentoValido = true; }
        
        if(!movimentoValido) {
            // Tecla inválida - descartar teclas extras do buffer
            while(_kbhit()) _getch();
            continue;
        }

        // Verificar se está dentro dos limites
        if(nx<0||ny<0||nx>=lab.size()||ny>=lab[0].size()) {
            // Movimento fora dos limites - descartar teclas extras
            while(_kbhit()) _getch();
            continue;
        }
        
        // Verificar se é parede
        if(lab[nx][ny]=='X'){
            // Parede - apenas mostrar mensagem temporária sem limpar tela
            GetConsoleScreenBufferInfo(hConsole, &csbi);
            COORD pos = {0, (SHORT)(csbi.dwCursorPosition.Y)};
            SetConsoleCursorPosition(hConsole, pos);
            cout<<"PAREDE! Escolha outro caminho.                    "<<flush;
            Sleep(300);
            // Limpar a mensagem
            SetConsoleCursorPosition(hConsole, pos);
            cout<<"                                                    "<<flush;
            SetConsoleCursorPosition(hConsole, csbi.dwCursorPosition);
            // Descartar teclas extras do buffer
            while(_kbhit()) _getch();
            continue;
        }

        // Movimento válido - atualizar posição
        pxAntiga = px;
        pyAntiga = py;
        px=nx; 
        py=ny;

        // Atualizar apenas a posição do jogador no labirinto (sem limpar tela)
        COORD posAntiga = {(SHORT)(pyAntiga * 2), (SHORT)(linhaInicioLabirinto + pxAntiga)};
        COORD posNova = {(SHORT)(py * 2), (SHORT)(linhaInicioLabirinto + px)};
        
        // Limpar posição antiga
        SetConsoleCursorPosition(hConsole, posAntiga);
        if(lab[pxAntiga][pyAntiga]=='X'){
            SetConsoleTextAttribute(hConsole, 8); cout<<"■ "; SetConsoleTextAttribute(hConsole, 7);
        } else if(lab[pxAntiga][pyAntiga]=='D'){
            SetConsoleTextAttribute(hConsole, 12); cout<<"♦ "; SetConsoleTextAttribute(hConsole, 7);
        } else if(lab[pxAntiga][pyAntiga]=='S'){
            SetConsoleTextAttribute(hConsole, 10); cout<<"S "; SetConsoleTextAttribute(hConsole, 7);
        } else {
            cout<<"  ";
        }
        
        // Desenhar jogador na nova posição
        SetConsoleCursorPosition(hConsole, posNova);
        SetConsoleTextAttribute(hConsole, 14); cout<<"★ "; SetConsoleTextAttribute(hConsole, 7);

        // Verificar se é desafio
        if(lab[px][py]=='D'){
            bool acerto = DesafioVetores(rng, configRNGGlobal);
            if(!acerto){
                // Errou - perde uma vida, desafio continua no mapa e jogador volta para posição anterior
                tentativas--;
                if(tentativas==0){
                    LimparTela();
                    cout<<"=== FIM DE JOGO ===\n\n";
                    cout<<"Você perdeu todas as tentativas.\n";
                    cout<<"Pontuação final: "<<pontuacao<<" pontos\n";
                    PlaySound(NULL, 0, 0); // Parar música do labirinto
                    Sleep(3000);
                    break;
                }
                // Voltar jogador para posição anterior quando erra
                px = pxAntiga;
                py = pyAntiga;
                // Não remove o desafio do mapa quando erra
            } else {
                // Acertou - remove o desafio do mapa e ganha pontos
                pontuacao += 10; // Ganha 10 pontos por desafio acertado
                lab[px][py]=' '; // Remove o desafio apenas quando acerta
            }
            
            // Redesenhar o labirinto completo após o desafio
            LimparTela();
            cout<<"=== LABIRINTO DOS VETORES ===\n";
            cout<<"\nTentativas restantes: "<<tentativas<<endl;
            cout<<"Pontuação: "<<pontuacao<<" pontos\n";
            
            // Redesenhar o labirinto
            for(int i=0;i<lab.size();i++){
                for(int j=0;j<lab[i].size();j++){
                    if(i==px && j==py){
                        SetConsoleTextAttribute(hConsole, 14); cout<<"★ "; SetConsoleTextAttribute(hConsole, 7);
                    } else if(lab[i][j]=='X'){
                        SetConsoleTextAttribute(hConsole, 8); cout<<"■ "; SetConsoleTextAttribute(hConsole, 7);
                    } else if(lab[i][j]=='D'){
                        SetConsoleTextAttribute(hConsole, 12); cout<<"♦ "; SetConsoleTextAttribute(hConsole, 7);
                    } else if(lab[i][j]=='S'){
                        SetConsoleTextAttribute(hConsole, 10); cout<<"S "; SetConsoleTextAttribute(hConsole, 7);
                    } else cout<<lab[i][j]<<" ";
                }
                cout<<endl;
            }
            cout<<"Use as setas (↑↓←→) ou W/A/S/D para mover.\nSeu movimento: ";
            
            // Atualizar variáveis de controle
            tentativasAntiga = tentativas;
            pontuacaoAntiga = pontuacao;
            continue; // Voltar ao início do loop para aguardar próximo movimento
        }

        // Atualizar informações de tentativas e pontuação se mudaram
        if(tentativas != tentativasAntiga || pontuacao != pontuacaoAntiga){
            COORD posTentativas = {0, 2};
            SetConsoleCursorPosition(hConsole, posTentativas);
            cout<<"Tentativas restantes: "<<tentativas<<"   "<<endl;
            COORD posPontuacao = {0, 3};
            SetConsoleCursorPosition(hConsole, posPontuacao);
            cout<<"Pontuação: "<<pontuacao<<" pontos   "<<endl;
            tentativasAntiga = tentativas;
            pontuacaoAntiga = pontuacao;
        }

        // Verificar se chegou ao fim
        if(lab[px][py]=='S'){
            // Descartar todas as teclas do buffer para evitar que continue processando
            while(_kbhit()) _getch();
            
            // Tocar música de vitória ao chegar na saída
            PlaySound(TEXT("saida.wav"), NULL, SND_FILENAME | SND_ASYNC);
            
            LimparTela();
            HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
            SetConsoleTextAttribute(hConsole, 14); // Amarelo
            cout<<"=== PARABÉNS! ===\n\n";
            SetConsoleTextAttribute(hConsole, 7); // Branco
            cout<<"🎉 Você chegou ao final do labirinto!\n";
            cout<<"Pontuação final: "<<pontuacao<<" pontos\n\n";
            Sleep(3000);
            
            // Mostrar créditos
            MostrarCreditos();
            
            PlaySound(NULL, 0, 0); // Parar música
            break;
        }

        // Mover cursor para posição de entrada (sem reimprimir a mensagem)
        GetConsoleScreenBufferInfo(hConsole, &csbi);
        COORD posEntrada = {0, (SHORT)(linhaInicioLabirinto + lab.size() + 1)};
        SetConsoleCursorPosition(hConsole, posEntrada);
        // Não reimprimir a mensagem, apenas posicionar o cursor
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
        LimparTela();
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
