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
    uint64_t serialized = serialize(state);

    // Check if the serialized state already exists in the queue
    struct list_node *node = q->data.head;
    while (node != NULL)
    {
        if (node->value == serialized)
        {
            return; // Already in queue, skip enqueue
        }
        node = node->next;
    }

    insert_at_tail(&(q->data), serialized);
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
    struct queue *q = malloc(sizeof(struct queue));
    insert_at_head(&(q->data), serialize(start));

    uint64_t visited[100000];
    size_t visited_count = 0;

    int max_iterations = 1000000;
    int iterations = 0;

    while (q->data.head != NULL)
    {
        if (++iterations >= max_iterations)
        {
            free_list(q->data);
            free(q);
            return -1;
        }

        struct game_state cur = dequeue(q);
        uint64_t ser = serialize(cur);

        // Check if already visited
        int seen = 0;
        for (size_t i = 0; i < visited_count; i++)
        {
            if (visited[i] == ser)
            {
                seen = 1;
                break;
            }
        }
        if (seen)
            continue;

        visited[visited_count++] = ser;

        // Check if goal
        int solved = 1;
        for (int i = 0, v = 1; i < 4 && solved; i++)
        {
            for (int j = 0; j < 4; j++, v++)
            {
                if ((i == 3 && j == 3 && cur.tiles[i][j] != 0) ||
                    ((i != 3 || j != 3) && cur.tiles[i][j] != v))
                {
                    solved = 0;
                    break;
                }
            }
        }
        if (solved)
        {
            free_list(q->data);
            free(q);
            return cur.num_steps;
        }

        // Generate moves manually and insert only unseen ones
        struct game_state next;
        uint64_t next_ser;
        int is_dup;

        // Up
        if (cur.empty_row != 3)
        {
            next = cur;
            move_up(&next);
            next_ser = serialize(next);
            is_dup = 0;
            for (size_t i = 0; i < visited_count; i++)
            {
                if (visited[i] == next_ser)
                {
                    is_dup = 1;
                    break;
                }
            }
            if (!is_dup)
                insert_at_head(&(q->data), next_ser);
        }

        // Down
        if (cur.empty_row != 0)
        {
            next = cur;
            move_down(&next);
            next_ser = serialize(next);
            is_dup = 0;
            for (size_t i = 0; i < visited_count; i++)
            {
                if (visited[i] == next_ser)
                {
                    is_dup = 1;
                    break;
                }
            }
            if (!is_dup)
                insert_at_head(&(q->data), next_ser);
        }

        // Left
        if (cur.empty_col != 3)
        {
            next = cur;
            move_left(&next);
            next_ser = serialize(next);
            is_dup = 0;
            for (size_t i = 0; i < visited_count; i++)
            {
                if (visited[i] == next_ser)
                {
                    is_dup = 1;
                    break;
                }
            }
            if (!is_dup)
                insert_at_head(&(q->data), next_ser);
        }

        // Right
        if (cur.empty_col != 0)
        {
            next = cur;
            move_right(&next);
            next_ser = serialize(next);
            is_dup = 0;
            for (size_t i = 0; i < visited_count; i++)
            {
                if (visited[i] == next_ser)
                {
                    is_dup = 1;
                    break;
                }
            }
            if (!is_dup)
                insert_at_head(&(q->data), next_ser);
        }
    }

    free_list(q->data);
    free(q);
    return -1;
}
