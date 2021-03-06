#include "Socket.hpp"

#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <unistd.h>
#include <cassert>

#include <signal.h>

namespace Socket
{

    Conversable::Conversable(enum BlockingType bt) : Socket(bt)
    {
        signal(SIGPIPE, SIG_IGN);
    }

    Conversable::Conversable(int sock, enum BlockingType bt) : Socket(sock,bt)
    {
        signal(SIGPIPE, SIG_IGN);
    }

    Conversable::~Conversable()
    {
    }

    Socket::Socket(enum BlockingType bt) : blockingType_(bt)
    {
    }

    Socket::Socket(int sock, enum BlockingType bt) : sockfd(sock), blockingType_(bt)
    {
        socklen_t len = sizeof hostAddress;
        if(getpeername(sock,(sockaddr*)&hostAddress,&len)!=0)
        {
            throw WrongAddressException("",errno);
        }
    }

    Socket::~Socket()
    {
        if(sockfd != 0)
            ::close(sockfd);
        //if(hostAddress != NULL)
        //    free(hostAddress); // TODO: check this! Is it correct way to free this?
    }

    bool Socket::isValidIP(const std::string& ipAddress)
    {
        in6_addr byteAddress;
        std::string ip = ipAddress;
        size_t pos = ip.find('%');
        if(pos != std::string::npos)
            ip.erase(pos); // delete everything from % forwards.
        return (inet_pton(AF_INET6,ip.c_str(),&byteAddress) == 1);
    }

    addrinfo* Socket::getResolvedAddrinfo(const std::string& address, const unsigned int port)
    {
        addrinfo hints;
        addrinfo *result;
        char portString[6];
        int resolveResult;

        if(port>65535)
            throw WrongPortException(port);

        memset(&hints, 0, sizeof(struct addrinfo));
        hints.ai_family = AF_INET6;     /* Allow only IPv6 */
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_flags = 0;
        hints.ai_protocol = 0;          /* Any protocol */

        sprintf(portString,"%d",port);

        resolveResult = getaddrinfo(address.c_str(), portString, &hints, &result); // resolve address and port from DNS and put it in proper place in inet_addr structure
        if(resolveResult != 0)
        {
           throw ResolveException(resolveResult); 
        }
        return result;
    }

    //TODO: fix this method. Looks fine, but ::connect() can't use it's products to connect.
    std::pair<const sockaddr,size_t> Socket::getAddressStructure(const std::string& address, const unsigned int port)
    {
        addrinfo *result = getResolvedAddrinfo(address,port);
        std::pair<const sockaddr,size_t> resultPair(*(result->ai_addr),result->ai_addrlen);
        freeaddrinfo(result); // free addrinfo linked list, which is no more needed
        return resultPair;
    }

    void Socket::getBindedSocket(const std::string& address, const unsigned int port) throw(SocketCreationFailureException)
    {
        addrinfo *result, *rp;
        result = getResolvedAddrinfo(address,port);

        for (rp = result; rp != NULL; rp = rp->ai_next)
        {
            sockfd = socket(rp->ai_family, rp->ai_socktype,rp->ai_protocol);
            if (sockfd == -1)
                continue;

            if (bind(sockfd, rp->ai_addr, rp->ai_addrlen) == 0)
                break; // Success
            else
            {
               // TODO: consider throwing bind exception ?
            }
            close(sockfd);
        }
        if((sockfd == -1) || (rp == NULL))
            throw SocketCreationFailureException(errno);
            
        //TODO: possible place of weird behavior - copying between structures of different type. In bytes, PROBABLY ok
        memcpy(&hostAddress,rp->ai_addr,sizeof(rp->ai_addr));
        freeaddrinfo(result); 
    }

    int Socket::getHostPort() const
    {
        return static_cast<int>(hostAddress.sin6_port);
    }

    void Socket::setHostPort(const unsigned int port) throw(WrongPortException)
    {
        if(port>65535)
            throw WrongPortException(port);
        hostAddress.sin6_port = htons(port);
    }

    std::string Socket::getHostAddress() const
    {
        char humanReadableAddress[INET6_ADDRSTRLEN]; // 15 dots + 32 chars describes IPv6 as string
        const char* result = inet_ntop(AF_INET6,&hostAddress.sin6_addr,humanReadableAddress,INET6_ADDRSTRLEN);
        assert("Address stored in Socket cannot be invalid." && result != NULL);
        return std::string(humanReadableAddress);
    }

    void Socket::setHostAddress(const std::string& address) throw(WrongAddressException)
    {
        in6_addr byteAddress;
        if(inet_pton(AF_INET6,address.c_str(),&byteAddress) != 1)
            throw WrongAddressException(address,errno);
        hostAddress.sin6_addr = byteAddress;
    }

