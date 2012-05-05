#include "ChatWindow.hpp"
#include "ChatTabFactory.hpp"
#include <cassert>

ChatWindow::ChatWindow( FromViewParser& fromViewParser ) 
    : sendButton("Send"), fromViewParser(fromViewParser)
{
    set_title("ChatTIN");
    set_default_size(600,400);
    set_border_width(10);

    buildTreeModel();    
    initializeTabs();
    initializeFriends();
    registerSignals();
    createInterface();

    set_focus(chatField);
}
ChatWindow::~ChatWindow()
{
    //All alocated memory is being freed by gtk or smart pointers.
}

ChatWindow::FriendData::FriendData()
{
    add(alias);
    add(fullAlias);
}       

void ChatWindow::textInHandle()
{
    selectedTab->doCommand( fromViewParser, chatField.get_text() );
    chatField.set_text("");
}

Glib::ustring ChatWindow::cutAlias( Glib::ustring alias )
{
    return alias; 
}

bool ChatWindow::validateAlias( Glib::ustring )
{
    //TODO: Implement method - it should find if alias is ok, or if given name is correct IPv6 adress
    return true;
}

void ChatWindow::friendPickHandle(const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* /*column*/)
{
    Gtk::TreeModel::iterator iter = friendListModel->get_iter(path);
    if(iter)
    {
        openDialogTab((*iter)[friends.fullAlias]);
    } 
}


void ChatWindow::openDialogTab( Glib::ustring alias )
{
    if( dialogBoxes.find(alias) == dialogBoxes.end() )
    {
        /*create page if it is necessary*/            
        dialogBoxes[alias] = std::shared_ptr<ChatTab>(new ChatTabLog(alias));    
        chatTabs.insert_page(*dialogBoxes[alias], cutAlias(alias), 1);
        show_all_children();
    }
    chatTabs.set_current_page(chatTabs.page_num(*dialogBoxes[alias]));
}

void ChatWindow::switchTabHandle( GtkNotebookPage* /*page*/, guint page_num )
{
    if(page_num!=0)
    {
        selectedTab = dialogBoxes[
                static_cast<ChatTab*>(chatTabs.get_nth_page(page_num))->getFullAlias()
        ];    
        selName = selectedTab->getFullAlias();

        chatBoxBuffer = selectedTab->get_buffer();
        appendTextToCurrentTab("Teraz piszesz do: "+selName+"\n");
        selectedTab->set_editable(false);
    }
    else
    {
        selName = logBox->getFullAlias();
        chatBoxBuffer = logBox->get_buffer();
    }
    chatField.grab_focus(); //Always set focus to chatField
}


void ChatWindow::addFriend( Glib::ustring name )
{
    Gtk::TreeModel::Row row = *(friendListModel->append());
    row[friends.alias] = cutAlias(name);
    row[friends.fullAlias] = name;
}

void ChatWindow::closeCurrentTab()
{
    if( selName != "LOG" )
    {
        Glib::ustring toRemove = selName;
        chatTabs.remove_page(*selectedTab);
        dialogBoxes.erase(toRemove);
        selectedTab = NULL;
        selName = "LOG";
        chatTabs.set_current_page(0);
    }
    else
    {
        exit(0);
    }
}

void ChatWindow::appendTextToCurrentTab( Glib::ustring text )
{
    chatBoxBuffer->insert(chatBoxBuffer->end(), text);
    Gtk::TextIter iter = chatBoxBuffer->end();
    selectedTab->scroll_to_iter(iter,0.0);
}

void ChatWindow::initializeFriends()
{
    //TODO: const size names
    addFriend("Janek");
    addFriend("Maciej");
    addFriend("Andrzej");
}

void ChatWindow::registerSignals()
{
    sendButton.signal_clicked().connect(sigc::mem_fun(*this, &ChatWindow::textInHandle));    
    friendList.signal_row_activated().connect(sigc::mem_fun(*this, &ChatWindow::friendPickHandle));
    chatTabs.signal_switch_page().connect(sigc::mem_fun(*this, &ChatWindow::switchTabHandle));    
    chatField.signal_activate().connect(sigc::mem_fun(*this, &ChatWindow::textInHandle));
}

void ChatWindow::createInterface()
{
    friendListScroll.add(friendList);
    friendListScroll.set_policy(Gtk::POLICY_NEVER, Gtk::POLICY_AUTOMATIC);
    mainBox.pack_start(friendListScroll, Gtk::PACK_SHRINK);
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
    friendList.set_can_focus(false);
    friendListModel = Gtk::TreeStore::create(friends);
    friendList.set_model(friendListModel);
    friendList.append_column("Znajomi", friends.alias);
}

void ChatWindow::initializeTabs()
{
    logBox = ChatTabFactory::create(Glib::ustring("LOG"));
    chatTabs.set_scrollable();

    selName = logBox->getFullAlias(); //fist seleceted tab

    chatTabs.insert_page(*logBox, cutAlias(selName), 0);
    selectedTab = logBox;
    chatBoxBuffer = logBox->get_buffer(); //select LOG TextView as default text buffer
    logBox->set_editable(false);
    logBox->set_can_focus(false);
    chatTabs.set_can_focus(false);
}


