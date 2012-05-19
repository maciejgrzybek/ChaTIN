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
    std::unordered_map<const ChaTIN::IPv6,const Dialog*>::const_iterator iter;
    int port = config.getValue<int>("clientPort");
    {
        ReadLock lock(mutexLock); // lock for readers
        iter = dialogMap.find(ip);
    } // lock no more needed
    if(iter == dialogMap.end()) // if no dialog found
    {
        const std::string interface = config.getValue<std::string>("clientInterface");
        WriteLock lock(mutexLock); // lock for insertion
        dialogMap[ip] = new Dialog(ip,interface,port); // create new one and assign to map of IPs.
    } // now uniqueLock is destroyed and others can read
    ReadLock lock(mutexLock); // lock for reading
    const Dialog& result = *(dialogMap[ip]); // MBO
    return result;
}

void DialogManager::startServer() throw(Socket::ResolveException,Socket::WrongPortException)
{
    if(serverSocket == NULL)
    {
        std::string host = config.getValue<std::string>("serverHost");
        unsigned int port = config.getValue<int>("serverPort");
        unsigned int backlog = config.getValue<int>("backlog");
        serverSocket = new Socket::ServerSocket(host,port,backlog);
    }
    serverSocket->listen();
    Socket::ServerSocket::ClientIncomeSocket* incomeSocket = NULL;
    working.reset(new bool(true));
    while(*working)
    {
        incomeSocket = serverSocket->pickClient(); // can hang up here when no client to pick is available
        assert("incomeSocket can not be NULL" && incomeSocket != NULL);
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

    {
        WriteLock lock(dialogManager.mutexLock); // lock for insertion
        dialogManager.dialogMap[ip] = dialog;
    } // let others read data I already put in map

    while(1)//*(dialogManager.working))
    {
        try
        {
            std::string receivedMessage = dialog->receive(); // can hang up here
            dialogManager.toViewParser.doCommand(ip,receivedMessage); // send received message to parser
        }
        catch(Socket::ReceiveFailureException& e)
        {
            std::cout << "Something went wrong on receive." << std::endl;
            std::cout << e.what() << std::endl;
        }
        catch(Socket::NotConnectedException&)
        {
            // FIXME add quit_message packet building
            //dialogManager.toViewParser.doCommand(ip,PACKET_WITH_QUIT_MESSAGE);
            std::cout << "Client closed connection." << std::endl;
            break;
        }
    }
    {
        WriteLock lock(dialogManager.mutexLock);
        dialogManager.dialogMap.erase(ip);
    }
    delete dialog;
}
