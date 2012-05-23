#include <boost/thread.hpp>
#include "FromViewParser.hpp"
#include "ChatWindow.hpp"
#include "DBDriver.hpp"
#include "ToViewParser.hpp"
#include "SafeQueue.hpp"
#include "Config.hpp"
#include "types.hpp"
#include "ConferenceManager.hpp"
#include "AliasManager.hpp"
#include "DialogManager.hpp"
#include "Event.hpp"


int main(int argc, char *argv[])
{
    //create objects
    Gtk::Main kit(argc, argv);
    DBDriver db;
    SafeQueue<ChaTIN::IncomingMassage> toViewParserQueue;
    SafeQueue<EPtr> fromViewParserQueue;
    SafeQueue<Action> actionQueue;
    const Config config;
    ConferenceManager conferenceManager;
    AliasManager aliasManager( db );
    ToViewParser toViewParser(conferenceManager, toViewParserQueue, actionQueue);
    DialogManager dialogManager( toViewParser, aliasManager, conferenceManager, config);
    aliasManager.setDialogManager( dialogManager );
    FromViewParser fromViewParser( dialogManager, conferenceManager, fromViewParserQueue, actionQueue );
    ChatWindow win( fromViewParserQueue, actionQueue );
    fromViewParser.setView( &win );

    //create threads
    boost::thread dialogThread( dialogManager );
    boost::thread toViewThread( toViewParser  );
    boost::thread fromViewThread( fromViewParser  );

    Gtk::Main::run(win);
    //FIXME: killing window should kill aplication
    //          but not in a brute way like this (no joins)
//    toViewThread.join();
//    dialogThread.join();  
    return 0;
}
