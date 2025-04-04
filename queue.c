#include "queue.h"
#include "tile_game.h"

void enqueue(struct queue *q, struct game_state state)
{
    // inserts at the _head_
}

struct game_state dequeue(struct queue *q)
{
    // dequeue removes from the _tail_
    return (struct game_state){0};
}

int number_of_moves(struct game_state start)
{
    // implement a BFS that determines the shortest moves to solve a Tiles game
    return 0;
}
