//
//  main.cpp
//  Laboratory exercise no. 10
//  Lyapunow fractal C++ 11 Intel TBB
//
//  Created by Paweł Kalicki on 14/05/2020.
//  Copyright © 2020 Paweł Kalicki. All rights reserved.
//
//  -------------------------------
//  1. technique of creating ppm file is  based on the code of Claudio Rocchini
//  http://en.wikipedia.org/wiki/Image:Color_complex_plot.jpg
//  create 24 bit color graphic file ,  portable pixmap file = PPM
//  see http://en.wikipedia.org/wiki/Portable_pixmap
//  to see the file use external application ( graphic viewer)
//  -------------------------------
//

#include <iostream>
#include <cmath>
#include <tbb/tbb.h>

using namespace std;

const int size = 500;
char pixels[size][size][3];

class Lapunov
{
public:
    char Sequence[16];
    const int noColors = 255;
    float MinR, MaxR, MinX, MaxX;
    char NoSeq = 16;
    float LapMin = -2;
    int sample;

    int DecToBin(int Dec)
    {
        int pow;
        char rest;
        pow = 65536;
        rest = 0;

        while (rest == 0 && pow > 0)
        {
            rest = (char)floor(Dec / pow);
            if (rest == 0)
                pow = floor(pow / 2);
        }
        while (pow > 1)
        {
            Dec = Dec - pow * rest;
            pow = floor(pow / 2);
            rest = (char)floor(Dec / pow);
            NoSeq++;
            cout << NoSeq;
            Sequence[NoSeq] = rest;
        }
        cout << '\n';
        return 0;
    }
    /*void DecToBin(int Dec){
		  int i;
		  for(i=0; i<16; i++){
		     Sequence[i] = Dec >> i & 1;

		  }
		}*/

    void Draw(float Seed, int NoIter, int RozX, int RozY, int start, int koniec, float RXMin, float RXMax, float RYMin, float RYMax, int val, int s)
    {
        float rx, ry, deltaX, deltaY, tmpLap = 0;
        int k, w;
        char tmp;

        for (k = 0; k < 16; k++)
            Sequence[k] = 0;

        sample = s;
        NoSeq = 0;
        Sequence[0] = 1;
        DecToBin(val);
        LapMin = -2;
        MinR = RXMin;
        MaxR = RXMax;
        MinX = RYMin;
        MaxX = RYMax;
        deltaX = (MaxR - MinR) / RozX;
        deltaY = (MaxX - MinX) / RozY;
        rx = MinR;
        ry = MaxX - (start - 1) * deltaY;
        double z;
        for (k = 0; k < 15; k++)
            std::cout << (int)Sequence[k];

        std::cout << "\n";

        tbb::parallel_for(tbb::blocked_range2d<size_t>(0, RozY, 16, 0, RozX, 32), [=](const tbb::blocked_range2d<size_t> &r) {
            float rx, ry, tmpLap = 0;
            double z;
            char tmp;

            float rxInit = MinR;
            float ryInit = MaxX - (start - 1) * deltaY;
            rx = rxInit;
            ry = ryInit;

            for (size_t w = r.rows().begin(); w != r.rows().end(); w++)
            {
                for (size_t k = r.cols().begin(); k != r.cols().end(); k++)
                {
                    rx = rxInit + deltaX * k;
                    ry = ryInit - deltaY * w;
                    tmpLap = ValLap(Seed, NoIter, rx, ry);

                    //std::cout << (int)tmpLap;
                    if (tmpLap <= 0)
                    {
                        z = noColors * tmpLap / LapMin;
                        tmp = (int)(z) % noColors;
                        pixels[k][w][0] = tmp;
                        pixels[k][w][1] = tmp;
                        pixels[k][w][2] = tmp;
                    }
                    else
                    {
                        pixels[k][w][0] = 0;
                        pixels[k][w][1] = 0;
                        pixels[k][w][2] = 0;
                    }
                    rx = rx + deltaX;
                }
                rx = MinR;
                ry = ry - deltaY;
            }
        });
        //return pixels;
    }

    float ValLap(float Seed, int NoIter, float rx, float ry)
    {
        float x, sumlap, elem, ValLap;
        int i, poz, NoElem;
        float R;

        x = Seed;
        sumlap = 0;
        NoElem = 0;
        poz = 0;

        for (i = 1; i <= NoIter; i++)
        {
            if (Sequence[poz] == 0)
                R = ry;
            else
                R = rx;
            poz++;
            if (poz > NoSeq)
                poz = 0;
            x = fun(x, R);
            elem = (float)abs(dfun(x, R));
            if (elem > 1000)
            {
                ValLap = 10;
                break;
            }
            if (elem != 0)
            {
                sumlap = sumlap + (float)log2(elem);
                NoElem++;
            }
        }
        if (NoElem > 0)
            ValLap = sumlap / NoElem;
        else
            ValLap = 0;
        return ValLap;
    }

    float fun(float x, float r)
    {
        float y = 0;
        switch (sample)
        {
        case (0):
            y = r * sin(x) + r;
            break;
        case (1):
            y = r * cos(x) + r;
            break;
        case (2):
            y = r * cos(x) * (1 - sin(x));
            break;
        }
        return y;
    }

    float dfun(float x, float r)
    {
        float y = 0;
        switch (sample)
        {
        case (0):
            y = r * cos(x);
            break;
        case (1):
            y = -r * sin(x);
            break;
        case (2):
            y = r * (1 - sin(x)) - 2 * cos(x) * cos(x);
            break;
        }
        return y;
    }
};

int main()
{

    FILE *fp;
    int i, j;
    char const *filename = "Lyapunow.ppm";
    char const *comment = "# "; /* comment should start with # */
    const int MaxColorComponentValue = 255;
    int a;
    Lapunov lp;
    fp = fopen(filename, "wb"); /* b -  binary mode */
    /*write ASCII header to the file*/

    fprintf(fp, "P6\n %s\n %d\n %d\n %d\n", comment, size, size, MaxColorComponentValue);

    tbb::tick_count s1, e1;
    int nThreads = 2;
    tbb::task_scheduler_init init(nThreads);
    s1 = tbb::tick_count::now();

    lp.Draw(5, 100, size, size, 0, size, -3, 9, -5, 2, 2477, 1);

    e1 = tbb::tick_count::now();
    cout << (e1 - s1).seconds() << endl;

    fwrite(pixels, 1, 3 * size * size, fp);

    fclose(fp);

    return 0;
}
