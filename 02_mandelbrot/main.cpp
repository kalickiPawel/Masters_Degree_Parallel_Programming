//
//  main.cpp
//  Laboratory exercise no. 02
//  Mandelbrot set C++ 11 Threads
//
//  Created by Paweł Kalicki on 26/03/2020.
//  Copyright © 2020 Paweł Kalicki. All rights reserved.
//
//  used external project from http://rosettacode.org/wiki/Mandelbrot_set#PPM_non_interactive
//  c program:
//  --------------------------------
//  1. draws Mandelbrot set for Fc(z)=z*z +c
//  using Mandelbrot algorithm ( boolean escape time )
//  -------------------------------
//  2. technique of creating ppm file is  based on the code of Claudio Rocchini
//  http://en.wikipedia.org/wiki/Image:Color_complex_plot.jpg
//  create 24 bit color graphic file ,  portable pixmap file = PPM
//  see http://en.wikipedia.org/wiki/Portable_pixmap
//  to see the file use external application ( graphic viewer)
//

#include <iostream>
#include <stdio.h>
#include <math.h>
#include <thread>

#define thread_count 4

using namespace std;

const int iXmax = 800;
const int iYmax = 800;

const double CxMin = -2.5;
const double CxMax = 1.5;
const double CyMin = -2.0;
const double CyMax = 2.0;

double PixelWidth = (CxMax - CxMin) / iXmax;
double PixelHeight = (CyMax - CyMin) / iYmax;

const int MaxColorComponentValue = 255;

FILE *fp;
char const *filename = "new1.ppm";
char const *comment = "# ";

static unsigned char color[iXmax][iYmax][3];

const int IterationMax = 2000;

const double EscapeRadius = 2;
double ER2 = EscapeRadius * EscapeRadius;

chrono::duration<double> elapsed_seconds;

void mandelBrotDraw(int min, int max, int i)
{
    chrono::system_clock::time_point start = chrono::system_clock::now();
    double Cx, Cy, Zx, Zy, Zx2, Zy2;
    int iX, iY, Iteration, k;
    for (iY = min; iY < max; iY++)
    {
        Cy = CyMin + iY * PixelHeight;
        if (fabs(Cy) < PixelHeight / 2)
            Cy = 0.0; /* Main antenna */
        for (iX = 0; iX < iXmax; iX++)
        {
            Cx = CxMin + iX * PixelWidth;
            /* initial value of orbit = critical point Z= 0 */
            Zx = 0.0;
            Zy = 0.0;

            Zx2 = Zx * Zx;
            Zy2 = Zy * Zy;

            for (Iteration = 0; Iteration < IterationMax && ((Zx2 + Zy2) < ER2); Iteration++)
            {
                Zy = 2 * Zx * Zy + Cy;
                Zx = Zx2 - Zy2 + Cx;
                Zx2 = Zx * Zx;
                Zy2 = Zy * Zy;
            }

            /* compute  pixel color (24 bit = 3 bytes) */
            for (k = 0; k < 3; k++)
            {
                if (Iteration == IterationMax)
                    color[iX][iY][k] = (i * 255) / (thread_count - 1);
                else
                    color[iX][iY][k] = 255 - (i * 255) / (thread_count - 1);
            }
        }
    }
    chrono::system_clock::time_point end = chrono::system_clock::now();
    elapsed_seconds += end - start;
}

int main()
{
    fp = fopen(filename, "wb"); /* b -  binary mode */
    fprintf(fp, "P6\n %s\n %d\n %d\n %d\n", comment, iXmax, iYmax, MaxColorComponentValue);

    thread magazynWatkow[thread_count];

    for (int i = 0; i < thread_count; i++)
        magazynWatkow[i] = thread(mandelBrotDraw, i * (iYmax / thread_count), (i + 1) * (iYmax / thread_count), i);

    for (int i = 0; i < thread_count; i++)
        magazynWatkow[i].join();

    fwrite(color, 1, 3, fp);
    fclose(fp);

    cout << "elapsed time mandelbrot set: " << elapsed_seconds.count() << '\n';
    return 0;
}
