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

class Field {
    private:
    Cell** field = nullptr;
    size_t rows = 9;
    size_t columns = 9;

    public:
    Field();

    Cell** GetField() { return field; }
    void FillField();
    bool IsFull();
    bool IsEmpty();
    void MakeNCellsInvisible(int);
    friend std::ostream& operator<<(std::ostream& out, const Field& source);

    ~Field();
};
