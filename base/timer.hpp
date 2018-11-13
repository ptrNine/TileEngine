#ifndef _TIMER_HPP_
#define _TIMER_HPP_

#include <chrono>
#include <ctime>
#include "types.hpp"


namespace zog {

	struct SDateTimePoint {
		U32 ms;
		U32 sec;
		U32 min;
		U32 hour;
		U32 day;
		U32 month;
		U32 year;

		SDateTimePoint(const std::tm& rTime, U32 rMs) {
		    ms      = rMs;
            sec     = static_cast<U32>(rTime.tm_sec);
            min     = static_cast<U32>(rTime.tm_min);
            hour    = static_cast<U32>(rTime.tm_hour);
            day     = static_cast<U32>(rTime.tm_mday);
            month   = static_cast<U32>(rTime.tm_mon + 1);
            year    = static_cast<U32>(rTime.tm_year + 1900);
		}
	};

	class CTimer
	{
		typedef std::chrono::steady_clock			steady_t;
		typedef steady_t::time_point				time_point_t;

	private:
		CTimer() = default;
		~CTimer() = default;
	public:
		CTimer(const CTimer&) = delete;
		CTimer&	operator= (const CTimer&) = delete;


	public:
		class TimeDuration {
		private:
			std::chrono::duration<intmax_t, std::nano> _duration;

		public:
			explicit TimeDuration(const std::chrono::duration<intmax_t, std::nano>& d): _duration(d) {}
			double sec() { return std::chrono::duration<double>(_duration).count(); }
			S64 milli() { return std::chrono::duration_cast<std::chrono::milliseconds>(_duration).count(); }
			S64 micro() { return std::chrono::duration_cast<std::chrono::microseconds>(_duration).count(); }
			S64 nano() { return _duration.count(); }
		};

		class Timestamp {
		private:
			time_point_t _timestamp;

		public:
			Timestamp() = default;
			explicit Timestamp(const time_point_t& t): _timestamp(t) {}
			TimeDuration operator-(const Timestamp& ts) { return TimeDuration(_timestamp - ts._timestamp); }
		};


		Timestamp timestamp() { return Timestamp(steady_t::now()); }


		SDateTimePoint getSystemDateTime() {
			std::chrono::time_point<std::chrono::system_clock> tp = std::chrono::system_clock::now();
			std::chrono::duration<intmax_t, std::nano> time = tp.time_since_epoch();
			auto now = time.count();
			now /= 1000000;
			time_t res =  std::chrono::system_clock::to_time_t(tp);

			std::tm res2 = *localtime(&res);
			return SDateTimePoint(res2, static_cast<U32>(now % 1000));
		}


	public:
		static CTimer&	_instance() { static CTimer instanse; return instanse; }
	};


	inline CTimer&		timer() { return CTimer::_instance(); }




	inline void sleep(U32 milliseconds) {
		auto cur = timer().timestamp();
		while ((timer().timestamp() - cur).milli() < milliseconds);
	}

}


#endif //_TIMER_HPP_