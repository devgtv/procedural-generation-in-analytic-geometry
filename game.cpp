#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <windows.h>
using namespace std;

struct Vetor {
    int x;
    int y;
};

// Função que gera desafios para a soma de vetores
bool desafioVetores() {
    Vetor A{rand() % 11 - 5, rand() % 11 - 5};
    Vetor B{rand() % 11 - 5, rand() % 11 - 5};

    cout << "\n=== Desafio Vetores ===" << endl;
    cout << "A = (" << A.x << "," << A.y << ")\n";
    cout << "B = (" << B.x << "," << B.y << ")\n";
    cout << "Calcule A + B = (?,?)\n";

    int rx, ry;
    cout << "Digite o valor de x: ";
    cin >> rx;
    cout << "Digite o valor de y: ";
    cin >> ry;

    if (rx == A.x + B.x && ry == A.y + B.y) {
        cout << "CORRETO! Caminho liberado!\n";
        return true;
    } else {
        cout << "ERRADO!\n";
        return false;
    }
}

// Função para limpar a tela de forma compatível
void limpaTela() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// Função para imprimir o labirinto
void mostraLabirinto(const vector<vector<char>> &lab, int px, int py) {
    for (int i = 0; i < lab.size(); i++) {
        for (int j = 0; j < lab[i].size(); j++) {
            if (i == px && j == py)
                cout << "P ";
            else
                cout << lab[i][j] << " ";
        }
        cout << endl;
    }
}

int main() {
    srand(time(0));

    vector<vector<char>> lab = {
        {'E',' ','X','D',' '},
        {'X',' ','X',' ',' '},
        {' ',' ',' ',' ','X'},
        {'D','X',' ','X',' '},
        {' ',' ','D',' ','S'}
    };

    int px = 0, py = 0;
    int tentativas = 3;

    cout << "=== LABIRINTO DOS VETORES ===" << endl;
    cout << "Movimentos: W (cima), S (baixo), A (esquerda), D (direita)" << endl;
    cout << "Resolva desafios para continuar!\n" << endl;

    while (true) {
        limpaTela(); // 🔹 limpa o terminal antes de mostrar o labirinto

        cout << "=== LABIRINTO DOS VETORES ===" << endl;
        cout << "\nTentativas restantes: " << tentativas << endl;
        mostraLabirinto(lab, px, py);
        cout << "Seu movimento: ";
        char move;
        cin >> move;

        int nx = px, ny = py;

        if (move == 'W' || move == 'w') nx--;
        else if (move == 'S' || move == 's') nx++;
        else if (move == 'A' || move == 'a') ny--;
        else if (move == 'D' || move == 'd') ny++;
        else {
            cout << "Comando inválido!" << endl;
            continue;
        }

        if (nx < 0 || ny < 0 || nx >= lab.size() || ny >= lab[0].size()) {
            cout << "Movimento inválido (fora do mapa)!" << endl;
            Sleep(50);
            continue;
        }

        if (lab[nx][ny] == 'X') {
            cout << "PAREDE! Escolha outro caminho." << endl;
            Sleep(50);
            continue;
        }

        if (lab[nx][ny] == 'D') {
            bool acerto = desafioVetores();
            if (!acerto) {
                tentativas--;
                if (tentativas == 0) {
                    cout << "\nVocê perdeu todas as tentativas. FIM DE JOGO.\n";
                    Sleep(50);
                    break;
                }
                continue;
            }
            lab[nx][ny] = ' ';
        }

        px = nx;
        py = ny;

        if (lab[px][py] == 'S') {
            limpaTela();
            cout << "\nPARABÉNS! Você chegou à saída!\n";
            break;
        }
    }

    return 0;
}
