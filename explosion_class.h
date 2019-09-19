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

    bool limit(int row, int col)
    {
        if(row < 1 || row >= 59 || col < 1 || col >= 54) return false;
        return true;
    }
public:
    int status;

    explosion_class(const position_class& pos);

    void update_explosion(screen_class& screen);
    bool operator==(const explosion_class& ob)
    {
        return (status == ob.status) && (explosion_position == ob.explosion_position);
    }
};
//================================//end of explosion_class\\================================//
explosion_class::explosion_class(const position_class& pos)
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

