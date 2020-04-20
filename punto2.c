#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>

void* factorial (void*);

struct fact_param
{
    int number;
};

int main (int argc, char *argv[]) 
{
    srand(time(0));
    if (argc > 1) 
    {
        pthread_t threads_ids[argc-1];
        struct fact_param thread_args[argc-1];
        for (int i = 0; i < (argc - 1); i++)
        {
            thread_args[i].number = atoi (argv[i+1]);
            pthread_create (&threads_ids[i], NULL, &factorial, &thread_args[i]);
        }
        for (int i = 0; i < (argc - 1); i++)
        {
            pthread_join(threads_ids[i],NULL);
        }
    } else 
    {
        printf("Le falto ingresar datos, sin datos no va a funcionar\n");
    }
    return 0;
}

void* factorial (void *parameters) 
{
    struct fact_param* param = (struct fact_param*) parameters;
    long long int resultado = 1;
    int num;
    for (num = 2; num <= param->number; num++) 
    {
        resultado= resultado*num;
        printf ("Factorial de %d, resultado parcial %lld\n", param->number, resultado);
        sleep (random()%3);
    }
    printf ("El factorial de %d es %lld\n", param->number, resultado);
    printf("Thread PID: %lu \n", (unsigned long)pthread_self());
    return NULL;
}

