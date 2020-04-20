#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>

#define MAXLON 1000000

float sum;

void *sum_vector(void *);

struct vector_params {
    float *data;
    int min;
    int max;
    float sum;
};


int main (int argc, char *argv[])
{ 
    char *line_buf = NULL;
    size_t line_buf_size = 0;
    int line_count = 0;
    ssize_t line_size;
    FILE *fp = fopen(argv[1], "r");
    if (!fp)
    {
        fprintf(stderr, "Error opening file '%s'\n", argv[1]);
        return 0;
    }

    do
    {
        line_size = getline(&line_buf, &line_buf_size, fp);
        if (line_size > 1)
        {
            line_count++;
        }
        /* Show the line details 
        printf("line[%06d]: chars=%06zd, buf size=%06zu, contents: %s", line_count + 1, line_size, line_buf_size, line_buf);
        */
    } while (line_size >= 0);

    free(line_buf);
    line_buf = NULL;
    fclose(fp);
    FILE *fp2 = fopen(argv[1], "r");
    line_buf_size = 0;
    float vector[line_count];
    int vector_size = 0;

    do
    {
        line_size = getline(&line_buf, &line_buf_size, fp2);
        if (line_size > 1)
        {
            vector[vector_size] = atof (line_buf);
            vector_size++;
        }
    } while (line_size >= 0);

    free(line_buf);
    line_buf = NULL;
    fclose(fp2);
    struct vector_params myVector[2];
    myVector[0].data = vector;
    myVector[0].min = 0;
    myVector[0].max = (int) vector_size/2;
    myVector[0].sum = 0;
    myVector[1].data = vector;
    myVector[1].min = (int) vector_size/2;
    myVector[1].max = vector_size;
    myVector[1].sum = 0;
    pthread_t threads_ids[2];
    clock_t begin = clock();
    pthread_create (&threads_ids[0], NULL, &sum_vector, &myVector[0]);
    pthread_create (&threads_ids[1], NULL, &sum_vector, &myVector[1]);
    pthread_join(threads_ids[0], NULL);
    pthread_join(threads_ids[1], NULL);
    sum = myVector[0].sum + myVector[1].sum;
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Vector de tamaño: %d\nLa suma es: %f\nTiempo empleado de la suma: %f\n", vector_size, sum, time_spent);
}

void *sum_vector(void *parameters)
{
    struct vector_params *param = (struct vector_params *) parameters;
    for (int i = param->min; i < param->max; i++)
    {
        param->sum += *(param->data + i);
    }
    //printf("Suma: %d\n", sum);
    return NULL;
}