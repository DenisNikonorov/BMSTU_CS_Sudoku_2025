#include <iostream>
#include "menuWindow.hpp"

Button::Button() {
    this->xPosition = 0;
    this->yPosition = 0;
    this->width = 0;
    this->height = 0;
    this->toggled = false;
    this->clickable = false;
}

Button::Button(int x, int y, int w, int h, std::string label) {
    this->xPosition = x;
    this->yPosition = y;
    this->width = w;
    this->height = h;
    this->label = label;
    this->toggled = false;
    this->clickable = false;
}

Button::Button(const Button& source) {
    this->xPosition = source.xPosition;
    this->yPosition = source.yPosition;
    this->width = source.width;
    this->height = source.height;
    this->label = source.label;
    this->toggled = source.toggled;
    this->clickable = source.clickable;
}

sf::Vector2i Button::getSize() {
    sf::Vector2i size(width, height);
    return size;
}

void Button::setSize(sf::Vector2i size) {
    this->width = size.x;
    this->height = size.y;
}

void Button::setPosition(int x, int y) {
    this->xPosition = x;
    this->yPosition = y;
}

void Button::Toggle(sf::Vector2i clickPosition) {
    if (clickPosition.x > xPosition && clickPosition.x < xPosition + width && clickPosition.y > yPosition && clickPosition.y < yPosition + height && clickable) {
        this->toggled = !toggled;
    }
}

void Button::Press(sf::Event event, sf::Vector2i clickPosition) {
    if (clickPosition.x > this->xPosition && clickPosition.x < this->xPosition + this->width && clickPosition.y > this->yPosition && clickPosition.y < this->yPosition + this->height && clickable) {
        if (event.type == sf::Event::MouseButtonPressed) {
            this->toggled = true;
        }
    }
    if (event.type == sf::Event::MouseButtonReleased) {
        this->toggled = false;
    }
}

void Button::DrawButton(sf::RenderWindow& window, sf::Font& font) {
    sf::Color toggledColor(250, 0, 0);

    sf::RectangleShape frame(sf::Vector2f(width, height));
    sf::Color buttonColor = toggled ? toggledColor : sf::Color::Blue;
    frame.setFillColor(buttonColor);
    frame.setPosition(xPosition, yPosition);

    sf::Text text;
    text.setFont(font);
    text.setString(this->label);
    text.setCharacterSize(40);
    text.setFillColor(sf::Color::White);
    text.setPosition((xPosition + width / 2 - label.length()*10), (height / 2 + yPosition - 25));

    window.draw(frame);
    window.draw(text);
}

namespace {
    const int kEasyCellsInvisible = 20;
    const int kMediumCellsInvisible = 40;
    const int kHardCellsInvisible = 50;

    enum class Pages : int {
        MainMenu = 1,
        SelectDifficulty,
    };
}

namespace MenuWindow {
void RunMenuWindow(Field& f, int& invisibleCellsCount, bool& nextWindow, bool& isRunning) {
    sf::RenderWindow window(sf::VideoMode(600, 600), "window");
    sf::Font font;
    if (!font.loadFromFile("../fonts/Ubuntu-B.ttf")) {
        std::cerr << "Error loading font!\n";
    }

    sf::Text mainText;
    mainText.setFont(font);
    mainText.setCharacterSize(50);
    mainText.setString("SUDOKU");
    mainText.setPosition(210, 110);
    mainText.setFillColor(sf::Color::Black);

    sf::Text selectDifficultyText;
    selectDifficultyText.setFont(font);
    selectDifficultyText.setCharacterSize(50);
    selectDifficultyText.setString("Difficulty");
    selectDifficultyText.setPosition(210, 110);
    selectDifficultyText.setFillColor(sf::Color::Black);

    // Buttons
    Pages page = Pages::MainMenu;
    Button startGameButton(150, 200, 300, 70, "Start");
    Button contnueGameButton(150, 280, 300, 70, "Continue");
    Button leaveGameButton(150, 360, 300, 70, "Leave");

    Button easyDifficulty(150, 200, 300, 70, "EASY");
    Button mediumDifficulty(150, 280, 300, 70, "MEDIUM");
    Button hardDifficulty(150, 360, 300 , 70, "HARD");
    Button returnButton(200, 440, 200, 70, "return");
    // Buttons

    while (window.isOpen()) {
        window.clear(sf::Color::White);
        sf::Event event;

        sf::Vector2i clickPosition = sf::Mouse::getPosition();
        sf::Vector2i windowPosition = window.getPosition();

        sf::Vector2i currentMousePosition;
        currentMousePosition.x = clickPosition.x - windowPosition.x - 38;
        currentMousePosition.y = clickPosition.y - windowPosition.y - 59;

        while (window.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed :{
                    isRunning = false;
                    window.close();
                    break;
                }
            }
            startGameButton.Press(event, currentMousePosition);
            contnueGameButton.Press(event, currentMousePosition);
            leaveGameButton.Press(event, currentMousePosition);

            easyDifficulty.Press(event, currentMousePosition);
            mediumDifficulty.Press(event, currentMousePosition);
            hardDifficulty.Press(event, currentMousePosition);
            returnButton.Press(event, currentMousePosition);
        }

        if (page == Pages::MainMenu) {
            startGameButton.setClickable(true);
            contnueGameButton.setClickable(true); // if field is not empty
            leaveGameButton.setClickable(true);

            easyDifficulty.setClickable(false);
            mediumDifficulty.setClickable(false);
            hardDifficulty.setClickable(false);
            returnButton.setClickable(false);

            if (startGameButton.IsClicked()) {
                page = Pages::SelectDifficulty;
            } else if (contnueGameButton.IsClicked()) {
                //read data from file
                //В ПРОЦЕССЕ ВЫПОЛНЕНИЯ ПРОГРАММЫ ФАЙЛ ДОЛЖЕН ОБНОВЛЯТЬСЯ
                //В НАЧАЛЕ СОХРАНИТЬ ИЗНАЧАЛЬНОЕ СОСТОЯНИЕ ПОЛЯ В initialField!!!!!
                // (это все есть на гите) :)
            } else if (leaveGameButton.IsClicked()) {
                nextWindow = false;
                isRunning = false;
                window.close();
            }

            window.draw(mainText);
            startGameButton.DrawButton(window, font);
            contnueGameButton.DrawButton(window, font);
            leaveGameButton.DrawButton(window, font);
        } else if (page == Pages::SelectDifficulty) {
            startGameButton.setClickable(false);
            contnueGameButton.setClickable(false);
            leaveGameButton.setClickable(false);

            easyDifficulty.setClickable(true);
            mediumDifficulty.setClickable(true);
            hardDifficulty.setClickable(true);
            returnButton.setClickable(true);

            if (easyDifficulty.IsClicked()) {
                invisibleCellsCount = kEasyCellsInvisible;
                window.close();
            } else if (mediumDifficulty.IsClicked()) {
                invisibleCellsCount = kMediumCellsInvisible;
                window.close();
            } else if (hardDifficulty.IsClicked()) {
                invisibleCellsCount = kHardCellsInvisible;
                window.close();
            } else if (returnButton.IsClicked()) {
                page = Pages::MainMenu;
            }

            window.draw(selectDifficultyText);
            easyDifficulty.DrawButton(window, font);
            mediumDifficulty.DrawButton(window, font);
            hardDifficulty.DrawButton(window, font);
            returnButton.DrawButton(window, font);
        }

        window.display();
    }
}
} //MenuWindow
