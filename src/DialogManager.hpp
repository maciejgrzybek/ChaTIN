#ifndef _DIALOG_MANAGER_HPP_
#define _DIALOG_MANAGER_HPP_

#include "Dialog.hpp"
#include "ToViewParser.hpp"
#include "AliasManager.hpp"
#include "ConferenceManager.hpp"
#include "Config.hpp"

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
     */
    void startServer();

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
    std::map<const ChaTIN::IPv6,const Dialog*> dialogMap;

    /**
     * Reference to incoming network packets grammar parser.
     */
    const ToViewParser& toViewParser;

    const AliasManager& aliasManager;

    const ConferenceManager& conferenceManager;

    const Config& config;

private:
    /**
     * Send message to given socket.
     * Method is invoked by sendTo(const Glib::ustring&), when grammar parser demand or on user action.
     */
    void sendTo(const Socket::Conversable&) const;
};

#endif
