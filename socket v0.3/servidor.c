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
#include <stdbool.h>

// essas variaveis sao globais entao podem ser acessadas por qualquer funcao
int nParcelas;
int nClientes;
int modo;
int *parcelasInt;

struct conexao { //aqui vai passar os paramentros que a thread vai receber
    struct sockaddr_in* estrutura;
    int sockId;
    //int parcela;

};
///////////////////////////////////Formula Interativo/////////////////////////////////////////////////////
int distrParcelasInterativo() // coloca as parcelas recebidas pelo servidor em um array
{
    parcelasInt = (int*) malloc(nParcelas* sizeof(int)); // essa uma maneira de criar um vetor de forma dinamica, ou sem precisar definir o tamanho dele

    for (int i = 0; i< nParcelas; i++){
        parcelasInt[i]=i;
    }

}

int  dividirParcelasInt()
{
	int parcelaAtual = -1;
	int cont = 0;
	bool parar = true;
	
	while(cont < nParcelas && parar)
	{
		if(parcelasInt[cont] != -1 ){
			parcelaAtual = 	parcelasInt[cont];
			parar = false;
		}
		parcelasInt[cont] = -1;
		cont++;
	}

	return parcelaAtual;
}
//////////////////////////////////////////////////////////////////////////////////////////////////
void* rotinaThread(void* arg)
{
    struct conexao* conn = (struct conexao*) arg; // chamando a struct conexao para a thread

    //int qualquer = conn->parcela; //  exemplo de como setar valores dentro da thread pela struct

    /*Cast do ponteiro*/
    int sockEntrada = *(int *) arg;
    printf("\n entrou na funcao servidor \n");
    printf("id da thread: %ld\n", pthread_self());

    /*Buffer de entrada (armazena buffer do cliente)*/
    char buffer_do_cliente[256];
    bzero(buffer_do_cliente, 256);
    
    if(modo == 1){

        int parcela = dividirParcelasInt();
            
            snprintf(buffer_do_cliente,12,"%d",parcela);// converte o tipo de int para char
			send(sockEntrada, buffer_do_cliente, sizeof(buffer_do_cliente), 0);// envia o tipo 
        
        //printf("%d",parcela);
        
    }
    
    // 
    // /*Loop "infinito"*/
    // printf("Aguardando as mensagens... \n");
    // funcionalidades();
    // for (;;)
    // {
    //     /*Le o que vem do cliente*/
    //     read(sockEntrada, buffer_do_cliente, sizeof (buffer_do_cliente));
    //     bzero((char *) &buffer_do_cliente, sizeof(buffer_do_cliente));

    //     /*Mostrar a parcela na tela do servidor*/
    //     printf("Enviando a parcela: ");
    //     snprintf(buffer_do_cliente,2,"%d", nParcelas); // converte de int para char

    /*Envia a parcela em char para o cliente*/
    //     send(sockEntrada, buffer_do_cliente, strlen(buffer_do_cliente), 0); //envia 
    //     if (strcmp(buffer_do_cliente, "sair") != 0)
    //     {
    //         /*Se buffer == sair cai fora*/
    //         printf("%s\n",buffer_do_cliente);
    //     }
    else
    {
        /*Encerra o descritor*/
        close(sockEntrada);
        /*Encerra a thread*/
        pthread_exit((void*) 0);
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
    /*Faz a bindagem */
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
    } else {
        printf("Aguardando %d clientes\n", cliente);
    }
    return sockfd;
}

int main(int argc, char* argv[])
{

    if (argc == 1) { //sem parametros
        fprintf(stderr,"ERRO, nenhum parametro fornecido\n");
        exit(1);
     }
    if (argc != 4) { //so nPedidos
        fprintf(stderr,"ERRO, informe os parametros indicados -> nPedidos, nClientes, modo\n");
        exit(1);
     }

    nParcelas = atoi(argv[1]);
    nClientes = atoi(argv[2]);
    modo = atoi(argv[3]);

    if (modo == 1){
        distrParcelasInterativo();
    }
    if (modo == 2){
        printf("Aqui sera o modo lote");
    }
    /*declarando uma thread*/
    pthread_t thread[nClientes];


    system("clear");
    /*Declaracao da estrutura*/
    struct sockaddr_in server_addr;
    /*Retorna da funcao e o descritor*/
    int sockfd = configuracaoServidor(nClientes);
 
    /*Loop*/
    for(int i = 0; i < nClientes; i++) {
        struct sockaddr_in* clienteAddr = (struct sockaddr_in*)malloc(sizeof(struct sockaddr_in));
        struct conexao* conn = (struct conexao*) malloc(sizeof(struct conexao));
        int clienteSockfd;
        unsigned int clntLen;
        clntLen = sizeof (sizeof(struct sockaddr_in));
        printf("Aguardando conexao %d\n", i);
        if ((clienteSockfd = accept(sockfd, (struct sockaddr *)clienteAddr, &clntLen)) < 0)
        {
            printf("Erro no Socket\n");
            exit(1);
        } else {
            printf("Conexao pronta!!\n");
            conn->sockId =  clienteSockfd;
            conn->estrutura = clienteAddr;
            //conn->valor = 10;

    /*Fica no aguardo da conexao do cliente*/
        
        /*Inicializa a thread*/
            if (pthread_create(&thread[i], NULL, rotinaThread, conn) != 0)
            { 
                    printf("Erro na Thread\n");
                    exit(1);
            }
        }
    }
    for(int i = 0; i < nClientes; i++) {
        
        pthread_join(thread[i], NULL);
        printf("INiciando thread %d\n", i);
        //cont ++;
        

    }


    // while (1)
    // {
    //     int clienteSockfd;
    //     s
    //     /*tamanho da estrutura*/
        
    //     /*declara uma thread*/

    //     pthread_detach(thread);
    // }
    // exit(0);
}