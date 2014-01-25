
#include <stdlib.h>
#include <time.h>
#include "game.h"
#include "color.h"
#include "engine.h"

void game_init(struct game_board *board)
{
	int i; int j;

	srand(time(NULL));

	for (i = 0; i < GAME_TABLE_WIDTH; i++)
	{
		for (j = 0; j < GAME_TABLE_HEIGHT; j++)
		{
			board->cell[i][j].flooded = false;

			/* Random color within all possible cell colors */
			int index = random_int_between(0, 5);
			board->cell[i][j].color = engine.colors[index];
		}
	}

	board->flood_count = 0;
	board->moves       = 0;
	board->last_color  = board->cell[0][0].color;

	flood(board, 0, 0, board->cell[0][0].color);
}

bool game_is_over(struct game_board *board)
{
	if (board->flood_count == (GAME_TABLE_WIDTH * GAME_TABLE_HEIGHT))
		return true;
	else
		return false;
}

int flood(struct game_board *board, int x, int y, color_pair_t pair)
{
	if ((x >= GAME_TABLE_WIDTH) || (y >= GAME_TABLE_HEIGHT))
		return 1;

	if ((x < 0) || (y < 0))
		return 1;

	/* The cell is flooded and now we need to refresh it's color */
	if (board->cell[x][y].flooded == true)
	{
		if (board->cell[x][y].color != pair)
		{
			/* Cell' already flooded with the same color */
			board->cell[x][y].color = pair;
		}
		else
			return 1;
	}
	else
	{
		if (board->cell[x][y].color == pair)
		{
			/* Cell's not flooded but the color is the same */
			board->cell[x][y].flooded = true;
			board->flood_count++;
		}

		else
		{
			/* Cell's not flooded and the color is not the same */
			return 1;
		}
	}

	flood(board, x + 1, y,     pair);
	flood(board, x,     y + 1, pair);
	flood(board, x - 1, y,     pair);
	flood(board, x,     y - 1, pair);

	return 0;
}

