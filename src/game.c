
#include <stdlib.h>
#include <time.h>
#include "game.h"
#include "color.h"
#include "engine.h"

void game_init(struct game_board_t *board)
{
	srand(time(NULL));

	/* just to make sure */
	flood(board, 0, 0, board->cell[0][0].color);
}

bool game_is_over(struct game_board_t *board)
{
	return (board->flood_count >= (int)(board->width*board->height));
}


