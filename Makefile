GEN=-std=c++0x -O$(O)
O=2
GLIBI=`pkg-config glibmm-2.4 --cflags`
GLIBL=`pkg-config glibmm-2.4 --libs`
GTKI=`pkg-config gtkmm-2.4 --cflags`
GTKL=`pkg-config gtkmm-2.4 --libs`
LFLAGS=-ltinyxml

all: obj/ChatWindow.o obj/main.o obj/Socket.o obj/Exception.o obj/AliasManager.o obj/Config.o
	g++ $^ $(GEN) $(GTKL) $(LFLAGS) -o bin/ChaTIN
obj/main.o: src/main.cpp src/ChatWindow.hpp
	g++ src/main.cpp -c $(GEN) $(GTKI) -o $@
obj/ChatWindow.o: src/ChatWindow.cpp src/ChatWindow.hpp
	g++ src/ChatWindow.cpp -c $(GEN) $(GTKI) -o $@
obj/Socket.o: src/Socket.cpp src/Socket.hpp
	g++ src/Socket.cpp -c $(GEN) -o $@
obj/Exception.o: src/Exception.cpp src/Exception.hpp
	g++ src/Exception.cpp -c $(GEN) -o $@
obj/AliasManager.o: src/AliasManager.cpp src/AliasManager.hpp
	g++ src/AliasManager.cpp -c $(GEN) $(GLIBI) $(GLIBL)  -o $@
obj/Config.o: src/Config.cpp src/Config.hpp
	g++ src/Config.cpp -c $(GEN) -o $@
clean:
	rm obj/*.o
debug: GEN += -DDEBUG -Wall -Wextra -g3
debug: O=0
debug: all
