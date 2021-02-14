// The MIT License (MIT)
//
// Copyright (C) 2020 Michael Theall
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#pragma once

#include <3ds.h>
#include <chrono>

namespace imgui
{
namespace ctru
{
/// \brief Initialize 3ds platform
bool init ();

/// \brief Prepare 3ds for a new frame
void newFrame ();

struct steady_clock
{
	/// \brief Type representing number of ticks
	using rep = std::uint64_t;

	/// \brief Type representing ratio of clock period in seconds
	using period = std::ratio<1, SYSCLOCK_ARM11>;

	/// \brief Duration type
	using duration = std::chrono::duration<rep, period>;

	/// \brief Timestamp type
	using time_point = std::chrono::time_point<steady_clock>;

	/// \brief Whether clock is steady
	constexpr static bool is_steady = true;

	/// \brief Current timestamp
	static time_point now () noexcept;
};

}
}