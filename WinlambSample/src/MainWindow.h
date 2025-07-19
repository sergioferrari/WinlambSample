#pragma once

#include <winlamb/window_main.h>
#include <winlamb/menu.h>
#include <winlamb/statusbar.h>


class MainWindow : public wl::window_main
{
public:
	MainWindow();
	virtual ~MainWindow() = default;

protected: 
	void initMenu();
	void initStatusBar();
	void initCommands();

private:
	wl::menu m_menu;
	wl::statusbar m_statusbar;

};
