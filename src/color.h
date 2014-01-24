#ifndef COLOR_H_DEFINED
#define COLOR_H_DEFINED

/** Possible engine color pairs (FOREGROUND_BACKGROUND).
 *  This defines all possible combinations of color pairs
 *  on ncurses.
 *  Note that the order of definition is important,
 *  also the intial value of 1.
 *
 */
typedef enum
{
	BLACK_BLACK = 1, BLACK_RED,   BLACK_GREEN,   BLACK_YELLOW,   BLACK_BLUE,   BLACK_MAGENTA,   BLACK_CYAN,   BLACK_WHITE,
	RED_BLACK,       RED_RED,     RED_GREEN,     RED_YELLOW,     RED_BLUE,     RED_MAGENTA,     RED_CYAN,     RED_WHITE,
	GREEN_BLACK,     GREEN_RED,   GREEN_GREEN,   GREEN_YELLOW,   GREEN_BLUE,   GREEN_MAGENTA,   GREEN_CYAN,   GREEN_WHITE,
	YELLOW_BLACK,    YELLOW_RED,  YELLOW_GREEN,  YELLOW_YELLOW,  YELLOW_BLUE,  YELLOW_MAGENTA,  YELLOW_CYAN,  YELLOW_WHITE,
	BLUE_BLACK,      BLUE_RED,    BLUE_GREEN,    BLUE_YELLOW,    BLUE_BLUE,    BLUE_MAGENTA,    BLUE_CYAN,    BLUE_WHITE,
	MAGENTA_BLACK,   MAGENTA_RED, MAGENTA_GREEN, MAGENTA_YELLOW, MAGENTA_BLUE, MAGENTA_MAGENTA, MAGENTA_CYAN, MAGENTA_WHITE,
	CYAN_BLACK,      CYAN_RED,    CYAN_GREEN,    CYAN_YELLOW,    CYAN_BLUE,    CYAN_MAGENTA,    CYAN_CYAN,    CYAN_WHITE,
	WHITE_BLACK,     WHITE_RED,   WHITE_GREEN,   WHITE_YELLOW,   WHITE_BLUE,   WHITE_MAGENTA,   WHITE_CYAN,   WHITE_WHITE,
	BLACK_DEFAULT,   RED_DEFAULT, GREEN_DEFAULT, YELLOW_DEFAULT, BLUE_DEFAULT, MAGENTA_DEFAULT, CYAN_DEFAULT, WHITE_DEFAULT
} color_t;

/** Initializes color support and all color pairs.
 *  @return false (0) if something weird happened, else true (1).
 */
bool color_init();

/** Restores color support.
 *  @note Need to be called before quitting the program!
 */
void color_exit();

/** Returns a random color within all 64 possible color pairs.
 *  @note Doesn't include default background
 */
color_t color_random();

/** Returns a random color with default background
 *  @note 8 possibilities.
 */
color_t color_random_default();

/** Changes how `color` appears on the screen.
 *
 *  @note You can only change the 8 default nCurses colors:
 *        COLOR_BLACK, COLOR_RED, COLOR_GREEN, COLOR_YELLOW,
 *        COLOR_BLUE, COLOR_MAGENTA, COLOR_CYAN and COLOR_WHITE.
 *
 *  @note `r`, `g` and `b` accept values from 0 to 1000.
 */
void color_customize(short color, short r, short g, short b);

#endif /* COLOR_H_DEFINED */

