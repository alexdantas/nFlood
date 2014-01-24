
#include <ncurses.h>
#include "game.h"
#include "engine.h"
#include "color.h"

int engine_init()
{
	initscr();

	if (engine_set_center(TRUE))
		return -1;

	if (!color_init())
	{
		/* TODO replace colors by characters, as in:
		 *      XXMM##XX
		 *      XX####MM
		 */
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

int engine_set_center(bool center)
{
	if (!center)
	{
		engine.center_top = NOT_CENTER_TOP;
		engine.center_left = NOT_CENTER_LEFT;
		return 0;
	}

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

	return 0;
}

int engine_center_board(struct game_board *board, int hscore, bool center, bool redraw)
{
	if (engine_set_center(center))
		return -1;

	if (redraw)
	{
		erase();
		engine_draw_ui(board, hscore);
		engine_draw_board(board);
	}

	return 0;
}

void engine_draw_ui(struct game_board *board, int hscore)
{
	if (!game_is_over(board))
	{
		/* The problem with creating a hard-coded formula
		 * is that it's tied to nCurses' internal color
		 * values :(
		 */
		int i, x, y;
		for (i = 1; i <= 6; i++)
		{
			if (i == board->last_color)
				continue;

			y = (i - 1)/4;
			x = i - 1 - 4*y;
			change_color(i);
			mvaddch(engine.center_top + 3*y,
			        engine.center_left + 3*x + 1,
			        ACS_CKBOARD);
			mvaddch(engine.center_top + 3*y,
			        engine.center_left + 3*x + 2,
			        ACS_CKBOARD);

			change_color(WHITE_DEFAULT);
			mvaddch(engine.center_top + 3*y + 1,
			        engine.center_left + 3*x + 1,
			        '1' + (i - 1));
		}

		change_color(WHITE_DEFAULT);
		mvprintw(engine.center_top, engine.center_left + 13, "->");
	}
	else
	{
		change_color(WHITE_DEFAULT);
		mvprintw(engine.center_top,     engine.center_left + 1, "Congrats!");
		mvprintw(engine.center_top + 1, engine.center_left + 1, "one more game?");
	}

	change_color(WHITE_DEFAULT);

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
			mvaddch(engine.center_top + j, engine.center_left + 16 + (i*2),   ACS_CKBOARD);
			mvaddch(engine.center_top + j, engine.center_left + 16 + (i*2+1), ACS_CKBOARD);
		}
	}
}

void change_color(int color)
{
	if (has_colors() == TRUE)
		attron(COLOR_PAIR(color));
}

bool is_center()
{
	return !(engine.center_top == NOT_CENTER_TOP && engine.center_left == NOT_CENTER_LEFT);
}

bool is_hit(int x, int y, int tx, int ty, int tw, int th)
{
	return x >= tx && x < tx + tw && y >= ty && y < ty + th;
}
