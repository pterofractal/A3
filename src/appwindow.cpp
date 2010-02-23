#include "appwindow.hpp"

AppWindow::AppWindow(std::string filename) :
	m_viewer(filename)
{
	set_title("Advanced Ergonomics Laboratory");

	// A utility class for constructing things that go into menus, which
	// we'll set up next.
	using Gtk::Menu_Helpers::MenuElem;
	using Gtk::Menu_Helpers::CheckMenuElem;

	// Set up the application menu
	// The slot we use here just causes AppWindow::hide() on this,
	// which shuts down the application.
	m_menu_app.items().push_back(MenuElem("_Quit", Gtk::AccelKey("q"),
	  sigc::mem_fun(*this, &AppWindow::hide)));
	
	sigc::slot1<void, Viewer::Mode> mode_slot = sigc::mem_fun(m_viewer, &Viewer::set_mode);
	m_mode.items().push_back( Gtk::Menu_Helpers::RadioMenuElem(m_group_modes, "_Position/Orientation", Gtk::AccelKey("p"), sigc::bind( mode_slot, Viewer::MODEL_TRANSLATE ) ) );
	m_mode.items().push_back( Gtk::Menu_Helpers::RadioMenuElem(m_group_modes, "_Joints", Gtk::AccelKey("j"), sigc::bind( mode_slot, Viewer::JOINTS ) ) );
		
	m_edit.items().push_back(MenuElem("_Undo", Gtk::AccelKey("u"), sigc::mem_fun(m_viewer, &Viewer::undo) ) );
	m_edit.items().push_back(MenuElem("_Redo", Gtk::AccelKey("r"), sigc::mem_fun(m_viewer, &Viewer::redo) ) );

	m_options.items().push_back(CheckMenuElem("_Circle", Gtk::AccelKey("C"), sigc::mem_fun(m_viewer, &Viewer::toggle_draw_circle) ) );
	m_options.items().push_back(CheckMenuElem("_Z-Buffer", Gtk::AccelKey("Z"), sigc::mem_fun(m_viewer, &Viewer::toggle_z_buffer) ) );
	m_options.items().push_back(CheckMenuElem("_Backface cull", Gtk::AccelKey("B"), sigc::mem_fun(m_viewer, &Viewer::toggle_backface_cull) ) );
	m_options.items().push_back(CheckMenuElem("_Frontface cull", Gtk::AccelKey("F"), sigc::mem_fun(m_viewer, &Viewer::toggle_frontface_cull) ) );

	// Set up the menu bar
	m_menubar.items().push_back(Gtk::Menu_Helpers::MenuElem("_Application", m_menu_app));
	m_menubar.items().push_back(Gtk::Menu_Helpers::MenuElem("_Mode", m_mode));
	m_menubar.items().push_back(Gtk::Menu_Helpers::MenuElem("_Edit", m_edit));
	m_menubar.items().push_back(Gtk::Menu_Helpers::MenuElem("_Options", m_options));

	// Pack in our widgets

	// First add the vertical box as our single "top" widget
	add(m_vbox);

	// Put the menubar on the top, and make it as small as possible
	m_vbox.pack_start(m_menubar, Gtk::PACK_SHRINK);

	// Put the viewer below the menubar. pack_start "grows" the widget
	// by default, so it'll take up the rest of the window.
	m_viewer.set_size_request(600, 600);
	m_vbox.pack_start(m_viewer);

	show_all();
}
