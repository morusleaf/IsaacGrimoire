#pragma once

#include <stdexcept>

class get_task_error : public std::exception {
public:
	get_task_error();
	~get_task_error();
};
