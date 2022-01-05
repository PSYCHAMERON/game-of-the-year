#pragma once
#include <vector>

class screen_class {
private:
    uint width;// = 55;
    uint height;// = 60;

    string screen_string[100];
    int turn; // for making the screen run
public:
    screen_class(uint w = 55, uint h = 60);
    friend ostream& operator<<(ostream& stream, screen_class ob)
    {
        string s;
        for(int i = 0; i < ob.height; i++) s += ob.screen_string[i];
        stream << s;

        return stream;
    }
    string& operator[](int i) { return screen_string[i]; } // overloaded subscript operator
    void clear_screen();
};
//================================//end of screen_class\\================================//
screen_class::screen_class(uint w, uint h)
 : width(w), height(h)
{
    //width = 55;
    //height = 60;

    turn = 0;
    clear_screen();
}
//================================//end\\================================//
void screen_class::clear_screen()
{
    string temp;
    for(int i = 0; i < width; i++) temp += " ";
    temp += "\n";

    screen_string[0] = temp;
    screen_string[height-1] = temp;

    string temp2 = "\"";
    for(int i = 0; i < width - 2; i++) temp2 += " ";
    temp2 += "\"\n";

    for(int i = 1; i <= height-2; i++)
    {
        if(i%2 == turn)
        {
            screen_string[i] = temp2;
        }
        else
        {
            screen_string[i] = temp;
        }
    }

    turn = abs(turn - 1);
}
