#ifndef _DIALOG_MANAGER_HPP_
#define _DIALOG_MANAGER_HPP_

#include "Dialog.hpp"
#include "ToViewParser.hpp"
#include "AliasManager.hpp"
#include "ConferenceManager.hpp"
#include "Config.hpp"
#include "types.hpp"

#include <glibmm/ustring.h>
#include <unordered_map>
#include <string>

class AliasManager;

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
     * @param const AliasManager& Reference to alias manager.
     * @param const ConferenceManager& Reference to conference manager.
     * @param const Config& Reference to config class.
     */
    DialogManager(const ToViewParser&, const AliasManager&, const ConferenceManager&, const Config&);

    /**
     * Send message to given IPv6 address or alias.
     * @param const ChaTIN::Alias& Reference to IPv6 or alias as unicode string.
     * @param const Glib::ustring& Reference to message to send as unicode string.
     */
    void sendTo(const ChaTIN::Alias&,const Glib::ustring&) throw(Socket::SendFailureException);

    /**
     * Send message to given conference, by it's name.
     * @param const ChaTIN::ConferenceId& Reference to conference id of conference we want broadcast message to.
     * @param const Glib::ustring& Reference to unicode string containing message to send.
     */
    void sendTo(const ChaTIN::ConferenceId&,const Glib::ustring&) throw(Socket::SendFailureException);

    /**
     * Starts server, which handles incomming connections and dispatches it.
     * @throw Socket::ResolveException Exception thrown when resolvation of address failed.
     * @throw Socket::WrongPortException Exception thrown when chosen port to listen on is wrong.
     */
    void startServer() throw(Socket::ResolveException,Socket::WrongPortException);

    /**
     * This is the same as calling startServer() but can be used as argument to boost::thread.
     * @see startServer()
     * @throw Socket::ResolveException Exception thrown when resolvation of address failed.
     * @throw Socket::WrongPortException Exception thrown when chosen port to listen on is wrong.
     */
    void operator()() throw(Socket::ResolveException,Socket::WrongPortException);

protected:
    /**
     * Get dialog by it's IPv6 address or alias.
     * @param const ChaTIN::IP& Reference to IPv6.
     * @return const Dialog& Reference to found dialog.
     */
    const Dialog& getDialog(const ChaTIN::IPv6&);

    /**
     * Method dispatches incoming connections.
     * Runs new thread and enters infinity loop handling client's requests.
     * @param const Socket::Server::ClientIncomeSocket& Reference to incame socket.
     */
    void dispatchIncomingSocket(const Socket::ServerSocket::ClientIncomeSocket&);

    /**
     * Unordered map to store association IPv6 address -> Dialog.
     */
    std::unordered_map<const ChaTIN::IPv6,const Dialog*> dialogMap;

    /**
     * Reference to incoming network packets grammar parser.
     */
    const ToViewParser& toViewParser;

    /**
     * Reference to alias manager
     */
    const AliasManager& aliasManager;

    /**
     * Reference to conference manager
     */
    const ConferenceManager& conferenceManager;

    /**
     * Reference to config
     */
    const Config& config;

    /**
     * Pointer to server socket which will be in listen mode after startServer().
     */
    Socket::ServerSocket* serverSocket;

    /**
     * Variable set to true, when server is in working state (listening and ready for incoming clients), or false when it should stop accepting clients.
     */
    bool working;

private:
    /**
     * Send message to given socket.
     * Method is invoked by sendTo(const Glib::ustring&), when grammar parser demand or on user action.
     */
    void sendTo(const Socket::Conversable&) const;
};

#endif
