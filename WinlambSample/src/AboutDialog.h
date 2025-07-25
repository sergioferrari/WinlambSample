#pragma once

#include <winlamb/dialog_modal.h>
#include <winlamb/button.h>
#include <winlamb/font.h>
#include <winlamb/label.h>

class AboutDialog : public wl::dialog_modal {
public:
    AboutDialog();
    virtual ~AboutDialog() = default;

    void init_controls();

private:  
	wl::button          m_okBtn;
    wl::font            m_fontBold;
    wl::label           m_appnameLbl, m_descriptionLbl, m_authorLbl;
    HBITMAP             m_bmpGioconda = nullptr;
};