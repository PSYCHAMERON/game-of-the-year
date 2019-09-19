
class player_class;

class enemy_class {
/*
[[[O]]]
 \|o|/ enemy ship will look like this
  \o/
   |
*/
private:
    uint height;
    uint width;

    string enemy_string[4];
    vector<position_class> shots;
    vector<position_class> enemy_body; // holds the current positions of the body

    int enemy_health; // health of the enemy ship
    bool limit(int row, int col)
    {
        if(row < 1 || row >= 59 || col < 1 || col >= 54) return false;
        return true;
    }

public:
    bool enemy_destroyed;
    bool gone_out;
    position_class head;

    enemy_class(const position_class& pos);
    void update_enemy(screen_class& screen);
    void move_enemy();
    void fire_shot();
    void update_shots(screen_class& screen);

    // overloaded == operator
    bool operator==(const enemy_class& ob)
    {
        return (enemy_destroyed ==  ob.enemy_destroyed) && (head == ob.head);
    }
    friend int collision_result(player_class& player, enemy_class& enemy); // friend to both classes
};
//================================//end of enemy_class\\================================//
enemy_class::enemy_class(const position_class& pos)
{
/*
[[[O]]]
 \[o]/ enemy ship will look like this
  \o/
   |
*/  height = 4;
    width = 7;

    enemy_destroyed = false;

    head = pos;
    enemy_health = 15;

    enemy_string[0] = "|"; // down to up
    enemy_string[1] = "\\o/";
    enemy_string[2] = "\\[o]/";
    enemy_string[3] = "[[[O]]]";
}
//================================//end\\================================//
void enemy_class::update_enemy(screen_class& screen)
{

    enemy_body.clear(); // clear the old positions of the body first
    enemy_body.push_back(head); // first, push the head

    int tr, tc = head.col, ind = 0;

    for(tr = head.row; tr != head.row - height; tr--) // down to up
    {
        for(int i = 0; i < enemy_string[ind].size(); i++) // left to right
        {
            if( limit( tr, tc + i ) )
            { // this will be a part of the new position of the enemy ship
                enemy_body.push_back( position_class(tr, tc + i) );
                screen[tr][tc+i] = enemy_string[ind][i];
            }
        } // end of for

        ind++;
        tc--;
    } // end of for
}
//================================//end\\================================//
void enemy_class::move_enemy()
{
    if(enemy_destroyed) return;
    if(enemy_destroyed) return;
    head.row++;
}
//================================//end\\================================//
void enemy_class::fire_shot()
{
    if(enemy_destroyed) return;

    if(head.row < 59)
    {
        //cout << "pushing row: " << head.row; getch();
        shots.push_back(head); // if head is in the screen
    }
}
//================================//end\\================================//
void enemy_class::update_shots(screen_class& screen)
{
    if(enemy_destroyed) return;

    vector<position_class> dele; // contains the shots that must be deleted

    for(int i = 0; i < shots.size(); i++)
    {
        shots[i].row++; // shots go down

        if(shots[i].row != 59) screen[ shots[i].row ][ shots[i].col ] = 'o';
        else
        {
            /*cout << "erasing row: " << (*(shots.begin() + i)).row<< endl; getch();
            cout << "before erasing: ";
            for(int j = 0; j < shots.size(); j++) cout << shots[j].row << " ";
            cout << "size: " << shots.size();
            cout << endl;*/
            //shots.erase(shots.begin() + i);
            dele.push_back(shots[i]);
            /*cout << "after erasing: ";
            for(int j = 0; j < shots.size(); j++) cout << shots[j].row << " ";
            cout << "size: " << shots.size();
            getch();*/
        } // end of else
    } // end of for
    vector<position_class>::iterator it;

    for(int i = 0; i < dele.size(); i++)
    {
        it = find(shots.begin(), shots.end(), dele[i]);
        shots.erase(it);
    }
}
//================================//end\\================================//
