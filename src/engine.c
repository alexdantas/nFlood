
#include <ncurses.h>
#include "game.h"
#include "engine.h"

#define TEXT 10

int engine_init()
{
	initscr();

	/* Getting current width and height */
	int current_height, current_width;
	getmaxyx(stdscr, current_height, current_width);

	if ((current_width  < GAME_UI_WIDTH) ||
	    (current_height < GAME_UI_HEIGHT))
	{
		endwin();
		return -1;
	}
	engine.width  = current_width;
	engine.height = current_height;

	engine.center_top  = engine.height/2 - GAME_UI_HEIGHT/2;
	engine.center_left = engine.width/2  - GAME_UI_WIDTH/2;

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
	if (!game_is_over(board))
	{
                int i, x, y;
                for (i = 1; i <= 6; i++)
                {
                        if (i == board->last_color)
                                continue;

			y = (i - 1)/4;
			x = i - 1 - 4*y;
                        change_color(i);
                        mvaddstr(engine.center_top + 3*y,
                                 engine.center_left + 3*x + 1,
                                 "  ");

                        change_color(TEXT);
                        mvaddch(engine.center_top + 3*y + 1,
                                engine.center_left + 3*x + 1,
                                '1' + (i - 1));
                }

                change_color(TEXT);
                mvprintw(engine.center_top, engine.center_left + 13, "->");
	}
	else
	{
                change_color(TEXT);
                mvprintw(engine.center_top,     engine.center_left + 1, "Congrats!");
                mvprintw(engine.center_top + 1, engine.center_left + 1, "one more game?");
	}

	change_color(TEXT);

	mvprintw(engine.center_top + 6, engine.center_left + 1, "nFlood v" VERSION);

	mvprintw(engine.center_top + 8, engine.center_left + 1, "r: New Game");
	mvprintw(engine.center_top + 9, engine.center_left + 1, "q: Quit");

	mvprintw(engine.center_top + 11, engine.center_left + 1, "Moves:   %d", board->moves);
	mvprintw(engine.center_top + 12, engine.center_left + 1, "Best:    %d", hscore);
	mvprintw(engine.center_top + 13, engine.center_left + 1, "Flooded: %.0f%%", (float)board->flood_count/(GAME_TABLE_WIDTH * GAME_TABLE_HEIGHT) * 100);
}

void engine_draw_board(struct game_board *board)
{
	int i; int j;

	for (i = 0; i < GAME_TABLE_WIDTH; i++)
	{
		for (j = 0; j < GAME_TABLE_HEIGHT; j++)
		{
			change_color(board->cell[i][j].color);
			mvaddch(engine.center_top + j, engine.center_left + 16 + (i*2),   ' ');
			mvaddch(engine.center_top + j, engine.center_left + 16 + (i*2+1), ' ');
		}
	}
}

void change_color(int color)
{
	if (has_colors () == TRUE)
		attron (COLOR_PAIR(color));
}

