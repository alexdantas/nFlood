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
 * @file game.h
 * [Insert Brief Description Here]
 */

#ifndef GAME_H_DEFINED
#define GAME_H_DEFINED

enum cell_colors { BLUE = 1, MAGENTA, RED, YELLOW, GREEN, WHITE };
enum flooded_state { FLOODED = 1, NOT_FLOODED = 0};

#define GAME_WIDTH  44
#define GAME_HEIGHT 16

#define GAME_TABLE_WIDTH  14
#define GAME_TABLE_HEIGHT 14

struct game_cell
{
	int color;
	int flooded;
};

struct game_board
{
	struct game_cell cell[GAME_TABLE_WIDTH][GAME_TABLE_HEIGHT];
	int finished;
	int flood_count; /** Keeps counting how many cells have been flooded */
	int moves;
};

void game_init(struct game_board *board);
int game_is_over(struct game_board *board);
int flood (struct game_board *board, int x, int y, int color);
int random_int_between (int upper, int lower);


#endif /* GAME_H_DEFINED */
