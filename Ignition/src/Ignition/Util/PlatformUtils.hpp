#pragma once

#include <string>

namespace Ignition::Util {
	class FileDialog {
	public:
		static std::string OpenFile(const char* filter);
		static std::string SaveFile(const char* filter);
	};
}