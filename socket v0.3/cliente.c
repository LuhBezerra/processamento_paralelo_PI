#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

int parcela;

float interativo()
{
    dividendo = pow(-1,parcela );
    divisor = (2 * parcela) +1;

    resultado = dividendo/divisor
}

float lote()
{
    dividendo = pow(-1,parcela );
    divisor = (2 * parcela) +1;

    resultado = dividendo/divisor

}
 
int configuracaoCliente()
{
    /*Descritor do cliente*/
    int clienteSockfd;
    /*Estrutura do Cliente*/
    struct sockaddr_in serv_addr;
    /*Tamanho da estrutura*/
    socklen_t addrlen = sizeof (serv_addr);
 
    /*Socket familia TCP declarado SOCK_STREAM e criado*/
    clienteSockfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clienteSockfd < 0)
    {
        printf("Erro no Socket\n");
        exit(1);
    }
    /*Zera a estrutura*/
    bzero((char *) & serv_addr, sizeof (serv_addr));
    /*Seta a familia*/
    serv_addr.sin_family = AF_INET;
    /*Define o IP nesse caso o localhost*/
    serv_addr.sin_addr.s_addr = inet_addr("192.168.21.53");
    /*Define a porta de conexao*/
    serv_addr.sin_port = htons(5000);
   /*Faz a conexao com o servidor*/
   if(connect(clienteSockfd,(struct sockaddr *)&serv_addr, addrlen) < 0)
     {
            printf("Erro no Socket\n");
        exit(1);
    }
        return clienteSockfd;
}
 
void Cliente(int clienteSockfd)
{
  /*Buffer de dados a ser mandado para o servidor*/
  char buffer_para_servidor[256];
  printf("Digite uma mensagem para o servidor: ");
   do {
        /*envia mensagem para o servidor, isso vai ser desconsiderado depois*/
        scanf("%s",buffer_para_servidor);
        fflush(stdin);
        bzero((char *) &buffer_para_servidor, sizeof(buffer_para_servidor));

        /*Escreve para o servidor*/
        write(clienteSockfd, buffer_para_servidor, sizeof (buffer_para_servidor));


        /*Recebe a parcela do servidor*/
        read(clienteSockfd, buffer_para_servidor, sizeof (buffer_para_servidor));
        printf("Recebi: %s", buffer_para_servidor);
        parcela = atoi(buffer_para_servidor); // converte a parcela que veio char (do servidor) em inteiro
        bzero((char *) &buffer_para_servidor, sizeof(buffer_para_servidor));

 
      /*Mensagem para sair*/
    } while (strcmp(buffer_para_servidor, "sair") != 0);
    /**Encerra o descritor*/
    close(clienteSockfd);
}
 
int main()
{
    system("clear");
    /*Estrutura cliente*/
    struct sockaddr_in serv_addr;
    /*Tamanho da estrutura*/
    socklen_t addrlen = sizeof (serv_addr);
    /*Define o descritor cliente*/
    int descritorCliente;
    descritorCliente = configuracaoCliente();
    /*chama funcao do cliente passando o descritor*/
    Cliente(descritorCliente);
    exit(0);
}