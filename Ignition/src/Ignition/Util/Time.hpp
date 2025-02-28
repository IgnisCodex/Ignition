#pragma once

namespace Ignition::Util {
	class DeltaTime {
	public:
		DeltaTime(float time = 0.0f)
			: mTime(time)
		{}

		float s() const { return mTime; }
		float ms() const { return mTime * 1000.0f; }

	private:
		float mTime;
	};
}