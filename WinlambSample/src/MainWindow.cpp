#include "pch.h"
#include <winlamb/sysdlg.h>
#include "MainWindow.h"
#include "AboutDialog.h"
#include "../res/resource.h"

MainWindow::MainWindow()
{
	setup.wndClassEx.lpszClassName = L"SampleMainWindow";
	setup.title = L"WinLamb Sample";
	setup.position = { 200, 150 };
	setup.size = { 800, 500 };
	setup.style |= WS_MINIMIZEBOX;
	setup.style |= CS_HREDRAW | CS_VREDRAW;
	setup.wndClassEx.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	setup.wndClassEx.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);

	initMenu();
	initWinMessages();
	initAppCommands();
}

void MainWindow::initMenu()
{ 
	// assign the menu to the window
	setup.wndClassEx.lpszMenuName = MAKEINTRESOURCEW(IDR_MAINMENU);
	m_menu = GetMenu(this->hwnd());
}

void MainWindow::initStatusBar()
{
	m_statusbar.create(hwnd());

	// divided into several sections
	m_statusbar.add_fixed_part(60);
	m_statusbar.add_fixed_part(240);
	m_statusbar.set_text(L"Ready!", 0);
	m_statusbar.set_text(L"WinLambSample is running.", 1);

}

void MainWindow::initWinMessages() {
	// Gestione messaggi
	this->on_message(WM_CREATE, [this](wl::wm::create) -> LRESULT {
		initStatusBar();
		return 0;
		});

	this->on_message(WM_SIZE, [this](wl::params p) -> LRESULT {
		m_statusbar.adjust(p);
		return 0;
		});

	this->on_message(WM_CLOSE, [this](wl::wm::close) -> LRESULT {
		if (wl::sysdlg::msgbox(this->hwnd(), L"Close the appliation?", L"Confirm", MB_YESNO | MB_ICONQUESTION) == IDYES) {
			::PostQuitMessage(0);	// close the window and exit the application
		}
		return 0;
		});
}

void MainWindow::initAppCommands() {

	this->on_command(ID_FILE_OPEN, [this](wl::wm::command) -> LRESULT {
		 wl::sysdlg::open_file(this->hwnd(), 
			 L"Text file (*.txt) | *.txt | All files (*.*) | *.*",	// C# style filter
			m_filepath);
			
		if (!m_filepath.empty()) {
			// You selected a valid file
			wl::sysdlg::msgbox(this->hwnd(), L"Selected file", m_filepath, MB_OK);
		}
		return 0;
	});


	this->on_command(ID_FILE_SAVE, [this](wl::wm::command) -> LRESULT {
		wl::sysdlg::save_file(this->hwnd(),
			L"Text file (*.txt) | *.txt | All files (*.*) | *.*",	// C# style filter
			m_filepath, 
			L"New text file.txt");

		if (!m_filepath.empty()) {
			// You give uo saving the file
			wl::sysdlg::msgbox(this->hwnd(), L"Attention!", L"Don't you want to save the file?", MB_OK);
		}

		return 0;
	});

	this->on_command(ID_FILE_EXIT, [this](wl::wm::command) -> LRESULT {
		::PostMessageW(this->hwnd(), WM_CLOSE, 0, 0);
		return 0;
		});

	this->on_command(ID_HELP_ABOUT, [this](wl::wm::command) -> LRESULT {
		AboutDialog about;

		about.show(this->hwnd());
		return 0;
		});
}

