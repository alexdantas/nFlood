/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 * nSnake - The classic snake game with ncurses.                              *
 * Copyright (C) 2011-2012  Alexandre Dantas (kure)                           *
 *                                                                            *
 * This file is part of nSnake.                                               *
 *                                                                            *
 * nSnake is free software: you can redistribute it and/or modify             *
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
 * homepage: http://sourceforge.net/projects/nsnake/                          *
 *                                                                            *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/** @file hscores.c
 *
 *	Definition of the functions related to high scores.
 */

//	We also get a define from the Makefile called SCORE_PATH
//	It indicates the whole path to the scores file.
//	By default, it is /var/games/nsnake/high.scores
//	but it depends on the installation directory set by the user


#define SCORE_PATH "./nflood.scores"

// If we're on Windows, there's no /var/games!
// So we create the file on the same directory as the .exe
#if (defined __WIN32__) && (!defined __CYGWIN__)
	#undef  SCORE_PATH
	#define SCORE_PATH "hscores.bin"
#endif


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hscores.h"


/** The Default HighScore */
int HIGH_SCORE_DEFAULT = 25;

/** Restore the Highscores to the default.
 *
 *  @note If the Highscores file doesn't exist, it is created.
 */
void hscore_clean ()
{
	FILE* file = fopen (SCORE_PATH, "wb");

	if (file != NULL)
	{
		fwrite (&HIGH_SCORE_DEFAULT, sizeof (int), 1, file);
		fclose (file);
	}
}


/**	Creates/reads from the High Score file
 *
 * 	First, we try to open for reading. If it doesn't exist, then we
 *  open it for writing, effectively creating it.
 *
 *  @note The path to the score file is defined by the Makefile.
 *        Currently, it is /var/games/.nsnake.scores
 *	@todo clean this code. Lots of useless ifs and elses.
 */
int hscore_init ()
{
	FILE* file = fopen (SCORE_PATH, "rb");

	int hscore;

	if (file == NULL)
		hscore_clean ();

	else
	{
		int items_read;

		items_read = fread (&hscore, sizeof (int), 1, file);
		if (items_read != 1)
			return -1;
	}

	file = fopen (SCORE_PATH, "rb");
	if (file == NULL)
		//This time it really couldnt open the score file
		return -1;
	else
		fclose (file);

	return 0;
}


/**	Records the player High Score
 */
int hscore_store (int score)
{
	FILE* file = fopen (SCORE_PATH, "r+b");

	if (file == NULL)
		hscore_clean ();

	if (file == NULL)
	{
		// We really couldn't open the high scores file
		return -1;
	}
	else
	{
		fwrite (&score, sizeof (int), 1, file);
		fclose (file);
	}
	return 0;
}

int hscore_get ()
{
	int hscore = HIGH_SCORE_DEFAULT;
	FILE* file = fopen (SCORE_PATH, "rb");

	if (file == NULL)
		hscore_clean();

	else
	{
		int items_read;

		items_read = fread (&hscore, sizeof (int), 1, file);
		if (items_read != 1)
			//nsnake_abort ("Highscore File I/O error!\nTry cleaning the scores file!");
			return -1;
	}

	file = fopen (SCORE_PATH, "rb");
	if (file == NULL)
	{
		//This time it really couldnt open the score file
		//cant_open_hscore_file = 1;
		return -1;
	}
	else
		fclose (file);

	return hscore;
}
