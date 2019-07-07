#pragma once

#include "logger.h"

#define CHECK(expr, ...) \
	if (!(expr)) z::Log<z::LFATAL>(__FILE__, __LINE__, ##__VA_ARGS__);