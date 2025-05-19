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
    Field initialField;

    while (isRunning) {
        int errorsCount = 0;
        bool isContinueButtonAvalible = false;
        bool isNewGame = false;

        FieldData::GetDataFromFile(initialField, "fieldData.txt", errorsCount);

        MenuWindow::RunMenuWindow(field, invisibleCellsCount, nextWindow, isRunning, isContinueButtonAvalible, isNewGame, errorsCount);
        if (isNewGame) {
            field.MakeEmpty();
            field.FillField();
            field.MakeNCellsInvisible(invisibleCellsCount);
            errorsCount = 0;
            FieldData::WriteData(field, "fieldData.txt", 0);
            initialField = field;
        }
        if (nextWindow && isRunning) {
            Window::RunGameWindow(field, initialField, isRunning, errorsCount);
        }
    }
}

int main() {
    RunGame();
    return 0;
}
