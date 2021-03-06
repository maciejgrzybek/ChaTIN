GEN=-std=c++0x -O$(O) $(GTKI)
O=2
GLIBI=`pkg-config glibmm-2.4 --cflags`
GLIBL=`pkg-config glibmm-2.4 --libs`
GTKI=`pkg-config gtkmm-2.4 --cflags`
GTKL=`pkg-config gtkmm-2.4 --libs`
LFLAGS=-ltinyxml -lboost_thread -lpthread -lwt -lwtdbo -lwtdbosqlite3
OBJS = obj/ChatWindow.o obj/Socket.o obj/AliasManager.o obj/ConferenceManager.o obj/Dialog.o obj/ToViewParser.o obj/DialogManager.o obj/Config.o obj/types.o obj/XMLException.o obj/ConferenceException.o obj/Exception.o obj/FromViewParser.o obj/XMLPackageCreator.o obj/ChatTab.o obj/Event.o obj/DBDriver.o


all: $(OBJS) obj/main.o
	g++ $^ $(GEN) $(GTKL) $(LFLAGS) -o bin/ChaTIN
obj/main.o: src/main.cpp src/ChatWindow.hpp
	g++ src/main.cpp -c $(GEN) $(GTKI) -o $@
obj/ChatWindow.o: src/ChatWindow.cpp src/ChatWindow.hpp
	g++ src/ChatWindow.cpp -c $(GEN) $(GTKI) -o $@
obj/Socket.o: src/Socket.cpp src/Socket.hpp
	g++ src/Socket.cpp -c $(GEN) $(GLIBI) -o $@
obj/Exception.o: src/Exception.cpp src/Exception.hpp
	g++ src/Exception.cpp -c $(GEN) $(GLIBI) -o $@
obj/AliasException.o: src/AliasException.hpp
	g++ src/AliasException.hpp -c $(GEN) $(GLIBI) -o $@
obj/AliasManager.o: obj/AliasException.o obj/DBDriver.o src/AliasManager.cpp src/AliasManager.hpp
	g++ src/AliasManager.cpp -c $(GEN) $(GTKI)  -o $@
obj/ConferenceManager.o: obj/SchemaConference.o src/ConferenceManager.cpp src/ConferenceManager.hpp
	g++ src/ConferenceManager.cpp -c $(GEN) -o $@
obj/Dialog.o: src/Dialog.cpp src/Dialog.hpp src/types.hpp obj/Socket.o
	g++ src/Dialog.cpp -c $(GEN) $(GTKI) -o $@
obj/DialogManager.o: src/DialogManager.cpp src/DialogManager.hpp obj/Dialog.o obj/Config.o
	g++ src/DialogManager.cpp -c $(GEN) $(GTKI) -o $@
obj/Config.o: src/Config.cpp src/Config.hpp  obj/XMLException.o
	g++ src/Config.cpp -c $(GEN) -o $@
obj/types.o: src/types.cpp src/types.hpp obj/Exception.o
	g++ src/types.cpp -c $(GEN) $(GLIBI) -o $@
obj/XMLException.o: src/XMLException.cpp src/XMLException.hpp
	g++ src/XMLException.cpp -c $(GEN) -o $@
obj/ConferenceException.o: src/ConferenceException.cpp src/ConferenceException.hpp
	g++ src/ConferenceException.cpp -c $(GEN) -o $@
obj/ToViewParser.o: src/ToViewParser.cpp src/ToViewParser.hpp
	g++ src/ToViewParser.cpp -c $(GEN) $(GTKI) -o $@
obj/FromViewParser.o: src/FromViewParser.cpp src/FromViewParser.hpp
	g++ src/FromViewParser.cpp -c $(GEN) $(GTKI) -o $@
obj/XMLPackageCreator.o: src/XMLPackageCreator.cpp src/XMLPackageCreator.hpp
	g++ src/XMLPackageCreator.cpp -c $(GEN) -o $@
obj/ChatTab.o: src/ChatTab.cpp src/ChatTab.hpp
	g++ src/ChatTab.cpp -c $(GEN) $(GTKI) -o $@
obj/Event.o: src/Event.cpp src/Event.hpp
	g++ src/Event.cpp -c $(GEN) $(GTKI) -o $@
obj/DBDriver.o: obj/SchemaAlias.o obj/SchemaGroup.o obj/SchemaMessage.o obj/SchemaConference.o obj/SchemaConferenceMember.o obj/SchemaSubscription.o src/DB/DBDriver.cpp src/DB/DBDriver.hpp
	g++ src/DB/DBDriver.cpp -c $(GEN) $(GTKI) -o $@
obj/SchemaAlias.o: src/DB/Alias.hpp
	g++ src/DB/Alias.hpp -c $(GEN) -o $@
obj/SchemaGroup.o: src/DB/Group.hpp
	g++ src/DB/Group.hpp -c $(GEN) -o $@
obj/SchemaMessage.o: src/DB/Message.hpp
	g++ src/DB/Message.hpp -c $(GEN) -o $@
obj/SchemaConference.o: src/DB/Conference.hpp
	g++ src/DB/Conference.hpp -c $(GEN) -o $@
obj/SchemaConferenceMember.o: src/DB/ConferenceMember.hpp
	g++ src/DB/ConferenceMember.hpp -c $(GEN) -o $@
obj/SchemaSubscription.o: src/DB/Subscription.hpp
	g++ src/DB/Subscription.hpp -c $(GEN) -o $@
clean:
	rm obj/*.o
debug: GEN += -DDEBUG -Wall -Wextra -g3
debug: O=0
debug: all

test: LFLAGS += -lboost_test_exec_monitor
test: $(OBJS) obj/test.o
	g++ $^ $(GEN) $(GTKL) $(LFLAGS) -o test/run
obj/test.o: obj/Socket.o test/SocketTest.cpp
	g++ test/SocketTest.cpp -c $(GEN) -o $@
