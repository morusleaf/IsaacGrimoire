#include "Core/exception/get_process_error.h"

get_process_error::get_process_error(const std::string& what_arg) :
	std::runtime_error(what_arg) {}

get_process_error::~get_process_error() throw() {}
