#pragma once
#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>
#include "../sudokuLib/sudokuLib.hpp"
#include "../window/window.hpp"

class Button {
private:
    int xPosition{};
    int yPosition{};
    int width{};
    int height{};
    std::string label;
    bool toggled{};
    bool clickable{};

public:
    Button();
    Button(int x, int y, int w, int h, std::string label);
    Button(const Button& source);

    sf::Vector2i getSize();
    void setSize(sf::Vector2i size);
    void setPosition(int x, int y);
    void setLabel(std::string label) { this->label = label; }
    void setClickable(bool clickable) { this->clickable = clickable; }

    void Toggle(sf::Vector2i clickPosition);
    void Press(sf::Event event, sf::Vector2i clickPosition);
    bool IsClicked() { return this->toggled; }
    void DrawButton(sf::RenderWindow& window, sf::Font& font);
};

namespace MenuWindow {
    void RunMenuWindow(Field& f, int& invisibleCellsCount, bool& nextWindow, bool& isRunning);
}
