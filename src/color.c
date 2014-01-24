#include <ncurses.h>
#include "color.h"

bool color_init()
{
	if (has_colors() != TRUE)	/* ncurses BOOL */
		return false;

	int bg_color;

	start_color();

	/* Let's try grabbing the current terminal
	 * background color
	 */
	if (use_default_colors () == ERR)
		bg_color = COLOR_BLACK;
	else
		bg_color = -1;

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
	return true;
}

