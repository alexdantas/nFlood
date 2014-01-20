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
 * @file engine.h
 * [Insert Brief Description Here]
 */


#ifndef ENGINE_H_DEFINED
#define ENGINE_H_DEFINED

#include "game.h"


int engine_init ();
void engine_draw_board (struct game_board *board);
void engine_exit ();
void change_color (int color);
void engine_draw_ui (struct game_board *board, int hscore);


#endif /* ENGINE_H_DEFINED */
