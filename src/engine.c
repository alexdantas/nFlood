
#include <ncurses.h>
#include "game.h"
#include "engine.h"

#define TEXT 10

int engine_init()
{
	initscr();

	int current_height, current_width;
	getmaxyx (stdscr, current_height, current_width);

	if ((current_width < GAME_WIDTH) || (current_height < GAME_HEIGHT))
	{
		// ABORT!
		endwin ();
		return -1;
	}

	if (has_colors() == TRUE)
	{
		int bg_color;

		start_color();

		// Let's try grabbing the current terminal background color
		if (use_default_colors () == ERR)
			bg_color = COLOR_BLACK;
		else
			bg_color = -1;
/*
  init_pair (BLUE,    COLOR_BLUE,    bg_color);
  init_pair (MAGENTA, COLOR_MAGENTA, bg_color);
  init_pair (WHITE,   COLOR_WHITE,   bg_color);
  init_pair (RED,     COLOR_RED,     bg_color);
  init_pair (GREEN,   COLOR_GREEN,   bg_color);
  init_pair (YELLOW,  COLOR_YELLOW,  bg_color);
*/
		init_pair (TEXT,     COLOR_WHITE, bg_color);
		init_pair (BLUE,    bg_color,  COLOR_BLUE);
		init_pair (MAGENTA, bg_color,  COLOR_MAGENTA);
		init_pair (WHITE,   bg_color,  COLOR_WHITE);
		init_pair (RED,     bg_color,  COLOR_RED);
		init_pair (GREEN,   bg_color,  COLOR_GREEN);
		init_pair (YELLOW,  bg_color,  COLOR_YELLOW);
	}
	else
	{
		/* need to replace colors by blocks */
	}

	cbreak();
	curs_set(0);
	noecho();
	keypad(stdscr, TRUE);
	refresh();

	mousemask(BUTTON1_CLICKED, NULL);

	return 0;
}

void engine_exit()
{
	erase();
	refresh();
	endwin();
}

void engine_draw_ui(struct game_board *board, int hscore)
{
    /* Gets the current width and height
     * FIXME: May be expensive to call this every frame
     */
	int width, height;
    getmaxyx(stdscr, height, width);

	int center_top  = height/2 - 5;
	int center_left = width/2  - 30;

	if (!game_is_over(board))
	{
		int i;
		for (i = 1; i <= 6; i++)
		{
			if (i == board->last_color)
				continue;

			change_color(i);
			mvaddch(center_top,
			        center_left - 1 + (2*i),
			        ' ');

			change_color(TEXT);
			mvaddch(center_top + 1,
			        center_left - 1 + (2*i),
			        '1' + (i - 1));
		}

		change_color(TEXT);
		mvprintw(center_top, center_left + 13, "->");
	}

	change_color(TEXT);
	mvprintw(center_top + 5, center_left + 1, "r: New Game");
	mvprintw(center_top + 7, center_left + 1, "q: Quit");

	mvprintw(center_top + 11, center_left + 1, "Moves:   %d", board->moves);
	mvprintw(center_top + 12, center_left + 1, "Best:    %d", hscore);
	mvprintw(center_top + 13, center_left + 1, "Flooded: %.0f%%", (float)board->flood_count/(GAME_TABLE_WIDTH * GAME_TABLE_HEIGHT) * 100);
}

void engine_draw_board(struct game_board *board)
{
    /* Gets the current width and height
     * FIXME: May be expensive to call this every frame
     */
	int width, height;
    getmaxyx(stdscr, height, width);

	int center_top  = height/2 - 5;
	int center_left = width/2  - 30;

	int i; int j;

	for (i = 0; i < GAME_TABLE_WIDTH; i++)
	{
		for (j = 0; j < GAME_TABLE_HEIGHT; j++)
		{
			change_color(board->cell[i][j].color);
			mvaddch(center_top + j, center_left + 16 + (i*2),   ' ');
			mvaddch(center_top + j, center_left + 16 + (i*2+1), ' ');
		}
	}
}

void change_color(int color)
{
	if (has_colors () == TRUE)
		attron (COLOR_PAIR(color));
}

