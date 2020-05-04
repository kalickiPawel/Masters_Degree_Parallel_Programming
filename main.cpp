//
//  main.cpp
//  Lab05_labirynt
//
//  Created by Paweł Kalicki on 17/04/2020.
//  Copyright © 2020 Paweł Kalicki. All rights reserved.
//

#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <ctime>

#define N 441

using namespace std;

static unsigned char color[N][N][3];

int color_r = rand() % 256;
int color_g = rand() % 256;
int color_b = rand() % 256;

int randomX, randomY;

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

int getRandomColor()
{
    return rand() % 256;
}

void drawing_points(int x, int y, int color_r, int color_g, int color_b)
{
    // Drawing points
    int i, j;
    for (i = 0; i < 21; i++)
        for (j = 0; j < 21; j++)
        {
            color[x * 21 + i][y * 21 + j][0] = color_r;
            color[x * 21 + i][y * 21 + j][1] = color_g;
            color[x * 21 + i][y * 21 + j][2] = color_b;
        }
}

void changeColor()
{
    color_r = rand() % 256;
    color_g = rand() % 256;
    color_b = rand() % 256;
}

void getRandomStart()
{
    // Selecting start point
    do
    {
        randomX = (rand() % (N / 21));
        randomY = (rand() % (N / 21));
    } while (world_map[randomX][randomY] == 1);
}

