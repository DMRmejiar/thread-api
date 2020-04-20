#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <math.h>
#include <pthread.h>

int threads;
float mean;

struct fvector_params
{
    float *data;
    int start;
    int size;
    float sum;
    float sd;
};

float calculateSD(float [], int);

int main (int argc, char *argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "Error I need two arguments.\n");
        exit (0);
    }
    threads = atoi (argv[2]);
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
    float notas[line_count];
    int notas_size = 0;

    do
    {
        line_size = getline(&line_buf, &line_buf_size, fp2);
        if (line_size > 1)
        {
            notas[notas_size] = atof (line_buf);
            notas_size++;
        }
    } while (line_size >= 0);

    free(line_buf);
    line_buf = NULL;
    fclose(fp2);
    for (int i = 0; i < notas_size; i++)
    {
        printf("Nota: %f\n", notas[i]);
    }
    printf("\nStandard Deviation = %.4f\n", calculateSD(notas, notas_size));
    return 0;
}

int sum = 0;

void *first_for(void *parameters)
{
    struct fvector_params *param = (struct fvector_params *) parameters;
    for (int i = param->start; i < param->size; ++i) 
    {
        param->sum += param->data[i];
    }
    return NULL;
}

void *second_for(void *parameters)
{
    struct fvector_params *param = (struct fvector_params *) parameters;
    for (int i = param->start; i < param->size; ++i) 
    {
        param->sd += pow(param->data[i] - mean, 2);
    }
    return NULL;
}

float calculateSD(float data[], int size) {
    float sum = 0.0, sd = 0.0;
    int i;
    int start = 0;
    pthread_t threads_ids[threads];
    struct fvector_params vectors[threads];
    for (i = 0; i < threads; i++)
    {
        vectors[i].data = data;
        vectors[i].start = start;
        start = ((int) size/threads) + start;
        vectors[i].size = start;
        vectors[i].sum = 0.0;
        vectors[i].sd = 0.0;
    }
    vectors[threads-1].size = size;
    for (i = 0; i < threads; i++)
    {
        pthread_create (&threads_ids[i], NULL, (void *)first_for, &vectors[i]);
    }
    /* first_for
    for (i = 0; i < size; ++i) 
    {
        sum += data[i];
    }
    */
    for (i = 0; i < threads; i++)
    {
        pthread_join (threads_ids[i], NULL);
    }
    for (i = 0; i < threads; i++)
    {
        sum += vectors[i].sum;
    }
    mean = sum / size;
    for (i = 0; i < threads; i++)
    {
        pthread_create (&threads_ids[i], NULL, (void *)second_for, &vectors[i]);
    }
    /* second_for
    for (i = 0; i < size; ++i)
    {
        sd += pow(data[i] - mean, 2);
    }
    */
    for (i = 0; i < threads; i++)
    {
        pthread_join (threads_ids[i], NULL);
    }
    for (i = 0; i < threads; i++)
    {
        sd += vectors[i].sd;
    }
    return sqrt(sd / size);
}