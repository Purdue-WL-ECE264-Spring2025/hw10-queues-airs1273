#include "queue.h"
#include "tile_game.h"
#include "linked_list.h"
#include <stdlib.h>
#include "tile_game.h"

void enqueue(struct queue *q, struct game_state state)
{
    // inserts at the head
    if (q == NULL)
    {
        q = malloc(sizeof(struct queue));
    }
    insert_at_head(&(q->data), serialize(state));
}

struct game_state dequeue(struct queue *q)
{
    // dequeue removes from the tail
    return deserialize(remove_from_tail(&(q->data)));
}

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

int number_of_moves(struct game_state start)
{
    // implement a BFS that determines the shortest moves to solve a Tiles game
    struct queue q = {0};
    enqueue(&q, start);

    struct game_state visited[100000];
    size_t count = 0;

    while (q.data.head)
    {
        struct game_state s = dequeue(&q);
        if (already_visited(visited, count, s))
            continue;
        visited[count++] = s;

        int done = 1;
        for (int i = 0, v = 1; i < 4 && done; i++)
            for (int j = 0; j < 4; j++, v++)
                if ((i == 3 && j == 3 && s.tiles[i][j] != 0) ||
                    ((i != 3 || j != 3) && s.tiles[i][j] != v))
                    done = 0;
        if (done)
            return s.num_steps;

        struct game_state n;
        if (s.empty_row < 3)
        {
            n = s;
            move_up(&n);
            enqueue(&q, n);
        }
        if (s.empty_row > 0)
        {
            n = s;
            move_down(&n);
            enqueue(&q, n);
        }
        if (s.empty_col < 3)
        {
            n = s;
            move_left(&n);
            enqueue(&q, n);
        }
        if (s.empty_col > 0)
        {
            n = s;
            move_right(&n);
            enqueue(&q, n);
        }
    }

    return -1;
}
