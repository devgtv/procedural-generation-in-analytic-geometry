#include <windows.h>
#include "consoleutils.h"

// limpa a tela
void LimparTela()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coordScreen = {0, 0};
    DWORD cCharsWritten;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD dwConSize;
    
    
    // PEGA o tamanho do buffer da tela
    if (!GetConsoleScreenBufferInfo(hConsole, &csbi))
        return;
    
    dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
    
    // Preencher toda a tela com espaços
    if (!FillConsoleOutputCharacter(hConsole, (TCHAR)' ', dwConSize, coordScreen, &cCharsWritten))
        return;
    
    // Preencher com os atributos atuais
    if (!FillConsoleOutputAttribute(hConsole, csbi.wAttributes, dwConSize, coordScreen, &cCharsWritten))
        return;
    
    // Mover o cursor para o topo
    SetConsoleCursorPosition(hConsole, coordScreen);
}

