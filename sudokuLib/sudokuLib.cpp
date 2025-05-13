#include <iostream>
#include <random>
#include "sudokuLib.hpp"

namespace {
const double kDefaultCellSide = 60.;

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
                value = RandInt(1, 9);
            } while (!UniqInBox(field, row, col, value));
            field[row + i][col + j].SetValue(value);
            field[row + i][col + j].SetPlaced(true);
        }
    }
}

bool ElementUniq(Cell** field, size_t row, size_t col, int element) {
    for (int i = 0; i < 9; ++i) {
        if (field[row][i].GetValue() == element) {
            return false;
        }
    }

    for (int i = 0; i < 9; ++i) {
        if (field[i][col].GetValue() == element) {
            return false;
        }
    }

    for (int i = row-row%3; i < row-row%3+3; ++i) {
        for (int j = col-col%3; j < col-col%3+3; ++j) {
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
            field[i][j].SetPlaced(true);
            if (FillOther(field, i, j + 1)) {
                return true;
            }
        }
        field[i][j].SetValue(0);
    }
    return false;
}
} // namespace

// class Cell functions
Cell::Cell(double xPosition, double yPosition, double width, double height) {
    this->xPosition = xPosition;
    this->yPosition = yPosition;
    this->width = kDefaultCellSide;
    this->height = kDefaultCellSide;
    this->onClick = false;
    this->currentValue = 0;
    this->textColor = sf::Color::Black;
    this->isPlaced = false;
}

bool Cell::IsClickInCell(sf::Vector2i clickPosition) {
    return (clickPosition.x > this->xPosition && clickPosition.x < this->xPosition + this->width && clickPosition.y > this->yPosition && clickPosition.y < this->yPosition + this->height);
}

void Cell::Toggle() {
    this->onClick = !onClick;
}

void Cell::PlaceValue(int value, int& errorsCount) {
    if (value != currentValue) {
        textColor = sf::Color::Red;
        this->visibleValue = value;
        this->isPlaced = false;
        errorsCount++;
    } else if (value == currentValue) {
        textColor = sf::Color::Black;
        this->visibleValue = this->currentValue;
        this->isPlaced = true;
    }
    this->isVisible = true;
}

void Cell::Draw(sf::RenderWindow& window, int startX, int startY, sf::Vector2i mousePosition, sf::Font& font) {
    sf::RectangleShape cell;
    cell.setPosition(xPosition + startX, yPosition + startY);
    cell.setSize(sf::Vector2f(width - 5, height - 5));

    sf::Color clickedColor(190, 190, 190);
    if (onClick) {
        cell.setFillColor(clickedColor);
    } else {
        cell.setFillColor(sf::Color::White);
    }

    sf::Text cellValue;
    cellValue.setFont(font);
    cellValue.setCharacterSize(24);

    if (isPlaced) {
        visibleValue = currentValue;
    }
    cellValue.setString(std::to_string(visibleValue));

    cellValue.setFillColor(textColor);
    cellValue.setPosition(xPosition + startX + width / 4 + 5, yPosition + startY + height / 4);

    window.draw(cell);
    if (isVisible) {
        window.draw(cellValue);
    }
}
// class Cell functions

Field::Field() {
    field = new Cell*[9];
    for (size_t i = 0; i < 9; ++i) {
        field[i] = new Cell[9];
    }

    this->rows = 9;
    this->columns = 9;
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

bool Field::IsFull() {
    for (size_t i = 0; i < 9; ++i) {
        for (size_t j = 0; j < 9; ++j) {
            if (field[i][j].GetVisible() == false) {
                return false;
            }
        }
    }
    return true;
}

bool Field::IsEmpty() {
    for (size_t i = 0; i < 9; ++i) {
        for (size_t j = 0; j < 9; ++j) {
            if (field[i][j].GetValue() != 0) {
                return false;
            }
        }
    }
    return true;
}

void Field::MakeNCellsInvisible(int invisibleCellsCount) {
    int currentInvisibleCellsCount = 0;
    while (currentInvisibleCellsCount < invisibleCellsCount){
        size_t row = RandInt(0, 8);
        size_t col = RandInt(0, 8);

        if (field[row][col].GetVisible() != false) {
            field[row][col].SetVisible(false);
            currentInvisibleCellsCount++;
        }
    }
}

Field::~Field() {
    delete[] field;
    field = nullptr;
}
