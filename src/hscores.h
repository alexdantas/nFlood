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

#include <stdbool.h>

/* Where we'll save the high scores,
 * relative to user's home directory.
 */
#define SCORE_PATH ".local/share/nflood/"
#define SCORE_FILE "nflood.scores"

#define HIGH_SCORE_DEFAULT 25;

/**	Creates/opens the high score file, reading from it if existing.
 *  @return false (0) if something weird happened, else true (1).
 */
bool hscore_init();

/** Opens default high score file, returning a FILE* handle to it.
 *  @note Remember to call `fclose()` later!
 *  @return NULL if couldn't open the file.
 */
FILE* hscore_open(char* flags);

/** Restores highscores file to default.
 *  @note If file doesn't exist, creates it.
 */
void hscore_clean();

/**	Records the player high score at the file. */
bool hscore_store(int hscore);

/** Returns the high score at the file. */
int hscore_get();

#endif /* HSCORES_H_DEFINED */

