#ifndef _DIALOG_MANAGER_HPP_
#define _DIALOG_MANAGER_HPP_

#include "Dialog.hpp"
#include "ToViewParser.hpp"

#include <glibmm/ustring.h>
#include <unordered_map>

/**
 * Singleton class stores dialogs and provides functionality of dispatching incomming connections.
 */
class DialogManager
{
public:
    /**
     * Constructor of DialogManager.
     * Stores reference to network packets grammar parser.
     * @param const ToViewParser& Reference to network packets grammar parser.
     */
    DialogManager(const ToViewParser&);
    /**
     * Send message to given IPv6 address or alias.
     * @param const Glib::ustring& Reference to IPv6 or alias as unicode string.
     */
    void sendTo(const Glib::ustring&) const;

    /**
     * Starts server, which handles incomming connections and dispatches it.
     */
    void startServer();

protected:
    /**
     * Get dialog by it's IPv6 address or alias.
     * @param const Glib::ustring& Reference to IPv6 or alias as unicode string.
     * @return Dialog& Reference to found dialog.
     */
    Dialog& getDialog(const Glib::ustring&) const;

    /**
     * Method dispatches incoming connections.
     * Runs new thread and enters infinity loop handling client's requests.
     * @param const Socket::Server::ClientIncomeSocket& Reference to incame socket.
     */
    void dispatchIncomingSocket(const Socket::ServerSocket::ClientIncomeSocket&);

    /**
     * Unordered map to store association IPv6 address -> Dialog.
     */
    std::unordered_map<const Glib::ustring,const Dialog*> dialogMap;
    /**
     * Reference to incoming network packets grammar parser.
     */
    const ToViewParser& toViewParser;
private:
    /**
     * Send message to given socket.
     * Method is invoked by sendTo(const Glib::ustring&), when grammar parser demand or on user action.
     */
    void sendTo(const Socket::Conversable&) const;
};

#endif
