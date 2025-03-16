#include "sudokuLib.h"
#include <iostream>
#include <iomanip>
#include <random>

#define startRedColor "\033[1;31m"
#define endRedColor "\033[0m"

namespace {
const int kDefaultFieldRows = 9;
const int kDefaultFieldColumns = 9;

const int kCellValuesRangeBegin = 0;
const int kCellValuesRangeEnd = 9;

int RandInt(int rangeBegin, int rangeEnd) {
    std::random_device r{};
    std::default_random_engine randomEngine(r());
    std::uniform_int_distribution distribution(rangeBegin, rangeEnd);

    return distribution(randomEngine);
}

bool UniqInBox(Cell** field, size_t row, size_t col, int element) {
    for (size_t i = 0; i < 3; ++i) {
        for (size_t j = 0; j < 3; ++j) {
            if (field[row + i][col + j].GetValue() == element) {
                return false;
            }
        }
    }
    return true;
}

void FillBox(Cell** field, size_t row, size_t col) {
    int value = 0;
    for (size_t i = 0; i < 3; ++i) {
        for (size_t j = 0; j < 3; ++j) {
            do {
                value = RandInt(kCellValuesRangeBegin, kCellValuesRangeEnd);
            } while (!UniqInBox(field, row, col, value));
            field[row + i][col + j].SetValue(value);
        }
    }
}

bool ElementUniq(Cell** field, size_t row, size_t col, int element) {
    for (size_t i = 0; i < kDefaultFieldRows; ++i) {
        if (field[row][i].GetValue() == element) {
            return false;
        }
    }

    for (size_t i = 0; i < kDefaultFieldColumns; ++i) {
        if (field[i][col].GetValue() == element) {
            return false;
        }
    }

    for (size_t i = row-row%3; i < row-row%3+3; ++i) {
        for (size_t j = col-col%3; j < col-col%3+3; ++j) {
            if (field[i][j].GetValue() == element) {
                return false;
            }
        }
    }

    return true;
}

bool FillOther(Cell** field, size_t i, size_t j) {
    if (i == 9) {
        return true;
    }

    if (j == 9) {
        return FillOther(field, i + 1, 0);
    }

    if (field[i][j].GetValue() != 0) {
        return FillOther(field, i, j + 1);
    }

    for (int value = 1; value <= 9; ++value) {
        if (ElementUniq(field, i, j, value)) {
            field[i][j].SetValue(value);
            if (FillOther(field, i, j + 1)) {
                return true;
            }
        }
        field[i][j].SetValue(0);
    }
    return false;
}

void PrintLine(int size, const char* symbol) {
    for (int i = 0; i < size; ++i) {
        std::cout << symbol;
    }
}
} // namespace

Cell::Cell(bool isVisible, int value) {
    if (value > kCellValuesRangeEnd || value < kCellValuesRangeBegin) {
        throw std::runtime_error("Error: invalid value!");
    }

    this->isVisible = isVisible;
    this->value = value;
}

Field::Field() {
    field = new Cell*[kDefaultFieldRows];
    for (size_t i = 0; i < kDefaultFieldRows; ++i) {
        field[i] = new Cell[kDefaultFieldColumns];
    }

    this->rows = kDefaultFieldRows;
    this->columns = kDefaultFieldColumns;
}

void Field::FillField() {
    for (size_t i = 0; i < 9; i += 3) {
        FillBox(field, i, i);
    }

    for (size_t i = 0; i < 9; ++i) {
        for (size_t j = 0; j < 9; ++j) {
            FillOther(field, i, j);
        }
    }
}

std::ostream& operator<<(std::ostream& out, const Field& source) {
    const char kCnnectorSymbol = '+';
    const char kSideSymbol = '|';
    const char kLineSymbol = '-';
    const char kEmpty = ' ';

    std::cout << startRedColor << kCnnectorSymbol << endRedColor;
    for (int i = 0; i < 9; ++i) {
        std::cout << startRedColor << std::setw(6) << std::setfill(kLineSymbol) << kCnnectorSymbol << endRedColor;
    }
    std::cout << '\n';

    for (size_t i = 0; i < source.rows; ++i) {
        std::cout << startRedColor << kSideSymbol << endRedColor;
        for (size_t j = 0; j < source.columns; ++j) {
            if (source.field[i][j].GetValue() != 0 && source.field[i][j].GetVisible() == true) {
                if ((j+1)%3 == 0) {
                    std::cout << "  " << source.field[i][j].GetValue() << "  " << startRedColor << kSideSymbol << endRedColor;
                } else {
                    std::cout << "  " << source.field[i][j].GetValue() << "  " << kSideSymbol;
                }
            } else {
                if ((j+1)%3 == 0) {
                    PrintLine(5, " ");
                    std::cout << startRedColor << kSideSymbol << endRedColor;
                } else {
                    std::cout << std::setw(6) << std::setfill(kEmpty) << kSideSymbol;
                }
            }
        }
        std::cout << '\n';

        std::cout << startRedColor << kCnnectorSymbol << endRedColor;
        for (int j = 0; j < 9; ++j) {
            if ((i+1)%3 == 0) {
                std::cout << startRedColor << std::setw(6) << std::setfill(kLineSymbol) << kCnnectorSymbol << endRedColor;
            } else {
                PrintLine(5, "-");
                if ((j+1)%3 == 0) {
                    std::cout << startRedColor << kCnnectorSymbol << endRedColor;
                } else {
                    std::cout << kCnnectorSymbol;
                }
            }
        }
        std::cout << '\n';
    }
    return out;
}

Field::~Field() {
    delete[] field;
    field = nullptr;
}
