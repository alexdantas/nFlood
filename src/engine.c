
#include "engine.h"
#include "options.h"

int engine_init()
{
	initscr();

	if (!engine_set_center(true))
		return -1;

	/* Default cell appearance - nice checkboard */
	engine.cell_appearance = ACS_CKBOARD;

	if (!color_init())
	{
		options.colors = false;

		/* TODO replace colors by characters, as in:
		 *      XXMM##XX
		 *      XX####MM
		 */
	}

	/* default cell colors */
	engine.colors[0] = color_pair_from_string("blue",    "default");
	engine.colors[1] = color_pair_from_string("magenta", "default");
	engine.colors[2] = color_pair_from_string("red",     "default");
	engine.colors[3] = color_pair_from_string("yellow",  "default");
	engine.colors[4] = color_pair_from_string("green",   "default");
	engine.colors[5] = color_pair_from_string("white",   "default");
	engine.text      = color_pair_from_string("white",   "default");
	engine.hilite    = color_pair_from_string("red",     "default");

	cbreak();					/* get keys without pressing enter */
	curs_set(0);				/* hiding blinking cursor */
	noecho();					/* don't echo the keys pressed */
	keypad(stdscr, TRUE);		/* accept extra keys (F1, etc) */

	mousemask(BUTTON1_CLICKED, NULL); /* mouse support */
	refresh();
	return 0;
}

void engine_exit()
{
	erase();
	refresh();
	endwin();
}

bool engine_set_center(bool center)
{
	if (!center)
	{
		engine.center_top = NOT_CENTER_TOP;
		engine.center_left = NOT_CENTER_LEFT;
		return true;
	}

	/* Getting current width and height */
	int current_height, current_width;
	getmaxyx(stdscr, current_height, current_width);

	if ((current_width  < GAME_UI_WIDTH) ||
	    (current_height < GAME_UI_HEIGHT))
	{
		endwin();
		return false;
	}
	engine.width  = current_width;
	engine.height = current_height;

	engine.center_top  = engine.height/2 - GAME_UI_HEIGHT/2;
	engine.center_left = engine.width/2  - GAME_UI_WIDTH/2;

	return true;
}

int engine_center_board(struct game_board_t *board, int hscore, bool center, bool redraw)
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

void engine_draw_ui(struct game_board_t *board, int hscore)
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
			if (engine.colors[i - 1] == board->last_color)
				continue;

			y = (i - 1)/4;
			x =  i - 1 - 4*y;

			print_char(engine.center_left + 3*x + 1,
			           engine.center_top + 3*y,
			           engine.cell_appearance,
			           engine.colors[i - 1]);

			print_char(engine.center_left + 3*x + 2,
			           engine.center_top + 3*y,
			           engine.cell_appearance,
			           engine.colors[i - 1]);

			print_char(engine.center_left + 3*x + 1,
			           engine.center_top + 3*y + 1,
			           '1' + (i - 1),
			           engine.text);
		}
		print_string(engine.center_left + 13, engine.center_top, "->", engine.text);
	}
	else
	{
		color_activate(COLOR_WHITE, COLOR_BLACK);
		mvaddstr(engine.center_top,     engine.center_left + 1, "Congrats!");
		mvaddstr(engine.center_top + 1, engine.center_left + 1, "one more game?");
	}

	print_string(engine.center_left + 1, engine.center_top + 6, "nFlood v" VERSION, engine.text);

	print_string(engine.center_left + 1, engine.center_top + 8, "r: New Game", engine.text);
	print_string(engine.center_left + 1, engine.center_top + 9, "q: Quit", engine.text);

	mvprintw(engine.center_top + 11, engine.center_left + 1, "Moves:   %d", board->moves);
	mvprintw(engine.center_top + 12, engine.center_left + 1, "Best:    %d", hscore);

	float percent = board->flood_count/(board->width * board->height) * 100;
	mvprintw(engine.center_top + 13, engine.center_left + 1, "Flooded: %.0f%%", percent);

//	refresh();
}

void engine_draw_board(struct game_board_t *board)
{
	unsigned int i, j;

	for (i = 0; i < (board->width); i++)
	{
		for (j = 0; j < (board->height); j++)
		{
			print_char(engine.center_left + 16 + (i*2),
			           engine.center_top + j,
			           engine.cell_appearance,
			           board->cell[i][j].color);

			print_char(engine.center_left + 16 + (i*2 + 1),
			           engine.center_top + j,
			           engine.cell_appearance,
			           board->cell[i][j].color);
		}
	}
//	refresh();
}
bool is_center()
{
	return !(engine.center_top  == NOT_CENTER_TOP &&
	         engine.center_left == NOT_CENTER_LEFT);
}

bool is_hit(int x, int y, int tx, int ty, int tw, int th)
{
	return x >= tx && x < tx + tw && y >= ty && y < ty + th;
}
void print_char(int x, int y, const chtype c, color_pair_t pair)
{
	color_pair_activate(pair);
	mvaddch(y, x, c);
}
void print_string(int x, int y, const char* str, color_pair_t pair)
{
	color_pair_activate(pair);
	mvaddstr(y, x, str);
}

