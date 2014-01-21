
#include <stdlib.h>
#include <time.h>
#include "game.h"

/** Initializes the `board` with random colors for
 *  all cells, cleaning the `flooded` state
 */
void game_init(struct game_board *board)
{
	int i; int j;

	srand(time(NULL));

	for (i = 0; i < GAME_TABLE_WIDTH; i++)
		for (j = 0; j < GAME_TABLE_HEIGHT; j++)
		{
			board->cell[i][j].flooded = 0;
			board->cell[i][j].color = random_int_between(6, 1); //TODO magic numbers?
		}

	board->flood_count = 0;
	board->moves       = 0;
	board->last_color  = board->cell[0][0].color;

	flood(board, 0, 0, board->cell[0][0].color);
}

/** Tells if the board is completely flooded.
 */
bool game_is_over(struct game_board *board)
{
	if (board->flood_count == (GAME_TABLE_WIDTH * GAME_TABLE_HEIGHT))
		return true;
	else
		return false;
}

/** Returns a random integer between the limits
 *  passed as parameters.
 */
int random_int_between (int upper, int lower)
{
	return rand() % (upper - lower + 1) + lower;
}

/** Recursive function that floads `board` on `x`/`y`
 *  with `color`.
 */
int flood (struct game_board *board, int x, int y, int color)
{
	if ((x >= GAME_TABLE_WIDTH) || (y >= GAME_TABLE_HEIGHT))
		return 1;
	if ((x < 0) || (y < 0))
		return 1;

	// The cell is flooded and now we need to refresh it's color
	if (board->cell[x][y].flooded == true)
		if (board->cell[x][y].color != color)
			board->cell[x][y].color = color;
	// The cell is already flooded and with the same color
		else
			return 1;
	else
	{
		// The cell is not flooded but the color is the same
		if (board->cell[x][y].color == color)
		{
			board->cell[x][y].flooded = true;
			board->flood_count++;
		}
		// The cell is not flooded and the color is not the same
		else
			return 1;
	}

	flood (board, x + 1, y,     color);
	flood (board, x,     y + 1, color);
	flood (board, x - 1, y,     color);
	flood (board, x,     y - 1, color);

/*

  if (board->cell[i][j].color != color)
  return 1;
  if (board->cell[i][j].flooded == FLOODED)
  return 1;

  board->cell[i][j].flooded = FLOODED;
  board->flood_count++;

*/
	return 0;
}

