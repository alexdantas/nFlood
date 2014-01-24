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
 * homepage: http://nflood.alexdantas.net
 */

#ifndef COLOR_H_DEFINED
#define COLOR_H_DEFINED

/** This file handles all color schemes of nCurses.
 *
 *  Basically, <INSERT EXPLANATION HERE>
 *
 * @note COLORS is nCurses' alias for the maximum number of
 *       currently supported colors.
 *       For old terminals that's 8 and for modern ones is 256.
 *
 */

/** So people don't get confused  */
typedef int color_t;

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

/** Changes how `color` appears on screen based on `hex` string.
 *
 *  @note Same notes as `color_customize` apply.
 *
 *  @note The format accepted or `hex` is "#RRGGBB".
 *        RR, GG and BB are hex values to red, green and blue.
 *
 *  @bug If the format string is invalid, won't do anything.
 */
void color_customize_hex(short color, char* hex);

/** Returns the color pair for `foreground` and `background`.
 *
 *  The value returned is only useful for `color_change_pair`.
 *
 *  @note `foreground` and `background` are internal vaues.
 *        To see them, check the comment at the beginning
 *        of this module.
 */
color_t color_pair(short foreground, short background);

/** Returns the color pair for strings `foreground`
 *  and `background`.
 *
 *  @note Currently accepted values are
 *        the same as `color_from_string`.
 */
color_t color_pair_from_string(char* foreground, char* background);

/** Returns a internal color value from string `str`.
 *
 *  @note Currently accepted values are the most simple colors:
 *        "black", "red", "green", "yellow",
 *        "blue", "magenta", "cyan", "white" and
 *        "default" (for user terminal's default).
 *
 *  @note You'll probably use the returned value on
 *        `color_pair`.
 */
short color_from_string(char* str);

#endif /* COLOR_H_DEFINED */

