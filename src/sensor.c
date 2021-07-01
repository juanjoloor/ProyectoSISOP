#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>
#define BUFLEN 128
#define QLEN 10

#ifndef HOST_NAME_MAX
#define HOST_NAME_MAX 256
#endif

#define QUEUESIZE 10

void generarInfo(const char *buffers, char *info);
void *conexionSocket(void *args);
void *recibirCommands(void *args);

typedef struct SocketArgs
{
    char *buffers;
    int port;
} SocketArgs;

int main(int argc, char const *argv[])
{
    if (argc < 0)
    {
        fprintf(stderr, "Necesita dos argumentos\n");
        return -1;
    }
    char *buffers = (char *)argv[1];
    const int port = atoi(argv[2]);
    pthread_t tid[2];
    pthread_create(&tid[0], NULL, recibirCommands, NULL);
    struct SocketArgs sa;
    sa.buffers = buffers;
    sa.port = port;
    pthread_create(&tid[1], NULL, conexionSocket, (void *)&sa);
    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL);
    return 0;
}

void *recibirCommands(void *args)
{
    while (1)
    {
        char command[10];
        printf(":\n");
        scanf("%s", command);
        if (strcmp(command, "Pausar") == 0)
        {
        }
        else if (strcmp(command, "Continuar") == 0)
        {
        }
    }
}

void *conexionSocket(void *args)
{
    struct SocketArgs *var = (struct SocketArgs *)args;
    struct sockaddr_in direccion_destino;
    memset(&direccion_destino, 0, sizeof(direccion_destino)); 
    direccion_destino.sin_family = AF_INET;                  
    direccion_destino.sin_port = htons(var->port);              
    direccion_destino.sin_addr.s_addr = inet_addr("127.0.0.1"); 
    int fd;
    if ((fd = socket(direccion_destino.sin_family, SOCK_STREAM, 0)) < 0)
    {
        printf("Error al crear socket\n");
    };

    int res = connect(fd, (struct sockaddr *)&direccion_destino, sizeof(direccion_destino));
    if (res < 0)
    {
        printf("Error al conectar\n");
    }
    else
    {
        printf("Conectado\n");
        while (1)
        {
            int n = 0;
            char info[100];
            generarInfo(var->buffers, info);
            n = write(fd, info, 100);
            if (n < 0)
            {
                printf("Error de conexion con framework");
            }
            sleep(5);
        }
    }
    return NULL;
}

int numeroALAzar(int lower, int upper)
{
    return (rand() %
            (upper - lower + 1)) +
           lower;
}

void generarInfo(const char *buffers, char *info)
{
    time_t seconds;
    seconds = time(NULL);
    int numeroRandom = numeroALAzar(0, 255);
    sprintf(info, "%s %d %ld", buffers, numeroRandom, seconds);
}
