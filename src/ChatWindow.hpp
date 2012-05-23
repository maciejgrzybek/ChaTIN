#pragma once
#include <gtkmm.h>
#include <iostream>
#include <string>
#include <map>
#include <set>
#include <memory>
#include <boost/function.hpp>
#include "ChatTab.hpp"
#include "SafeQueue.hpp"
#include "Event.hpp"
#include "FromViewParser.hpp"
#include "types.hpp"


/**
 * Class holding whole GUI in it
 * It uses gtkmm to create simple window for chating
 * It can be used by creating and calling Gtk::run(created_object);
 * @author: Andrzej Fiedukowicz
 */
class ChatWindow : public Gtk::Window
{
    SafeQueue<EPtr>& bq;
    SafeQueue<Action>& aq;
public: 
    /**
     * Main constructor of window
     * It creates and sets all components it their positions
     * It also register all handlers for Gtk signals.
     */
    ChatWindow( SafeQueue<EPtr>&, SafeQueue<Action>& );
    /**
     * For now it does not do anything because 
     * all object are being released by smart poionters
     */
    virtual ~ChatWindow();

protected:
    /**
     * Class holding informations about single position on friend list
     * @author: Andrzej Fiedukowicz
     */
    class FriendData : public Gtk::TreeModel::ColumnRecord
    {
        public:
            FriendData();
            Gtk::TreeModelColumn<Glib::ustring> alias; 
            Gtk::TreeModelColumn<Glib::ustring> fullAlias;
    };

    /* SIGNAL HANDLERS */

    /**
     * Handler for send button (called when button is pressed)
     * It cheks the command in chatField and it parse it
     * If the command is massage it send it to current tab
     * It shouldnt be called manualy
     */
    void textInHandle();

    /**
     * Handler for picking option from friend list 
     * It shouldnt be called manualy
     */
    void friendPickHandle(const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column);

    /**
     * Handler for switching between tabs (by user clicks or program signals)
     * It shouldnt be called manualy
     */
    void switchTabHandle( GtkNotebookPage* page, guint page_num );

    /* COMMUNICATION WITH CONTROLER */   

    /**
     * @param Glib::ustring string to validate
     * @return true if given string is valid alias or IPv6 adress
     *          (meaning in can be used to communicate with controller)     
     */
    bool validateAlias( Glib::ustring alias );

    /**
     * Cuts full alias to short form ready to present for user.
     * @param Glib::ustring alias to cut
     * @return Glib::ustring cutted alias
     */
    Glib::ustring cutAlias( Glib::ustring alias );    


    TPtr unsafeOpenTab( TIPtr );

public:
    /* GUI Actions */
    
    std::pair<bool,TPtr> isTabExist( TIPtr );

    /**
     * Method opening dialog tab or swiching to existing one
     * @param Glib::ustring name of tab
     */
    TPtr openTab( TIPtr, bool changeTab = true );

//    bool doesTabExist( TabId& );

    /**
     * Check for actions and do if they are there
     */
    bool idle();

    /**
     * Adds position to friend list which will have given name
     * @param Glib::ustring name of friend
     */
    void addFriend( Glib::ustring name );

    /**
     * Method witch will close current tab and destroy it TextView 
     * (meaning you can no loger read what was on this tab,
     * even if you open new tab with the same name it had before)
     */
    void closeCurrentTab();


    /**
     * Shows incoming massage.
     * If there is no card connected to given alias/id/string it creates new one
     */
    void showIncomingMessage( TIPtr, Glib::ustring, Glib::ustring, bool incoming = true );


    /**
     * Appends text to chatBox in current tab
     * @param Glib::ustring text to append
     */
    void appendTextToTab( TPtr tab, Glib::ustring text );


private:
    /* INITIALIZATION */

    /**
     * Loads friends list (adds friends to the list by calling addFriend)
     */
    void initializeFriends();
    
    /**
     * register signals functions in gtk
     */
    void registerSignals();

    /**
     * Ads all widgets to window and show them
     */
    void createInterface();
    
    /**
     * Creates model for friendList (must be used before addFriend(Glib::ustring)
     */
    void buildTreeModel();

    /**
     * Creates LOG tab, initialize tabs managment, set LOG as default tab
     */
    void initializeTabs(); 

    protected:
     
    FriendData friends;
    Gtk::Button sendButton;
    Gtk::Entry chatField;
    TPtr logBox;
    Gtk::TreeView friendList;
    Gtk::HBox mainBox;
    Gtk::VBox rightBox;    
    Gtk::HBox bottomBox;    
    Gtk::Notebook chatTabs;
    Gtk::ScrolledWindow friendListScroll;
    Glib::RefPtr<Gtk::TextBuffer> chatBoxBuffer;
    Glib::RefPtr<Gtk::TreeStore>  friendListModel;
    Glib::ustring selName;
    ChatTab* selectedTab;
    std::set< TPtr > dialogBoxes;   //Glib::RefPtr cannot be used here
                                    //becouse it hasnt operator*
};
