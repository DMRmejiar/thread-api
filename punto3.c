#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>

#define MAXLON 1000

void *cuenta (void *);

struct cuenta_param 
{
    char *name;
};

int main (int argc, char *argv[])
{ 
    if (argc!= 2)
    {
        printf ("Indica el nombre de un fichero.\n");
        exit (0);
    }
    pthread_t threads_ids;
    struct cuenta_param thread_args;
    thread_args.name = argv[1];
    pthread_create (&threads_ids, NULL, &cuenta, &thread_args);
    pthread_join (threads_ids, NULL);
    return 0;
}

void *cuenta (void *parameters) 
{
    struct cuenta_param* param = (struct cuenta_param*) parameters;
    int pos, cont = 0, leidos;
    char cadena[MAXLON];
    int fd;
    fd = open (param->name, O_RDONLY);
    while ((leidos = read (fd, cadena, MAXLON)) != 0) 
    {
        for (pos = 0; pos < leidos; pos++) 
        {
            if ((cadena[pos] == 'a') || (cadena[pos] == 'A')) 
            {
                cont++;
            }
        }
    }
    printf("Fichero %s: %d caracteres 'a' o 'A' encontrados\n", param->name, cont);
    close(fd);
    return NULL;
}
