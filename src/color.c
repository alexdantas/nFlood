#include <ncurses.h>
#include <string.h>				/* strlen() */
#include <stdlib.h>				/* strtol() */
#include "color.h"
#include "utils.h"

/* Local functions - implementations at the end*/

/* initializes all 8 colors and 8x8 color pairs */
void color_init_basic();

/* initializes all 256 colors and the rest of color pairs*/
void color_init_256();

/* returns a nCurses internal color value from `str` */
short color_from_string(char* str);

/* A stange thing I found was that when calling `color_customize`,
 * it changes the colors from ALL open ncurses programs on the
 * machine!
 *
 * So we need to get previous values and reset after finishing up.
 * These are the default values we'll restore to later:
 */
struct default_color { short r, g, b; };
struct default_color default_colors[256];

bool color_init()
{
	if (has_colors() != TRUE)	/* ncurses BOOL */
		return false;

	start_color();

	/* Local functions that starts all colors */
	color_init_basic();

	if (COLORS == 256)
		color_init_256();

	/* Finally, saving current color definitions.
	 * See the comment right on the beginning
	 * of this file.
	 */
	int i;
	for (i = 1; i < COLORS; i++)
	{
		short r, g, b;
		color_content(i, &r, &g, &b);

		default_colors[i].r = r;
		default_colors[i].g = g;
		default_colors[i].b = b;
	}

	return true;
}
void color_exit()
{
	/* Restoring default color definitions.
	 * See the comment right on the beginning
	 * of this file.
	 */
	int i;
	for (i = 1; i < COLORS; i++)
	{
		color_customize(i,
		                default_colors[i].r,
		                default_colors[i].g,
		                default_colors[i].b);
	}
}
color_t color_random()
{
	return random_int_between(1, COLORS-1);
}

color_t color_random_default()
{
	/* Magic numbers - check `color_init_basic` */
	return random_int_between(253, 263);
}
void color_customize(short color, short r, short g, short b)
{
	if (can_change_color() == FALSE)
		return;

	init_color(color, r, g, b);
}
void color_customize_hex(short color, char* hex)
{
	if (hex[0] != '#')    return; /* sorry */
	if (strlen(hex) != 7) return; /* #RRGGBB format */

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

	color_customize(color, r, g, b);
}
color_t color_pair(short foreground, short background)
{
	/* To see the raw values these formulas use,
	 * go to `color_init_basic` and `color_init_256`.
	 */

	if (foreground == -1)
		return COLOR_PAIR(256 + background);

	if (background == -1)
		return COLOR_PAIR(264 + foreground);

	return COLOR_PAIR(foreground*255 + background);
}
color_t color_pair_from_string(char* foreground, char* background)
{
	short f = color_from_string(foreground);
	short b = color_from_string(background);

	return color_pair(f, b);
}
short color_from_string(char* str)
{
#define STRING_EQUAL(a, b) (strcasecmp(a, b) == 0)

	/* All ncurses' internal values */
	if (STRING_EQUAL(str, "default")) return -1;
	if (STRING_EQUAL(str, "black"))   return 0;
	if (STRING_EQUAL(str, "red"))     return 1;
	if (STRING_EQUAL(str, "green"))   return 2;
	if (STRING_EQUAL(str, "yellow"))  return 3;
	if (STRING_EQUAL(str, "blue"))    return 4;
	if (STRING_EQUAL(str, "magenta")) return 5;
	if (STRING_EQUAL(str, "cyan"))    return 6;
	if (STRING_EQUAL(str, "white"))   return 7;

	return 0;
}

/* Local functions */

