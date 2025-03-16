#include "game.h"
#include <iostream>

namespace {
const int kEasyDifficultyInvisibleCells = 30;
const int kMediumDifficultyInvisibleCells = 50;
const int kHardDifficultyInvisibleCells = 60;

void PrintMainMenuItems() {
    std::cout << "1. Новая игра\n2. Продолжить\n3. Выйти\n";
}

void PrintDifficulty() {
    std::cout << "Выберите сложность:\n  1. Легкая\n  2. Средняя\n  3. Сложная\n";
}
} // namespace

namespace Game {
void SelectDifficulty(Field& field) {
    int select = 0;
    PrintDifficulty();
    std::cout << "Ваш выбор: ";
    std::cin >> select;

    switch (static_cast<Difficulty>(select)) {
        case Difficulty::Easy: {
            field.MakeNCellsInvisible(kEasyDifficultyInvisibleCells);
            break;
        }
        case Difficulty::Medium: {
            field.MakeNCellsInvisible(kMediumDifficultyInvisibleCells);
            break;
        }
        case Difficulty::Hard: {
            field.MakeNCellsInvisible(kHardDifficultyInvisibleCells);
            break;
        }
        default:{
            std::cout << "Неверный формат ввода выбора сложности. Попробуйте ввести цифру 1-3.";
            break;
        }
    }
}

void SelectMenuItem(Field& field, bool& isRunning) {
    int select = 0;
    PrintMainMenuItems();
    std::cout << "Ваш выбор: ";
    std::cin >> select;
    switch (static_cast<MainMenu>(select)) {
        case MainMenu::NewGame: {
            field.FillField();
            SelectDifficulty(field);
            break;
        }
        case MainMenu::ContinueGame: {
            //вызов фунции ContinueGame
            break;
        }
        case MainMenu::Exit: {
            isRunning = false;
            break;
        }
        default:
            std::cout << "Неверный формат ввода выбора пункта меню. Попробуйте ввести цифру 1-3.";
            break;
    }
}

void RunGame() {
    bool isRunning = true;
    Field field;
    while (isRunning == true) {
        SelectMenuItem(field, isRunning);
    }
    field.~Field();
}
} // namespace Game
