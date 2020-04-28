//
//  main.cpp
//  Lab05_labirynt
//
//  Created by Paweł Kalicki on 17/04/2020.
//  Copyright © 2020 Paweł Kalicki. All rights reserved.
//

#include <iostream>
#include <cstdlib>
#include <string>

#define N 441

using namespace std;

static unsigned char color[N][N][3];

const int MAP_WIDTH = 21;
const int MAP_HEIGHT = 21;

int world_map[MAP_WIDTH][MAP_HEIGHT] =
    {1, 9, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
     1, 9, 1, 9, 9, 9, 1, 9, 9, 9, 9, 9, 1, 9, 9, 9, 9, 9, 9, 9, 1,
     1, 9, 1, 9, 1, 9, 1, 1, 1, 9, 1, 1, 1, 1, 1, 1, 1, 1, 1, 9, 1,
     1, 9, 1, 9, 1, 9, 9, 9, 9, 9, 9, 9, 9, 9, 1, 9, 1, 9, 9, 9, 1,
     1, 9, 1, 1, 1, 1, 1, 9, 1, 1, 1, 1, 1, 9, 1, 9, 1, 1, 1, 9, 1,
     1, 9, 1, 9, 1, 9, 9, 9, 1, 9, 9, 9, 1, 9, 9, 9, 9, 9, 1, 9, 1,
     1, 9, 1, 9, 1, 1, 1, 9, 1, 9, 1, 9, 1, 9, 1, 1, 1, 1, 1, 9, 1,
     1, 9, 9, 9, 9, 9, 1, 9, 9, 9, 1, 9, 1, 9, 9, 9, 9, 9, 9, 9, 1,
     1, 1, 1, 9, 1, 1, 1, 1, 1, 9, 1, 9, 1, 1, 1, 9, 1, 9, 1, 1, 1,
     1, 9, 9, 9, 9, 9, 9, 9, 9, 9, 1, 9, 9, 9, 1, 9, 1, 9, 9, 9, 1,
     1, 9, 1, 9, 1, 1, 1, 9, 1, 9, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
     1, 9, 1, 9, 9, 9, 1, 9, 1, 9, 9, 9, 9, 9, 1, 9, 1, 9, 9, 9, 1,
     1, 1, 1, 1, 1, 1, 1, 9, 1, 9, 1, 1, 1, 1, 1, 9, 1, 1, 1, 9, 1,
     1, 9, 9, 9, 1, 9, 1, 9, 1, 9, 9, 9, 1, 9, 9, 9, 9, 9, 9, 9, 1,
     1, 9, 1, 1, 1, 9, 1, 9, 1, 1, 1, 9, 1, 9, 1, 9, 1, 1, 1, 9, 1,
     1, 9, 1, 9, 9, 9, 9, 9, 9, 9, 1, 9, 1, 9, 1, 9, 1, 9, 9, 9, 1,
     1, 9, 1, 9, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 9, 1, 1, 1, 9, 1,
     1, 9, 1, 9, 9, 9, 9, 9, 9, 9, 9, 9, 1, 9, 1, 9, 1, 9, 1, 9, 1,
     1, 9, 1, 9, 1, 9, 1, 1, 1, 1, 1, 9, 1, 9, 1, 1, 1, 9, 1, 9, 1,
     1, 9, 9, 9, 1, 9, 9, 9, 1, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 1,
     1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

void drawLabirynth()
{
    // Here I create a color value with
    // empty maze black/white
    int i, j, k;
    for (i = 0; i <= N; i++)
        for (j = 0; j <= N; j++)
            for (k = 0; k < 3; k++)
                if (world_map[i / 21][j / 21] == 9)
                    color[i][j][k] = 255;
}

void mazeSolution()
{
    int i, j;
    for (i = 0; i < N / 21; i++)
        for (j = 0; j < N / 21; j++)
            world_map[i][j];
}

int main(int argc, const char *argv[])
{
    FILE *fp;
    char const *filename = "labirynt.ppm";
    char const *comment = "# "; /* comment should start with # */

    drawLabirynth();
    mazeSolution();

    fp = fopen(filename, "wb");
    fprintf(fp, "P6\n %s\n %d\n %d\n %d\n", comment, N, N, 255);
    fwrite(color, 1, 3 * N * N, fp);
    fclose(fp);

    return 0;
}
