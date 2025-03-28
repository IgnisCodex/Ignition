#pragma once

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>

#include <string>
#include <sstream>
#include <vector>
#include <array>
#include <unordered_map>
#include <unordered_set>

#include "Ignition/Log.hpp"
#include "Ignition/Debug/Instrumentor.hpp"

#ifdef IG_PLATFORM_WINDOWS
	#include <Windows.h>
#endif