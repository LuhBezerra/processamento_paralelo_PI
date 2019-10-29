#include "ClientSocket.h"
#include "SocketException.h"
#include <iostream>
#include <string>

int main ( int argc, int argv[] )
{
  try
    {

      ClientSocket client_socket ( "localhost", 30000 );

      std::string reply;

      try
	{
	  client_socket << "Messagem teste";
	  client_socket >> reply;
	}
      catch ( SocketException& ) {}

      std::cout << "Recebemos está resposta do servidor:\n\"" << reply << "\"\n";;

    }
  catch ( SocketException& e )
    {
      std::cout << "A exceção foi capturada:" << e.description() << "\n";
    }

  return 0;
}