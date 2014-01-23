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

int main(int argc, char* argv[])
{
	arguments_parse(argc, argv);

	struct game_board board;
	int hscore;

	if (engine_init() == -1)
	{
		printf("ERROR: Failed to initialize nCurses");
		return -1;
	}

	game_init(&board);

	hscore_init();
	hscore = hscore_get();

	engine_draw_ui(&board, hscore);
	engine_draw_board(&board);

	int c = 0;
	while (c != 'q')
	{
		int will_flood = 0;
		int color = 0;
		MEVENT event;

		c = getch();

		switch (c)
		{
		case '1': will_flood = 1; color = BLUE;    break;
		case '2': will_flood = 1; color = MAGENTA; break;
		case '3': will_flood = 1; color = RED;     break;
		case '4': will_flood = 1; color = YELLOW;  break;
		case '5': will_flood = 1; color = GREEN;   break;
		case '6': will_flood = 1; color = WHITE;   break;
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
					will_flood = 1;
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
				will_flood = 1;
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
			continue;

		if (will_flood && board.last_color != color)
		{
			flood(&board, 0, 0, color);
			will_flood = 0;
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

