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

	struct game_board board;
	int hscore;

	if (engine_init() == -1)
	{
		printf("ERROR: Failed to initialize nCurses");
		return -1;
	}
	engine_set_center(options.center);

	game_init(&board);

	hscore_init();
	hscore = hscore_get();

	engine_draw_ui(&board, hscore);
	engine_draw_board(&board);

	bool will_quit = false;
	while (!will_quit)
	{
		bool will_flood = false;
		MEVENT event;
		color_pair_t color = board.cell[0][0].color;

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
			engine_center_board(&board, hscore, !is_center(), TRUE);
			break;
		case 'r':
			game_init(&board);
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
					color = i;
					will_flood = true;
					break;
				}
			}

			// the main board
			if (is_hit(event.x,
			           event.y,
				   engine.center_left + 16,
				   engine.center_top,
				   GAME_TABLE_WIDTH*2,
				   GAME_TABLE_HEIGHT))
			{
				will_flood = true;
				color = board.cell[(event.x - (engine.center_left + 16)) / 2][event.y - engine.center_top].color;
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
				game_init(&board);
			}
			if (is_hit(event.x,
				   event.y,
				   engine.center_left + 1,
				   engine.center_top + 9,
				   strlen("q: Quit"),
				   1))
			{
				engine_exit();
				return 0;
			}

			break;
		default:
			break;
		}

		if (game_is_over(&board))
			will_quit = true;

		if (will_flood && (board.last_color != color))
		{
			flood(&board, 0, 0, color);
			will_flood = false;
			board.last_color = color;
			board.moves++;
		}

		erase();
		engine_draw_ui(&board, hscore);
		engine_draw_board(&board);

		if (game_is_over(&board))
		{
			hscore = hscore_get();
			if (board.moves < hscore)
				hscore_store(board.moves);
		}
	}

	engine_exit();

	return 0;
}

