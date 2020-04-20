#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>

#define MAXLON 1000000

float sum = 0;

void *sum_vector(void *);

struct vector_params {
    float *data;
    int size;
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
    struct vector_params myVector;
    myVector.data = vector;
    myVector.size = vector_size;
    clock_t begin = clock();
    sum_vector(&myVector);
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Vector de tamaño: %d\nLa suma es: %f\nTiempo empleado de la suma: %f\n", vector_size, sum, time_spent);
}

void *sum_vector(void *parameters)
{
    struct vector_params *param = (struct vector_params *) parameters;
    float l_sum = 0;
    for (int i = 0; i < param->size; i++)
    {
        l_sum += *(param->data + i);
    }
    sum += l_sum;
    return NULL;
}