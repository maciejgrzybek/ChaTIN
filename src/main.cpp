#include "ChatWindow.hpp"


int main(int argc, char *argv[])
{
    Gtk::Main kit(argc, argv);
    ChatWindow win;
    Gtk::Main::run(win);
    return 0;
}
