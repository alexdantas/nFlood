
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>				/* exit() */
#include "arguments.h"
#include "options.h"

/* These are local functions that may be called
 * depending on the arguments passed by the user
 */

void version(command_t* self)
{
	(void)(self);

	printf("nFlood v"VERSION" ("DATE")\n");

	command_free(self);
	exit(EXIT_SUCCESS);
}
void help(command_t* self)
{
	(void)(self);

	printf("            _______ __                __\n"
	       "     .-----|    ___|  .-----.-----.--|  |\n"
	       "     |     |    ___|  |  _  |  _  |  _  |\n"
	       "     |__|__|___|   |__|_____|_____|_____|\n"
	       "     v"VERSION"               (built "DATE")\n"
	       "\n"
	       "nFlood is a simple clone of the Flood-It puzzle game.\n"
	       "\n"
	       "You need to fill the whole board with a single color.\n"
	       "To do that, alternate between them, flooding with the\n"
	       "minimum number of moves possible.\n"
	       "\n"
	       "Usage:\n"
	       "	nflood [options]\n"
	       "\n"
	       "	-h, --help     Show this message\n"
	       "	-v, --version  Show game version and build date\n"
	       "	-W <WIDTH>, --width  <WIDTH>\n"
	       "	               Set width of the game board\n"
	       "	-H <HEIGHT>, --height <HEIGHT>\n"
	       "	               Set height of the game board\n"
	       "	-c, --center   Center the game board at start\n");

	command_free(self);
	exit(EXIT_SUCCESS);
}

void width(command_t* self)
{
	(void)(self);

	options.game_width = atoi(self->arg);
}

void height(command_t* self)
{
	(void)(self);

	options.game_height = atoi(self->arg);
}

void center(command_t* self)
{
	(void)(self);

	options.center = true;
}

void arguments_parse(int argc, char* argv[])
{
	/* commander internal data structure */
	command_t cmd;
	command_init(&cmd, argv[0], VERSION);

	command_option(&cmd, "-v", "--version", "Show game version and build date", version);
	command_option(&cmd, "-h", "--help",    "Show instructions", help);

	command_option(&cmd, "-W", "--width <WIDTH>",  "Set width of the game board", width);
	command_option(&cmd, "-H", "--height <HEIGHT>", "Set height of the game board", height);

	command_option(&cmd, "-c", "--center", "Center the game board at start", center);

	command_parse(&cmd, argc, argv);
	command_free(&cmd);
}

