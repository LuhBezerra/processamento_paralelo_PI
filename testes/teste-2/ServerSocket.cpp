#include "ServerSocket.h"
#include "SocketException.h"


ServerSocket::ServerSocket ( int port )
{
  if ( ! Socket::create() )
    {
      throw SocketException ( "Não foi possível criar o soquete do servidor." );
    }

  if ( ! Socket::bind ( port ) )
    {
      throw SocketException ( "Não foi possível ligar à porta." );
    }

  if ( ! Socket::listen() )
    {
      throw SocketException ( "Não foi possível atender o soquete." );
    }

}

ServerSocket::~ServerSocket()
{
}


const ServerSocket& ServerSocket::operator << ( const std::string& s ) const
{
  if ( ! Socket::send ( s ) )
    {
      throw SocketException ( "Não foi possível gravar no soquete." );
    }

  return *this;

}


const ServerSocket& ServerSocket::operator >> ( std::string& s ) const
{
  if ( ! Socket::recv ( s ) )
    {
      throw SocketException ( "Não foi possível ler do soquete." );
    }

  return *this;
}

void ServerSocket::accept ( ServerSocket& sock )
{
  if ( ! Socket::accept ( sock ) )
    {
      throw SocketException ( "Não foi possível aceitar o soquete." );
    }
}