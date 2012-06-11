#include <boost/thread.hpp>
#include "FromViewParser.hpp"
#include "ChatWindow.hpp"
#include "DB/DBDriver.hpp"
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
    DB::DBDriver* db = DB::DBDriver::getInstance();
    SafeQueue<ChaTIN::IncomingMassage> toViewParserQueue;
    SafeQueue<EPtr> fromViewParserQueue;
    SafeQueue<Action> actionQueue;
    const Config config;
    ConferenceManager conferenceManager;

    AliasManager aliasManager( *db );
    
    ToViewParser toViewParser( aliasManager, 
                               conferenceManager, 
                               toViewParserQueue, 
                               actionQueue);

    DialogManager dialogManager( toViewParser, 
                                 aliasManager, 
                                 conferenceManager, 
                                 config);

    aliasManager.setDialogManager( dialogManager );

    FromViewParser fromViewParser( dialogManager, 
                                   aliasManager, 
                                   conferenceManager,                                    
                                   fromViewParserQueue, 
                                   actionQueue );

    ChatWindow win( fromViewParserQueue, 
                    actionQueue );

    fromViewParser.setView( &win );

    //create threads
    boost::thread dialogThread( dialogManager );
    boost::thread toViewThread( toViewParser  );
    boost::thread fromViewThread( fromViewParser  );

    Gtk::Main::run(win);
    return 0;
}
