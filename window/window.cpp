#include <iostream>
#include "window.hpp"
#include "../menuWindow/menuWindow.hpp"

namespace {
bool IsCorrect(char input) {
    const char kOneChar = '1';
    const char kNineChar = '9';

    return (input >= kOneChar && input <= kNineChar);
}
} // namespace

namespace Window {
void RunGameWindow(Field& f, bool& isRunning) {
    sf::RenderWindow window(sf::VideoMode(600, 650), "window");
    sf::Font font;
    if (!font.loadFromFile("../fonts/Ubuntu-B.ttf")) {
        std::cerr << "Error loading font!" << '\n';
    }

    Cell** field = f.GetField();

    bool winGame = false;
    int errorsCount = 0;

    while (window.isOpen()) {
        sf::Event event;

        sf::Vector2i currentMousePosition;
        sf::Vector2i mousePosition = sf::Mouse::getPosition();
        sf::Vector2i widnowPosition = window.getPosition();

        currentMousePosition.x = mousePosition.x - widnowPosition.x - 38;
        currentMousePosition.y = mousePosition.y - widnowPosition.y - 59;

        while (window.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed :{
                    isRunning = false;
                    window.close();
                    break;
                }
                case sf::Event::MouseButtonPressed :{
                    if (f.GetClickable()) {
                        sf::Vector2i clickPosition = sf::Mouse::getPosition();
                        CellFunctions::ToggleCell(f, currentMousePosition);
                    }
                    break;
                }
                case sf::Event::TextEntered :{
                    char input{};
                    if (f.GetClickable()) {
                        for (size_t i = 0; i < 9; ++i) {
                            for (size_t j = 0; j < 9; ++j) {
                                if (field[i][j].IsClicked() && !field[i][j].GetPlaced()) {
                                    input = event.text.unicode;
                                    if (IsCorrect(input)) {
                                        field[i][j].PlaceValue(input - 48, errorsCount);
                                    }
                                }
                            }
                        }
                    }
                    break;
                }
            }
        }

        window.clear(sf::Color::White);
        if (!f.IsFilled()) {
            f.DrawField(window, 30, 60, currentMousePosition, font, errorsCount);
        }

        if (f.IsFilled() && errorsCount < 3) {
            f.ToggleClickable(false);

            Button newGameButton(150, 270, 300, 70, "New game");
            newGameButton.setClickable(true);
            newGameButton.Press(event, currentMousePosition);

            sf::Text win;
            win.setFont(font);
            win.setCharacterSize(100);
            sf::Color winColor(0, 0, 139);
            win.setFillColor(winColor);
            win.setPosition(widnowPosition.x / 2 - 180, widnowPosition.y / 2 + 24);
            win.setString("You win!");

            if (newGameButton.IsClicked()) {
                window.close();
            }

            newGameButton.DrawButton(window, font);
            window.draw(win);
        } else if (errorsCount >= 3) {
            f.ToggleClickable(false);

            Button tryAgainButton(150, 280, 300, 70, "try again");
            tryAgainButton.setClickable(true);
            tryAgainButton.Press(event, currentMousePosition);

            if (tryAgainButton.IsClicked()) {
                window.close();
            }

            sf::Text lose;
            lose.setFont(font);
            lose.setCharacterSize(100);
            lose.setFillColor(sf::Color::Red);
            lose.setPosition(widnowPosition.x / 2 - 210, widnowPosition.y / 2 + 24);
            lose.setString("You lose!");
            window.clear(sf::Color::White);

            tryAgainButton.DrawButton(window, font);
            window.draw(lose);
        }
        window.display();
    }

}
} // Window
