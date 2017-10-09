#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define GRIDSIZE 204
int grid[GRIDSIZE][GRIDSIZE] = {0};
int nextgrid[GRIDSIZE][GRIDSIZE] = {0};

void clear_grid(int g[][GRIDSIZE])
{
    memset(g, 0, GRIDSIZE * GRIDSIZE);
}

int count_neighbors(int y, int x)
{
    int neighbor_count = 0;

    for (int row_pos = y - 1; row_pos <= y + 1; row_pos++)
    {
        if (row_pos < 0 || row_pos >= GRIDSIZE)
            continue;

        for (int col_pos = x - 1; col_pos <= x + 1; col_pos++)
        {
            if (col_pos < 0 ||col_pos >= GRIDSIZE)
                continue;

            if (row_pos == y && col_pos == x)
                continue;

            if (grid[row_pos][col_pos])
                neighbor_count++;
        }
    }

    return neighbor_count;
}

void generate_grid(int g[][GRIDSIZE])
{
    for (int i = 0; i < GRIDSIZE; i++)
    {
        for (int j = 0; j < GRIDSIZE; j++)
        {
            g[i][j] = rand() % 2;
        }
    }
}

int generate_next_generation_grid(int num_rows, int num_cols)
{
    int neighbor_count = 0;
    clear_grid(nextgrid);

    int population_count = 0;

    for (int row = 0; row < GRIDSIZE; row++)
    {
        for (int col = 0; col < GRIDSIZE; col++)
        {
            if (row >= num_rows || col >= num_cols)
                continue;

            neighbor_count = count_neighbors(row, col);
            bool alive = false;
            if (grid[row][col]) // currently alive
            {
                alive = true;
                if (neighbor_count < 2 || neighbor_count > 3)
                    alive = false;
            }
            else if (neighbor_count == 3)
                alive = true;

            if (alive)
                population_count++;

            nextgrid[row][col] = alive;
        }
    }

    clear_grid(grid);
    memcpy(grid, nextgrid, sizeof(nextgrid));

    return population_count;
}

void print_grid(int g[][GRIDSIZE])
{
    for (int i = 0; i < GRIDSIZE; i++)
    {
        for (int j = 0; j < GRIDSIZE; j++)
        {
            printf("%d ", g[i][j]);
        }
        printf("\n");
    }
}

void wprint_grid(int g[][GRIDSIZE], int row, int col)
{
    for (int i = 0; i < GRIDSIZE; i++)
    {
        for (int j = 0; j < GRIDSIZE; j++)
        {
            if (i >= (row-2) || j >= col)
                continue;
            if (g[i][j])
                mvprintw(i, j, "#");
        }
    }
}

int main()
{
    srand(time(NULL));

    generate_grid(grid);

    //while(true)
    //{
    //    generate_next_generation_grid();
    //    print_grid(grid);
    //    sleep(1);
    //}

    //char msg[] = "CURSES< DAMN YOU!";
    int row, col;
    initscr();
    cbreak();

    int ch;
    int counter = 0;

    int population_count = 0;

    while ((ch = getch()) != 'q')
    {
        if (ch == 'r')
            generate_grid(grid);

        clear();
        getmaxyx(stdscr, row, col);
        mvprintw(row-2, 0, "Rows:%d Cols:%d Counter:%d Population:%d\n", row, col, counter++, population_count);
        population_count = generate_next_generation_grid(row, col);
        wprint_grid(grid, row, col);
        if (population_count < 100)
            generate_grid(grid);
    }

    endwin();
}
