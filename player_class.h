#pragma once
#include "position_class.h"
#include "enemy_class.h"

class enemy_class;

/*
    |
   /#\
  /###\
 /#####\
/"\/"\/"\
*/
class player_class {
private:
    uint width;// = 9;
    uint height;// = 5;
    uint screen_width; // 55
    uint screen_height; // 60


    string player_string[5];
    int player_health;
    position_class head; // position of the head of the player
    vector<position_class> player_body; // contains the positions occupied by the player ship
    vector<position_class> shots;


    bool limit(int row, int col)
    {
        if(row < 1 || row >= screen_height-1 || col < 1 || col >=  screen_width /*54*/) return false;
        return true;
    }
public:
    vector<int> special_shots; // only row needed
    int get_health() { return player_health; } // returns the health

    bool player_destroyed;

    player_class(int s_width = 55, int s_height = 60); // constructor

    void fire_shot(); // fire weapon
    void update_shots(screen_class& screen); // update the shots
    void fire_special_shot(); // fire the special weapon
    void update_special_shots(screen_class& screen); // update the special shots

    void update_player(screen_class& screen);
    void move_player(screen_class& screen, const string& direction); // move left, right, up, down

    friend int collision_result(player_class& player, enemy_class& enemy); // friend to both classes
};
//================================//end of player_class\\================================//
void player_class::update_player(screen_class& screen)
{
    if(player_destroyed) return;

    player_body.clear(); // clear the vector

    player_body.push_back(head); // start with the head

    int ind = 0, tr, tc = head.col;

    for(tr = head.row; tr < head.row + height; tr++)
    {
        for(int i =  0; i < player_string[ind].size(); i++)
        {
            if( limit(tr, tc + i) )
            {
                screen[tr][tc+i] = player_string[ind][i];

                player_body.push_back( position_class(tr, tc+i) ); // this is a part of the body
            } // end of if
        }

        ind++;
        tc--;
    } // end of for
}
//================================//end\\================================//
player_class::player_class(int s_width, int s_height)
    : screen_width(s_width), screen_height(s_height)
{
/*
    |
   /#\
  /###\ ship will look like this
 /#####\
/"\/"\/"\
*/
    width = 9;
    height = 5;

    player_destroyed = false;
    player_health = 7;
    head = position_class(screen_height - height - 5, screen_width / 2); // in the middle of the screen

    player_string[0] = "|";
    player_string[1] = "/#\\";
    player_string[2] = "/###\\";
    player_string[3] = "/#####\\";
    player_string[4] = "/\"\\/\"\\/\"\\";
}
//================================//end\\================================//
void player_class::move_player(screen_class& screen, const string& direction)
{
    if(player_destroyed) return;
    position_class change;
    int movement_speed = 2;

    if(direction == "left") change = position_class(0, -movement_speed);
    else if(direction == "right") change = position_class(0, movement_speed);
    else if(direction == "up") change = position_class(-movement_speed, 0);
    else if(direction == "down") change = position_class(movement_speed, 0);

    if ( limit(head.row + change.row, head.col + change.col)  )
    {                                                           // ^ don't let any part of the player go down
        head.row = head.row + change.row;
        head.col = head.col + change.col;
        int kek = screen_height - height - 1;
        if(head.row > kek) head.row = kek;
    } // end of if
}
//================================//end\\================================//
void player_class::fire_shot()
{
    if(player_destroyed) return;

    shots.push_back(head);
}
//================================//end\\================================//

void player_class::update_shots(screen_class& screen)
{
    if(player_destroyed) return;

    vector<position_class> dele;
    for(int i = 0; i < shots.size(); i++)
    {
        shots[i].row--; // go up

        if(shots[i].row != 0) screen[ shots[i].row ][ shots[i].col ] = '|';
        else dele.push_back(shots[i]);
    } // end of for

    vector<position_class>::iterator it;

    for(int i = 0; i < dele.size(); i++)
    {
        it = find(shots.begin(), shots.end(), dele[i]);
        shots.erase(it);
    }
}
//================================//end\\================================//
void player_class::fire_special_shot()
{
    if(player_destroyed) return;

    special_shots.push_back( head.row );
}

void player_class::update_special_shots(screen_class& screen)
{
    if(player_destroyed) return;

    vector<int> dele;

    for(int i = 0; i < special_shots.size(); i++)
    {
        special_shots[i]--;

        if(special_shots[i] == 0) dele.push_back(special_shots[i]);
        else
        for(int j = 1; j <= screen_width - 2; j++) screen[ special_shots[i] ] [j] = '_'; // goes along the rows

    }

    // delete
    for(int i = 0; i < dele.size(); i++)
    {
        special_shots.erase( find(special_shots.begin(), special_shots.end(), dele[i]) );
    }
}
