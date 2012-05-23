#include "ChatWindow.hpp"
#include "ChatTabFactory.hpp"
#include <cassert>

ChatWindow::ChatWindow( SafeQueue<EPtr>& bq, SafeQueue<Action>& aq) 
    : sendButton("Send"), bq(bq), aq(aq)
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
    if( chatField.get_text() == "" )
        return;
    Glib::ustring text = chatField.get_text();
    assert("Selected tab cannot be NULL" && selectedTab != NULL);
    EPtr event = selectedTab->createEvent( text );
    assert("Event cannot be NULL" && event != EPtr(NULL));
    bq.push(event);
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
    /*
    Gtk::TreeModel::iterator iter = friendListModel->get_iter(path);
    if(iter)
    {
        openDialogTab((*iter)[friends.fullAlias]);
    }*/ 
}

TPtr ChatWindow::unsafeOpenTab( TIPtr tabId )
{
    TPtr tab = tabId->createTab();
    dialogBoxes.insert(tab);    
    chatTabs.insert_page(*tab, cutAlias(tab->getFullAlias()), 1);
    show_all_children();
    return tab;
}

template<class InputIterator, class T>
InputIterator myfind( InputIterator first, InputIterator last, const T& value)
{
    for( ; first!=last; first++) if(**first==*value) break;
    return first;
}

template<class InputIterator, class T>
InputIterator pointerFind( InputIterator first, InputIterator last, const T& value)
{
    for( ; first!=last; first++) if(&**first==value) break;
    return first;
}

std::pair<bool, TPtr> ChatWindow::isTabExist( TIPtr tabId )
{
    std::set<TPtr>::iterator i = myfind(dialogBoxes.begin(), dialogBoxes.end(), tabId);
    bool result = ( i != dialogBoxes.end() );
    return std::pair<bool,TPtr>(result, result ? *i : TPtr(NULL) );
}

TPtr ChatWindow::openTab( TIPtr tabId, bool changeTab )
{
    TPtr result;
    std::pair<bool, TPtr> finder = isTabExist(tabId);
    if( finder.first == true )
        result = finder.second;
    else
        result = unsafeOpenTab( tabId );

    if( changeTab )
        chatTabs.set_current_page(chatTabs.page_num(*result));

    return result;
}

bool ChatWindow::idle()
{
    if( !aq.empty() )
    {
        aq.front()(this); //try_front...
        aq.pop();
    }
    return true;
}

void ChatWindow::switchTabHandle( GtkNotebookPage* /*page*/, guint page_num )
{
    if(page_num!=0)
    {
        selectedTab = static_cast<ChatTab*>(chatTabs.get_nth_page(page_num));
        chatBoxBuffer = selectedTab->get_buffer();

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
    if( selectedTab->getType() != LOG )
    {
        auto i = pointerFind( dialogBoxes.begin(), dialogBoxes.end(), selectedTab );
        chatTabs.remove_page(*selectedTab);
        dialogBoxes.erase(i);
        selectedTab = &*logBox;
        chatTabs.set_current_page(0);
    }
    else
    {
        exit(0);
    }    
}

void ChatWindow::showIncomingMessage( TIPtr id, Glib::ustring author, Glib::ustring message, bool incoming )
{
    TPtr tab = openTab(id, false);
    
    Glib::ustring toShow;
    if( incoming )
        toShow = author+" >> "+message+"\n";
    else
        toShow = "ME ----> "+message+"\n";

    appendTextToTab( tab, toShow ); //FIXME should be ChatTab method
}


void ChatWindow::appendTextToTab( TPtr tab, Glib::ustring text )
{
    Glib::RefPtr<Gtk::TextBuffer> buffer = tab->get_buffer();
    buffer->insert(buffer->end(), text);
    Gtk::TextIter iter = buffer->end();
    tab->scroll_to_iter(iter,0.0);
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
    Glib::signal_timeout().connect( sigc::mem_fun(*this, &ChatWindow::idle), 50 );
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
    logBox = TPtr( new ChatTabLog(ChaTIN::LogName("LOG")) );//ChatTabFactory::create(Glib::ustring("LOG"));
    chatTabs.set_scrollable();

    selName = logBox->getFullAlias(); //fist seleceted tab

    chatTabs.insert_page(*logBox, cutAlias(selName), 0);
    selectedTab = &*logBox;
    chatBoxBuffer = logBox->get_buffer(); //select LOG TextView as default text buffer
    logBox->set_editable(false);
    logBox->set_can_focus(false);
    chatTabs.set_can_focus(false);
}


