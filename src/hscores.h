/* nFlood - Flood-it puzzle game on the terminal.
 * Copyright (C) 2011-2014  Alexandre Dantas <eu@alexdantas.net>
 * Copyright (C) 2014 Yu-Jie Lin <livibetter@gmail.com>
 *
 * nFlood is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * homepage: http://nflood.alexdantas.net/
 */

#ifndef HSCORES_DEFINED
#define HSCORES_DEFINED

/** HighScore of the game mode with borders */
extern int HIGH_SCORE_BORDERS;

/** HighScore of the game mode without borders */
extern int HIGH_SCORE_BORDERS_OFF;

/** Restore the Highscores to the default.
 *
 *  @note If the Highscores file doesn't exist, it is created.
 */
void hscore_clean();

/**	Creates/reads from the High Score file
 *
 * 	First, we try to open for reading. If it doesn't exist, then we
 *  open it for writing, effectively creating it.
 *
 *  @note The path to the score file is defined by the Makefile.
 *        Currently, it is /var/games/.nsnake.scores
 *	@todo clean this code. Lots of useless ifs and elses.
 */
int hscore_init();

/**	Records the player High Score
 */
int hscore_store(int hscore);

int hscore_get();

#endif /* HSCORES_H_DEFINED */

