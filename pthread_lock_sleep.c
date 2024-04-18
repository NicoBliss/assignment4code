#include <pthread.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // compile with cc mutex.c -lpthread

int count = 0;
int current_value;
float ftime = 0.6;
pthread_mutex_t pthread_lock = PTHREAD_MUTEX_INITIALIZER;

int inc()
{
    if (pthread_mutex_lock(&pthread_lock))
        printf("ERROR\n");
    current_value = count;
    current_value++;
    sleep(ftime);
    count = current_value;
    pthread_mutex_unlock(&pthread_lock);
}

int dec()
{
    if (pthread_mutex_lock(&pthread_lock))
        printf("ERROR\n");
    current_value = count;
    sleep(2*ftime);
}

int get_count(int *countp)
{
    if (pthread_mutex_lock(&pthread_lock))
        printf("ERROR\n");
    *countp = count;
    pthread_mutex_unlock(&pthread_lock);
}

void *myThreadFun()
{
    inc();
    dec();
    get_count(&count);
    printf("%d ", count);
}

int main(int argc, char* argv[])
{

    if (argc > 1) {
        sscanf(argv[1], "%f", &ftime);
    }
    ftime = ftime / 6;
    pthread_t thread_id[2];
    cpu_set_t mask;
    CPU_ZERO(&mask);
    CPU_SET(0, &mask);
    sched_setaffinity(0, sizeof(mask), &mask);
    for (int i = 0; i < 2; i++)
        pthread_create(&(thread_id[i]), NULL, myThreadFun, NULL);
    for (int i = 0; i < 2; i++)
        pthread_join(thread_id[i], NULL);
    printf("\n");
}
