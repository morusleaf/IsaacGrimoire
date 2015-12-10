#pragma once

#include <stdexcept>

class not_in_game : public std::exception {
public:
	not_in_game();
	~not_in_game();
};
