GEN=-std=c++0x -O$(O) $(GLIBI)
O=2
GLIBI=`pkg-config glibmm-2.4 --cflags`
GLIBL=`pkg-config glibmm-2.4 --libs`
GTKI=`pkg-config gtkmm-2.4 --cflags`
GTKL=`pkg-config gtkmm-2.4 --libs`
LFLAGS=-ltinyxml

all: obj/ChatWindow.o obj/main.o obj/Socket.o obj/AliasManager.o obj/ConferenceManager.o obj/Dialog.o obj/DialogManager.o obj/Config.o obj/types.o obj/XMLException.o obj/ConferenceException.o obj/Exception.o 
	g++ $^ $(GEN) $(GTKL) $(LFLAGS) -o bin/ChaTIN
obj/main.o: src/main.cpp src/ChatWindow.hpp
	g++ src/main.cpp -c $(GEN) $(GTKI) -o $@
obj/ChatWindow.o: src/ChatWindow.cpp src/ChatWindow.hpp
	g++ src/ChatWindow.cpp -c $(GEN) $(GTKI) -o $@
obj/Socket.o: src/Socket.cpp src/Socket.hpp
	g++ src/Socket.cpp -c $(GEN) $(GLIBI) -o $@
obj/Exception.o: src/Exception.cpp src/Exception.hpp
	g++ src/Exception.cpp -c $(GEN) $(GLIBI) -o $@
obj/AliasManager.o: src/AliasManager.cpp src/AliasManager.hpp
	g++ src/AliasManager.cpp -c $(GEN) $(GLIBI)  -o $@
obj/ConferenceManager.o: src/ConferenceManager.cpp src/ConferenceManager.hpp
	g++ src/ConferenceManager.cpp -c $(GEN) -o $@
obj/Dialog.o: src/Dialog.cpp src/Dialog.hpp
	g++ src/Dialog.cpp -c $(GEN) $(GLIBI) -o $@
obj/DialogManager.o: src/DialogManager.cpp src/DialogManager.hpp
	g++ src/DialogManager.cpp -c $(GEN) $(GLIBI) -o $@
obj/Config.o: src/Config.cpp src/Config.hpp
	g++ src/Config.cpp -c $(GEN) -o $@
obj/types.o: src/types.cpp src/types.hpp
	g++ src/types.cpp -c $(GEN) $(GLIBI) -o $@
obj/XMLException.o: src/XMLException.cpp src/XMLException.hpp
	g++ src/XMLException.cpp -c $(GEN) -o $@
obj/ConferenceException.o: src/ConferenceException.cpp src/ConferenceException.hpp
	g++ src/ConferenceException.cpp -c $(GEN) -o $@
clean:
	rm obj/*.o
debug: GEN += -DDEBUG -Wall -Wextra -g3
debug: O=0
debug: all
