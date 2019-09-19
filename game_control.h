
vector<explosion_class> explosions; // stores the explosions

class game_control {
private:
    screen_class screen;
    player_class player;
    vector<enemy_class> enemies;

    delayClass delay;

    uint num_of_enemies;
    ullint score;
    ullint high_score;
    uint level;
    uint enemy_shot_interval;
    uint begin_score; // to reward an extra life
    uint num_of_special_shots;

    int life; // player life
    char command;

    position_class get_enemy_pos();

    void enemy_control();
    void explosion_control();
    void level_control(); // upgrade the level

    void print_health(int h)
    {
        if(h <= 0) { cout << "!!! DESTROYED !!!"; return; }
        for(int i = 0; i < h; i++) cout << "#";
        if(h <= 5) cout << "     !!! DANGER CLOSE !!!";
    }
    void print_life()
    {
        if(life <= 0) { cout << "NO LIFE LEFT"; return; }
        for(int i = 0; i < life; i++) cout << "* ";
    }

    void load_high_score();
    void save_high_score();

public:
    game_control(); // constructor
    void welcome();
    void play_game();
};
//================================//end of game_control\\================================//
game_control::game_control() : screen(), player(), delay() // constructor
{
    srand(time(NULL));

    level = 1;
    num_of_special_shots = 3;

    life = 1;
    score = 0;
    begin_score = 0;
    enemy_shot_interval = 30;

    num_of_enemies = 2;
}
//================================//end\\================================//
position_class game_control::get_enemy_pos()
{
    int c;
    bool done;

    while(true)
    {
        c = (rand() % 47) + 4;
        done = true;

        for(int i = 0; i < enemies.size(); i++)
        {
            if(enemies[i].head.col + 6 >= c && enemies[i].head.col - 6 <= c)
            {
                done = false;
                break;
            }
        }

        if(done) return position_class(1, c);
    }

}
//================================//end\\================================//
void game_control::enemy_control()
{
    // clear the unnecessary enemies
    vector<enemy_class> dele; // that have to be deleted

    for(int i = 0; i < enemies.size(); i++)
    {
        if(enemies[i].enemy_destroyed) score += 5;

        if(enemies[i].enemy_destroyed || enemies[i].head.row > 61) dele.push_back(enemies[i]);
    } // end of for

    for(int i = 0; i < dele.size(); i++)
    {
        enemies.erase( find(enemies.begin(), enemies.end(), dele[i]) );
    }
    dele.clear();
    /////////////////////////////////////////
    if(player.special_shots.size() != 0) return; // if a special shot is in the game, then return

    int needed = num_of_enemies - enemies.size();
    // add needed enemies
    for(int i = 0 ; i < needed; i++)
    {
        enemies.push_back( enemy_class( get_enemy_pos() ) );
    }
    ////////////////////////////////
}
//================================//end\\================================//
void game_control::play_game()
{
    int move_update = 0;
    int shot_update = enemy_shot_interval;
    delay.resetTimer();
    int temp = 0;

    load_high_score(); // load the high score

    while(life >= 0)
    {
        cout << screen;

        cout << "SCORE: " << score << "                   PREVIOUS HIGH SCORE: "  << high_score <<  '\n';

        cout << "\nHEALTH: "; print_health( player.get_health() );

        cout << "\n\nLIFE LEFT: "; print_life();
        cout << "\n\nPRIMARY WEAPON: INFINITY           SECONDARY WEAPON: " << num_of_special_shots;

        player.update_player(screen);

        delay.freeze(0.1); delay.resetTimer();

        if(kbhit())
        {
            command = getch();
            command = tolower(command);

            if(command == 'a') player.move_player(screen, "left");
            else if(command == 'd') player.move_player(screen, "right");
            else if(command == 'w') player.move_player(screen, "up");
            else if(command == 's') player.move_player(screen, "down");

            else if(command == ' ') player.fire_shot();
            else if(command == 'q')
            {
                if(num_of_special_shots != 0)
                {
                    player.fire_special_shot();
                    num_of_special_shots--;
                }
            }
        } // end of if

        system("CLS");
        screen.clear_screen();

        //player.update_player(screen);

        player.update_shots(screen);
        player.update_special_shots(screen);

        enemy_control(); // add or delete enemies
        //cout << "size: " << enemies.size() << endl;

        // move every enemy
        if(move_update == 5)
        {
            move_update = 0;

            for(int i =0; i < enemies.size(); i++)
            {
                enemies[i].move_enemy();
            }
        }

        // take shot by every enemy
        if(shot_update >= enemy_shot_interval)
        {
            shot_update = 0;
            for(int i =0; i < enemies.size(); i++) enemies[i].fire_shot();
        }

        for(int i = 0; i < enemies.size(); i++)
        {

            //cout << "col: " << enemies[i].head.col << endl;
            enemies[i].update_enemy(screen);

            enemies[i].update_shots(screen);

            collision_result(player, enemies[i]);
        } // end of for

        explosion_control(); // delete explosions that are not needed

        for(int i =0; i < explosions.size(); i++) explosions[i].update_explosion(screen);

        player.update_player(screen);

        if(player.player_destroyed) // player is destroyed
        {
            temp++;
        } // end of if
        if(player.player_destroyed && temp == 10) // time to use the next life, if any remaining
        {
            player = player_class();
            life--; // costs one life
            temp = 0; // reset
        }

        move_update++;
        shot_update++;
        level_control();
    } // end of while

    save_high_score(); // save high score
}
//================================//end\\================================//
void game_control::explosion_control()
{
    vector<explosion_class> dele; // that must be deleted

    for(int i = 0; i < explosions.size(); i++)
    {
        if(explosions[i].status == 4) dele.push_back( explosions[i] );
    }

    for(int i = 0; i < dele.size(); i++)
    {
        explosions.erase( find(explosions.begin(), explosions.end(), dele[i]) );
    }
}
//================================//end\\================================//
void game_control::level_control()
{
    if(score - begin_score >= 100) // every 100 points scored will be rewarded with an extra life
    {
        begin_score = score;
        life++;
        num_of_special_shots++; // get an extra special shot
    }

    if(life > 2) life = 2; // maximum extra life will be 2

    if(level == 3) return; // max level is 3

    level = score/50 + 1; // every 50 score will raise the level by 1


    num_of_enemies = level + 1;
    enemy_shot_interval = 30 - (level - 1) * 5; // and also their shot interval will be reduced

    /*if(level == 2) {
    cout << "level: " << level << endl << " en: " << num_of_enemies
    << endl << enemy_shot_interval << endl;
    getch();
    }*/
}
//================================//end\\================================//
void game_control::welcome()
{
    cout << "\n\n\n\n\n\n\n\n\n                PAGLAGHORA GAMES presents...\n\n";
    cout << "                             GAME OF THE YEAR :p\n\n\n\n";

    delay.resetTimer();
    delay.freeze(2); delay.resetTimer();

    cout << "Instructions: \nW, A, S, D to move\n";
    cout << "SPACE to fire primary weapon\n";
    cout << "Q to fire secondary weapon\n";

    cout << "\n\n# Shoot and destroy enemies to earn points\n\n# Avoid enemy-fire and collision\n";
    cout << "\n# Every 100 points you score, will be rewarded with an extra life (MAX: 2)\nand a secondary weapon shell\n";
    cout << "\n# Difficulty will increase with your score\n";

    cout << "\n\nPRESS ANY KEY...";
    getch();
}
//================================//end\\================================//
void game_control::load_high_score()
{
    ifstream fin("D://game.dat");

    if(!fin) // for the first time
    {
        high_score = 0;
        return;
    }

    fin >> high_score; // read the high score

    fin.close();
}
//================================//end\\================================//
void game_control::save_high_score()
{
    if(score < high_score) return;

    ofstream fout("D://game.dat");

    fout << score;

    fout.close();
}
//================================//end\\================================//
int collision_result(player_class& player, enemy_class& enemy)
{
    // return value:
    // 0 if no collision
    // 1 if player is hit
    // 2 if enemy is hit
    // 3 if player and enemy ship collides

    for(int i = 0; i < enemy.shots.size(); i++)
    {
        for(int j = 0; j < player.player_body.size(); j++)
        {
            if(enemy.shots[i] == player.player_body[j])
            { // player ship is hit
                player.player_health--; // lower player health
                enemy.shots.erase(enemy.shots.begin() + i); // delete the enemy shot
                //cout << "\a"; // alert

                if(player.player_health <= 0)
                {  // player is destroyed
                    explosions.push_back( position_class( player.head.row + 2, player.head.col ) );

                    player.player_body.clear();
                    player.player_destroyed = true;
                }

                return 1;
            }
        } // end of for
    } // end of for

    for(int i = 0; i < player.shots.size(); i++)
    {
        for(int j = 0; j < enemy.enemy_body.size(); j++)
        {
            if(player.shots[i] == enemy.enemy_body[j] ||
               position_class(player.shots[i].row + 1, player.shots[i].col) == enemy.enemy_body[j]
               )
            { // enemy ship is hit
                enemy.enemy_health--; // lower enemy health
                player.shots.erase(player.shots.begin() + i); // delete the player shot

                if(enemy.enemy_health <= 0)
                { // enemy is destroyed
                    explosions.push_back( position_class(enemy.head.row-2, enemy.head.col) );

                    enemy.enemy_body.clear();
                    enemy.enemy_destroyed = true;
                }
                return 2;
            }
        }
    }
    ///////////////////////////////

    for(int i = 0; i < player.player_body.size(); i++)
    {
        for(int j = 0; j < enemy.enemy_body.size(); j++)
        {
            if( player.player_body[i] == enemy.enemy_body[j] )
            { // enemy and player collides
                player.player_health--;
                enemy.enemy_health--;
                //cout << "\a";
                if(player.player_health <= 0)
                {  // player is destroyed
                    explosions.push_back( position_class( player.head.row + 2, player.head.col ) );

                    player.player_body.clear();
                    player.player_destroyed = true;
                }
                if(enemy.enemy_health <= 0)
                { // enemy is destroyed
                    explosions.push_back( position_class(enemy.head.row-2, enemy.head.col) );

                    enemy.enemy_body.clear();
                    enemy.enemy_destroyed = true;
                }

                return 3;
            }
        }
    }

    // for special shots
    for(int i = 0; i < player.special_shots.size(); i++)
    {
        for(int j = 0; j < enemy.shots.size(); j++)
        {
            if(player.special_shots[i] == enemy.shots[j].row ||
               player.special_shots[i] == enemy.shots[j].row-1 ||
               player.special_shots[i] == enemy.shots[j].row+1
               )
            {
                // special shot has hit an enemy shot
                enemy.shots.erase(enemy.shots.begin() + j); // delete the enemy shot
            }
        } // end of for

        for(int j = 0; j < enemy.enemy_body.size(); j++)
        {

            if( enemy.enemy_body[j].row == player.special_shots[i] )
            {
                // enemy is destroyed by one touch of the special shot

                enemy.enemy_health = 0;

                explosions.push_back( position_class(enemy.head.row-2, enemy.head.col) );

                enemy.enemy_body.clear();
                enemy.enemy_destroyed = true;

                return 2;
            }

        }
    }

    return 0;
}
//================================//end\\================================//
