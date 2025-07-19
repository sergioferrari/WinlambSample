#include "pch.h"
#include "MainWindow.h"
#include "../res/resource.h"

MainWindow::MainWindow()
{

	setup.wndClassEx.lpszClassName = L"SampleMainWindow";
	setup.title = L"WinLamb Sample";
	setup.position = { 200, 150 };
	setup.size = { 600, 400 };
	setup.style |= WS_MINIMIZEBOX;

	setup.wndClassEx.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	setup.wndClassEx.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);

	initMenu();
	initStatusBar();
	initCommands();
}

void MainWindow::initMenu()
{ 
	setup.wndClassEx.lpszMenuName = MAKEINTRESOURCEW(IDR_MAINMENU);
	m_menu = GetMenu(this->hwnd());
}

void MainWindow::initStatusBar()
{
	m_statusbar.create(hwnd());

	// divided into several sections
	m_statusbar.add_fixed_part(100);
	m_statusbar.add_fixed_part(180);
	m_statusbar.set_text(L"Ready!", 0);
	m_statusbar.set_text(L"WinLambSample is running.", 1);
}

void MainWindow::initCommands()
{
	this->on_command(ID_FILE_NEW, [this](wl::wm::command) {
		MessageBox(this->hwnd(), L"Function not yet implemented!", L"Information", MB_OK | MB_ICONINFORMATION);
		return 0;
		});


	this->on_command(ID_FILE_EXIT, [this](wl::wm::command) {
		::SendMessage(this->hwnd(), WM_CLOSE, 0, 0);
		return 0;
		});
}
