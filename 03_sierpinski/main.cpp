//
//  main.cpp
//  Lab03_sierpinski
//
//  Created by Paweł Kalicki on 03/04/2020.
//  Copyright © 2020 Paweł Kalicki. All rights reserved.
//

#include <iostream>
#include <stdio.h>
#include <math.h>
#include <thread>

#define N 1024
#define L 8

using namespace std;

static unsigned char color [N][N][3];

void fun(int x, int y, int level)
{
    /* Sierpinski triangle */
    int size = N / (pow(2, level));
    int x2 , y2;
    
    y2 = y + (size/2);
    x2 = x + (size/2);
    
    int i, j;

    for (j = y; j < y2; ++j)
        for (i = x; i < x2; ++i)
            if (i == j || i == (x2 - 1) || j == y)
                for (int l = 0; l<3; l++)
                    color[i][j][l] = 255;
            else
                for (int l = 0; l<3; l++)
                    color[i][j][l] = 0;
    level++;
    
    /* C++ 11 Theads */
    if (level < L)
    {
        thread tab[3];
        tab[0] = thread(fun, x, y, level);
        tab[1] = thread(fun, x2, y, level);
        tab[2] = thread(fun, x2, y2, level);
        
        for (int k=0; k<3; k++)
            tab[k].join();
    }
}

int main() {
    srand(time_t(NULL));
    FILE * fp;
    
    char const *filename="sierpinski.ppm";
    char const *comment="# ";
    
    fun(0,0,0);

    fp= fopen(filename,"wb");

    fprintf(fp,"P6\n %s\n %d\n %d\n %d\n",comment,N,N,255);

    for (int j = 0; j < N; ++j)
        for (int i = 0; i < N; ++i)
            fwrite(color[j][i],1,3,fp);
    
    fclose(fp);
    return 0;
}
