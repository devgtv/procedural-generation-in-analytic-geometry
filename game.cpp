#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <windows.h>
#include <limits>
#include <cmath>
using namespace std;

struct Vetor {
    int x;
    int y;
    int z;
};

bool desafioVetores() {
    // Gera vetores aleatórios
    Vetor A{rand() % 11 - 5, rand() % 11 - 5, rand() % 11 - 5};
    Vetor B{rand() % 11 - 5, rand() % 11 - 5, rand() % 11 - 5};

    system("cls"); // LIMPA TELA
    cout << "\n=== Desafio Vetores (R³) ===" << endl;
    cout << "A = (" << A.x << "," << A.y << "," << A.z << ")\n";
    cout << "B = (" << B.x << "," << B.y << "," << B.z << ")\n";

    // Escolhe operação aleatória (0 = subtração, 1 = soma, 2 = módulo)
    int operacao = rand() % 3; 

    int rx = 0, ry = 0, rz = 0;
    double rm = 0.0;
    bool correto;

    if (operacao == 1) { 
        cout << "Calcule A + B = (?, ?, ?)\n";
        cout << "Digite o valor de x: ";
        cin >> rx;
        cout << "Digite o valor de y: ";
        cin >> ry;
        cout << "Digite o valor de z: ";
        cin >> rz;
        correto = (rx == A.x + B.x && ry == A.y + B.y && rz == A.z + B.z);
    } else if (operacao == 0) { 
        cout << "Calcule A - B = (?, ?, ?)\n";
        cout << "Digite o valor de x: ";
        cin >> rx;
        cout << "Digite o valor de y: ";
        cin >> ry;
        cout << "Digite o valor de z: ";
        cin >> rz;
        correto = (rx == A.x - B.x && ry == A.y - B.y && rz == A.z - B.z);
    } else { 
        cout << "Calcule |A + B| = ?\n";
        cout << "Digite o valor do módulo: ";
        cin >> rm;
        double m = sqrt((A.x + B.x) * (A.x + B.x) +
                        (A.y + B.y) * (A.y + B.y) +
                        (A.z + B.z) * (A.z + B.z));
        correto = (fabs(rm - m) < 0.1); // tolerância de uma casa decimal
    }

    // Mostra resultado, espera 2s e limpa tela
    if (correto)
        cout << "CORRETO! Caminho liberado!\n";
    else
        cout << "ERRADO!\n";

    Sleep(2000);
    system("cls");

    return correto;
}

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
    SetConsoleOutputCP(65001);
    srand(time(0)); // inicializa o rand() com base no tempo atual

    vector<vector<char>> lab = {
        {'E',' ','X','D',' '},
        {'X',' ','X',' ',' '},
        {' ',' ',' ',' ','X'},
        {'D','X',' ','X',' '},
        {' ',' ','D',' ','S'}
    };

    int px = 0, py = 0;
    int tentativas = 3;

    while (true) {
        system("cls");

        cout << "=== LABIRINTO DOS VETORES ===" << endl;
        cout << "\nTentativas restantes: " << tentativas << endl;
        mostraLabirinto(lab, px, py);
        cout << "Seu movimento: ";

        char move;
        cin >> move;
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        int nx = px, ny = py;

        if (move == 'W' || move == 'w') nx--;
        else if (move == 'S' || move == 's') nx++;
        else if (move == 'A' || move == 'a') ny--;
        else if (move == 'D' || move == 'd') ny++;
        else {
            cout << "Comando inválido!" << endl;
            Sleep(2000);
            continue;
        }

        if (nx < 0 || ny < 0 || nx >= lab.size() || ny >= lab[0].size()) {
            cout << "Movimento inválido (fora do mapa)!" << endl;
            Sleep(2000);
            continue;
        }

        if (lab[nx][ny] == 'X') {
            cout << "PAREDE! Escolha outro caminho." << endl;
            Sleep(2000);
            continue;
        }

        if (lab[nx][ny] == 'D') {
            bool acerto = desafioVetores();
            if (!acerto) {
                tentativas--;
                if (tentativas == 0) {
                    cout << "\nVocê perdeu todas as tentativas. FIM DE JOGO.\n";
                    Sleep(3000);
                    break;
                }
                continue;
            }
            lab[nx][ny] = ' ';
        }

        px = nx;
        py = ny;

        if (lab[px][py] == 'S') {
            cout << "\nPARABÉNS! Você chegou ao final!\n";
            Sleep(3000);
            break;
        }
    }

    return 0;
}
