
#include <stdlib.h>
#include <time.h>
#include "game.h"
#include "color.h"
#include "engine.h"

struct game_board_t* board_new(unsigned int width, unsigned int height)
{
	struct game_board_t* board = malloc(sizeof(struct game_board_t));
	if (board == NULL)
		return NULL;

	/* We'll create a 2D matrix of cells */
	board->cell = calloc(width, sizeof(struct game_cell_t*));
	if (board->cell == NULL)
		return NULL;

	unsigned int i, j;
	for (i = 0; i < width; i++)
	{
		board->cell[i] = calloc(height, sizeof(struct game_cell_t));

		if (board->cell[i] == NULL)
			return NULL;

		/* Yes, I should've freed() everything up til now
		 * But if this function fails, I expect the program to
		 * abort. Then, all memory malloc()'d will be free()'d
		 * by the operational system.
		 */
	}


	/* Initializing all cells */
	for (i = 0; i < width; i++)
	{
		for (j = 0; j < height; j++)
		{
			/* Random color within all possible cell colors */
			int index = random_int_between(0, 5);
			board->cell[i][j].color = engine.colors[index];

			board->cell[i][j].flooded = false;
		}
	}

	board->width       = width;
	board->height      = height;
	board->flood_count = 1;
	board->moves       = 0;
	board->last_color  = board->cell[0][0].color;

	return board;
}
void board_free(struct game_board_t* board)
{
	if (board == NULL)
		return;

	unsigned int i;
	for (i = 0; i < (board->width); i++)
		free(board->cell[i]);

	free(board->cell);
	free(board);
	board = NULL;
}

void game_init(struct game_board_t *board)
{
	srand(time(NULL));

	flood(board, 0, 0, board->cell[0][0].color);
}

bool game_is_over(struct game_board_t *board)
{
	return (board->flood_count == (board->width*board->height));
}

int flood(struct game_board_t *board, unsigned int x, unsigned int y, color_pair_t pair)
{
	if ((x >= board->width) || (y >= board->height))
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

