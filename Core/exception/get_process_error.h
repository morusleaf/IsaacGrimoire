#pragma once

#include <stdexcept>

class get_process_error : public std::runtime_error {
public:
	get_process_error(const std::string& what_arg);
    ~get_process_error() throw();
};
