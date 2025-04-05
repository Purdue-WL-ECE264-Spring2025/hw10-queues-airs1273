#include "queue.h"
#include "tile_game.h"
#include <stdlib.h>
#include <string.h>

// Used to copy game state so we can queue 4 possible moves
struct game_state *copy_state(struct game_state *state)
{
    struct game_state *new_state = (struct game_state *)malloc(sizeof(struct game_state));
    if (!new_state)
    {
        return NULL;
    }
    memcpy(new_state, state, sizeof(struct game_state));
    return new_state;
}

void enqueue(struct queue *q, struct game_state state)
{
    // inserts at the _head_
    // check if it is possible to move up
    if (state.empty_row != 3)
    {
        struct game_state *up_state = copy_state(&state);
        move_up(up_state);
        insert_at_head(&(q->data), serialize(*up_state));
        free(up_state);
    }

    // check if it is possible to move down
    if (state.empty_row != 0)
    {
        struct game_state *down_state = copy_state(&state);
        move_down(down_state);
        insert_at_head(&(q->data), serialize(*down_state));
        free(down_state);
    }

    // check if it is possible to move left
    if (state.empty_col != 3)
    {
        struct game_state *left_state = copy_state(&state);
        move_left(left_state);
        insert_at_head(&(q->data), serialize(*left_state));
        free(left_state);
    }

    // check if it is possible to move right
    if (state.empty_col != 0)
    {
        struct game_state *right_state = copy_state(&state);
        move_right(right_state);
        insert_at_head(&(q->data), serialize(*right_state));
        free(right_state);
    }
}

struct game_state dequeue(struct queue *q)
{
    // dequeue removes from the _tail_
    return deserialize(remove_from_tail(&(q->data)));
}

size_t finished_check(struct game_state cur_state)
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (i * 4 + j != cur_state.tiles[i][j])
            {
                return 0;
            }
        }
    }
    return 1;
}

int number_of_moves(struct game_state start)
{
    // implement a BFS that determines the shortest moves to solve a Tiles game
    struct queue *q = (struct queue *)malloc(sizeof(struct queue));

    // inserts start state into ll
    insert_at_head(&(q->data), serialize(start));

    // we run until there are no more cases; runs forever if solution is not found
    int max_cases = 50;
    while (q->data.head != NULL && max_cases < 50)
    {
        // dequeue next in line
        struct game_state cur_state = dequeue(q);
        // check if we reached final state
        if (finished_check(cur_state) != 0)
        {
            // clean up
            free_list(q->data);
            free(q);
            // return steps taken to reach end
            return cur_state.num_steps;
        }
        // loads next possible moves
        enqueue(q, cur_state);
        max_cases++;
    }
    // clean up
    free_list(q->data);
    free(q);
    return 0;
}
