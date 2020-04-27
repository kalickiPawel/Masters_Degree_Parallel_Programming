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

#define N 512

using namespace std;

enum directions
{
    NONE,
    NOR = 1,
    EAS = 2,
    SOU = 4,
    WES = 8
};

static unsigned char color[N][N][3];
unsigned char *_world;
int _s, _ptX, _ptY;

bool testDir(int d)
{
    switch (d)
    {
    case NOR:
        return (_ptY - 1 > -1 && !_world[_ptX + _s * (_ptY - 1)]);
    case EAS:
        return (_ptX + 1 < _s && !_world[_ptX + 1 + _s * _ptY]);
    case SOU:
        return (_ptY + 1 < _s && !_world[_ptX + _s * (_ptY + 1)]);
    case WES:
        return (_ptX - 1 > -1 && !_world[_ptX - 1 + _s * _ptY]);
    }
    return false;
}

int getDirection()
{
    int d = 1 << rand() % 4;
    while (true)
    {
        for (int x = 0; x < 4; x++)
        {
            if (testDir(d))
                return d;
            d <<= 1;
            if (d > 8)
                d = 1;
        }
        d = (_world[_ptX + _s * _ptY] & 0xf0) >> 4;
        if (!d)
            return -1;
        switch (d)
        {
        case NOR:
            _ptY--;
            break;
        case EAS:
            _ptX++;
            break;
        case SOU:
            _ptY++;
            break;
        case WES:
            _ptX--;
            break;
        }

        d = 1 << rand() % 4;
    }
}

void carve()
{
    while (true)
    {
        int d = getDirection();
        if (d < NOR)
            return;

        switch (d)
        {
        case NOR:
            _world[_ptX + _s * _ptY] |= NOR;
            _ptY--;
            _world[_ptX + _s * _ptY] = SOU | SOU << 4;
            break;
        case EAS:
            _world[_ptX + _s * _ptY] |= EAS;
            _ptX++;
            _world[_ptX + _s * _ptY] = WES | WES << 4;
            break;
        case SOU:
            _world[_ptX + _s * _ptY] |= SOU;
            _ptY++;
            _world[_ptX + _s * _ptY] = NOR | NOR << 4;
            break;
        case WES:
            _world[_ptX + _s * _ptY] |= WES;
            _ptX--;
            _world[_ptX + _s * _ptY] = EAS | EAS << 4;
        }
    }
}

void generate()
{
    _world = new unsigned char[N * N];
    _ptX = rand() % N;
    _ptY = rand() % N;
    carve();
}

void generateLabirynth()
{
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
}

int main(int argc, const char *argv[])
{
    FILE *fp;
    char const *filename = "labirynt.ppm";
    char const *comment = "# "; /* comment should start with # */

    generateLabirynth();
    // mazeGenerator objekt = mazeGenerator();
    // objekt.create(N);

    fp = fopen(filename, "wb");
    fprintf(fp, "P6\n %s\n %d\n %d\n %d\n", comment, N, N, 255);
    fwrite(color, 1, 3 * N * N, fp);
    fclose(fp);

    return 0;
}
