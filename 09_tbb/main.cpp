#include <iostream>
#include <fstream>
#include <tbb/tbb.h>

using namespace std;

unsigned char **beforeFilter[3], tmp[3];
int iXmax, iYmax, i, j, k;
double **afterFilter[3];
double filterMask[3][3] = {{1.0, 2.0, 1.0},
                           {0.0, 0.0, 0.0},
                           {-1.0, -2.0, -1.0}};

void allocateArrays()
{
    for (i = 0; i < 3; i++)
    {
        beforeFilter[i] = new unsigned char *[iYmax];
        afterFilter[i] = new double *[iYmax];
    }

    for (i = 0; i < iYmax; i++)
        for (j = 0; j < 3; j++)
        {
            beforeFilter[j][i] = new unsigned char[iXmax];
            afterFilter[j][i] = new double[iXmax];
        }
}

void deleteAllocates()
{
    for (i = 0; i < iYmax; i++)
        for (j = 0; j < 3; j++)
        {
            delete[] beforeFilter[j][i];
            delete[] afterFilter[j][i];
        }
    for (i = 0; i < 3; i++)
    {
        delete[] beforeFilter[i];
        delete[] afterFilter[i];
    }
}

void loadPPM(string filename)
{
    string fLine, line;
    ifstream streamReader;
    int beforeFilterVal;
    char reader;

    streamReader.open(filename, ios::in | ios::binary);
    getline(streamReader, fLine);

    getline(streamReader, line);
    sscanf(line.c_str(), "%d %d\n", &iXmax, &iYmax);

    getline(streamReader, line);
    sscanf(line.c_str(), "%d\n", &beforeFilterVal);

    allocateArrays();

    for (i = 0; i < iYmax; i++)
        for (j = 0; j < iXmax; j++)
            for (k = 0; k < 3; k++)
            {
                streamReader.read(&reader, 1);
                beforeFilter[k][i][j] = (unsigned char)reader;
            }
}

int main()
{
    tbb::tick_count start, end;

    loadPPM("09_tbb/Lenaclor.ppm");

    start = tbb::tick_count::now();

    tbb::parallel_for(0, iXmax, 1, [=](int x) {
        int i, j, k, l;
        for (i = 0; i < iYmax; i++)
        {
            double it = 0;
            for (j = -2; j <= 2; j++)
            {
                if ((j + x) >= iXmax || (j + x) < 0)
                    continue;
                for (k = -2; k <= 2; k++)
                {
                    if ((k + i) >= iYmax || (k + i) < 0)
                        continue;
                    for (l = 0; l < 3; l++)
                        afterFilter[l][x][i] += filterMask[j + 1][k + 1] * beforeFilter[l][x + j][i + k];
                    it += filterMask[j + 1][k + 1];
                }
            }
            for (k = 0; k < 3; k++)
            {
                afterFilter[k][x][i] /= it;

                if (afterFilter[k][x][i] > 255.0)
                    afterFilter[k][x][i] = 255.0;
                else if (afterFilter[k][x][i] < 0.0)
                    afterFilter[k][x][i] = 0.0;
            }
        }
    });

    end = tbb::tick_count::now();

    cout << "Time elapsed: " << (end - start).seconds() << "s" << endl;

    FILE *fp;
    char const *filename = "result.ppm";
    char const *comment = "# ";

    fp = fopen(filename, "wb");
    fprintf(fp, "P6\n %s\n %d\n %d\n %d\n", comment, iXmax, iYmax, 255);

    for (i = 0; i < iXmax; i++)
        for (j = 0; j < iYmax; j++)
        {
            for (k = 0; k < 3; k++)
                tmp[k] = (unsigned char)afterFilter[k][i][j];
            fwrite(&tmp, 1, 3, fp);
        }

    fclose(fp);
    deleteAllocates();
    return 0;
}
