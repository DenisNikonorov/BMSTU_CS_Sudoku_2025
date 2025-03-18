#pragma once

#include "../sudokuLib/sudokuLib.h"

enum class MainMenu : int {
    NewGame = 1,
    ContinueGame,
    Exit,
};

enum class Difficulty : int {
    Easy = 1,
    Medium,
    Hard,
};

namespace Game {
void GetDataFromFile(Field&, const char*);
void WriteData(Field&, const char*, int);
void PlaceNumbers(Field&, int&);

void SelectDifficulty(Field&);
void SelectMenuItem(Field&, bool&);
void RunGame();
}
