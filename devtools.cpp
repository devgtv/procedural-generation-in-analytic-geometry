#include <iostream>
#include <ctime>
#include <windows.h>
#include <iomanip>
#include <random>
#include <conio.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
#include "devtools.h"
#include "procgen.h"
#include "consoleutils.h"

using namespace std;

void ConfigurarProbabilidades(ConfigRNG &config)
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
        LimparTela();
        cout<<"=== CONFIGURAR PROBABILIDADES DE DESAFIOS ===\n\n";
        cout<<"Probabilidade atual (peso relativo):\n\n";
        
        int totalPeso = 0;
        for(int i = 0; i < 7; i++){
            if(i < config.pesosOperacoes.size())
                totalPeso += config.pesosOperacoes[i];
        }
        
        for(int i = 0; i < 7; i++){
            int peso = (i < config.pesosOperacoes.size()) ? config.pesosOperacoes[i] : 0;
            double percentual = (totalPeso > 0) ? (peso * 100.0 / totalPeso) : 0.0;
            cout<<(i+1)<<". "<<nomesDesafios[i]<<": peso="<<peso;
            if(totalPeso > 0) cout<<" ("<<fixed<<setprecision(1)<<percentual<<"%)";
            cout<<endl;
        }
        
        cout<<"\n8. Multiplicador de quantidade de desafios no labirinto: ";
        cout<<fixed<<setprecision(2)<<config.multiplicadorDesafios<<"x\n";
        
        cout<<"\n9. Configurar tamanho do labirinto\n";
        cout<<"10. Resetar para padrão\n";
        cout<<"0. Voltar\n\n";
        cout<<"Escolha uma opção: ";
        cin>>opcao;
        
        if(opcao >= 1 && opcao <= 7){
            int indice = opcao - 1;
            if(config.pesosOperacoes.size() <= indice){
                config.pesosOperacoes.resize(indice + 1, 0);
            }
            
            cout<<"\nPeso atual para "<<nomesDesafios[indice]<<": "<<config.pesosOperacoes[indice]<<endl;
            cout<<"Novo peso (0-100): ";
            int novoPeso;
            cin>>novoPeso;
            if(novoPeso >= 0 && novoPeso <= 100){
                config.pesosOperacoes[indice] = novoPeso;
            }
            else{
                cout<<"Peso inválido! Deve ser entre 0 e 100.\n";
                Sleep(2000);
            }
        }
        else if(opcao == 8){
            cout<<"\nMultiplicador atual: "<<config.multiplicadorDesafios<<endl;
            cout<<"Novo multiplicador (0.1 a 10.0): ";
            double novoMult;
            cin>>novoMult;
            if(novoMult >= 0.1 && novoMult <= 10.0){
                config.multiplicadorDesafios = novoMult;
            }
            else{
                cout<<"Multiplicador inválido! Deve ser entre 0.1 e 10.0.\n";
                Sleep(2000);
            }
        }
        else if(opcao == 9){
            cout<<"\nTamanho atual do labirinto:\n";
            cout<<"Linhas: "<<config.linhasLabirinto<<endl;
            cout<<"Colunas: "<<config.colunasLabirinto<<endl;
            cout<<"\nNovo número de linhas (mínimo 5, máximo 50): ";
            int novasLinhas;
            cin>>novasLinhas;
            if(novasLinhas >= 5 && novasLinhas <= 50){
                config.linhasLabirinto = novasLinhas;
            }
            else{
                cout<<"Valor inválido! Deve ser entre 5 e 50.\n";
                Sleep(2000);
                continue;
            }
            cout<<"Novo número de colunas (mínimo 5, máximo 50): ";
            int novasColunas;
            cin>>novasColunas;
            if(novasColunas >= 5 && novasColunas <= 50){
                config.colunasLabirinto = novasColunas;
            }
            else{
                cout<<"Valor inválido! Deve ser entre 5 e 50.\n";
                Sleep(2000);
            }
        }
        else if(opcao == 10){
            config.pesosOperacoes = {1, 1, 1, 1, 1, 1, 2};
            config.multiplicadorDesafios = 1.0;
            config.linhasLabirinto = 15;
            config.colunasLabirinto = 21;
            cout<<"\nConfigurações resetadas para padrão!\n";
            Sleep(2000);
        }
        else if(opcao == 0){
            break;
        }
    }
}

