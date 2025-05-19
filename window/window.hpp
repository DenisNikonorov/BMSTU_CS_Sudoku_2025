#pragma once

#include <SFML/Graphics.hpp>
#include "../sudokuLib/sudokuLib.hpp"


namespace Window {
    void RunGameWindow(Field& f, Field& initialField, bool& isRunning, int& errorsCount);
}
