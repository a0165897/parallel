#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

long                thread_count;
double              a = 0.0;
double              b = 3.0;
int                 n = 1024;
double              h;
int                 local_n;
double              sum;
int                 flag;
double f(double x) {
    return x * x;
}

void* calc(void* rank) {
    /*calculate own part*/
    int        my_rank = (int)rank;
    double     local_a = a + my_rank * local_n * h;
    double     local_b = local_a + local_n * h;
    double     approx = (f(local_a) + f(local_b)) / 2.0;
    double     x;
    int        i;
    for (i = 1; i <= local_n - 1; i++) {
        x = local_a + i * h;
        approx += f(x);
    }
    approx = approx * h;
    printf("%d %f\n", my_rank, approx);
    while (flag != my_rank);                  // busy waiting
    sum += approx;
    flag  = (flag + 1) % (int)thread_count;
    return NULL;
}

int main(int argc, char* argv[]) {
    /*initialize*/
    long        thread;
    pthread_t*  threads_handles;
    flag = 0;
    thread_count = strtol(argv[1], NULL, 10);
    int rest = n % (int)thread_count;
    if (!rest) {
        local_n = n / (int) thread_count;
    }
    else {
        local_n = (n - rest) / (int) thread_count;
    }
    h = (b - a) / (1.0 * n);
    sum = 0.0;

    /*thread thing*/
    threads_handles = (pthread_t*)malloc(thread_count * sizeof(pthread_t));
    for (thread = 0; thread < thread_count; thread++) {
        pthread_create(&threads_handles[thread], NULL, calc, (void*)thread);
    }
    for (thread = 0; thread < thread_count; thread++) {
        pthread_join(threads_handles[thread], NULL);
    }
    /*rest thing*/
    if (rest) {
        double tmp_x;
        double tmp_a = b - (rest - 1) * h;
        double tmp_approx = (f(tmp_a) + f(b)) / 2.0;
        int i;
        for (i = 1; i < rest; i++) {
            tmp_x = tmp_a + i * h;
            tmp_approx += f(tmp_x);
        }
        tmp_approx = tmp_approx * h;
        sum += tmp_approx;
    }
    printf("Main thread %e.\n", sum);
    free(threads_handles);

    return 0;
}

