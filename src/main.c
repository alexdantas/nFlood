/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 * nFlood - Puzzle game on the terminal.                                                  *
 * Copyright (C) 2011  Alexandre Dantas (kure)                                *
 *                                                                            *
 * This file is part of nFlood.                                            *
 *                                                                            *
 * nFlood is free software: you can redistribute it and/or modify          *
 * it under the terms of the GNU General Public License as published by       *
 * the Free Software Foundation, either version 3 of the License, or          *
 * any later version.                                                         *
 *                                                                            *
 * This program is distributed in the hope that it will be useful,            *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of             *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              *
 * GNU General Public License for more details.                               *
 *                                                                            *
 * You should have received a copy of the GNU General Public License          *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.      *
 *                                                                            *
 * homepage: http://alexdantas.net/projects/nflood                                                       *
 *                                                                            *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/**
 * @file main.c
 * [Insert Brief Description Here]
 */

#include <ncurses.h>
#include <stdlib.h>

#include "engine.h"
#include "game.h"
#include "hscores.h"

int main()
{
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
			// 6 colors bar on top-left
			if ((event.x >= 1 && (event.x - 1) / 2 < 6) && event.y == 1)
			{
				will_flood = 1;
				color = (event.x + 1) / 2;
			}
			// the main board
			else if ((event.x >= 16 && event.x < 16 + GAME_TABLE_WIDTH * 2)
			&&	 (event.y >=  1 && event.y <  1 + GAME_TABLE_HEIGHT))
			{
				will_flood = 1;
				color = board.cell[(event.x - 16) / 2][event.y - 1].color;
			}
			break;
		default:
			break;
		}

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
