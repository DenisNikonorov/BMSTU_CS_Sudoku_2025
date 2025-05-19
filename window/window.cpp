#include <iostream>
#include "window.hpp"
#include "../menuWindow/menuWindow.hpp"

namespace {
bool IsCorrect(char input) {
    const char kOneChar = '1';
    const char kNineChar = '9';

    return (input >= kOneChar && input <= kNineChar);
}

enum class GameStatus : int {
    Win = 1,
    Lose,
    Play,
};
} // namespace

namespace Window {
void RunGameWindow(Field& f, Field& initialField, bool& isRunning, int& errorsCount) {
    sf::RenderWindow window(sf::VideoMode(600, 650), "sudoku");
    sf::Font font;
    if (!font.loadFromFile("../fonts/Ubuntu-B.ttf")) {
        std::cerr << "Error loading font!" << '\n';
    }

    Cell** field = f.GetField();

    GameStatus gameStatus;

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
                    if (gameStatus == GameStatus::Lose) {
                        FieldData::WriteData(f, "fieldData.txt", errorsCount);
                    } else if (gameStatus == GameStatus::Win) {
                        f.MakeEmpty();
                        FieldData::WriteData(f, "fieldData.txt", 0);
                    }
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
                                    if (!field[i][j].GetPlaced()) {
                                        field[i][j].SetVisible(false);
                                        FieldData::WriteData(f, "fieldData.txt", errorsCount);
                                        if (IsCorrect(input)) {
                                            field[i][j].SetVisible(true);
                                        }
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
            gameStatus = GameStatus::Win;

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
            gameStatus = GameStatus::Lose;

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
        } else if (errorsCount < 3 && !f.IsFilled()) {
            gameStatus = GameStatus::Play;
        }
        window.display();
    }

    if (gameStatus == GameStatus::Win) {
       f.MakeEmpty();
       FieldData::WriteData(f, "fieldData.txt", 0);
    } else if (gameStatus == GameStatus::Play) {
        for (size_t i = 0; i < 9; ++i) {
            for (size_t j = 0; j < 9; ++j) {
                if (!field[i][j].GetPlaced()) {
                    field[i][j].SetVisible(false);
                }
            }
        }
        FieldData::WriteData(f, "fieldData.txt", errorsCount);
    } else if (gameStatus == GameStatus::Lose) {
        FieldData::WriteData(initialField, "fieldData.txt", 0);
    }
}
} // Window
