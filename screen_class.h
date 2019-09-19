
class screen_class {
private:
    uint width;// = 55;
    uint height;// = 60;

    string screen_string[60];

    int turn; // for making the screen run
public:
    screen_class();
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
screen_class::screen_class()
{
    width = 55;
    height = 60;

    turn = 0;
    clear_screen();
}
//================================//end\\================================//
void screen_class::clear_screen()
{
    screen_string[0] = "                                                       \n";
    screen_string[height-1] = "                                                       \n";



    for(int i = 1; i <= height-2; i++)
    {
        if(i%2 == turn)
        screen_string[i] = "\"                                                     \"\n";
        else
        screen_string[i] = "                                                       \n";
    }

    turn = abs(turn - 1);
}
