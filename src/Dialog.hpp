#ifndef _DIALOG_HPP_
#define _DIALOG_HPP_

#include "Socket.hpp"
#include "Exception.hpp"
#include <glibmm/ustring.h>

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
     * @param const Glib::ustring& Reference for unicode string with IPv6 address.
     * @throw Socket::ConnectionFailureException Exception thrown when socket cannot establish connection to given ipv6 address.
     */
    Dialog(const Glib::ustring&) throw(Socket::ConnectionFailureException);

    /**
     * Constructor creates Dialog with given ConversableSocket (i.e. received from DialogManager (it's ServerSocket) ).
     * Socket has to be connected before constructor invokation.
     * @param const ConversableSocket& Reference to ConversableSocket to be used to conversate.
     * @throw SocketNotWritableException Exception thrown when socket is not writable.
     */
    Dialog(const Socket::Conversable*);

    /**
     * Method sends message through socket.
     * @param const Glib::ustring& Reference to unicode string to be send through socket.
     */
    void send(const Glib::ustring&) const;

    /**
     * Receives unicode message from socket.
     * Depending on implementation of ConversableSocket this method can be hanging up or not.
     * @return Glib::ustring Unicode string received from socket.
     * @throw ReceiveFailureException Exception thrown when receive failed. Details are stored in exception object.
     */
    Glib::ustring receive() const throw(Socket::ReceiveFailureException);
protected:
    const Socket::Conversable* conversationSocket;
};

#endif
