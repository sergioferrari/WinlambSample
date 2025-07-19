/**
 * Part of WinLamb - Win32 API Lambda Library
 * https://github.com/rodrigocfd/winlamb
 * Copyright 2017-present Rodrigo Cesar de Freitas Dias
 * This library is released under the MIT License
 */

#pragma once
#include <memory>
#include <vector>
#include "radio.h"

namespace wl {

// Automates a group of native radio buttons.
class radio_group final {
private:
	size_t                   _sz = 0;
	std::unique_ptr<radio[]> _items; // vector requires movable object, so we use an ordinary array

public:
	radio_group() = default;
	radio_group(radio_group&&) = default;
	radio_group& operator=(radio_group&&) = default; // movable only

	radio_group& assign(HWND hParent, std::initializer_list<int> ctrlIds) {
		if (this->_items) {
			throw std::logic_error("Trying to assign a radio group twice.");
		}

		this->_sz = ctrlIds.size();
		this->_items = std::make_unique<radio[]>(ctrlIds.size());
		size_t i = 0;
		for (int ctrlId : ctrlIds) {
			this->_items[i++].assign(hParent, ctrlId);
		}
		this->_items[0].set_check(true) // first is checked by default
			.style.first_in_group(true); // and receives WS_GROUP
		return *this;
	}

	radio_group& assign(const wnd* parent, std::initializer_list<int> ctrlIds) {
		return this->assign(parent->hwnd(), ctrlIds);
	}

	size_t size() const noexcept {
		return this->_sz;
	}

	std::vector<int> get_all_ids() const {
		std::vector<int> ids;
		ids.reserve(this->_sz);
		for (size_t i = 0; i < this->_sz; ++i) {
			ids.emplace_back(this->_items[i].ctrl_id());
		}
		return ids;
	}

	const radio& get_by_pos(size_t index) const {
		return this->_items[index];
	}

	radio& get_by_pos(size_t index) {
		return this->_items[index];
	}

	const radio& get_by_id(int ctrlId) const {
		for (size_t i = 0; i < this->_sz; ++i) {
			if (this->_items[i].ctrl_id() == ctrlId) {
				return this->_items[i];
			}
		}
		throw std::out_of_range("Radio ID doesn't exist.");
	}

	radio& get_by_id(int ctrlId) {
		for (size_t i = 0; i < this->_sz; ++i) {
			if (this->_items[i].ctrl_id() == ctrlId) {
				return this->_items[i];
			}
		}
		throw std::out_of_range("Radio ID doesn't exist.");
	}

	radio_group& set_enabled(bool enabled) noexcept {
		for (size_t i = 0; i < this->_sz; ++i) {
			this->_items[i].set_enabled(enabled);
		}
		return *this;
	}

	radio_group& set_checked_by_pos(size_t index) {
		for (size_t i = 0; i < this->_sz; ++i) {
			if (i != index) {
				this->_items[i].set_check(false);
			}
		}
		this->_items[index].set_check_and_trigger(true);
		return *this;
	}

	radio_group& set_checked_by_id(int ctrlId) {
		size_t target = -1;
		for (size_t i = 0; i < this->_sz; ++i) {
			if (this->_items[i].ctrl_id() == ctrlId) {
				target = i;
			} else {
				this->_items[i].set_check(false);
			}
		}
		this->_items[target].set_check_and_trigger(true);
		return *this;
	}

	size_t get_checked_pos() const {
		for (size_t i = 0; i < this->_sz; ++i) {
			if (this->_items[i].is_checked()) {
				return i;
			}
		}
		throw std::logic_error("Radio group has no assigned value.");
	}

	int get_checked_id() const {
		for (size_t i = 0; i < this->_sz; ++i) {
			if (this->_items[i].is_checked()) {
				return this->_items[i].ctrl_id();
			}
		}
		throw std::logic_error("Radio group has no assigned value.");
	}
};

}//namespace wl
