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

// Enqueues all possible valid moves from a given game state to the front of the queue
void enqueue(struct queue *q, struct game_state state)
{
    // Move empty tile UP if not already at the bottom row
    if (state.empty_row != 3)
    {
        struct game_state *up_state = copy_state(&state);
        move_up(up_state);
        up_state->num_steps = state.num_steps + 1;
        insert_at_head(&(q->data), serialize(*up_state));
        free(up_state);
    }

    if (state.empty_row != 0)
    {
        struct game_state *down_state = copy_state(&state);
        move_down(down_state);
        down_state->num_steps = state.num_steps + 1;
        insert_at_head(&(q->data), serialize(*down_state));
        free(down_state);
    }

    if (state.empty_col != 3)
    {
        struct game_state *left_state = copy_state(&state);
        move_left(left_state);
        left_state->num_steps = state.num_steps + 1;
        insert_at_head(&(q->data), serialize(*left_state));
        free(left_state);
    }

    if (state.empty_col != 0)
    {
        struct game_state *right_state = copy_state(&state);
        move_right(right_state);
        right_state->num_steps = state.num_steps + 1;
        insert_at_head(&(q->data), serialize(*right_state));
        free(right_state);
    }
}

// Removes the next game state from the back of the queue and returns the deserialized struct
struct game_state dequeue(struct queue *q)
{
    // dequeue removes from the _tail_
    return deserialize(remove_from_tail(&(q->data)));
}

// Checks if the current state is the solved configuration
size_t finished_check(struct game_state cur_state)
{
    for (int i = 0; i < 15; i++)
    {
        if (i + 1 != cur_state.tiles[i / 4][i % 4])
        {
            return 0;
        }
    }
    return 1;
}

int number_of_moves(struct game_state start)
{
    const int MAX_MOVES = 9; // You can adjust this value

    struct queue *q = (struct queue *)malloc(sizeof(struct queue));
    if (q == NULL)
    {
        return 0;
    }
    q->data.head = NULL;

    // Insert the starting state into the queue
    insert_at_head(&(q->data), serialize(start));

    // Process all possible states in BFS manner until solved or out of states
    while (q->data.head != NULL)
    {
        struct game_state cur_state = dequeue(q);

        if (cur_state.num_steps > MAX_MOVES)
        {
            // Clean up and return -1 if we exceed allowed max depth
            free_list(q->data);
            free(q);
            return -1;
        }

        // Check if the current state is solved
        if (finished_check(cur_state))
        {
            free_list(q->data);
            free(q);
            printf("steps: %d\n", cur_state.num_steps);
            return cur_state.num_steps;
        }

        enqueue(q, cur_state);
    }

    // If no solution was found within the allowed steps
    free_list(q->data);
    free(q);
    return -1;
}
