//
//  main.cpp
//  Laboratory exercise no. 06
//  Mandelbrot set OpenMP
//
//  Created by Paweł Kalicki on 06/05/2020.
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
#include <omp.h>

#define thread_count 4
const int chunk = 256;

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
char const *filename = "result.ppm";
char const *comment = "# ";

static unsigned char color[iXmax][iYmax][3];

const int IterationMax = 2000;

const double EscapeRadius = 2;
double ER2 = EscapeRadius * EscapeRadius;

double Cx, Cy, Zx, Zy, Zx2, Zy2, start, end_stop;
int i, j, k;
int iX, iY, Iteration, thread_num;

int main()
{
    fp = fopen(filename, "wb"); /* b -  binary mode */
    fprintf(fp, "P6\n %s\n %d\n %d\n %d\n", comment, iXmax, iYmax, MaxColorComponentValue);

    start = omp_get_wtime();

    int suma[thread_count];
    for (i = 0; i < thread_count; i++)
        suma[i] = 0;

#pragma omp parallel private(thread_num) shared(PixelWidth, PixelHeight, ER2, color) num_threads(thread_count)
    {
        thread_num = omp_get_thread_num();
#pragma omp for private(iY, iX, Iteration, Cy, Cx, Zx, Zy, Zx2, Zy2) schedule(guided, chunk)
        for (iY = 0; iY < iYmax; iY++)
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

                suma[thread_num] += Iteration; // iteration count added to threads matrix

                /* compute  pixel color (24 bit = 3 bytes) */
                if (Iteration == IterationMax)
                { /*  interior of Mandelbrot set = black */
                    color[iY][iX][0] = 0;
                    color[iY][iX][1] = 0;
                    color[iY][iX][2] = 0;
                }
                else
                { /* exterior of Mandelbrot set = white */
                    color[iY][iX][0] = (sin(thread_num % 3) / 2.0 + 0.5) * 255;
                    color[iY][iX][1] = (sin(thread_num % 5) / 2.0 + 0.5) * 255;
                    color[iY][iX][2] = (sin(thread_num % 7) / 2.0 + 0.5) * 255;
                }
            }
        }
    }

    end_stop = omp_get_wtime();

    cout << "Number of iteration in thread: " << endl;
    for (i = 0; i < thread_count; i++)
        cout << i << ": " << suma[i] << endl;

    for (i = 0; i < iYmax; i++)
        for (j = 0; j < iXmax; j++)
            fwrite(color[i][j], 1, 3, fp);

    fclose(fp);

    cout << "elapsed time mandelbrot set: " << end_stop - start << '\n';
    return 0;
}
