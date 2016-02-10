#pragma once

#include <stdexcept>

class read_memory_error : public std::exception {
public:
	read_memory_error();
    ~read_memory_error() throw();
};
