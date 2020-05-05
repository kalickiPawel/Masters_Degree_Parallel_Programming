//
//  main.cpp
//  Laboratory exercise no. 01
//  Large size matrix multiplication
//
//  Created by Paweł Kalicki on 18/03/2020.
//  Copyright © 2020 Paweł Kalicki. All rights reserved.
//

#include <iostream>
#include <chrono>
#include <ctime>
#include <random>
#include <thread>

const int N = 4000;
static const int num_threads = 4;

int A[N][N];
int B[N][N];
int C[N][N];

std::chrono::duration<double> elapsed_seconds;

std::random_device r;

void call_from_thread(int num_thread)
{
    std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
    for (int i = num_thread * (N / num_threads); i < N / num_threads; i++)
        for (int j = 0; j < N; j++)
            for (int k = 0; k < N; k++)
                C[i][j] += A[i][k] * B[k][j];
    std::chrono::system_clock::time_point end = std::chrono::system_clock::now();
    elapsed_seconds += end - start;
}

void call_without_thread()
{
    std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            for (int k = 0; k < N; k++)
                C[i][j] += A[i][k] * B[k][j];
    std::chrono::system_clock::time_point end = std::chrono::system_clock::now();
    elapsed_seconds += end - start;
}

int main()
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            std::default_random_engine e1(r());
            std::default_random_engine e2(r());

            std::uniform_int_distribution<int> uniform_dist(0, N);

            int number1 = uniform_dist(e1);
            int number2 = uniform_dist(e2);

            A[i][j] = number1;
            B[i][j] = number2;
        }
    }

    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            B[i][j] = B[j][i];

    std::thread t[num_threads];

    for (int i = 0; i < num_threads; ++i)
        t[i] = std::thread(call_from_thread, i);

    // Join the threads with the main thread
    for (int i = 0; i < num_threads; ++i)
        t[i].join();

    // call_without_thread(); /* -> Uncomment to test matrix multiplication without threads */

    std::cout << "elapsed time AxB threads summary: " << elapsed_seconds.count() << '\n';

    return 0;
}
