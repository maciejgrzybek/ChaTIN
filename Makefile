GEN=-std=c++0x
GLIBI=`pkg-config glibmm-2.4 --cflags`
GLIBL=`pkg-config glibmm-2.4 --libs`
GTKI=`pkg-config gtkmm-2.4 --cflags`
GTKL=`pkg-config gtkmm-2.4 --libs`
DEBUG=-g

all: obj/ChatWindow.o obj/main.o obj/Socket.o obj/Exception.o obj/AliasManager.o
	g++ $^ $(GEN) $(GTKL) -o bin/ChaTIN
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
clean:
	rm obj/*.o
