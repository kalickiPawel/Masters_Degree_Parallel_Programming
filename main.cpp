//
//  main.cpp
//  Lab05_labirynt
//
//  Created by Paweł Kalicki on 17/04/2020.
//  Copyright © 2020 Paweł Kalicki. All rights reserved.
//

#include <iostream>

#define N 512

using namespace std;

static unsigned char color[N][N][3];

int main(int argc, const char *argv[])
{
    FILE *fp;
    char const *filename = "labirynt.ppm";
    char const *comment = "# "; /* comment should start with # */
    int i, j, k;
    for (i = 0; i < N; i++)
        for (j = 0; j < N; j++)
            if (i % 10 == 0 && j % 10 == 0)
                for (k = 0; k < 3; k++)
                    color[i][j][k] = 255;
    for (i = 1; i < 9; i++)
        for (j = 1; j < 9; j++)
            for (k = 0; k < 3; k++)
                color[i][j][k] = 100;

    fp = fopen(filename, "wb");
    fprintf(fp, "P6\n %s\n %d\n %d\n %d\n", comment, N, N, 255);
    fwrite(color, 1, 3 * N * N, fp);
    fclose(fp);

    return 0;
}
