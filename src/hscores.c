
#include <stdio.h>
#include <string.h>
#include <stdlib.h>				/* getenv() */
#include <sys/stat.h>			/* mkdir() */
#include "hscores.h"

/* Local function that creates a directory tree recurstively,
 * just like `mkdir -p` on UNIX-like systems.
 *
 * Doesn't check for errors though.
 * Based on this: http://stackoverflow.com/a/2336245/1094964
 */
void mkdir_p(const char* dir)
{
	char   tmp[256];
	char*  p = NULL;
	size_t len;

	snprintf(tmp, sizeof(tmp), "%s", dir);
	len = strlen(tmp);

	if (tmp[len - 1] == '/')
		tmp[len - 1] = '\0';

	/* Traverse `dir` making all directories,
	 * temporarily replacing '/' by '\0' so mkdir()
	 * can work.
	 */
	for (p = tmp; *p != '\0'; p++)
	{
		if (*p == '/')
		{
			*p = '\0';
			mkdir(tmp, S_IRWXU);
			*p = '/';
		}
	}
	mkdir(tmp, S_IRWXU);
}

/** Opens default high score file, returning a FILE* handle to it.
 *  @return NULL if couldn't open the file.
 */
FILE* hscore_open(char* flags)
{
	/* We'll expand `~`, leaving here full path + file name */
	char full_path[256];
	memset(full_path, '\0', 256);

	/* If there's no `$HOME`, we'll save the score at `/tmp` */
	if (getenv("HOME") == NULL)
		strncpy(full_path, "/tmp", 256);
	else
		strncpy(full_path, getenv("HOME"), 256);

	/* yep, static strings are concatenated like this */
	strncat(full_path, "/" SCORE_PATH, (256 - strlen(full_path)));

	/* Making sure all directories exist first */
	mkdir_p(full_path);

	strncat(full_path, SCORE_FILE, (256 - strlen(full_path)));

	return fopen(full_path, flags);
}

bool hscore_init()
{
	FILE* file = hscore_open("rb");

	/* First time creating the file */
	if (file == NULL)
	{
		hscore_clean();

		file = hscore_open("rb");

		/* really couldn't open file, we give up */
		if (file == NULL)
			return false;
	}

	fclose(file);
	return true;
}

void hscore_clean()
{
	FILE* file = hscore_open("wb");

	if (file != NULL)
	{
		int val = HIGH_SCORE_DEFAULT;

		fwrite(&val, sizeof(int), 1, file);
		fclose(file);
	}
}

bool hscore_store(int score)
{
	FILE* file = hscore_open("r+b");

	if (file == NULL)
		return false;

	fwrite(&score, sizeof(int), 1, file);
	fclose(file);
	return true;
}

int hscore_get()
{
	FILE* file = hscore_open("rb");

	if (file == NULL)
		return false;

	int hscore = HIGH_SCORE_DEFAULT;
	int items_read = fread(&hscore, sizeof(int), 1, file);
	fclose(file);

	/* badly formatted high score file - maybe erase it? */
	if (items_read != 1)
		return -1;

	return hscore;
}

