#include "game.hpp"
#include <iostream>
#include <fstream>
#include <cstring>

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

void CodeFill(Field& f, char* code, char* mask) {
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

void EmptyField(Field& f) {
    Cell** field = f.GetField();

    for (size_t i = 0; i < 9; ++i) {
        for (size_t j = 0; j < 9; ++j) {
            field[i][j].SetValue(0);
            field[i][j].SetVisible(true);
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

void WriteData(Field& f, const char* fileName, int errorsCount) {
    std::ofstream fout(fileName);

    if (!fout.is_open()) {
        throw std::runtime_error("Error opening file");
    }

    char* code = new char[81];
    char* mask = new char[81];

    Cell** field = f.GetField();

    int symbolIndex = 0;
    for (size_t i = 0; i < 9; ++i) {
        for (size_t j = 0; j < 9; ++j) {
            char buffer = field[i][j].GetValue();
            code[symbolIndex] = buffer + 48;

            if (field[i][j].GetVisible() == false) {
                mask[symbolIndex] = '0';
            } else {
                mask[symbolIndex] = '1';
            }

            symbolIndex++;
        }
    }

    fout << code << '\n' << mask << '\n' << errorsCount;

    delete[] code;
    delete[] mask;
    fout.close();
}

void PlaceNumbers(Field& f, int& errorsCount) {
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
        WriteData(f, "Data.txt", errorsCount);
    }
}

void Play(Field& field) {
    int errorsCount = 0;
    WriteData(field, "Data.txt", errorsCount);

    Field initialField;
    GetDataFromFile(initialField, "Data.txt");

    PlaceNumbers(field, errorsCount);
    if (field.IsFull()) {
        std::cout << field;
        std::cout << "УРА ПОБЕДА ЮХУ!!!!!\n";
        EmptyField(field);
        WriteData(field, "Data.txt", 0);
    } else {
        std::cout << "Нам тебя искренне жаль, попробуй еще раз :(\n";
        WriteData(initialField, "Data.txt", 0);
    }
}

void SelectDifficulty(Field& field) {
    int select = 0;
    PrintDifficulty();
    std::cout << "Ваш выбор: ";
    std::cin >> select;
    field.FillField();

    switch (static_cast<Difficulty>(select)) {
        case Difficulty::Easy: {
            field.MakeNCellsInvisible(kEasyDifficultyInvisibleCells);
            Play(field);
            break;
        }
        case Difficulty::Medium: {
            field.MakeNCellsInvisible(kMediumDifficultyInvisibleCells);
            Play(field);
            break;
        }
        case Difficulty::Hard: {
            field.MakeNCellsInvisible(kHardDifficultyInvisibleCells);
            Play(field);
            break;
        }
        default: {
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
            EmptyField(field);
            SelectDifficulty(field);
            break;
        }
        case MainMenu::ContinueGame: {
            GetDataFromFile(field, "Data.txt");
            if (field.IsEmpty()) {
                std::cout << "Продолжать нечего, поле пустое! Начните новую игру!\n";
                EmptyField(field);
                WriteData(field, "Data.txt", 0);
                break;
            } else {
                int errorsCount = 0;
                Play(field);
            }
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

        while (isRunning == true) {
            SelectMenuItem(field, isRunning);
        }

        field.~Field();
    } catch (std::runtime_error& error) {
        std::cerr << error.what() << '\n';
    }
}
} // namespace Game
