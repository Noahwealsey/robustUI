#include <stdio.h>
#include <ncurses.h>
#include <stdlib.h>

#define WIDTH 80
#define HEIGHT 20

int startx = 0;
int starty = 0;

char *choices[] = { 
            "Choice 1",
            "Choice 2",
            "Choice 3",
            "Choice 4",
            "Exit",
          };
int n_choices = sizeof(choices) / sizeof(char *);

void print_menu(WINDOW *menu_win, int selected){
    int x = 2, y = 2;
    box(menu_win, '-', ACS_DIAMOND);
    for(int i= 0; i < n_choices; i++){
        if(selected == i + 1){
            wattron(menu_win, A_REVERSE);
            mvwprintw(menu_win, y, x, "%s", choices[i]);
            wattroff(menu_win, A_REVERSE);

        }
        else{
            mvwprintw(menu_win, y, x, "%s", choices[i]); 
        }
        y++;
       
    }
    wrefresh(menu_win);    
}

int main(){
    WINDOW *menu_win;

    int exitCode;

    int i,j;
    int selected = 1;
    int choice = 0;
    int c;

    initscr();
    clear();
    noecho();
    cbreak();

    getmaxyx(stdscr, i ,j);
    mvprintw(28, 0, "max no. of rows and cols %d\t %d", i, j);
    startx = (80 - WIDTH) / 2;
    starty = (24 - HEIGHT) / 2;

    menu_win = newwin(HEIGHT, WIDTH, starty, startx);
    keypad(menu_win, selected);   
    mvprintw(0, 0, "GO UP AND DOWN WITH DIEWCTION KEYS, PRESS ENTER TO SELECT");
    refresh();
    print_menu(menu_win, selected);
    while(1)
    {
        c = wgetch(menu_win);
        switch(c)
        {
            case KEY_UP:
                if(selected == 1){
                    selected = n_choices;                    
                }
                else{
                    selected--;
                }
                break;
            case KEY_DOWN:
                if(selected == n_choices){
                    selected = 1;
                }
                else{
                    selected++;
                }
                break;
            case 10:
                choice = selected;
                break;
            default:
                mvprintw(40, 0, "the character pressed is = %3d Hopefully it can be printed as %c is ", c, c);
                refresh();
                break;
                
        }
        print_menu(menu_win, selected);
        refresh();
        if(choice != 0){
            break;
        }

    }
    clrtoeol();
    mvprintw(30, 0, "You chose %d which is %s", choice, choices[choice - 1]);
    refresh();
    
    attron(A_BLINK);
    mvprintw(40, 90, "PRESS ANYTHING TO EXIT");
    attroff(A_BLINK);
    exitCode = getch();


    endwin();
    return 0;


}