void color_init_basic()
{
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
		init_pair(256, COLOR_BLACK,   -1);
		init_pair(257, COLOR_RED,     -1);
		init_pair(258, COLOR_GREEN,   -1);
		init_pair(259, COLOR_YELLOW,  -1);
		init_pair(260, COLOR_BLUE,    -1);
		init_pair(261, COLOR_MAGENTA, -1);
		init_pair(262, COLOR_CYAN,    -1);
		init_pair(263, COLOR_WHITE,   -1);

		init_pair(264, -1, COLOR_BLACK);
		init_pair(265, -1, COLOR_RED);
		init_pair(266, -1, COLOR_GREEN);
		init_pair(267, -1, COLOR_YELLOW);
		init_pair(268, -1, COLOR_BLUE);
		init_pair(269, -1, COLOR_MAGENTA);
		init_pair(270, -1, COLOR_CYAN);
		init_pair(271, -1, COLOR_WHITE);
	}
}
void color_init_256()
{
	/* We have:
	 *
	 * color-0  to color-7:   normal colors
	 * color-8  to color-15:  bright colors
	 * color-15 to color-255: extra  colors
	 *
	 * I'm initializing these colors according
	 * to the "8-bit color specification"
	 *
	 * http://en.wikipedia.org/wiki/8-bit_color
	 */

	/* First, the bright colors */
	init_color(8,  127, 127, 127);
	init_color(9,  255, 0,   0);
	init_color(10, 0,   255, 0);
	init_color(11, 255, 255, 0);
	init_color(12, 92,  92,  255);
	init_color(13, 255, 0,   255);
	init_color(14, 0,   255, 255);
	init_color(15, 255, 255, 255);

	/* Now, the rest of the colors */
	init_color(16, 0, 0, 0);
	init_color(17, 0, 0, 95);
	init_color(18, 0, 0, 135);
	init_color(19, 0, 0, 175);
	init_color(20, 0, 0, 215);
	init_color(21, 0, 0, 255);
	init_color(22, 0, 95, 0);
	init_color(23, 0, 95, 95);
	init_color(24, 0, 95, 135);
	init_color(25, 0, 95, 175);
	init_color(26, 0, 95, 215);
	init_color(27, 0, 95, 255);
	init_color(28, 0, 135, 0);
	init_color(29, 0, 135, 95);
	init_color(30, 0, 135, 135);
	init_color(31, 0, 135, 175);
	init_color(32, 0, 135, 215);
	init_color(33, 0, 135, 255);
	init_color(34, 0, 175, 0);
	init_color(35, 0, 175, 95);
	init_color(36, 0, 175, 135);
	init_color(37, 0, 175, 175);
	init_color(38, 0, 175, 215);
	init_color(39, 0, 175, 255);
	init_color(40, 0, 215, 0);
	init_color(41, 0, 215, 95);
	init_color(42, 0, 215, 135);
	init_color(43, 0, 215, 175);
	init_color(44, 0, 215, 215);
	init_color(45, 0, 215, 255);
	init_color(46, 0, 255, 0);
	init_color(47, 0, 255, 95);
	init_color(48, 0, 255, 135);
	init_color(49, 0, 255, 175);
	init_color(50, 0, 255, 215);
	init_color(51, 0, 255, 255);
	init_color(52, 95, 0, 0);
	init_color(53, 95, 0, 95);
	init_color(54, 95, 0, 135);
	init_color(55, 95, 0, 175);
	init_color(56, 95, 0, 215);
	init_color(57, 95, 0, 255);
	init_color(58, 95, 95, 0);
	init_color(59, 95, 95, 95);
	init_color(60, 95, 95, 135);
	init_color(61, 95, 95, 175);
	init_color(62, 95, 95, 215);
	init_color(63, 95, 95, 255);
	init_color(64, 95, 135, 0);
	init_color(65, 95, 135, 95);
	init_color(66, 95, 135, 135);
	init_color(67, 95, 135, 175);
	init_color(68, 95, 135, 215);
	init_color(69, 95, 135, 255);
	init_color(70, 95, 175, 0);
	init_color(71, 95, 175, 95);
	init_color(72, 95, 175, 135);
	init_color(73, 95, 175, 175);
	init_color(74, 95, 175, 215);
	init_color(75, 95, 175, 255);
	init_color(76, 95, 215, 0);
	init_color(77, 95, 215, 95);
	init_color(78, 95, 215, 135);
	init_color(79, 95, 215, 175);
	init_color(80, 95, 215, 215);
	init_color(81, 95, 215, 255);
	init_color(82, 95, 255, 0);
	init_color(83, 95, 255, 95);
	init_color(84, 95, 255, 135);
	init_color(85, 95, 255, 175);
	init_color(86, 95, 255, 215);
	init_color(87, 95, 255, 255);
	init_color(88, 135, 0, 0);
	init_color(89, 135, 0, 95);
	init_color(90, 135, 0, 135);
	init_color(91, 135, 0, 175);
	init_color(92, 135, 0, 215);
	init_color(93, 135, 0, 255);
	init_color(94, 135, 95, 0);
	init_color(95, 135, 95, 95);
	init_color(96, 135, 95, 135);
	init_color(97, 135, 95, 175);
	init_color(98, 135, 95, 215);
	init_color(99, 135, 95, 255);
	init_color(100, 135, 135, 0);
	init_color(101, 135, 135, 95);
	init_color(102, 135, 135, 135);
	init_color(103, 135, 135, 175);
	init_color(104, 135, 135, 215);
	init_color(105, 135, 135, 255);
	init_color(106, 135, 175, 0);
	init_color(107, 135, 175, 95);
	init_color(108, 135, 175, 135);
	init_color(109, 135, 175, 175);
	init_color(110, 135, 175, 215);
	init_color(111, 135, 175, 255);
	init_color(112, 135, 215, 0);
	init_color(113, 135, 215, 95);
	init_color(114, 135, 215, 135);
	init_color(115, 135, 215, 175);
	init_color(116, 135, 215, 215);
	init_color(117, 135, 215, 255);
	init_color(118, 135, 255, 0);
	init_color(119, 135, 255, 95);
	init_color(120, 135, 255, 135);
	init_color(121, 135, 255, 175);
	init_color(122, 135, 255, 215);
	init_color(123, 135, 255, 255);
	init_color(124, 175, 0, 0);
	init_color(125, 175, 0, 95);
	init_color(126, 175, 0, 135);
	init_color(127, 175, 0, 175);
	init_color(128, 175, 0, 215);
	init_color(129, 175, 0, 255);
	init_color(130, 175, 95, 0);
	init_color(131, 175, 95, 95);
	init_color(132, 175, 95, 135);
	init_color(133, 175, 95, 175);
	init_color(134, 175, 95, 215);
	init_color(135, 175, 95, 255);
	init_color(136, 175, 135, 0);
	init_color(137, 175, 135, 95);
	init_color(138, 175, 135, 135);
	init_color(139, 175, 135, 175);
	init_color(140, 175, 135, 215);
	init_color(141, 175, 135, 255);
	init_color(142, 175, 175, 0);
	init_color(143, 175, 175, 95);
	init_color(144, 175, 175, 135);
	init_color(145, 175, 175, 175);
	init_color(146, 175, 175, 215);
	init_color(147, 175, 175, 255);
	init_color(148, 175, 215, 0);
	init_color(149, 175, 215, 95);
	init_color(150, 175, 215, 135);
	init_color(151, 175, 215, 175);
	init_color(152, 175, 215, 215);
	init_color(153, 175, 215, 255);
	init_color(154, 175, 255, 0);
	init_color(155, 175, 255, 95);
	init_color(156, 175, 255, 135);
	init_color(157, 175, 255, 175);
	init_color(158, 175, 255, 215);
	init_color(159, 175, 255, 255);
	init_color(160, 215, 0, 0);
	init_color(161, 215, 0, 95);
	init_color(162, 215, 0, 135);
	init_color(163, 215, 0, 175);
	init_color(164, 215, 0, 215);
	init_color(165, 215, 0, 255);
	init_color(166, 215, 95, 0);
	init_color(167, 215, 95, 95);
	init_color(168, 215, 95, 135);
	init_color(169, 215, 95, 175);
	init_color(170, 215, 95, 215);
	init_color(171, 215, 95, 255);
	init_color(172, 215, 135, 0);
	init_color(173, 215, 135, 95);
	init_color(174, 215, 135, 135);
	init_color(175, 215, 135, 175);
	init_color(176, 215, 135, 215);
	init_color(177, 215, 135, 255);
	init_color(178, 215, 175, 0);
	init_color(179, 215, 175, 95);
	init_color(180, 215, 175, 135);
	init_color(181, 215, 175, 175);
	init_color(182, 215, 175, 215);
	init_color(183, 215, 175, 255);
	init_color(184, 215, 215, 0);
	init_color(185, 215, 215, 95);
	init_color(186, 215, 215, 135);
	init_color(187, 215, 215, 175);
	init_color(188, 215, 215, 215);
	init_color(189, 215, 215, 255);
	init_color(190, 215, 255, 0);
	init_color(191, 215, 255, 95);
	init_color(192, 215, 255, 135);
	init_color(193, 215, 255, 175);
	init_color(194, 215, 255, 215);
	init_color(195, 215, 255, 255);
	init_color(196, 255, 0, 0);
	init_color(197, 255, 0, 95);
	init_color(198, 255, 0, 135);
	init_color(199, 255, 0, 175);
	init_color(200, 255, 0, 215);
	init_color(201, 255, 0, 255);
	init_color(202, 255, 95, 0);
	init_color(203, 255, 95, 95);
	init_color(204, 255, 95, 135);
	init_color(205, 255, 95, 175);
	init_color(206, 255, 95, 215);
	init_color(207, 255, 95, 255);
	init_color(208, 255, 135, 0);
	init_color(209, 255, 135, 95);
	init_color(210, 255, 135, 135);
	init_color(211, 255, 135, 175);
	init_color(212, 255, 135, 215);
	init_color(213, 255, 135, 255);
	init_color(214, 255, 175, 0);
	init_color(215, 255, 175, 95);
	init_color(216, 255, 175, 135);
	init_color(217, 255, 175, 175);
	init_color(218, 255, 175, 215);
	init_color(219, 255, 175, 255);
	init_color(220, 255, 215, 0);
	init_color(221, 255, 215, 95);
	init_color(222, 255, 215, 135);
	init_color(223, 255, 215, 175);
	init_color(224, 255, 215, 215);
	init_color(225, 255, 215, 255);
	init_color(226, 255, 255, 0);
	init_color(227, 255, 255, 95);
	init_color(228, 255, 255, 135);
	init_color(229, 255, 255, 175);
	init_color(230, 255, 255, 215);
	init_color(231, 255, 255, 255);
	init_color(232, 8, 8, 8);
	init_color(233, 18, 18, 18);
	init_color(234, 28, 28, 28);
	init_color(235, 38, 38, 38);
	init_color(236, 48, 48, 48);
	init_color(237, 58, 58, 58);
	init_color(238, 68, 68, 68);
	init_color(239, 78, 78, 78);
	init_color(240, 88, 88, 88);
	init_color(241, 98, 98, 98);
	init_color(242, 108, 108, 108);
	init_color(243, 118, 118, 118);
	init_color(244, 128, 128, 128);
	init_color(245, 138, 138, 138);
	init_color(246, 148, 148, 148);
	init_color(247, 158, 158, 158);
	init_color(248, 168, 168, 168);
	init_color(249, 178, 178, 178);
	init_color(250, 188, 188, 188);
	init_color(251, 198, 198, 198);
	init_color(252, 208, 208, 208);
	init_color(253, 218, 218, 218);
	init_color(254, 228, 228, 228);
	init_color(255, 238, 238, 238);

	/* Now, the color pairs...
	 *
	 * Normally, we have 8 colors (8x8=64 color pairs).
	 *
	 * That's why from 65 and on we'll have the extended
	 * color pairs.
	 */
	int i, j, k = 65;
	for (i = 16; i <= 255; i++)
	{
		for (j = 16; j <= 255; j++)
		{
			init_pair(k, i, j);
			k++;
		}
	}

}

