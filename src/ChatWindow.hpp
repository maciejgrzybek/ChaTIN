#pragma once
#include <gtkmm.h>
#include <iostream>
#include <string>
#include <map>
#include <memory>

/*
 * TODO: 
 *      - Scrolling friendList and chatBox
 *      - Cut View from "Model" in friendList (name of tab cannot be information)
 */


/**
 * Class holding whole GUI in it
 * It uses gtkmm to create simple window for chating
 * It can be used by creating and calling Gtk::run(created_object);
 * @author: Andrzej Fiedukowicz
 */
class ChatWindow : public Gtk::Window
{
public: 
    /**
     * Main constructor of window
     * It creates and sets all components it their positions
     * It also register all handlers for Gtk signals.
     */
    ChatWindow();
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
     * @params Glib::ustring - string to validate
     * @return true if given string is valid alias or IPv6 adress
     *          (meaning in can be used to communicate with controller)     
     */
    bool validateAlias( Glib::ustring );

    /* GUI Actions */
    
    /**
     * Method opening dialog tab or swiching to existing one
     * @params Glib::ustring - name of tab
     */
    void openDialogTab( Glib::ustring alias );

    /**
     * Adds position to friend list which will have given name
     * @param - Glib::ustring - name of friend
     */
    void addFriend( Glib::ustring name );

    /**
     * Method witch will close current tab and destroy it TextView 
     * (meaning you can no loger read what was on this tab,
     * even if you open new tab with the same name it had before)
     */
    void closeCurrentTab();


    /**
     * Appends text to chatBox in current tab
     * @param - Glib::ustring - text to append
     */
    void appendTextToCurrentTab( Glib::ustring text );


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
    Gtk::TextView chatBox;
    Gtk::TreeView friendList;
    Gtk::HBox mainBox;
    Gtk::VBox rightBox;    
    Gtk::HBox bottomBox;    
    Gtk::Notebook chatTabs;
    Glib::RefPtr<Gtk::TextBuffer> chatBoxBuffer;
    Glib::RefPtr<Gtk::TreeStore>  friendListModel;
    Glib::ustring selName;
    std::map<Glib::ustring, std::shared_ptr<Gtk::TextView> > dialogBoxes;   //Glib::RefPtr cannot be used here
                                                                            //becouse it hasnt operator*
};
