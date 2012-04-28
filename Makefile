all: obj/ChatWindow.o obj/main.o
	g++ $^ `pkg-config gtkmm-2.4 --libs` -o ChaTIN
obj/main.o: src/main.cpp src/ChatWindow.hpp
	g++ src/main.cpp -c -std=c++0x `pkg-config gtkmm-2.4 --cflags` -o $@
obj/ChatWindow.o: src/ChatWindow.cpp src/ChatWindow.hpp
	g++ src/ChatWindow.cpp -c -std=c++0x `pkg-config gtkmm-2.4 --cflags` -o $@
clean:
	rm obj/*.o
