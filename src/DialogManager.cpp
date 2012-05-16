#include "DialogManager.hpp"

DialogManager::DialogManager(ToViewParser& toViewParser, AliasManager& aliasManager, ConferenceManager& conferenceManager, const Config& config_) : toViewParser(toViewParser), aliasManager(aliasManager), conferenceManager(conferenceManager), config(config_), serverSocket(NULL)
{
    working.reset(new bool(false));
}

DialogManager::DialogManager( DialogManager& dm )
    : toViewParser(dm.toViewParser), aliasManager(dm.aliasManager),
      conferenceManager(dm.conferenceManager), config(dm.config)
{}

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
    boost::shared_lock<boost::shared_mutex> lock(mutexLock); // lock for readers
    std::unordered_map<const ChaTIN::IPv6,const Dialog*>::const_iterator iter = dialogMap.find(ip);
    lock.unlock(); // lock no more needed
    if(iter == dialogMap.end()) // if no dialog found
    {
        boost::upgrade_lock<boost::shared_mutex> lock(mutexLock);
        boost::upgrade_to_unique_lock<boost::shared_mutex> uniqueLock(lock); // lock for insertion
        dialogMap[ip] = new Dialog(ip,port); // create new one and assign to map of IPs.
    } // now uniqueLock is destroyed and others can read
    lock.lock(); // lock for reading
    const Dialog& result = *(dialogMap[ip]); // MBO
    lock.unlock();
    return result;
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
    working.reset(new bool(true));
    while(*working)
    {
        incomeSocket = serverSocket->pickClient(); // can hang up here when no client to pick is available
        dispatchIncomingSocket(*incomeSocket);
    }
}

void DialogManager::operator()() throw(Socket::ResolveException,Socket::WrongPortException)
{
    startServer();
}

DialogManager::dispatcher::dispatcher(const Socket::ServerSocket::ClientIncomeSocket& incomeSocket_, DialogManager& dialogManager_) : incomeSocket(incomeSocket_), dialogManager(dialogManager_)
{}

boost::thread DialogManager::dispatchIncomingSocket(const Socket::ServerSocket::ClientIncomeSocket& incomeSocket)
{
    dispatcher dispatcherCall(incomeSocket,*this);
    return boost::thread(dispatcherCall);//,&incomeSocket,boost::ref(*this)); // we assign pointer, because it's dynamically created, so the memory scope is the same in all threads.

}

void DialogManager::dispatcher::operator()()//const Socket::ServerSocket::ClientIncomeSocket* incomeSocket, DialogManager& dialogManager)
{
    ChaTIN::IPv6 ip = incomeSocket.getHostAddress();
    Dialog* dialog = new Dialog(&incomeSocket);

    boost::upgrade_lock<boost::shared_mutex> lock(dialogManager.mutexLock);
    boost::upgrade_to_unique_lock<boost::shared_mutex> uniqueLock(lock); // lock for insertion
    dialogManager.dialogMap[ip] = dialog;
    lock.unlock(); // let others read data I already put in map

    while(1) // TODO change condition
    {
        std::string receivedMessage = dialog->receive(); // can hang up here
        dialogManager.toViewParser.doCommand(ip,receivedMessage); // send received message to parser
    }
}
