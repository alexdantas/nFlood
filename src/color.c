#include <string.h>				/* strlen() */
#include <stdlib.h>				/* strtol() */
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
	for (i = COLOR_BLACK; i <= COLOR_WHITE; i++)
	{
		for (j = COLOR_BLACK; j <= COLOR_WHITE; j++)
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
		/* default background */
		init_pair(64, COLOR_BLACK,   COLOR_DEFAULT);
		init_pair(65, COLOR_RED,     COLOR_DEFAULT);
		init_pair(66, COLOR_GREEN,   COLOR_DEFAULT);
		init_pair(67, COLOR_YELLOW,  COLOR_DEFAULT);
		init_pair(68, COLOR_BLUE,    COLOR_DEFAULT);
		init_pair(69, COLOR_MAGENTA, COLOR_DEFAULT);
		init_pair(70, COLOR_CYAN,    COLOR_DEFAULT);
		init_pair(71, COLOR_WHITE,   COLOR_DEFAULT);
	}
	return true;
}
color_t color_random()
{
	return random_int_between(COLOR_BLACK, COLOR_WHITE);
}
color_t color_rgb(short r, short g, short b)
{
	if (can_change_color() == FALSE)
		return 0;
	if (COLORS < 256)
		return 0;

	static int color_no = 8;
	color_no++;
	if (color_no >= COLORS)
		color_no = 8;

	/* init_color receives values from 0 to 1000 */
	int expand = 1000/255;

	init_color((color_no - 1), r*expand, g*expand, b*expand);
	return (color_no - 1);
}
color_t color_hex(const char* hex)
{
	if (hex[0] != '#')    return 0; /* sorry */
	if (strlen(hex) != 7) return 0; /* #RRGGBB format */

	char col[3];
	col[2] = '\0';

	col[0] = hex[1];
	col[1] = hex[2];
	long r = strtol(col, NULL, 16);

	col[0] = hex[3];
	col[1] = hex[4];
	long g = strtol(col, NULL, 16);

	col[0] = hex[5];
	col[1] = hex[6];
	long b = strtol(col, NULL, 16);

	return color_rgb(r, g, b);
}
color_pair_t color_pair(color_t foreground, color_t background)
{
	/* Basic nCurses colors */
	if ((foreground < 8) && (background < 8))
	{
		if (background == COLOR_DEFAULT)
			return COLOR_PAIR(64 + foreground);

		return COLOR_PAIR(foreground*8 + background);
	}

	if (COLORS < 256)
		return COLOR_PAIR(0);

	/* Will create color pair
	 * (above the 64 regular ones plus 12 default = 72)*/
	static int color_pair_no = 72;
	color_pair_no++;
	if (color_pair_no >= COLOR_PAIRS)
		color_pair_no = 72;

	init_pair((color_pair_no - 1), foreground, background);

	return COLOR_PAIR(color_pair_no - 1);
}
color_t color_from_string(const char* str)
{
#define STRING_EQUAL(a, b) (strcasecmp(a, b) == 0)

	/* All ncurses' internal values */
	if (STRING_EQUAL(str, "default")) return COLOR_DEFAULT;
	if (STRING_EQUAL(str, "black"))   return COLOR_BLACK;
	if (STRING_EQUAL(str, "red"))     return COLOR_RED;
	if (STRING_EQUAL(str, "green"))   return COLOR_GREEN;
	if (STRING_EQUAL(str, "yellow"))  return COLOR_YELLOW;
	if (STRING_EQUAL(str, "blue"))    return COLOR_BLUE;
	if (STRING_EQUAL(str, "magenta")) return COLOR_MAGENTA;
	if (STRING_EQUAL(str, "cyan"))    return COLOR_CYAN;
	if (STRING_EQUAL(str, "white"))   return COLOR_WHITE;

	return 0;
}
color_pair_t color_pair_from_string(const char* foreground, const char* background)
{
	short f = color_from_string(foreground);
	short b = color_from_string(background);

	return color_pair(f, b);
}
void color_activate(color_t foreground, color_t background)
{
	attrset(color_pair(foreground, background));
}
void color_pair_activate(color_pair_t color)
{
	attrset(color);
}

