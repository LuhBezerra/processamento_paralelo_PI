#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> // biblioteca com varios tipos de dados usados em chamadas de sistemas
#include <sys/socket.h> // bliblioteca para usar o socket
#include <netinet/in.h> // necessaria para endereços de dominio  da internet

void funcionalidades(int); // precisa criar ela aqui em cima tbm para nao dar erro no escopo, mais necessariamente essa mensagem -> was not declared in this scope

void erro(const char *mensagemErro) // essa funcao vai ser usada quando alguma coisa do sistema falha
{
    perror(mensagemErro); // esse 'perror' vai mapear onde esta o erro e mostra a mensagem
    exit(1);
}

int main (int argc, char * argv [])
{
    int sockFd, novoSockFd, numPorta, novoProcesso; // Fd signifa File Descriptors e vai armazenas os valores que retornarem da chamada do socket e da chamada de sistema aceita
    //char modo = atoi(argv[4]);
    socklen_t tamanhoCliente;   //armazena o tamanho do endereço do cliente
    
    struct sockaddr_in serv_addr, cli_addr; //a sockaddr_in e a que contem um endereco na internet, vem da biblioteca <netinet ...>

    // parametro n2 -> numero de parcelas 5
    // parametro n3 -> numero de clientes
    // parametro n4 -> modo

    if (argc == 1) { //sem parametros
        fprintf(stderr,"ERRO, nenhum parametro fornecido\n");
        exit(1);
     }
    if (argc != 4) { //so nPedidos
        fprintf(stderr,"ERRO, informe os parametros indicados -> nPedidos, nClientes, modo\n");
        exit(1);
     }

    printf("%s",argv[4]);

    sockFd = socket(AF_INET, SOCK_STREAM, 0); // aqui e o primeiro socket que recebe como argumentos, 1º o dominio do endereco do socket, 2º o tipo do socket que nesse caso e TCP, 3º o protocolo que na maioria das vezes recebe zero
    
    if (sockFd < 0) // como a chamada de um socket retorna um FD (que é um numero inteiro), se retornar -1 signica que a chamada do socket falhou 
        erro("Erro na abertura do Socket");

     bzero((char *) &serv_addr, sizeof(serv_addr)); //essa funcao bzero define todos os valores do buffer pra zero, esse 'char *' e o ponterio para o buffer e 'sizeof' pega o tamanho
     
     //numPorta = atoi(argv[1]); // esse 'atoi' converte string em inteiro
     
     serv_addr.sin_family = AF_INET; // contem um codigo para a familia de enderecos
     serv_addr.sin_addr.s_addr = INADDR_ANY; //essa linha de codigo e onde fica o endereco IP do host, ou seja o endereco da maquina que o servidor ira ficar
     serv_addr.sin_port = htons(5000);//Essa função 'htons' converte o número da porta na ordem de bytes do host, em um número de porta na ordem de bytes da rede.

     if (bind(sockFd, (struct sockaddr *) &serv_addr, //A funcao 'bind' vincula um socket a um endereço, nesse caso, o endereço do host e o número das portas atuais nos quais o servidor será executado. 
              sizeof(serv_addr)) < 0) 
              erro("ERROR na chamada"); // pode ocorrer se por exemplo o socket ja estiver executando na maquina

    listen(sockFd,atoi(argv[3])); //A 'listen' permite que o processo escute no socket as conexões. O primeiro argumento é o descritor de arquivo de socket e o segundo é o número de conexões que podem estar aguardando enquanto o processo está lidando com uma conexão específica. 
    
    tamanhoCliente = sizeof(cli_addr);
    while (1) {
        novoSockFd = accept(sockFd,  
               (struct sockaddr *) &cli_addr, &tamanhoCliente); // a funcao 'accept faz com que o processo seja bloqueado ate que um cliente se conecte
        if (novoSockFd < 0) 
            erro("ERRO ao aceitar novo Socket ");
        novoProcesso = fork();
        if (novoProcesso < 0)
            erro("ERRO no fork");
        if (novoProcesso == 0)  {
            close(sockFd);

            funcionalidades(novoSockFd);
            exit(0);
        }
        else close(novoSockFd);
     } 
     close(sockFd);
     return 0; 
}

void funcionalidades (int sock) // So chega nessa parte se um cliente conseguir conectar-se com êxito ao servidor
{   
    char buffer[1024];
    int saida, x, modo,enviaModo;

    printf("\nAguardando Mensagem do cliente...\n");
    
    if (modo == 1)
    {
        enviaModo = write(sock,"Será no modo interativo",25);
    }else if (modo == 2) {
        enviaModo = write(sock,"Será no modo lote",25);
    } 


    bzero(buffer,256);
    saida = read(sock,buffer,255);
    if (saida < 0) erro("ERRO da leitura do socket");
    printf("Valor recebido: %s\n",buffer);
    saida = write(sock,"Recebi sua mensagem",20);
    if (saida < 0) erro("ERRO da escrita no socket");

    x = atoi(buffer);
    printf("Valor x: %d\n",x);

/*
    printf("msg >>");
    fgets(buffer, 1024, stdin);
    sscanf(buffer, "%d", &saida);
    printf("valor lido = %d\n", saida);
*/
}
 /*
void funcionalidades (int sock) // So chega nessa parte se um cliente conseguir conectar-se com êxito ao servidor
{
   int n;
   char buffer[256];
      
   bzero(buffer,256);
   n = read(sock,buffer,255);
   if (n < 0) erro("ERRO da leitura do socket");
   printf("Valor recebido: %s\n",buffer);
   n = write(sock,"Recebi sua mensagem",20);
   if (n < 0) erro("ERRO da escrita no socket");
}
*/

/*char buffer[1024];
        int saida = 0;
        printf("msg >>");
        fgets(buffer, 1024, stdin);
        sscanf(buffer, "%d", &saida);
        printf("valor lido = %d\n", saida);
        return 0;
*/
