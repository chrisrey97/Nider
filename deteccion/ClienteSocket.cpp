#include "ClienteSocket.hpp"

void nider::clientesocket::send(std::string host, std::string port, std::string data)
{
    std::string answer;
    answer.resize(32);
    libsocket::inet_dgram_client sock(LIBSOCKET_IPv4);
    try
    {
        std::cout << sock.gethost();
    } catch (const libsocket::socket_exception& exc)
    {
        std::cerr << exc.mesg;
    }
    try
    {
	    sock.connect(host,port);
	    sock << data;
	    sock >> answer;
	    std::cout << "Answer from server: " << answer << std::endl;
	    sock.deconnect();
    } catch ( const libsocket::socket_exception& exc )
    {
        std::cerr << exc.mesg;
    }
    sock.destroy();
}
