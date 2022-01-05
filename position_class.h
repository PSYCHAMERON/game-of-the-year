#pragma once
class position_class {
public:
    int row, col;

    position_class(int r, int c) { row = r; col = c; }
    position_class() { row = col = 0; }
    position_class(const position_class& pos) { row = pos.row; col = pos.col;  } // construct form another position_class object
    // overloaded operators
    bool operator==(const position_class& pos) { return (row==pos.row) && (col==pos.col); }

    bool operator!=(const position_class& pos) { return (row!=pos.row) || (col!=pos.col); }

    position_class operator=(const position_class& pos)
    {
        row = pos.row;
        col = pos.col;

        return *this;
    }
};
//================================//end of position_class\\================================//
