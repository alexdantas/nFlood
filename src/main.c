/*  _      ____  _     ___   ___   ___
 * | |\ | | |_  | |   / / \ / / \ | | \
 * |_| \| |_|   |_|__ \_\_/ \_\_/ |_|_/
 */

#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

#include "engine.h"
#include "game.h"
#include "hscores.h"
#include "arguments.h"
#include "options.h"
#include "color.h"

int main(int argc, char* argv[])
{
	options_init();
	arguments_parse(argc, argv);

	int hscore;

	if (engine_init() == -1)
	{
		fprintf(stderr, "ERROR: Failed to initialize nCurses");
		return -1;
	}
	engine_set_center(options.center);

	options.game_width = 5;
	options.game_height = 5;

	struct game_board_t* board = board_new(options.game_width,
	                                       options.game_height);
	if (board == NULL)
	{
		engine_exit();
		fprintf(stderr, "ERROR: Couldn't allocate memory");
		return 666;
	}

	game_init(board);

	hscore_init();
	hscore = hscore_get();

	engine_draw_ui(board, hscore);
	engine_draw_board(board);
	refresh();

	bool will_quit = false;
	while (!will_quit)
	{
		bool will_flood = false;
		MEVENT event;
		color_pair_t color = board->cell[0][0].color;

		int c = getch();

		switch (c)
		{
		case 'q':
			will_quit = true;
			break;

		case '1': will_flood = true; color = engine.colors[0]; break;
		case '2': will_flood = true; color = engine.colors[1]; break;
		case '3': will_flood = true; color = engine.colors[2]; break;
		case '4': will_flood = true; color = engine.colors[3]; break;
		case '5': will_flood = true; color = engine.colors[4]; break;
		case '6': will_flood = true; color = engine.colors[5]; break;

		case 'c':
			engine_center_board(board, hscore, !is_center(), TRUE);
			break;

		case 'r':
			board_free(board);
			board = board_new(options.game_width,
			                  options.game_height);
			game_init(board);
			break;

		case KEY_MOUSE:
			if (getmouse(&event) != OK || !(event.bstate & BUTTON1_CLICKED))
				break;

			int i, x, y;
			for (i = 1; i <= 6; i++)
			{
				y = (i - 1)/4;
				x = i - 1 - 4*y;
				if (is_hit(event.x,
				           event.y,
					   engine.center_left + 3*x + 1,
					   engine.center_top + 3*y,
					   2,
					   1))
				{
					color = engine.colors[i - 1];
					will_flood = true;
					break;
				}
			}

			// the main board
			if (is_hit(event.x,
			           event.y,
				   engine.center_left + 16,
				   engine.center_top,
				   board->width*2,
				   board->height))
			{
				will_flood = true;
				color = board->cell[(event.x - (engine.center_left + 16)) / 2][event.y - engine.center_top].color;
			}

			// menu
			// FIXME should use some data structure with
			// engine_draw_ui() without needing to use hard-coded
			// position and text
			if (is_hit(event.x,
				   event.y,
				   engine.center_left + 1,
				   engine.center_top + 8,
				   strlen("r: New Game"),
				   1))
			{
				game_init(board);
			}
			if (is_hit(event.x,
				   event.y,
				   engine.center_left + 1,
				   engine.center_top + 9,
				   strlen("q: Quit"),
				   1))
				will_quit = true;
			break;

		default:
			break;
		}

		if (game_is_over(board))
			will_quit = true;

		if (will_flood && (board->last_color != color))
		{
			flood(board, 0, 0, color);
			will_flood = false;
			board->last_color = color;
			board->moves++;
		}

		erase();
		engine_draw_ui(board, hscore);
		engine_draw_board(board);

		if (game_is_over(board))
		{
			hscore = hscore_get();
			if (board->moves < hscore)
				hscore_store(board->moves);
		}
	}
	board_free(board);
	engine_exit();

	return 0;
}

