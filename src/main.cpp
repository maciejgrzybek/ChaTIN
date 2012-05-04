#include <boost/thread.hpp>
#include "ChatWindow.hpp"
#include "DBDriver.hpp"
#include "ToViewParser.hpp"
#include "SafeQueue.hpp"
#include "Config.hpp"
#include "types.hpp"
#include "ConferenceManager.hpp"
#include "AliasManager.hpp"
#include "DialogManager.hpp"
#include "FromViewParser.hpp"


int main(int argc, char *argv[])
{
    //create objects
    Gtk::Main kit(argc, argv);
    DBDriver db;
    SafeQueue<ChaTIN::IncomingMassage> toViewParserQueue;
    ToViewParser toViewParser(toViewParserQueue);
    Config config;
    ConferenceManager conferenceManager;
    AliasManager aliasManager( db );
    DialogManager dialogManager( toViewParser, aliasManager, conferenceManager, config);
    aliasManager.setDialogManager( dialogManager );
    FromViewParser fromViewParser( dialogManager );

    //create threads
    boost::thread dialogThread( dialogManager );
    boost::thread toViewThread( toViewParser  );

    ChatWindow win;
    Gtk::Main::run(win);
    toViewThread.join();
    dialogThread.join();  
    return 0;
}