bool checkLeft(int x, int y)
{
    return (world_map[x][y - 1] == 1);
}
bool checkRight(int x, int y)
{
    return (world_map[x][y + 1] == 1);
}
bool checkUp(int x, int y)
{
    return (world_map[x - 1][y] == 1);
}
bool checkDown(int x, int y)
{
    return (world_map[x + 1][y] == 1);
}
int licznik = 0;
void mazeSolution(int tmpX, int tmpY)
{
    cout << "X: " << tmpX << " Y: " << tmpY << endl;
    world_map[tmpX][tmpY] = 1;
    drawing_points(tmpX, tmpY, color_r, color_g, color_b);

    int ilosc;
    int wybor;

    int tmpX0 = tmpX;
    int tmpY0 = tmpY;

    while (ilosc != 0)
    {
        bool up = checkUp(tmpX, tmpY);
        cout << "Sciana gora: " << up << endl;
        bool down = checkDown(tmpX, tmpY);
        cout << "Sciana dol: " << down << endl;
        bool right = checkRight(tmpX, tmpY);
        cout << "Sciana prawo: " << right << endl;
        bool left = checkLeft(tmpX, tmpY);
        cout << "Sciana lewo: " << left << endl;
        ilosc = !up + !down + !right + !left;
        cout << "Ile korytarzy?: " << ilosc << endl;

        if (up != true)
        {
            tmpX0 = tmpX - 1;
            tmpY0 = tmpY;
        }
        if (down != true)
        {
            tmpX0 = tmpX + 1;
            tmpY0 = tmpY;
        }
        if (right != true)
        {
            tmpX0 = tmpX;
            tmpY0 = tmpY - 1;
        }
        if (left != true)
        {
            tmpX0 = tmpX;
            tmpY0 = tmpY + 1;
        }

        switch (ilosc)
        {
        case 0:
            cout << "In case: " << ilosc << endl;
            drawing_points(tmpX, tmpY, color_r, color_g, color_b);
            ilosc = 0;
            break;
        case 1:
            if (up != true)
            {
                cout << "Korytarz gora" << endl;
                world_map[tmpX][tmpY] = 1;
                tmpX -= 1;
            }
            if (down != true)
            {
                cout << "Korytarz dol" << endl;
                world_map[tmpX][tmpY] = 1;
                tmpX += 1;
            }
            if (right != true)
            {
                cout << "Korytarz prawo" << endl;
                world_map[tmpX][tmpY] = 1;
                tmpY += 1;
            }
            if (left != true)
            {
                cout << "Korytarz lewo" << endl;
                world_map[tmpX][tmpY] = 1;
                tmpY -= 1;
            }
            drawing_points(tmpX, tmpY, color_r, color_g, color_b);
            break;
        case 2:
            wybor = 0;
            if (up != true)
            {
                cout << "Korytarz gora" << endl;
                world_map[tmpX][tmpY] = 1;
                if (wybor == 0)
                {
                    cout << "Ide gora" << endl;
                    tmpX -= 1;
                    drawing_points(tmpX, tmpY, color_r, color_g, color_b);
                }
                else
                {
                    world_map[tmpX0][tmpY0] = 1;
                    cout << "Tu GORA rozwidlenie X: " << tmpX0 << "Y: " << tmpY0 << endl;
                    drawing_points(tmpX0, tmpY0, 255, 0, 0);
                }
                wybor += 1;
            }
            if (down != true)
            {
                cout << "Korytarz dol" << endl;
                world_map[tmpX][tmpY] = 1;
                if (wybor == 0)
                {
                    cout << "Ide dol" << endl;
                    tmpX += 1;
                    drawing_points(tmpX, tmpY, color_r, color_g, color_b);
                }
                else
                {
                    world_map[tmpX0][tmpY0] = 1;
                    cout << "Tu DOL rozwidlenie X: " << tmpX0 << "Y: " << tmpY0 << endl;
                    drawing_points(tmpX0, tmpY0, 255, 0, 0);
                }
                wybor += 1;
            }
            if (right != true)
            {
                cout << "Korytarz prawo" << endl;
                world_map[tmpX][tmpY] = 1;
                if (wybor == 0)
                {
                    cout << "Ide prawo" << endl;
                    tmpY += 1;
                    drawing_points(tmpX, tmpY, color_r, color_g, color_b);
                }
                else
                {
                    tmpY0 += 2;
                    world_map[tmpX0][tmpY0] = 1;
                    cout << "Tu PRAWO rozwidlenie X: " << tmpX0 << "Y: " << tmpY0 << endl;
                    drawing_points(tmpX0, tmpY0, 255, 0, 0);
                }
                wybor += 1;
            }
            if (left != true)
            {
                cout << "Korytarz lewo" << endl;
                world_map[tmpX][tmpY] = 1;
                if (wybor == 0)
                {
                    cout << "Ide lewo" << endl;
                    tmpY -= 1;
                    drawing_points(tmpX, tmpY, color_r, color_g, color_b);
                }
                else
                {
                    tmpY0 -= 2;
                    world_map[tmpX0][tmpY0] = 1;
                    cout << "Tu LEWO rozwidlenie X: " << tmpX0 << "Y: " << tmpY0 << endl;
                    drawing_points(tmpX0, tmpY0, 255, 0, 0);
                }
                wybor += 1;
            }
            break;
        case 3:
            wybor = 0;
            if (up != true)
            {
                cout << "Korytarz gora" << endl;
                world_map[tmpX][tmpY] = 1;
                if (wybor == 0)
                {
                    cout << "Ide gora" << endl;
                    tmpX -= 1;
                    drawing_points(tmpX, tmpY, color_r, color_g, color_b);
                }
                else if (wybor == 1)
                {
                    world_map[tmpX0][tmpY0] = 1;
                    cout << "Tu GORA rozwidlenie X: " << tmpX0 << "Y: " << tmpY0 << endl;
                    drawing_points(tmpX0, tmpY0, 255, 0, 0);
                }
                else
                {
                    world_map[tmpX0][tmpY0] = 1;
                    cout << "Tu GORA rozwidlenie X: " << tmpX0 << "Y: " << tmpY0 << endl;
                    drawing_points(tmpX0, tmpY0, 255, 0, 0);
                }
                wybor += 1;
            }
            if (down != true)
            {
                cout << "Korytarz dol" << endl;
                world_map[tmpX][tmpY] = 1;
                if (wybor == 0)
                {
                    cout << "Ide dol" << endl;
                    tmpX += 1;
                    drawing_points(tmpX, tmpY, color_r, color_g, color_b);
                }
                else if (wybor == 1)
                {
                    // tmpY0 -= 1;
                    tmpY0 += 1;
                    tmpX0 += 1;
                    world_map[tmpX0][tmpY0] = 1;
                    cout << "Tu DOL rozwidlenie X: " << tmpX0 << "Y: " << tmpY0 << endl;
                    drawing_points(tmpX0, tmpY0, 255, 0, 0);
                    // tmpY0 += 1;
                    tmpY0 -= 1;
                    tmpX0 -= 1;
                }
                else
                {
                    world_map[tmpX0][tmpY0] = 1;
                    cout << "Tu DOL rozwidlenie X: " << tmpX0 << "Y: " << tmpY0 << endl;
                    drawing_points(tmpX0, tmpY0, 255, 0, 0);
                }
                wybor += 1;
            }
            if (right != true)
            {
                cout << "Korytarz prawo" << endl;
                world_map[tmpX][tmpY] = 1;
                if (wybor == 0)
                {
                    cout << "Ide prawo" << endl;
                    tmpY += 1;
                    drawing_points(tmpX, tmpY, color_r, color_g, color_b);
                }
                else if (wybor == 1)
                {
                    world_map[tmpX0][tmpY0] = 1;
                    cout << "Tu PRAWO rozwidlenie X: " << tmpX0 << "Y: " << tmpY0 << endl;
                    drawing_points(tmpX0, tmpY0, 255, 0, 0);
                }
                else
                {
                    tmpY0 += 2;
                    world_map[tmpX0][tmpY0] = 1;
                    cout << "Tu PRAWO rozwidlenie X: " << tmpX0 << "Y: " << tmpY0 << endl;
                    drawing_points(tmpX0, tmpY0, 255, 0, 0);
                    tmpY0 -= 2;
                }
                wybor += 1;
            }
            if (left != true)
            {
                cout << "Korytarz lewo" << endl;
                world_map[tmpX][tmpY] = 1;
                if (wybor == 0)
                {
                    cout << "Ide lewo" << endl;
                    tmpY -= 1;
                    drawing_points(tmpX, tmpY, color_r, color_g, color_b);
                }
                else if (wybor == 1)
                {
                    tmpY0 -= 2;
                    world_map[tmpX0][tmpY0] = 1;
                    cout << "Tu LEWO rozwidlenie X: " << tmpX0 << "Y: " << tmpY0 << endl;
                    drawing_points(tmpX0, tmpY0, 255, 0, 0);
                }
                else
                {
                    tmpY0 -= 2;
                    world_map[tmpX0][tmpY0] = 1;
                    cout << "Tu LEWO rozwidlenie X: " << tmpX0 << "Y: " << tmpY0 << endl;
                    drawing_points(tmpX0, tmpY0, 255, 0, 0);
                    tmpY0 += 2;
                }
                wybor += 1;
            }
            break;
        case 4:
            ilosc = 0;
            break;
        }
    }
}

int main(int argc, const char *argv[])
{
    srand((unsigned)time(0));
    FILE *fp;
    char const *filename = "labirynt.ppm";
    char const *comment = "# "; /* comment should start with # */

    drawLabirynth();

    getRandomStart();

    // Maze solution
    randomX = 7;
    randomY = 5;

    // randomX = 11;
    // randomY = 1;

    mazeSolution(randomX, randomY);

    fp = fopen(filename, "wb");
    fprintf(fp, "P6\n %s\n %d\n %d\n %d\n", comment, N, N, 255);
    fwrite(color, 1, 3 * N * N, fp);
    fclose(fp);

    return 0;
}
