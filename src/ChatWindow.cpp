#include "ChatWindow.hpp"

/*
 * TODO: 
 *      - Scrolling friendList and chatBox
 *      - Accept by [ENTER]
 *      - Cut View from "Model" in friendList (name of tab cannot be information)
 *      - Lock chatBox from user changes
 *      - First focus on chatField
 */

ChatWindow::ChatWindow() : sendButton("Send")
{
    set_title("ChatTIN");
    set_default_size(600,400);
    set_border_width(10);

    buildTreeModel();    
    initializeTabs();
    initializeFriends();
    registerSignals();
    createInterface();
}
ChatWindow::~ChatWindow()
{
    //All alocated memory is being freed by gtk or smart pointers.
}

ChatWindow::FriendData::FriendData()
{
    add(alias);
}       

void ChatWindow::textInHandle()
{
    if( chatField.get_text() != "" )
    {
        if( chatField.get_text()[0] != '/' )
        {
            chatBoxBuffer->insert_at_cursor("Wyslano: "+chatField.get_text()+"\n");
            chatField.set_text("");
        }
        else
        {
            /* command interpretation here */
            if(chatField.get_text() == "/exit")
                exit(0);
            if(chatField.get_text() == "/close")
            {
                closeCurrentTab();
            }
            if(chatField.get_text().substr(0,5) == "/open" )
            {
                Glib::ustring toOpen = chatField.get_text().substr(6); 
                if(validateAlias(toOpen))
                {
                    openDialogTab(toOpen);
                }
            }
        }
    }
}

bool ChatWindow::validateAlias( Glib::ustring )
{
    //TODO: Implement method - it should find if alias is ok, or if given name is correct IPv6 adress
    return true;
}

void ChatWindow::friendPickHandle(const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column)
{
    Gtk::TreeModel::iterator iter = friendListModel->get_iter(path);
    if(iter)
    {
        openDialogTab((*iter)[friends.alias]);
    } 
}


void ChatWindow::openDialogTab( Glib::ustring alias )
{
    if( dialogBoxes.find(alias) == dialogBoxes.end() )
    {
        /*create page if it is necessary*/            
        dialogBoxes[alias] = std::shared_ptr<Gtk::TextView>(new Gtk::TextView());        
        chatTabs.insert_page(*dialogBoxes[alias], alias, 1);
        show_all_children();
    }
    chatTabs.set_current_page( chatTabs.page_num( *dialogBoxes[alias] ) );
}

void ChatWindow::switchTabHandle( GtkNotebookPage* page, guint page_num )
{
    if(page_num!=0)
    {
        selName = chatTabs.get_tab_label_text(*chatTabs.get_nth_page(page_num));
        chatBoxBuffer = dialogBoxes[selName]->get_buffer();
        chatBoxBuffer->insert_at_cursor("Teraz piszesz do: "+selName+"\n");
    }
    else
    {
        selName = "LOG";
        chatBoxBuffer = chatBox.get_buffer();
    }
}

void ChatWindow::addFriend( Glib::ustring name )
{
    Gtk::TreeModel::Row row = *(friendListModel->append());
    row[friends.alias] = name;
}

void ChatWindow::closeCurrentTab()
{
    if( selName != "LOG" )
    {
        int currentPage = chatTabs.page_num(*dialogBoxes[selName]);
        chatTabs.remove_page(currentPage);
        dialogBoxes.erase(selName);
        selName = "LOG";
        chatTabs.set_current_page(0);
    }
    else
    {
        exit(0);
    }
}

void ChatWindow::initializeFriends()
{
    addFriend("Franek");
    addFriend("Janek");
    addFriend("Ganek");
    addFriend("Sranek");
    addFriend("Stanek");
    addFriend("SuperKolega");
}

void ChatWindow::registerSignals()
{
    sendButton.signal_clicked().connect(sigc::mem_fun(*this, &ChatWindow::textInHandle));
    friendList.signal_row_activated().connect(sigc::mem_fun(*this, &ChatWindow::friendPickHandle));
    chatTabs.signal_switch_page().connect(sigc::mem_fun(*this, &ChatWindow::switchTabHandle));
}

void ChatWindow::createInterface()
{
    mainBox.pack_start(friendList, Gtk::PACK_SHRINK);
    mainBox.pack_start(rightBox);                      
    rightBox.pack_start(chatTabs);
    rightBox.pack_start(bottomBox, Gtk::PACK_SHRINK);
    bottomBox.pack_start(chatField);
    bottomBox.pack_start(sendButton, Gtk::PACK_SHRINK);
    add(mainBox);        
    show_all_children();
}

void ChatWindow::buildTreeModel()
{
    friendListModel = Gtk::TreeStore::create(friends);
    friendList.set_model(friendListModel);
    friendList.append_column("Znajomi", friends.alias);
}

void ChatWindow::initializeTabs()
{
    chatTabs.set_scrollable();
    chatTabs.insert_page(chatBox, "LOG", 0);
    selName = "LOG"; //fist seleceted tab
    chatBoxBuffer = chatBox.get_buffer(); //select LOG TextView as default text buffer
}
