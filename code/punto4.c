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
    if (argc < 2)
    {
        printf ("Indica el nombre de un fichero o ficheros.\n");
        exit (0);
    }
    pthread_t threads_ids[argc-1];
    struct cuenta_param thread_args[argc-1];
    for (int i = 0; i < (argc-1); i++)
    {
        thread_args[i].name = argv[i+1];
        pthread_create (&threads_ids[i], NULL, &cuenta, &thread_args[i]);
    }
    for (int i = 0; i < (argc-1); i++)
    {
        pthread_join (threads_ids[i], NULL);
    }
    return 0;
}

void *cuenta (void *parameters) 
{
    struct cuenta_param* param = (struct cuenta_param*) parameters;
    int pos, cont = 0, leidos;
    char cadena[MAXLON];
    int fd;
    fd = open (param->name, O_RDONLY);
    if (fd == -1)
    {
        printf("El archivo %s no lo encuentro.\n", param->name);
        close(fd);
        return NULL;
    }
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