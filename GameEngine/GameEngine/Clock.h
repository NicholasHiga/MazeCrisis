#pragma once
#include <chrono>

class Clock
{
private:
	Clock() {};

public:
	/// getSeconds()
	/// Get number of seconds since system start.
	///
	static inline double
	getSeconds()
	{
		return (double)std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
	}

	/// getMilliseconds()
	/// Get number of milliseconds since system start.
	///
	static inline double
	getMilliseconds()
	{
		return (double)std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
	}
};