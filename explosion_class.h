#pragma once

class explosion_class {
/*
   ...
  .....
 .......
  .....
   ...
*/
private:
    string explosion_string[4];
    position_class explosion_position;

    uint screen_width; // 55
    uint screen_height; // 60

    bool limit(int row, int col)
    {
        if(row < 1 || row >= screen_height-1 || col < 1 || col >=  screen_width /*54*/) return false;
        return true;
    }
public:
    int status;

    explosion_class(const position_class& pos, int s_width, int s_height);

    void update_explosion(screen_class& screen);
    bool operator==(const explosion_class& ob)
    {
        return (status == ob.status) && (explosion_position == ob.explosion_position);
    }
};
//================================//end of explosion_class\\================================//
explosion_class::explosion_class(const position_class& pos, int s_width, int s_height)
 : screen_width(s_width), screen_height(s_height)
{
    status = 0;
    explosion_position = pos;

    explosion_string[0] = "*********";
    explosion_string[1] = "*******";
    explosion_string[2] = "*****";
    explosion_string[3] = "***";
}
//================================//end\\================================//
void explosion_class::update_explosion(screen_class& screen)
{
    if(status == 4) return;

    int tr, tc;

    tr = explosion_position.row - status;
    tc = explosion_position.col - (3 - status);

    for(int i = 0; i < explosion_string[status].size(); i++)
    {
        if( limit(tr, tc + i) )
        {
            screen[tr][ tc+i ] = explosion_string[status][i];
        }
    } // end of for

    tr = explosion_position.row + status;
    tc = explosion_position.col - (3 - status);

    for(int i = 0; i < explosion_string[status].size(); i++)
    {
        if( limit(tr, tc + i) )
        {
            screen[tr][ tc+i ] = explosion_string[status][i];
        }
    } // end of for

    status++;
}
//================================//end\\================================//

