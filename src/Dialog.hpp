#ifndef _DIALOG_HPP_
#define _DIALOG_HPP_

#include "types.hpp"
#include "Socket.hpp"
#include "Exception.hpp"
#include <glibmm/ustring.h>
#include <string>

/**
 * Class which objects are created everytime conversation is started.
 * Dialogs are stored in HashMap, managed by singleton class.
 */
class Dialog
{
public:
    /**
     * Constructor creates Dialog with given IPv6 address.
     * It creates ClientSocket as ConversableSocket and becomes ready to conversate after sucesfull connection.
     * @param const ChaTIN::IPv6& Reference for IPv6 address.
     * @param const unsigned int Port to send to (in most cases read from Config).
     * @param const std::string& Reference to string containing interface used for connection.
     * @throw Socket::ConnectionFailureException Exception thrown when socket cannot establish connection to given ipv6 address.
     */
    Dialog(const ChaTIN::IPv6&, const std::string&, const unsigned int) throw(Socket::ConnectionFailureException);

    /**
     * Constructor creates Dialog with given ConversableSocket (i.e. received from DialogManager (it's ServerSocket) ).
     * Socket has to be connected before constructor invokation.
     * @param const ConversableSocket& Reference to ConversableSocket to be used to conversate.
     * @throw SocketNotWritableException Exception thrown when socket is not writable.
     */
    Dialog(const Socket::Conversable*);

    ~Dialog();

    /**
     * Method sends message through socket.
     * @param const Glib::ustring& Reference to unicode string to be send through socket.
     * @see Socket::Conversable::send
     */
    void send(const Glib::ustring&) const throw(Socket::SendFailureException,Socket::NotConnectedException);

    /**
     * Receives unicode message from socket.
     * Depending on implementation of ConversableSocket this method can be hanging up or not.
     * @return Glib::ustring Unicode string received from socket.
     * @see Socket::Conversable::receive
     */
    Glib::ustring receive() const throw(Socket::ReceiveFailureException,Socket::NotConnectedException);
protected:
    const Socket::Conversable* conversationSocket;
};

#endif
