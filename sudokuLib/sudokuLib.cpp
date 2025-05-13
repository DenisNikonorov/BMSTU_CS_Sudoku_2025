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

//class Field functions
Field::Field() {
    this->xFieldPosition = 0;
    this->yFieldPosition = 0;
    this->isClickable = true;
    CreateField();
}

Field::Field(int fieldX, int fieldY) {
    this->xFieldPosition = fieldX;
    this->yFieldPosition = fieldY;
    CreateField();
}

Field::~Field() {
    for (size_t i = 0; i < 9; ++i) {
        delete[] field[i];
    }
    delete[] field;
}

void Field::FillField() {
    for (size_t i = 0; i < 9; i += 3) {
        FillBox(field, i, i);
    }

    for (size_t i = 0; i < 9; ++i) {
        for (size_t j = 0; j < 9; ++j) {
            FillOther(field, i, j);
            field[i][j].SetVisible(true);
        }
    }
}

void Field::MakeNCellsInvisible(int count) {
    int currentInvisibleCellsCount = 0;
    while (currentInvisibleCellsCount < count) {
        size_t row = RandInt(0, 8);
        size_t column = RandInt(0, 8);

        if (field[row][column].GetVisible() != false) {
            field[row][column].SetVisible(false);
            field[row][column].SetPlaced(false);
            currentInvisibleCellsCount++;
        }
    }
}

void Field::DrawField(sf::RenderWindow& window, int xFieldPosition, int yFieldPosition, sf::Vector2i mousePosition, sf::Font& font, int errorsCount) {
    this->xFieldPosition = xFieldPosition;
    this->yFieldPosition = yFieldPosition;

    for (size_t i = 0; i < 9 ; ++i) {
        for (size_t j = 0; j < 9; ++j) {
            field[i][j].Draw(window, xFieldPosition, yFieldPosition, mousePosition, font);
        }
    }

    sf::Color smallFrameColor(255, 190, 200);
    sf::Color bigFrameColor(65, 170, 255);
    sf::RectangleShape* horizontalSmallFrame = new sf::RectangleShape[8];
    for (size_t i = 0; i < 8; ++i) {
        horizontalSmallFrame[i].setFillColor(smallFrameColor);
        horizontalSmallFrame[i].setSize(sf::Vector2f(535, 5));
        horizontalSmallFrame[i].setPosition(xFieldPosition, 60*i + yFieldPosition + 55);
        window.draw(horizontalSmallFrame[i]);
    }
    delete[] horizontalSmallFrame;

    sf::RectangleShape* verticalSmallFrame = new sf::RectangleShape[8];
    for (size_t i = 0; i < 8; ++i) {
        verticalSmallFrame[i].setFillColor(smallFrameColor);
        verticalSmallFrame[i].setSize(sf::Vector2f(5, 535));
        verticalSmallFrame[i].setPosition(60*i + xFieldPosition + 55, yFieldPosition);
        window.draw(verticalSmallFrame[i]);
    }
    delete[] verticalSmallFrame;

    sf::RectangleShape* horizontalBigFrame = new sf::RectangleShape[4];
    for (size_t i = 0; i < 4; ++i) {
        horizontalBigFrame[i].setFillColor(bigFrameColor);
        horizontalBigFrame[i].setSize(sf::Vector2f(545, 5));
        horizontalBigFrame[i].setPosition(xFieldPosition - 5, 180*i + yFieldPosition - 5);
        window.draw(horizontalBigFrame[i]);
    }
    delete[] horizontalBigFrame;

    sf::RectangleShape* verticalBigFrame = new sf::RectangleShape[4];
    for (size_t i = 0; i < 4; ++i) {
        verticalBigFrame[i].setFillColor(bigFrameColor);
        verticalBigFrame[i].setSize(sf::Vector2f(5, 545));
        verticalBigFrame[i].setPosition(180*i + xFieldPosition - 5, yFieldPosition - 5);
        window.draw(verticalBigFrame[i]);
    }
    delete[] verticalBigFrame;

    sf::Text errors;
    errors.setFont(font);
    errors.setString("Errors: " + std::to_string(errorsCount));
    errors.setCharacterSize(24);
    errors.setFillColor(sf::Color::Black);
    errors.setPosition(10, 10);
    window.draw(errors);
}

bool Field::IsFilled() {
    for (size_t i = 0; i < 9; ++i) {
        for (size_t j = 0; j < 9; ++j) {
            if (field[i][j].GetPlaced() == false) {
                return false;
            }
        }
    }
    return true;
}

sf::Vector2i Field::getPosition() {
    sf::Vector2i position(xFieldPosition, yFieldPosition);
    return position;
}

void Field::MakeEmpty() {
    for (size_t i = 0; i < 9; ++i) {
        for (size_t j = 0; j < 9; ++j) {
            this->field[i][j].SetValue(0);
            this->field[i][j].SetVisible(false);
            this->field[i][j].SetPlaced(false);
            this->isClickable = true;
            this->field[i][j].SetColor(sf::Color::Black);
            if (field[i][j].IsClicked()) {
                field[i][j].SetClicked(false);
            }
        }
    }
}
// class Field functions

namespace CellFunctions {
void ToggleCell(Field& f, sf::Vector2i mousePosition) {
    Cell** field = f.GetField();

    sf::Vector2i fieldPosition = f.getPosition();
    mousePosition.x = mousePosition.x - fieldPosition.x;
    mousePosition.y = mousePosition.y - fieldPosition.y;

    int cellI = 0;
    int cellJ = 0;
    for (size_t i = 0; i < 9; ++i) {
        for (size_t j = 0; j < 9; ++j) {
            if (field[i][j].IsClickInCell(mousePosition)) {
                field[i][j].Toggle();
                cellI = i;
                cellJ = j;
            } else if (field[i][j].IsClicked() && !field[i][j].IsClickInCell(mousePosition)) {
                field[i][j].Toggle();
            }
        }
    }
}
} // CellFunctions
