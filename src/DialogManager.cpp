#include "DialogManager.hpp"

DialogManager::DialogManager(const ToViewParser& toViewParser, const AliasManager& aliasManager, const ConferenceManager& conferenceManager, const Config&) : toViewParser(toViewParser), aliasManager(aliasManager), conferenceManager(conferenceManager), config(config), working(false)
{
}

void DialogManager::sendTo(const ChaTIN::Alias& alias, const Glib::ustring& message) throw(Socket::SendFailureException)
{
    const ChaTIN::IPv6 ip = aliasManager.getIP(alias); // resolve ip from alias
    const Dialog& dialog = getDialog(ip); // get dialog for ip
    dialog.send(message);
}

void DialogManager::sendTo(const ChaTIN::ConferenceId& conferenceId, const Glib::ustring& message) throw(Socket::SendFailureException)
{
    const std::vector<ChaTIN::IPv6> &IPs = conferenceManager.getList(conferenceId);
    for(auto& ip : IPs)
    {
        const Dialog& dialog = getDialog(ip);
        dialog.send(message);
    }
}

const Dialog& DialogManager::getDialog(const ChaTIN::IPv6& ip)
{
    int port = config.getValue<int>("port");
    std::unordered_map<const ChaTIN::IPv6,const Dialog*>::const_iterator iter = dialogMap.find(ip);
    if(iter == dialogMap.end()) // if no dialog found
    {
        dialogMap[ip] = new Dialog(ip,port); // create new one and assign to map of IPs.
    }
    return *(dialogMap[ip]); //MBO
}

void DialogManager::startServer() throw(Socket::ResolveException,Socket::WrongPortException)
{
    if(serverSocket == NULL)
    {
        std::string host = config.getValue<std::string>("host");
        unsigned int port = config.getValue<int>("port");
        unsigned int backlog = config.getValue<int>("backlog");
        serverSocket = new Socket::ServerSocket(host,port,backlog);
    }
    serverSocket->listen();
    Socket::ServerSocket::ClientIncomeSocket* incomeSocket;
    working = true;
    while(working)
    {
        incomeSocket = serverSocket->pickClient(); // can hang up here when no client to pick is available
        dispatchIncomingSocket(*incomeSocket);
    }
}

void DialogManager::operator()() throw(Socket::ResolveException,Socket::WrongPortException)
{
    startServer();
}

void DialogManager::dispatchIncomingSocket(const Socket::ServerSocket::ClientIncomeSocket& incomeSocket)
{
    //FIXME implement this
}
