#ifndef _SOCKET_H_
#define _SOCKET_H_

#include "Exception.hpp"

#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

namespace Socket
{

/**
 * Class gives abstraction of socket connections.
 */
class Socket
{

public:
    /**
     * Type of I/O methods to be used: NonBlocking or Blocking
     */
    enum BlockingType { SocketNonBlocking, SocketBlocking };

    virtual ~Socket();
    /**
     * Getter for host port.
     * @return const int host port.
     */
    virtual int getHostPort() const;
    /**
     * Set host port, but do NOT bind again!
     * @param const unsigned int Port number. Should be number from range: 0-65535.
     * @throw WrongPortException Exception is thrown when given port is out of range.
     */
    virtual void setHostPort(const unsigned int) throw(WrongPortException);
    /**
     * Getter for host address.
     * @return const std::string Host address.
     */
    virtual std::string getHostAddress() const;
    /**
     * Sets host address by given string.
     * @param std::string& Reference to string of address to set as host address
     * @throw WrongAddressException Exception is thrown when given IP address cannot be parsed.
     */
    virtual void setHostAddress(const std::string&) throw(WrongAddressException);
protected:
    Socket();
    Socket(int);
    /**
     * Method creates and binds correct socket, resolved with DNS resolver by given address and port.
     * @param const std::string& Reference to address to be resolved and binded to
     * @param const unsigned int Port to be binded on.
     */
    void getBindedSocket(const std::string&, const unsigned int);

    /**
     * Method resolves with DNS resolver address and port and returns it as inetaddr_in6 structure.
     * @param const std::string& Reference to address.
     * @param const unsigned int Port.
     * @return inetaddr_in6 Structure containing resolved address and port.
     */
    sockaddr_in6 getAddressStructure(const std::string&, const unsigned int);
    
    int sockfd;
    sockaddr_in6 *hostAddress;

private:
    /**
     * Method resolves with DNS resolver address and port and returns it as linked list of addrinfo.
     * You have to manage this structure. After using, be sure it's free'd with freeaddrinfo(addrinfo*).
     * @param const std::string& address
     * @param const unsigned int port
     * @return addrinfo* Pointer to first element of linked list of addrinfos.
     */
    addrinfo* getResolvedAddrinfo(const std::string&, const unsigned int);

};

class ServerSocket : public Socket
{
public:
    /**
     * Constructor of ServerSocket. Creates socket, ready to listen. It binds on given IP address.
     * @param enum BlockingType type of I/O socket methods to be used (blocking or nonblocking)
     * @param std::string& Reference to string of local address to bind (example format: 2001:500:88:200::8). Most common case: you want use :: (bind on all net interfaces) )
     * @param const unsigned int Port to bind on. When no port given we assume system gives us ephemeral one.
     * @param const unsigned int Size of backlog.
     * @throw ResolveException Exception is thrown when given IP address cannot be resolved by DNS. Details are stored in thrown exception class.
     */
    ServerSocket(enum BlockingType, const std::string&, const unsigned int, const unsigned int) throw(ResolveException, WrongPortException);
    virtual ~ServerSocket();

    /**
     * Turns socket into listen mode.
     * After this method server can handle clients.
     * @throw ListenFailureException Exception is thrown when socket cannot be turned into listen state. Details about problem is stored in thrown class.
     */
    void listen() const throw(ListenFailureException);
    /**
     * Checks whether there is any client in queue to pick
     * @return bool true when there is an client to pick, false otherwise.
     */
    bool isClientEnqueued() const throw(SelectFailureException);
    
    class ClientIncomeSocket : public Socket
    {
    public:
        static const unsigned int buffer_size = 1024;
        /**
         * Constructor.
         * @param int Descriptor of socket handled by main server socket.
         */
        ClientIncomeSocket(int);

        /**
         * Send data to socket.
         * @param const std::string& Reference to string containing data to send.
         */
        void send(const std::string&) const;
        /**
         * Receive data from socket.
         * @return const std::string String containing received data.
         */
        std::string receive() const throw(ReceiveFailureException);
    };

    /**
     * Handle client. Get him from queue and return his socket descriptor. 
     * @return ClientIncomeSocket* Socket class ready to talk with handled client.
     */
    ClientIncomeSocket* pickClient() const throw(AcceptFailureException);


protected:
    unsigned int backlog;
};

class ClientSocket : public Socket
{
public:
    /**
     * Constructor of ClientSocket. Creates client socket, ready to connect to remote host. It binds on given IP address.
     * @param enum BlockingType type of I/O socket methods to be used (blocking or nonblocking)
     * @param std::string& Reference to string of local address to bind (example format: 2001:500:88:200::8). Most common case: you want use :: (let system guess network interface we need).
     * @throw WrongAddressException Exception is thrown when given IP address cannot be parsed.
     */
    ClientSocket(enum BlockingType, const std::string&, const unsigned int) throw(WrongAddressException, WrongPortException);
    virtual ~ClientSocket();

    /**
     * Connets to given IP address.
     * After using this method you are ready to send packets.
     * @param std::string& Reference to string of remote address to connect.
     * @param const unsigned int Port to connect.
     * @throw ConnectionFailureException Exception is thrown when connect failed. Details of connection problem are included in thrown exception class.
     */
    void connect(const std::string&, const unsigned int) throw(ConnectionFailureException);
    /**
     * Send data to server.
     * @param const std::string& Reference to string containing data to send.
     * @throw NotConnectedException Exception is thrown when send is invoked without earlier connection being established.
     */
    void send(const std::string&) const throw(NotConnectedException);
    /**
     * Receive data from server.
     * @return const std::string String containing received data.
     * @throw NotConnectedException Exception is thrown when receive is invoked without earlier connection being established.
     */
    std::string receive() const throw(NotConnectedException);
};

}; // namespace Socket

#endif
