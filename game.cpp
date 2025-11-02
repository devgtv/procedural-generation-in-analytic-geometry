#include <iostream>
#include <ctime>
#include <windows.h>
#include <cmath>
#include <conio.h>
#include <stack>
#include <algorithm>
#include <iomanip>
#include "vetoresutils.h"
#include "procgen.h"

using namespace std;

// Configuração global do RNG (pode ser ajustada no Devtools)
ConfigRNG configRNGGlobal;

void Jogar()
{
    mt19937 rng(time(nullptr));

    int linhas=15, colunas=21;
    vector<vector<char>> lab = GerarLabirinto(linhas, colunas, rng, configRNGGlobal);

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
            bool acerto = DesafioVetores(rng, configRNGGlobal);
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
}

void ConfigurarProbabilidades()
{
    int opcao;
    string nomesDesafios[] = {
        "Subtração (A - B)",
        "Soma (A + B)",
        "Módulo (|A + B|)",
        "Produto Escalar (A · B)",
        "Produto Vetorial (A x B)",
        "Ângulo entre vetores",
        "BOSS (Produto Misto)"
    };
    
    while(true){
        system("cls");
        cout<<"=== CONFIGURAR PROBABILIDADES DE DESAFIOS ===\n\n";
        cout<<"Probabilidade atual (peso relativo):\n\n";
        
        int totalPeso = 0;
        for(int i = 0; i < 7; i++){
            if(i < configRNGGlobal.pesosOperacoes.size())
                totalPeso += configRNGGlobal.pesosOperacoes[i];
        }
        
        for(int i = 0; i < 7; i++){
            int peso = (i < configRNGGlobal.pesosOperacoes.size()) ? configRNGGlobal.pesosOperacoes[i] : 0;
            double percentual = (totalPeso > 0) ? (peso * 100.0 / totalPeso) : 0.0;
            cout<<(i+1)<<". "<<nomesDesafios[i]<<": peso="<<peso;
            if(totalPeso > 0) cout<<" ("<<fixed<<setprecision(1)<<percentual<<"%)";
            cout<<endl;
        }
        
        cout<<"\n8. Multiplicador de quantidade de desafios no labirinto: ";
        cout<<fixed<<setprecision(2)<<configRNGGlobal.multiplicadorDesafios<<"x\n";
        
        cout<<"\n9. Resetar para padrão\n";
        cout<<"0. Voltar\n\n";
        cout<<"Escolha uma opção: ";
        cin>>opcao;
        
        if(opcao >= 1 && opcao <= 7){
            int indice = opcao - 1;
            if(configRNGGlobal.pesosOperacoes.size() <= indice){
                configRNGGlobal.pesosOperacoes.resize(indice + 1, 0);
            }
            
            cout<<"\nPeso atual para "<<nomesDesafios[indice]<<": "<<configRNGGlobal.pesosOperacoes[indice]<<endl;
            cout<<"Novo peso (0-100): ";
            int novoPeso;
            cin>>novoPeso;
            if(novoPeso >= 0 && novoPeso <= 100){
                configRNGGlobal.pesosOperacoes[indice] = novoPeso;
            }
            else{
                cout<<"Peso inválido! Deve ser entre 0 e 100.\n";
                Sleep(2000);
            }
        }
        else if(opcao == 8){
            cout<<"\nMultiplicador atual: "<<configRNGGlobal.multiplicadorDesafios<<endl;
            cout<<"Novo multiplicador (0.1 a 10.0): ";
            double novoMult;
            cin>>novoMult;
            if(novoMult >= 0.1 && novoMult <= 10.0){
                configRNGGlobal.multiplicadorDesafios = novoMult;
            }
            else{
                cout<<"Multiplicador inválido! Deve ser entre 0.1 e 10.0.\n";
                Sleep(2000);
            }
        }
        else if(opcao == 9){
            configRNGGlobal.pesosOperacoes = {1, 1, 1, 1, 1, 1, 2};
            configRNGGlobal.multiplicadorDesafios = 1.0;
            cout<<"\nConfigurações resetadas para padrão!\n";
            Sleep(2000);
        }
        else if(opcao == 0){
            break;
        }
    }
}

void Devtools()
{
    mt19937 rng(time(nullptr));
    int opcao;
    
    while(true){
        system("cls");
        cout<<"=== DEVTOOLS ===\n\n";
        cout<<"1. Configurar probabilidades de desafios (RNG)\n";
        cout<<"2. Gerar e mostrar labirinto\n";
        cout<<"3. Testar desafio de vetores\n";
        cout<<"4. Ver configuração atual\n";
        cout<<"5. Voltar ao menu\n\n";
        cout<<"Escolha uma opção: ";
        cin>>opcao;
        
        if(opcao==1){
            ConfigurarProbabilidades();
        }
        else if(opcao==2){
            system("cls");
            cout<<"=== GERADOR DE LABIRINTO ===\n\n";
            int linhas, colunas;
            cout<<"Número de linhas: ";
            cin>>linhas;
            cout<<"Número de colunas: ";
            cin>>colunas;
            
            vector<vector<char>> lab = GerarLabirinto(linhas, colunas, rng, configRNGGlobal);
            MostraLabirinto(lab, -1, -1);
            cout<<"\nPressione qualquer tecla para continuar...";
            _getch();
        }
        else if(opcao==3){
            system("cls");
            cout<<"=== TESTE DE DESAFIO ===\n\n";
            bool acerto = DesafioVetores(rng, configRNGGlobal);
            cout<<"\nResultado: "<<(acerto ? "ACERTOU!" : "ERROU!")<<endl;
            cout<<"\nPressione qualquer tecla para continuar...";
            _getch();
        }
        else if(opcao==4){
            system("cls");
            cout<<"=== CONFIGURAÇÃO ATUAL ===\n\n";
            cout<<"Pesos das operações:\n";
            for(int i = 0; i < configRNGGlobal.pesosOperacoes.size(); i++){
                cout<<"  Operação "<<i<<": "<<configRNGGlobal.pesosOperacoes[i]<<endl;
            }
            cout<<"\nMultiplicador de desafios: "<<configRNGGlobal.multiplicadorDesafios<<"x\n";
            cout<<"\nPressione qualquer tecla para continuar...";
            _getch();
        }
        else if(opcao==5){
            break;
        }
    }
}

void MostrarMenu()
{
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
                Jogar();
            }
            else if(opcaoSelecionada == 1){
                Devtools();
            }
        }
        else if(tecla == 27){ // ESC
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
