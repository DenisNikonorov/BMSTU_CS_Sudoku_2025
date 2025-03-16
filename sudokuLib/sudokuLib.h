#ifndef SUDOKULIB
#define SUDOKULIB

#include <iostream>

class Cell {
    private:
    bool isVisible = true;
    int value = 0;

    public:
    Cell() = default;
    Cell(bool isVisible, int value);

    void SetValue(int value) { this->value = value; }
    int GetValue() { return value; }

    void SetVisible(bool visible) { isVisible = visible; }
    bool GetVisible() { return isVisible; }

    friend std::ostream& operator<<(std::ostream& out, const Cell& source) {
        out << "is visible: " << source.isVisible << '\n' << "value: " << source.value << '\n';
        return out;
    }
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
    friend std::ostream& operator<<(std::ostream& out, const Field& source);

    ~Field();
};

#endif // SUDOKULIB
