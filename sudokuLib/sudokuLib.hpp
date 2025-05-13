#pragma once
#include <SFML/Graphics.hpp>

class Cell {
  protected:
    int currentValue{};
    int visibleValue{};
    bool isVisible{};
    bool isPlaced{};

    bool onClick{};
    double xPosition{};
    double yPosition{};
    double width{};
    double height{};
    sf::Color textColor = sf::Color::Black;

  public:
    Cell() = default;
    Cell(double xPosition, double yPosition, double width, double height);

    int GetValue() { return this->currentValue; }
    void SetValue(int value) { this->currentValue = value; }
    bool GetVisible() { return this->isVisible; }
    void SetVisible(bool visible) { this->isVisible = visible; }
    void SetPlaced(bool placed) { this->isPlaced = placed; }
    bool GetPlaced() { return isPlaced; }
    void SetClicked(bool clicked) { this->onClick = clicked; }
    void SetColor(sf::Color color) { this->textColor = color; }

    bool IsClickInCell(sf::Vector2i clickPosition);
    bool IsClicked() { return this->onClick; }
    void Toggle();
    void PlaceValue(int value, int& errorsCount);
    void Draw(sf::RenderWindow& window, int startX, int startY, sf::Vector2i mousePosition, sf::Font& font);
};

class Field : protected Cell {
  private:
    Cell** field{};
    int xFieldPosition{};
    int yFieldPosition{};
    bool isClickable{};

    void CreateField() {
        field = new Cell*[9];
        for (size_t i = 0; i < 9; ++i) {
            field[i] = new Cell[9];
            for (size_t j = 0; j < 9; ++j) {
              Cell cell(60*j + xPosition, 60*i + yPosition, 60, 60);
              field[i][j] = cell;
            }
        }
    }

  public:
    Field();
    Field(int fieldX, int fieldY);
    ~Field();

    Cell** GetField() { return field; }
    void FillField();
    void MakeNCellsInvisible(int count);
    void DrawField(sf::RenderWindow& window, int xFieldPosition, int yFieldPosition, sf::Vector2i mousePosition, sf::Font& font, int errorsCount);
    bool IsFilled();
    void ToggleClickable(bool clickable) { this->isClickable = clickable; }
    bool GetClickable() { return this->isClickable; }
    void MakeEmpty();

    sf::Vector2i getPosition();
};

namespace CellFunctions {
    void ToggleCell(Field& f, sf::Vector2i mousePosition); //Cell** field, sf::Vector2i mousePosition
} // CellFunctions