    std::string Conversable::receive() const throw(ReceiveFailureException,NotConnectedException)
    {
        char buffer[Conversable::buffer_size + 1];
        memset(buffer,'\0',Conversable::buffer_size + 1); // initialization of incomming messages buffer.
        int nread = ::recv(sockfd,buffer,sizeof(buffer),0);
        if(nread == -1)
            throw ReceiveFailureException(sockfd);
        if(nread == 0)
            throw NotConnectedException();
        return std::string(buffer); 
    }

    void Conversable::send(const std::string& message) const throw(SendFailureException)
    {
        if(::send(sockfd,message.c_str(),message.length(),0) == -1)
            throw SendFailureException(errno);
    }

    ServerSocket::ServerSocket(const std::string& address, const unsigned int port, const unsigned int backlog, enum BlockingType blockingType) throw(ResolveException, WrongPortException) : Socket(blockingType), backlog(backlog)
    {
        getBindedSocket(address, port);
    //        hostAddress = static_cast<sockaddr_in6*>(result->ai_addr);
    }

    ServerSocket::~ServerSocket()
    {
    }

    void ServerSocket::listen() const throw(ListenFailureException)
    {
        if(::listen(sockfd,backlog) != 0)
            throw ListenFailureException(errno);
    }

    bool ServerSocket::isClientEnqueued() const throw(SelectFailureException)
    {
        // TODO: here probably we should check BlockingType setting (set in constructor)...
        fd_set rfds;
        timeval tv;
        int retval;
        FD_ZERO(&rfds);
        FD_SET(sockfd,&rfds); // set our sockfd to be looked after anything to read from

        tv.tv_sec = 0;
        tv.tv_usec = 0;

        retval = select(sockfd+1,&rfds,NULL,NULL,&tv);

        if (retval == -1)
            throw SelectFailureException(errno);
        else if (retval)
            return true;
        else
            return false;
    }

    ServerSocket::ClientIncomeSocket* ServerSocket::pickClient() const throw(AcceptFailureException)
    {
        sockaddr clientAddr;
        socklen_t addrlen;

        int clientSocket = ::accept(sockfd,&clientAddr,&addrlen);
        if(clientSocket == -1)
            throw AcceptFailureException(errno);

        return new ClientIncomeSocket(clientSocket);
    }

    ServerSocket::ClientIncomeSocket::ClientIncomeSocket(int sock) : Conversable(sock)
    {
    }

    void ServerSocket::ClientIncomeSocket::send(const std::string& messageBuffer) const throw(SendFailureException)
    {
        ::send(sockfd,messageBuffer.c_str(),messageBuffer.length(),0);
    }

    std::string ServerSocket::ClientIncomeSocket::receive() const throw(ReceiveFailureException,NotConnectedException)
    {
        char buffer[ServerSocket::ClientIncomeSocket::buffer_size + 1];
        memset(buffer,'\0',ServerSocket::ClientIncomeSocket::buffer_size + 1); // initialization of incomming messages buffer.

        if(blockingType_ == SocketNonBlocking)
        {
            fd_set rfds;
            timeval tv;
            int retval;
            FD_ZERO(&rfds);
            FD_SET(sockfd,&rfds); // set our sockfd to be looked after anything to read from

            tv.tv_sec = 2; // 2s delay
            tv.tv_usec = 0;

            retval = select(sockfd+1,&rfds,NULL,NULL,&tv);

            if (retval == -1)
                throw SelectFailureException(errno);
            else if (!retval)
                return "";
        }
        int nread = ::recv(sockfd,buffer,sizeof(buffer),0);
        if(nread == -1)
            throw ReceiveFailureException(sockfd);
        if(nread == 0)
            throw NotConnectedException();
        return std::string(buffer);
    }


    ClientSocket::ClientSocket(const std::string& address, const unsigned int port, enum BlockingType blockingType) throw(WrongAddressException, WrongPortException) : Conversable(blockingType)
    {
        getBindedSocket(address, port);
    }

    ClientSocket::~ClientSocket()
    {
    }
    
    /**
     * TODO: correct this method to use getAddressStructure instead of getResolvedAddrinfo. If so, move getResolvedAddrinfo to private section.
     * Should be changed, because of memory allocation management.
     * Library should pack requested data and return safely. Pointers that should be free'd should NOT be returned.
     */
    void ClientSocket::connect(const std::string& address, const unsigned int port) throw(ConnectionFailureException)
    {
    //    std::pair<sockaddr,unsigned int> remoteAddress = getAddressStructure(address,port);

        addrinfo* rp = getResolvedAddrinfo(address,port);

//        if (::connect(sockfd, &(remoteAddress.first), remoteAddress.second) == -1)
        int result = ::connect(sockfd, rp->ai_addr, rp->ai_addrlen);
        freeaddrinfo(rp); // can be deleted when getAddressStructure is used.
        if (result == -1)
        {
            
            throw ConnectionFailureException(errno); 
        }
    }

}; // namespace socket
