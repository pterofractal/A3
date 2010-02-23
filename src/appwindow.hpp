#ifndef APPWINDOW_HPP
#define APPWINDOW_HPP

#include <gtkmm.h>
#include "viewer.hpp"
#include <iostream>

class AppWindow : public Gtk::Window {
public:
  AppWindow(std::string filename);
  
protected:

private:
	// A "vertical box" which holds everything in our window
	Gtk::VBox m_vbox;

	// The menubar, with all the menus at the top of the window
	Gtk::MenuBar m_menubar;
	// Each menu itself
	Gtk::Menu m_menu_app, m_options, m_edit, m_mode;

	// Mode group
	Gtk::RadioButtonGroup m_group_modes;

	// The main OpenGL area
	Viewer m_viewer;
};

#endif
