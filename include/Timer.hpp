//
// 2017-2024
//

#pragma once

#include <chrono>

namespace cube
{
/**
 * The lower overhead, non-thread safe implementation of the timer.
 * 2017-2024
 * */
class Timer
{
public:
   Timer()
   {
      Reset();
   }

   ~Timer() = default;

   void Reset()
   {
      mPrevTimePoint = std::chrono::high_resolution_clock::now();
   }

   [[nodiscard]] uint64_t Nanoseconds() const
   {
      const auto now = std::chrono::high_resolution_clock::now();
      return static_cast<uint64_t>(
         std::chrono::duration_cast<std::chrono::nanoseconds>(now - mPrevTimePoint).count());
   }

   [[nodiscard]] uint64_t Microseconds() const
   {
      const auto now = std::chrono::high_resolution_clock::now();
      return static_cast<uint64_t>(
         std::chrono::duration_cast<std::chrono::microseconds>(now - mPrevTimePoint).count());
   }

   [[nodiscard]] uint64_t Milliseconds() const
   {
      const auto now = std::chrono::high_resolution_clock::now();
      return static_cast<uint64_t>(
         std::chrono::duration_cast<std::chrono::milliseconds>(now - mPrevTimePoint).count());
   }

   [[nodiscard]] uint64_t Seconds() const
   {
      const auto now = std::chrono::high_resolution_clock::now();
      return static_cast<uint64_t>(
         std::chrono::duration_cast<std::chrono::seconds>(now - mPrevTimePoint).count());
   }

private:
   std::chrono::high_resolution_clock::time_point mPrevTimePoint;
};
}