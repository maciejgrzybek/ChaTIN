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

namespace Socket
{

    Socket::Socket()
    {
    }

    Socket::Socket(int sock) : sockfd(sock)
    {
    }

    Socket::~Socket()
    {
        if(sockfd != 0)
            ::close(sockfd);
        if(hostAddress != NULL)
            free(hostAddress); // TODO: check this! Is it correct way to free this?
    }

    addrinfo* getResolvedAddrinfo(const std::string& address, const unsigned int port)
    {
        addrinfo hints;
        addrinfo *result;
        char portString[6];
        int resolveResult;

        if(port>65535)
            throw WrongPortException(port);

        memset(&hints, 0, sizeof(struct addrinfo));
        hints.ai_family = AF_INET6;     /* Allow only IPv6 */
        hints.ai_socktype = SOCK_DGRAM; /* Datagram socket */
        hints.ai_flags = 0;
        hints.ai_protocol = 0;          /* Any protocol */

        sprintf(portString,"%d",port);

        resolveResult = getaddrinfo(address.c_str(), portString, &hints, &result); // resolve address and port from DNS and put it in proper place in inet_addr structure
        if(resolveResult!=0)
        {
           throw ResolveException(resolveResult); 
        }
        return result;
    }

    sockaddr_in6 getAddressStructure(const std::string& address = "::", const unsigned int port = 0)
    {
        addrinfo *result = getResolvedAddrinfo(address,port);
        sockaddr_in6 resultStruct;
        memcpy(&resultStruct,result->ai_addr,sizeof(result->ai_addr));
        freeaddrinfo(result);
        return resultStruct;
    }

    void Socket::getBindedSocket(const std::string& address = "::", const unsigned int port = 0)
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
        //TODO: possible place of weird behavior - copying between structures of different type. In bytes, PROBABLY ok
        memcpy(hostAddress,rp->ai_addr,sizeof(rp->ai_addr));
        freeaddrinfo(result); 
    }

    int Socket::getHostPort() const
    {
        return static_cast<int>(hostAddress->sin6_port);
    }

    void Socket::setHostPort(const unsigned int port) throw(WrongPortException)
    {
        if(port>65535)
            throw WrongPortException(port);
        hostAddress->sin6_port = htons(port);
    }

    std::string Socket::getHostAddress() const
    {
        char humanReadableAddress[INET6_ADDRSTRLEN]; // 15 dots + 32 chars describes IPv6 as string
        inet_ntop(AF_INET6,&hostAddress->sin6_addr,humanReadableAddress,sizeof(humanReadableAddress));
        return std::string(humanReadableAddress);
    }

    void Socket::setHostAddress(const std::string& address) throw(WrongAddressException)
    {
        in6_addr byteAddress;
        if(inet_pton(AF_INET6,address.c_str(),&byteAddress) != 1)
            throw WrongAddressException(address,errno);
        hostAddress->sin6_addr = byteAddress;
    }
    
    ServerSocket::ServerSocket(const std::string& address, const unsigned int port, const unsigned int backlog, enum BlockingType blockingType) throw(ResolveException, WrongPortException) : backlog(backlog)
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

        int clientSocket = accept(sockfd,&clientAddr,&addrlen);
        if(clientSocket == -1)
            throw AcceptFailureException(errno);

        return new ClientIncomeSocket(clientSocket);
    }

    ServerSocket::ClientIncomeSocket::ClientIncomeSocket(int sock) : Socket(sock)
    {
    }

    void ServerSocket::ClientIncomeSocket::send(const std::string& messageBuffer) const
    {
        ::send(sockfd,messageBuffer.c_str(),messageBuffer.length(),0);
    }

    std::string ServerSocket::ClientIncomeSocket::receive() const throw(ReceiveFailureException)
    {
        char buffer[ServerSocket::ClientIncomeSocket::buffer_size];
        int nread = recv(sockfd,buffer,sizeof(buffer),0);
        if(nread == -1)
            throw ReceiveFailureException(sockfd);
        return std::string(buffer);
    }


    ClientSocket::ClientSocket(const std::string& address, const unsigned int port, enum BlockingType blockingType) throw(WrongAddressException, WrongPortException)
    {
        getBindedSocket(address, port);
    }

    ClientSocket::~ClientSocket()
    {
    }
    
    void ClientSocket::connect(const std::string& address, const unsigned int port) throw(ConnectionFailureException)
    {
        sockaddr_in6 remoteAddress;
        remoteAddress = getAddressStructure(address,port);
        if (::connect(sockfd, (struct sockaddr*)(&remoteAddress), sizeof(remoteAddress)) == -1)
        {
            throw ConnectionFailureException(errno); 
        }
    }

    void ClientSocket::send(const std::string& message) const throw(SendFailureException)
    {
        if(::send(sockfd,message.c_str(),sizeof(message.c_str()),0) == -1)
            throw SendFailureException(errno);
    }

    std::string ClientSocket::receive() const throw(ReceiveFailureException)
    {
        char buffer[ClientSocket::buffer_size];
        int nread = recv(sockfd,buffer,sizeof(buffer),0);
        if(nread == -1)
            throw ReceiveFailureException(sockfd);
        return std::string(buffer);
    }

}; // namespace socket
