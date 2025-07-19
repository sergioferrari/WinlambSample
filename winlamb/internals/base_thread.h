/**
 * Part of WinLamb - Win32 API Lambda Library
 * https://github.com/rodrigocfd/winlamb
 * Copyright 2017-present Rodrigo Cesar de Freitas Dias
 * This library is released under the MIT License
 */

#pragma once
#include "base_msg.h"
#include <process.h>

namespace wl {
namespace _wli {

// Adds multithreading capabilities to the window.
template<typename retT, retT RET_VAL>
class base_thread final {
private:
	struct _callback_pack final {
		std::function<void()> func;
		HWND                  hWnd = nullptr;
		std::exception_ptr    curExcept = nullptr;
	};

	static const UINT WM_THREAD_MESSAGE = WM_APP + 0x3FFF;

	base_msg<retT>& _baseMsg;

public:
	base_thread(base_msg<retT>& baseMsg) :
		_baseMsg(baseMsg)
	{
		baseMsg.msgs.add(WM_THREAD_MESSAGE, [this](params p) noexcept -> retT {
			this->_process_thread_ui_msg(p);
			return RET_VAL; // 0 for windows, TRUE for dialogs
		});
	}

	// Runs code asynchronously in a new detached thread.
	void run_thread_detached(std::function<void()> func) const noexcept {
		// Analog to std::thread([](){ ... }).detach(), but exception-safe.
		_callback_pack* pPack = new _callback_pack{std::move(func), this->_baseMsg.hwnd()};

		uintptr_t hThread = _beginthreadex(nullptr, 0, [](void* ptr) noexcept -> unsigned int {
			_callback_pack* pPack = reinterpret_cast<_callback_pack*>(ptr);
			try {
				pPack->func(); // invoke user callback
			} catch (...) {
				_callback_pack* pCrashed = new _callback_pack{[]{}, pPack->hWnd, std::current_exception()};
				SendMessageW(pPack->hWnd, WM_THREAD_MESSAGE, 0, reinterpret_cast<LPARAM>(pCrashed));
			}
			delete pPack;
			_endthreadex(0); // http://www.codeproject.com/Articles/7732/A-class-to-synchronise-thread-completions/
			return 0;
		}, pPack, 0, nullptr);

		CloseHandle(reinterpret_cast<HANDLE>(hThread));
	}

	// Runs code synchronously in the UI thread.
	void run_thread_ui(std::function<void()> func) const noexcept {
		// This method is analog to SendMessage (synchronous), but intended to be called
		// from another thread, so a callback function can, tunelled by wndproc, run in
		// the original thread of the window, thus allowing GUI updates. This avoids the
		// user to deal with a custom WM_ message.
		_callback_pack* pPack = new _callback_pack{std::move(func), this->_baseMsg.hwnd()};
		SendMessageW(this->_baseMsg.hwnd(), WM_THREAD_MESSAGE, 0, reinterpret_cast<LPARAM>(pPack));
	}

private:
	void _process_thread_ui_msg(const params& p) const noexcept {
		_callback_pack* pPack = reinterpret_cast<_callback_pack*>(p.lParam);
		if (pPack->curExcept) { // catching an exception from run_thread_detached()
			try {
				std::rethrow_exception(pPack->curExcept);
			} catch (...) {
				lippincott();
				PostQuitMessage(-1);
			}
		} else { // from run_thread_ui()
			try {
				pPack->func(); // invoke user callback
			} catch (...) {
				lippincott();
				PostQuitMessage(-1);
			}
		}
		delete pPack;
	}
};

}//namespace _wli
}//namespace wl
