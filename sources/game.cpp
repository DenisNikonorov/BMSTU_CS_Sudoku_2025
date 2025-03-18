#include "game.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>

namespace {
const int kEasyDifficultyInvisibleCells = 2;
const int kMediumDifficultyInvisibleCells = 50;
const int kHardDifficultyInvisibleCells = 60;

void PrintMainMenuItems() {
    std::cout << "1. Новая игра\n2. Продолжить\n3. Выйти\n";
}

void PrintDifficulty() {
    std::cout << "Выберите сложность:\n  1. Легкая\n  2. Средняя\n  3. Сложная\n";
}

bool CodeIsNull(char* code) {
    int nullCellCount = 0;
    for (size_t i = 0; i < std::strlen(code); ++i) {
        if (code[i] == '0') {
            nullCellCount++;
        }
    }
    if (nullCellCount == 80) {
        return true;
    }

    return false;
}

void CodeFill(Field& f, char* code, char* mask) {
    // char a = '2'; -48
    Cell** field = f.GetField();

    size_t codeSymbolIndex = 0;
    for (size_t i = 0; i < 9; ++i) {
        for (size_t j = 0; j < 9; ++j) {
            field[i][j].SetValue(code[codeSymbolIndex] - 48);
            if (mask[codeSymbolIndex] == '0') {
                field[i][j].SetVisible(false);
            }
            codeSymbolIndex++;
        }
    }
}
} // namespace

namespace Game {
void GetDataFromFile(Field& field, const char* fileName) {
    std::ifstream fin(fileName);

    if (!fin.is_open()) {
        throw std::runtime_error("Error opening file");
    }

    char codeBuffer[82];
    char maskBuffer[82];

    fin >> codeBuffer >> maskBuffer;
    CodeFill(field, codeBuffer, maskBuffer);

    fin.close();
}

void Play(Field& f, int& errorsCount) {
    Cell** field = f.GetField();
    size_t currentRow = 0;
    size_t currentColumn = 0;
    int guessValue = 0;

    while (!f.IsFull() && errorsCount < 3) {
        std::cout << f;
        std::cout << "Введите строку, столбец и цифру: ";

        std::cin >> currentRow >> currentColumn >> guessValue;

        if (currentRow < 0 || currentRow > 9) {
            throw std::runtime_error("Error: invalid row");
        }

        if (currentColumn < 0 || currentColumn > 9) {
            throw std::runtime_error("Error: invalid column");
        }

        if (guessValue < 0 || guessValue > 9) {
            throw std::runtime_error("Error: invalid value");
        }

        if (guessValue == field[currentRow][currentColumn].GetValue()) {
            field[currentRow][currentColumn].SetVisible(true);
        } else {
            ++errorsCount;
            std::cout << "Ошибка, количество ошибок: " << errorsCount << '\n';
        }
    }
}

void SelectDifficulty(Field& field) {
    int select = 0;
    PrintDifficulty();
    std::cout << "Ваш выбор: ";
    std::cin >> select;
    int errorsCount = 0;

    switch (static_cast<Difficulty>(select)) {
        case Difficulty::Easy: {
            field.MakeNCellsInvisible(kEasyDifficultyInvisibleCells);
            Play(field, errorsCount);
            if (field.IsFull()) {
                std::cout << field;
                std::cout << "УРА ПОБЕДА ЮХУ!!!!!\n";
            } else {
                std::cout << "Нам тебя искренне жаль, попробуй еще раз :(\n";
            }
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
            std::cout << "Неверный формат ввода выбора сложности. Попробуйте ввести цифру 1-3.\n";
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
            std::cout << "Неверный формат ввода выбора пункта меню. Попробуйте ввести цифру 1-3.\n";
            break;
    }
}

void RunGame() {
    bool isRunning = true;
    try {
        Field field;

        GetDataFromFile(field, "Data.txt");
        std::cout << field;

        // while (isRunning == true) {
        //     SelectMenuItem(field, isRunning);
        // }
        field.~Field();
    } catch (std::runtime_error& error) {
        std::cerr << error.what() << '\n';
    }
}
} // namespace Game
