#include "pch.h"
#include "MainWindow.h"
#include <winlamb/internals/run.h>

namespace wli = wl::_wli;

int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE, LPWSTR, int) {
    return wli::run_main<MainWindow>(hInst, SW_SHOWNORMAL);
}