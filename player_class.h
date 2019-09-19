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

    string player_string[5];
    int player_health;
    position_class head; // position of the head of the player
    vector<position_class> player_body; // contains the positions occupied by the player ship
    vector<position_class> shots;


    bool limit(int row, int col)
    {
        if(row < 1 || row >= 59 || col < 1 || col >= 54) return false;
        return true;
    }
public:
    vector<int> special_shots; // only row needed
    int get_health() { return player_health; } // returns the health

    bool player_destroyed;

    player_class(); // constructor

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
player_class::player_class()
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
    player_health = 15;
    head = position_class(54, 27); // in the middle of the screen

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

    if(direction == "left") change = position_class(0, -2);
    else if(direction == "right") change = position_class(0, 2);
    else if(direction == "up") change = position_class(-2, 0);
    else if(direction == "down") change = position_class(2, 0);

    if ( limit(head.row + change.row, head.col + change.col) && (head.row + change.row) != 56 )
    {                                                           // ^ don't let any part of the player go down
        head.row = head.row + change.row;
        head.col = head.col + change.col;
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
        for(int j = 1; j <= 53; j++) screen[ special_shots[i] ] [j] = '_'; // goes along the rows

    }

    // delete
    for(int i = 0; i < dele.size(); i++)
    {
        special_shots.erase( find(special_shots.begin(), special_shots.end(), dele[i]) );
    }
}
