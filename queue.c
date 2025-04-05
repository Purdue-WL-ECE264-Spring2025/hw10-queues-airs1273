#include "queue.h"
#include "tile_game.h"
#include "linked_list.h"
#include <stdlib.h>

// Enqueue inserts at the tail (FIFO behavior)
void enqueue(struct queue *q, struct game_state state)
{
    insert_at_tail(&(q->data), serialize(state));
}

// Dequeue removes from the head (FIFO behavior)
struct game_state dequeue(struct queue *q)
{
    return deserialize(remove_from_head(&(q->data)));
}

// Check if this state has already been visited
int already_visited(struct game_state *visited, size_t count, struct game_state state)
{
    for (size_t i = 0; i < count; i++)
    {
        int match = 1;
        for (int r = 0; r < 4 && match; r++)
        {
            for (int c = 0; c < 4; c++)
            {
                if (visited[i].tiles[r][c] != state.tiles[r][c])
                {
                    match = 0;
                    break;
                }
            }
        }
        if (match)
            return 1;
    }
    return 0;
}

// BFS to find minimum number of moves
int number_of_moves(struct game_state start)
{
    struct queue q = {0};
    enqueue(&q, start);

    struct game_state visited[100000];
    size_t count = 0;

    while (q.data.head)
    {
        struct game_state s = dequeue(&q);

        if (already_visited(visited, count, s))
            continue;

        if (count < 100000)
            visited[count++] = s;

        // Check if solved
        int done = 1;
        for (int i = 0, v = 1; i < 4 && done; i++)
        {
            for (int j = 0; j < 4; j++, v++)
            {
                if ((i == 3 && j == 3 && s.tiles[i][j] != 0) ||
                    ((i != 3 || j != 3) && s.tiles[i][j] != v))
                {
                    done = 0;
                    break;
                }
            }
        }

        if (done)
            return s.num_steps;

        // Generate valid next states
        struct game_state n;
        if (s.empty_row > 0) // Can move tile down (empty up)
        {
            n = s;
            move_up(&n);
            enqueue(&q, n);
        }
        if (s.empty_row < 3) // Can move tile up (empty down)
        {
            n = s;
            move_down(&n);
            enqueue(&q, n);
        }
        if (s.empty_col > 0) // Can move tile right (empty left)
        {
            n = s;
            move_right(&n);
            enqueue(&q, n);
        }
        if (s.empty_col < 3) // Can move tile left (empty right)
        {
            n = s;
            move_left(&n);
            enqueue(&q, n);
        }
    }

    return -1; // Unsolvable (shouldn't happen with valid boards)
}
