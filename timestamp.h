#ifndef SDUZH_BASE_DATETIME_H
#define SDUZH_BASE_DATETIME_H

#include <string>
#include <time.h>
#include <stdint.h>

class Timestamp
{
public:
    static const int kMicroSecondsPerSecond = 1000000; // 10^6

    Timestamp(int64_t microseconds): microseconds_(microseconds) {}

    int64_t microseconds() const { return microseconds_; }

    const char *to_string() const;

    static Timestamp current();

    Timestamp add(const Timestamp &rhs) const {
		return add_microseconds(rhs.microseconds());
	}

	Timestamp add_seconds(int64_t s) const {
		return add_microseconds(s * kMicroSecondsPerSecond);
	}

	Timestamp add_microseconds(int64_t us) const {
		return Timestamp(microseconds_ + us);
	}

	Timestamp operator + (const Timestamp &rhs) const { 
		return add(rhs);
	}

	void operator += (const Timestamp &rhs) {
		*this = *this + rhs;
	}

private:
    static const char *kFormat;

    int64_t microseconds_; // micro seconds(us)
	mutable char stime_[32];
};

inline bool operator == (const Timestamp &lhs, const Timestamp &rhs) {
	return lhs.microseconds() == rhs.microseconds();
}

inline bool operator != (const Timestamp &lhs, const Timestamp &rhs) {
	return !(lhs == rhs);
}

inline bool operator < (const Timestamp &lhs, const Timestamp &rhs) {
	return lhs.microseconds() < rhs.microseconds();
}

inline bool operator <= (const Timestamp &lhs, const Timestamp &rhs) {
	return (lhs < rhs) || (lhs == rhs);
}

inline bool operator > (const Timestamp &lhs, const Timestamp &rhs) {
	return !(lhs <= rhs);
}

inline bool operator >= (const Timestamp &lhs, const Timestamp &rhs) {
	return (lhs > rhs) || (lhs == rhs);
}
#endif // 
