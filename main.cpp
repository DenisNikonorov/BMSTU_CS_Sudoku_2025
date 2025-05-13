#include <iostream>
#include <SFML/Graphics.hpp>
#include "sudokuLib/sudokuLib.hpp"
#include "window/window.hpp"
#include "menuWindow/menuWindow.hpp"

void RunGame() {
    int invisibleCellsCount = 0;
    bool nextWindow = true;
    bool isRunning = true;
    Field field;

    while (isRunning) {
        field.FillField();
        MenuWindow::RunMenuWindow(field, invisibleCellsCount, nextWindow, isRunning);
        field.MakeNCellsInvisible(invisibleCellsCount);
        if (nextWindow && isRunning) {
            Window::RunGameWindow(field, isRunning);
        }
        field.MakeEmpty();
    }
}

int main() {
    RunGame();
    return 0;
}
