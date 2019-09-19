#include <bits/stdc++.h>
#include <conio.h>
typedef unsigned int uint;
typedef unsigned long long int ullint;
using namespace std;
#include "position_class.h"
#include "screen_class.h"
#include "player_class.h"
#include "delay.h"
#include "enemy_class.h"
#include "explosion_class.h"
#include "game_control.h"

#define cls system("CLS")

//================================//begin of main\\================================//
int main()
{

    cout << "PLEASE MAXIMIZE THE SCREEN...                                             ^";
    cout << "\nPREES ANY KEY...";
    getch();
    cls;

    game_control game;

    game.welcome();

    cls;
    char choice;



    while(true)
    {
        game.play_game();

        cout << "GAME OVER\n";

        do{
            cout << "\nDo you want to play again? (y/n): ";
            choice = getch();
            choice = tolower(choice);
        } while(choice != 'y' && choice != 'n');
        //cout << "s";
        if(choice == 'n') break;

        else
        {
            game = game_control();
        }
    } // end of while

    cout << "\nGOOD BYE...";

    while(true) getch();

    return 0;
}
//================================//end of main\\================================//
