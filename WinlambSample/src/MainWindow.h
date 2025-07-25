#pragma once

#include <winlamb/window_main.h>
#include <winlamb/menu.h>
#include <winlamb/statusbar.h>
#include <string>


class MainWindow : public wl::window_main
{
public:
	MainWindow();
	virtual ~MainWindow() = default;

protected: 
	

	void initWinMessages();
	void initAppCommands();

	// Initializes the menu and the statusbar
	void initMenu();
	void initStatusBar();

private:

	std::wstring m_filepath;
	wl::menu m_menu;
	wl::statusbar m_statusbar;
	HWND hStatus = nullptr;

};
