#pragma once

#include "Logger.h"
#include "Debug.h"
#include "Singleton.h"
#include "Time.h"

#define SAFE_DELETE(x) do {	\
	delete x;				\
	x = nullptr;			\
} while (0);