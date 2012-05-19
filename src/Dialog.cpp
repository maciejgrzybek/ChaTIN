#include "Dialog.hpp"
#include <string>
#include <glibmm/ustring.h>

Dialog::Dialog(const ChaTIN::IPv6& ipAddress, const std::string& interface, const unsigned int port) throw(Socket::ConnectionFailureException) : conversationSocket(new Socket::ClientSocket())
{
    std::string ip = ipAddress;
    ip += interface;
    ((Socket::ClientSocket*)(conversationSocket))->connect(ip,port);
}

Dialog::Dialog(const Socket::Conversable* conversationSocket) : conversationSocket(conversationSocket)
{
}

void Dialog::send(const Glib::ustring& message) const throw(Socket::SendFailureException)
{
    conversationSocket->send(std::string(message.c_str()));
}

Glib::ustring Dialog::receive() const throw(Socket::ReceiveFailureException)
{
    std::string message = conversationSocket->receive();
    return Glib::ustring(message);
}
