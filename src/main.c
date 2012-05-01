/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 * _PROJECT_ - _DESCRIPTION_                                                  *
 * Copyright (C) 2011  Alexandre Dantas (kure)                                *
 *                                                                            *
 * This file is part of _PROJECT_.                                            *
 *                                                                            *
 * _PROJECT_ is free software: you can redistribute it and/or modify          *
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
 * homepage: _HOMEPAGE_                                                       *
 *                                                                            *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/**
 * @file main.c
 * [Insert Brief Description Here]
 */


#include <stdlib.h>
#include "ncurses.h"
#include "engine.h"
#include "game.h"


int main(int argc, char **argv)
{
	struct game_board board;

	if (engine_init() == -1)
		return -1;

	game_init(&board);

	engine_draw_ui(&board);
	engine_draw_board(&board);

	int c;
	do {
		c = getch();

		switch (c)
		{
		case '1':
			flood(&board, 0, 0, BLUE);
			board.moves++;
			break;
		case '2':
			flood(&board, 0, 0, MAGENTA);
			board.moves++;
			break;
		case '3':
			flood(&board, 0, 0, RED);
			board.moves++;
			break;
		case '4':
			flood(&board, 0, 0, YELLOW);
			board.moves++;
			break;
		case '5':
			flood(&board, 0, 0, GREEN);
			board.moves++;
			break;
		case '6':
			flood(&board, 0, 0, WHITE);
			board.moves++;
			break;
		case 'r':
			game_init(&board);
			break;
		default:
			break;
		}

		clear();
		engine_draw_ui(&board);
		engine_draw_board(&board);

	} while (c != 'q');


	engine_exit();

	return 0;
}
