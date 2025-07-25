#include "pch.h"
#include "AboutDialog.h"
#include "../res/resource.h"

AboutDialog::AboutDialog() {
    setup.dialogId = IDD_ABOUTDLG;

    on_message(WM_INITDIALOG, [this](wl::params p)
        {
			this->center_on_parent(); // Center the dialog on its parent
			init_controls();
            return TRUE;
        });
	
    this->on_command(IDOK, [this](wl::wm::command) -> LRESULT {
        ::EndDialog(this->hwnd(), IDOK);  // Chiude la finestra di dialogo
        return 0;
        });
}

void AboutDialog::init_controls() {

	m_okBtn.assign(this->hwnd(), IDOK);

    m_fontBold.create(L"Segoe UI", 16, wl::font::deco::BOLD);
	m_appnameLbl.assign(this->hwnd(), IDC_APPNAME);
    m_appnameLbl.set_text(L"WinLambSample Application");
    m_descriptionLbl.assign(this->hwnd(), IDC_DESCRIPTION);
    m_descriptionLbl.set_text(L"Win32 program using winlamb (by rodrigocfd)");
    m_authorLbl.assign(this->hwnd(), IDC_AUTHOR);
    m_authorLbl.set_text(L"© 2025 Sergio Ferrari");

    // Impostazione del font sul controllo label
    ::SendMessageW(m_appnameLbl.hwnd(), WM_SETFONT, (WPARAM)m_fontBold.hfont(), TRUE);

    // Load the logo bitmap
    m_bmpGioconda = LoadBitmap(GetModuleHandle(nullptr), MAKEINTRESOURCE(IDB_GIOCONDA));
    if (m_bmpGioconda) {
        SendDlgItemMessage(this->hwnd(), IDB_GIOCONDA, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)m_bmpGioconda);
    }
}