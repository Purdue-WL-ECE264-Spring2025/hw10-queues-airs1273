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
    struct queue *q = (struct queue *)malloc(sizeof(struct queue));
    insert_at_head(&(q->data), serialize(start));

    uint64_t visited[100000];
    size_t visited_count = 0;

    int max_iterations = 1000000; // Limit the number of while loop iterations
    int iterations = 0;

    while (q->data.head != NULL)
    {
        if (iterations++ >= max_iterations)
        {
            free_list(q->data);
            free(q);
            return -1; // Timeout or too deep
        }

        struct game_state cur_state = dequeue(q);
        uint64_t ser = serialize(cur_state);

        // Check if already visited
        int already_seen = 0;
        for (size_t i = 0; i < visited_count; i++)
        {
            if (visited[i] == ser)
            {
                already_seen = 1;
                break;
            }
        }

        if (already_seen)
            continue;

        // Mark as visited
        visited[visited_count++] = ser;

        // Check if solved
        if (finished_check(cur_state))
        {
            free_list(q->data);
            free(q);
            return cur_state.num_steps;
        }

        // Enqueue all next states (let enqueue handle move gen)
        enqueue(q, cur_state);
    }

    free_list(q->data);
    free(q);
    return -1;
}
