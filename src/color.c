#include <ncurses.h>
#include "color.h"
#include "utils.h"

bool color_init()
{
	if (has_colors() != TRUE)	/* ncurses BOOL */
		return false;

	start_color();

	/* This is a big hack to initialize all 64
	 * possible color pairs in ncurses.
	 *
	 * The thing is, all colors are between
	 * COLOR_BLACK and COLOR_WHITE.
	 * Since I've set a large number of enums covering
	 * all possibilities, I can do it all in a for loop.
	 * Check 'man init_pair' for more details.
	 *
	 * Here's the internal value of colors,
	 * taken straight from <curses.h>:
	 *
	 * #define COLOR_BLACK   0
	 * #define COLOR_RED     1
	 * #define COLOR_GREEN   2
	 * #define COLOR_YELLOW  3
	 * #define COLOR_BLUE    4
	 * #define COLOR_MAGENTA 5
	 * #define COLOR_CYAN    6
	 * #define COLOR_WHITE   7
	 */
	int i, j, k = 1;
	for (i = (COLOR_BLACK); i <= (COLOR_WHITE); i++)
	{
		for (j = (COLOR_BLACK); j <= (COLOR_WHITE); j++)
		{
			init_pair(k, i, j);
			k++;
		}
	}

	/* Besides the normal color pairs, we can use
	 * whatever colors the user has currently set to
	 * their terminal.
	 * It looks more "natural".
	 *
	 * So COLOR_PAIR(-1, -1) is the default foreground
	 * and background.
	 *
	 * Let's do it if the current terminal supports it.
	 */
	if (use_default_colors() != ERR)
	{
		init_pair(BLACK_DEFAULT,    COLOR_BLACK,   -1);
		init_pair(RED_DEFAULT,      COLOR_RED,     -1);
		init_pair(GREEN_DEFAULT,    COLOR_GREEN,   -1);
		init_pair(YELLOW_DEFAULT,   COLOR_YELLOW,  -1);
		init_pair(BLUE_DEFAULT,     COLOR_BLUE,    -1);
		init_pair(MAGENTA_DEFAULT,  COLOR_MAGENTA, -1);
		init_pair(CYAN_DEFAULT,     COLOR_CYAN,    -1);
		init_pair(WHITE_DEFAULT,    COLOR_WHITE,   -1);
	}
	else
	{
		/* If the terminal doesn't support, we'll default
		 * to black background anyways*/
		init_pair(BLACK_DEFAULT,    COLOR_BLACK,   COLOR_BLACK);
		init_pair(RED_DEFAULT,      COLOR_RED,     COLOR_BLACK);
		init_pair(GREEN_DEFAULT,    COLOR_GREEN,   COLOR_BLACK);
		init_pair(YELLOW_DEFAULT,   COLOR_YELLOW,  COLOR_BLACK);
		init_pair(BLUE_DEFAULT,     COLOR_BLUE,    COLOR_BLACK);
		init_pair(MAGENTA_DEFAULT,  COLOR_MAGENTA, COLOR_BLACK);
		init_pair(CYAN_DEFAULT,     COLOR_CYAN,    COLOR_BLACK);
		init_pair(WHITE_DEFAULT,    COLOR_WHITE,   COLOR_BLACK);
	}
	return true;
}

color_t color_random()
{
	return random_int_between(BLACK_BLACK, WHITE_WHITE);
}

color_t color_random_default()
{
	return random_int_between(BLACK_DEFAULT, WHITE_DEFAULT);
}
void color_customize(short color, short r, short g, short b)
{
	if (can_change_color() == FALSE)
		return;

	init_color(color, r, g, b);
}

