#include "ClientSocket.h"
#include "SocketException.h"


ClientSocket::ClientSocket ( std::string host, int port )
{
  if ( ! Socket::create() )
    {
      throw SocketException ( "Não foi possível criar o soquete do cliente." );
    }

  if ( ! Socket::connect ( host, port ) )
    {
      throw SocketException ( "Não foi possível ligar à porta." );
    }

}


const ClientSocket& ClientSocket::operator << ( const std::string& s ) const
{
  if ( ! Socket::send ( s ) )
    {
      throw SocketException ( "Não foi possível gravar no soquete." );
    }

  return *this;

}


const ClientSocket& ClientSocket::operator >> ( std::string& s ) const
{
  if ( ! Socket::recv ( s ) )
    {
      throw SocketException ( "Não foi possível ler do soquete." );
    }

  return *this;
}