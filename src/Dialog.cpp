#include "Dialog.hpp"
#include "Config.hpp"
#include <string>
#include <glibmm/ustring.h>

Dialog::Dialog(const Glib::ustring& ipAddress, const unsigned int port) throw(Socket::ConnectionFailureException) : conversationSocket(new Socket::ClientSocket())
{
    ((Socket::ClientSocket*)(conversationSocket))->connect(std::string(ipAddress.c_str()),port);
}

Dialog::Dialog(const Socket::Conversable* conversationSocket) : conversationSocket(conversationSocket)
{
}

void Dialog::send(const Glib::ustring& message) const
{
    conversationSocket->send(std::string(message.c_str()));
}

Glib::ustring Dialog::receive() const throw(Socket::ReceiveFailureException)
{
    std::string message = conversationSocket->receive();
    return Glib::ustring(message);
}
