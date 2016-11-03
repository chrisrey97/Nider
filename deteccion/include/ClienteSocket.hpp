#ifndef CLIENTE_SOCKET_HEADER
#define CLIENTE_SOCKET_HEADER

#define ASIO_STANDALONE

#include <iostream>
#include <string>
#include "libsocket/inetclientdgram.hpp"
#include "libsocket/exception.hpp"

namespace nider
{
    class clientesocket
    {
    public:
        void send(std::string host, std::string port, std::string data);
    };
}

#endif
