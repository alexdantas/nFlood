#ifndef BOARD_H_DEFINED
#define BOARD_H_DEFINED

#include "color.h"

#define DEFAULT_BOARD_WIDTH		14
#define DEFAULT_BOARD_HEIGHT	14

struct game_cell_t
{
	color_pair_t color;
	bool flooded;
};

struct game_board_t
{
	struct game_cell_t** cell; /**< The raw cells (2d matrix) */
	unsigned int width;
	unsigned int height;

	int flood_count;           /**< How many cells are flooded */
	int moves;                 /**< How many moves player did  */

	color_pair_t last_color;   /**< Last flooded color */
};

/** Creates a new board with `width` and `height` */
struct game_board_t* board_new(unsigned int width, unsigned int height);

/** Frees the content of `board` */
void board_free(struct game_board_t* board);

/** Recursive function that floads `board` on `x`/`y`
 *  with color `pair`.
 */
int flood(struct game_board_t *board, int x, int y, color_pair_t pair);

#endif /* BOARD_H_DEFINED */
