//
//  main.cpp
//  Laboratory exercise no. 04
//  Ulam Spiral C++ 11 Threads
//
//  Created by Paweł Kalicki on 08/04/2020.
//  Copyright © 2020 Paweł Kalicki. All rights reserved.
//

#include <iostream>
#include <stdio.h>
#include <math.h>
#include <thread>
#include <vector>
#include <chrono>
#include <algorithm>

#define thread_num 4
#define N 512

using namespace std;

static unsigned char color[N][N][3];

chrono::duration<double> elapsed_seconds;

int ulam_get_map(int x, int y, int n)
{
    x -= (n - 1) / 2;
    y -= n / 2;

    int mx = abs(x), my = abs(y);
    int l = 2 * max(mx, my);
    int d = y >= x ? l * 3 + x + y : l - x - y;

    return pow(l - 1, 2) + d;
}

int isprime(int n)
{
    int p;

    for (p = 2; p * p <= n; p++)
        if (n % p == 0)
            return 0;

    return n > 2;
}

int main()
{
    FILE *fp;
    char const *filename = "ulam.ppm";
    char const *comment = "# ";

    int i, start_width, stop_width, start_height, stop_height;

    vector<thread> workers;

    chrono::system_clock::time_point start = chrono::system_clock::now();

    for (i = 0; i < thread_num; i++)
    {
        start_width = (i % 2) * N / (thread_num / 2);
        stop_width = start_width + N / 2;
        start_height = floor(i / 2) * N / (thread_num / 2);
        stop_height = start_height + N / 2;

        workers.push_back(thread([start_width, stop_width, start_height, stop_height]() {
            int i, j, k;
            for (j = start_width; j < stop_width; j++)
                for (k = start_height; k < stop_height; k++)
                    if (isprime(ulam_get_map(k, j, N)))
                        for (i = 0; i < 3; i++)
                            color[j][k][i] = 255;
        }));
    }

    for_each(workers.begin(), workers.end(), [](thread &t) {
        t.join();
    });

    chrono::system_clock::time_point end = chrono::system_clock::now();
    elapsed_seconds += end - start;

    fp = fopen(filename, "wb");
    fprintf(fp, "P6\n %s\n %d\n %d\n %d\n", comment, N, N, 255);
    fwrite(color, 1, 3 * N * N, fp);
    fclose(fp);

    cout << "elapsed time ulama's spiral: " << elapsed_seconds.count() << '\n';

    return 0;
}
