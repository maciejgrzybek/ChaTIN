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
    
    ChatWindow win;
    Gtk::Main::run(win);
    return 0;
}
