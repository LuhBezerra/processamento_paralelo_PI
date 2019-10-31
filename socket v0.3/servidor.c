#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <pthread.h>

int nParcelas;
int nClientes;
int modo;


void funcionalidades () // So chega nessa parte se um cliente conseguir conectar-se com êxito ao servidor
{   
    printf("%d \n",nParcelas);
   
}

void* Servidor(void* arg)
{
    /*Buffer de entrada (armazena buffer do cliente)*/
    char buffer_do_cliente[256];
    /*Cast do ponteiro*/
    int sockEntrada = *(int *) arg;
    /*Loop "infinito"*/
    printf("Aguardando as mensagens... \n");
    funcionalidades();
    for (;;)
    {
        /*Le o que vem do cliente*/
        read(sockEntrada, buffer_do_cliente, sizeof (buffer_do_cliente));
        bzero((char *) &buffer_do_cliente, sizeof(buffer_do_cliente));

        /*Mostrar a parcela na tela do servidor*/
        printf("Enviando a parcela: ");
        snprintf(buffer_do_cliente,2,"%d", nParcelas); // converte o char parcela em int parcela

        /*Envia a parcela em char para o cliente*/
        send(sockEntrada, buffer_do_cliente, strlen(buffer_do_cliente), 0); //envia 
        if (strcmp(buffer_do_cliente, "sair") != 0)
        {
            /*Se buffer == sair cai fora*/
            printf("%s\n",buffer_do_cliente);
        }
        else
             {
                 /*Encerra o descritor*/
                 close(sockEntrada);
                 /*Encerra a thread*/
                 pthread_exit((void*) 0);
             }
    }
}
 
int configuracaoServidor(int cliente)
{
    /*Cria o descritor*/
    int sockfd;
    /*Declaracao da estrutura*/
    struct sockaddr_in server_addr, cli_addr;
    /*Cria o socket*/
    if ((sockfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
    {
      printf("Erro no Socket\n");
      exit(1);
    }
    /*Zera a estrutura*/
    memset(&server_addr, 0, sizeof (server_addr));
    /*Seta a familia*/
    server_addr.sin_family = AF_INET;
    /*Seta os IPS (A constante INADDR_ANY e todos os ips ou qualquer ip) htonl -> conversao*/
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    /*Define a porta*/
    server_addr.sin_port = htons(5000);
    /*Faz a bindagem (cola, gruda, conecta seja o que for)*/
    if (bind(sockfd, (struct sockaddr *) & server_addr, sizeof (server_addr)) < 0)
    {
      printf("Erro no Socket\n");
      exit(1);
     }
    /*Fica na escuta de ate 5 clientes*/
    if (listen(sockfd, cliente) < 0)
    {
      printf("Erro no Socket\n");
      exit(1);
    }
    return sockfd;
}

int main(int argc, char* argv[])
{
    nParcelas = atoi(argv[1]);
    nClientes = atoi(argv[2]);
    modo = atoi(argv[3]);

    if (argc == 1) { //sem parametros
        fprintf(stderr,"ERRO, nenhum parametro fornecido\n");
        exit(1);
     }
    if (argc != 4) { //so nPedidos
        fprintf(stderr,"ERRO, informe os parametros indicados -> nPedidos, nClientes, modo\n");
        exit(1);
     }

    system("clear");
    /*Declaracao da estrutura*/
    struct sockaddr_in server_addr;
    /*Retorna da funcao e o descritor*/
    int sockfd = configuracaoServidor(nClientes);
 
    /*Loop "infinito"*/
    while (1)
    {
        int clienteSockfd;
        struct sockaddr_in clienteAddr;
        /*tamanho da estrutura*/
        unsigned int clntLen;
        clntLen = sizeof (clienteAddr);
        /*declara uma thread*/
    pthread_t thread;
    /*Fica no aguardo da conexao do cliente*/
        if ((clienteSockfd = accept(sockfd, (struct sockaddr *) & clienteAddr, &clntLen)) < 0)
        {
      printf("Erro no Socket\n");
      exit(1);
    }
        /*Inicializa a thread*/
        if (pthread_create(&thread, NULL, Servidor, &clienteSockfd) != 0)
       {
            printf("Erro na Thread\n");
            exit(1);
       }
        pthread_detach(thread);
    }
    exit(0);
}