void Devtools(ConfigRNG &config)
{
    // A música do lobby já está tocando do menu, não precisa tocar novamente
    mt19937 rng(time(nullptr));
    int opcao;
    
    while(true){
        LimparTela();
        cout<<"=== DEVTOOLS ===\n\n";
        cout<<"1. Configurar probabilidades de desafios (RNG)\n";
        cout<<"2. Gerar e mostrar labirinto\n";
        cout<<"3. Testar desafio de vetores\n";
        cout<<"4. Ver configuração atual\n";
        cout<<"5. Configurar tamanho do labirinto\n";
        cout<<"6. Voltar ao menu\n\n";
        cout<<"Escolha uma opção: ";
        cin>>opcao;
        
        if(opcao==1){
            ConfigurarProbabilidades(config);
        }
        else if(opcao==2){
            LimparTela();
            cout<<"=== GERADOR DE LABIRINTO ===\n\n";
            cout<<"Usando tamanho configurado: "<<config.linhasLabirinto<<"x"<<config.colunasLabirinto<<"\n\n";
            vector<vector<char>> lab = GerarLabirinto(config.linhasLabirinto, config.colunasLabirinto, rng, config);
            MostraLabirinto(lab, -1, -1);
            cout<<"\nPressione qualquer tecla para continuar...";
            _getch();
        }
        else if(opcao==3){
            LimparTela();
            cout<<"=== TESTE DE DESAFIO ===\n\n";
            bool acerto = DesafioVetores(rng, config);
            cout<<"\nResultado: "<<(acerto ? "ACERTOU!" : "ERROU!")<<endl;
            cout<<"\nPressione qualquer tecla para continuar...";
            _getch();
        }
        else if(opcao==4){
            LimparTela();
            cout<<"=== CONFIGURAÇÃO ATUAL ===\n\n";
            cout<<"Pesos das operações:\n";
            for(int i = 0; i < config.pesosOperacoes.size(); i++){
                cout<<"  Operação "<<i<<": "<<config.pesosOperacoes[i]<<endl;
            }
            cout<<"\nMultiplicador de desafios: "<<config.multiplicadorDesafios<<"x\n";
            cout<<"Tamanho do labirinto: "<<config.linhasLabirinto<<"x"<<config.colunasLabirinto<<"\n";
            cout<<"\nPressione qualquer tecla para continuar...";
            _getch();
        }
        else if(opcao==5){
            LimparTela();
            cout<<"=== CONFIGURAR TAMANHO DO LABIRINTO ===\n\n";
            cout<<"Tamanho atual: "<<config.linhasLabirinto<<" linhas x "<<config.colunasLabirinto<<" colunas\n\n";
            cout<<"Novo número de linhas (mínimo 5, máximo 50): ";
            int novasLinhas;
            cin>>novasLinhas;
            if(novasLinhas >= 5 && novasLinhas <= 50){
                config.linhasLabirinto = novasLinhas;
            }
            else{
                cout<<"Valor inválido! Deve ser entre 5 e 50.\n";
                Sleep(2000);
                continue;
            }
            cout<<"Novo número de colunas (mínimo 5, máximo 50): ";
            int novasColunas;
            cin>>novasColunas;
            if(novasColunas >= 5 && novasColunas <= 50){
                config.colunasLabirinto = novasColunas;
                cout<<"\nTamanho do labirinto configurado com sucesso!\n";
            }
            else{
                cout<<"Valor inválido! Deve ser entre 5 e 50.\n";
            }
            Sleep(2000);
        }
        else if(opcao==6){
            break;
        }
    }
}

