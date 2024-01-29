#include <panel.h>

typedef struct _PANEL_DATA {
	int x, y, w, h;
	char label[80]; 
	int label_color;
	PANEL *next;
}PANEL_DATA;

#define NLINES 10
#define NCOLS 40


void init_wins(WINDOW **wins, int n)
{	int x, y, i;
	char label[80];

	y = 2;
	x = 10;
	for(i = 0; i < n; ++i)
	{	wins[i] = newwin(NLINES, NCOLS, y, x);
		sprintf(label, "Window Number %d", i + 1);
		win_show(wins[i], label, i + 1);
		y += 3;
		x += 7;
	}
}
void set_user_ptrs(PANEL **panels, int n){
	int x,y,h,w;
	char temp[80];
	PANEL_DATA *ptrs;
	WINDOW *win;

	ptrs = (PANEL_DATA*)calloc(n, sizeof(PANEL_DATA));
	for (int i = 0; i < n; i++)
	{
		win = panel_window(panels[i]);
		getbeg(win, y, x);
		getmax(win, h, w);
		ptrs[i].x = x;
		ptrs[i].y = y;
		ptrs[i].w = w;
		ptrs[i].h = h;	
		sprintf(temp, "WINDOW NUMBER %d", i + 1);
		strcpy(ptrs[i].label, temp);
		if(i + 1 == n){
			ptrs[i].next = panels[0];
		}
		else{
			ptrs[i].next = panels[i];
		}

	}
	

}
void print_in_middle(WINDOW *win, int starty, int startx, int width, char *string, chtype colorIndex){
	int x, y, h, w;

	getyx(win, y, x);
	if(startx != 0){
		x = startx;
	}
	if(starty != 0){
		y = starty;
	}

	int len = strlen(string);
	int temp = (width - len)/2;
	x = startx + (int)temp;
	wattron(win, colorIndex);
	mvprintw(y, x, "window number %s", string);
	wattroff(win, colorIndex);
	refresh();
}

void win_show(WINDOW *win, char *label, int label_color){	
	int width, height, startx, starty;

	box(win, 0, 0);
	mvwaddch(win, 2, 0, ACS_LTEE); 
	mvwhline(win, 2, 1, ACS_HLINE, width - 2); 
	mvwaddch(win, 2, width - 1, ACS_RTEE); 
	
	print_in_middle(win, 1, 0, width, label, COLOR_PAIR(label_color));

}

int main()
{	WINDOW *my_wins[3];
	PANEL  *my_panels[3];
	PANEL_DATA  *top;
	PANEL *stack_top;
	WINDOW *temp_win, *old_win;
	int ch;
	int newx, newy, neww, newh;
	int resize = FALSE, move = FALSE;

	/* Initialize curses */
	initscr();
	start_color();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);


	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_GREEN, COLOR_BLACK);
	init_pair(3, COLOR_BLUE, COLOR_BLACK);
	init_pair(4, COLOR_CYAN, COLOR_BLACK);

	init_wins(my_wins, 3);
	
	/* Attach a panel to each window */ 	/* Order is bottom up */
	my_panels[0] = new_panel(my_wins[0]); 	/* Push 0, order: stdscr-0 */
	my_panels[1] = new_panel(my_wins[1]); 	/* Push 1, order: stdscr-0-1 */
	my_panels[2] = new_panel(my_wins[2]); 	/* Push 2, order: stdscr-0-1-2 */

	set_user_ptrs(my_panels, 3);
	/* Update the stacking order. 2nd panel will be on top */
	update_panels();

	attron(COLOR_PAIR(4));
	mvprintw(LINES - 3, 0, "Use 'm' for moving, 'r' for resizing");
	mvprintw(LINES - 2, 0, "Use tab to browse through the windows (F1 to Exit)");
	attroff(COLOR_PAIR(4));
	doupdate();

	stack_top = my_panels[2];
	top = (PANEL_DATA *)panel_userptr(stack_top);
	newx = top->x;
	newy = top->y;
	neww = top->w;
	newh = top->h;
	while((ch = getch()) != KEY_F(1))
	{	switch(ch)
		{	case 9:		/* Tab */
				top = (PANEL_DATA *)panel_userptr(stack_top);
				top_panel(top->next);
				stack_top = top->next;
				top = (PANEL_DATA *)panel_userptr(stack_top);
				newx = top->x;
				newy = top->y;
				neww = top->w;
				newh = top->h;
				break;
			case 'r':	/* Re-resize*/
				resize = TRUE;
				attron(COLOR_PAIR(4));
				mvprintw(LINES - 4, 0, "Entered Resizing :Use Arrow Keys to reresize and press <ENTER> to end resizing");
				refresh();
				attroff(COLOR_PAIR(4));
				break;
			case 'm':	/* Move */
				attron(COLOR_PAIR(4));
				mvprintw(LINES - 4, 0, "Entered Moving: Use Arrow Keys to Move and press <ENTER> to end moving");
				refresh();
				attroff(COLOR_PAIR(4));
				move = TRUE;
				break;
			case KEY_LEFT:
				if(resize == TRUE)
				{	--newx;
					++neww;
				}
				if(move == TRUE)
					--newx;
				break;
			case KEY_RIGHT:
				if(resize == TRUE)
				{	++newx;
					--neww;
				}
				if(move == TRUE)
					++newx;
				break;
			case KEY_UP:
				if(resize == TRUE)
				{	--newy;
					++newh;
				}
				if(move == TRUE)
					--newy;
				break;
			case KEY_DOWN:
				if(resize == TRUE)
				{	++newy;
					--newh;
				}
				if(move == TRUE)
					++newy;
				break;
			case 10:	/* Enter */
				move(LINES - 4, 0);
				clrtoeol();
				refresh();
				if(resize == TRUE)
				{	old_win = panel_window(stack_top);
					temp_win = newwin(newh, neww, newy, newx);
					replace_panel(stack_top, temp_win);
					win_show(temp_win, top->label, top->label_color); 
					delwin(old_win);
					resize = FALSE;
				}
				if(move == TRUE)
				{	move_panel(stack_top, newy, newx);
					move = FALSE;
				}
				break;
			
		}
		attron(COLOR_PAIR(4));
		mvprintw(LINES - 3, 0, "Use 'm' for moving, 'r' for resizing");
	    	mvprintw(LINES - 2, 0, "Use tab to browse through the windows (F1 to Exit)");
	    	attroff(COLOR_PAIR(4));
	        refresh();	
		update_panels();
		doupdate();
	}
	endwin();
	return 0;
}

