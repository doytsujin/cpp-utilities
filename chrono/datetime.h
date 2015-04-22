#ifndef DATETIME_H
#define DATETIME_H

#include "timespan.h"

#include "../conversion/types.h"

#include <string>

namespace ChronoUtilities
{

/*!
 * \brief Specifies the output format.
 *
 * \sa DateTime::toString()
 */
enum class DateTimeOutputFormat
{
    DateAndTime, /**< date and time */
    DateOnly, /**< date only */
    TimeOnly, /**< time only */
    DateTimeAndWeekday, /**< date with weekday and time */
    DateTimeAndShortWeekday /**< date with abbreviated weekday and time */
};

/*!
 * \brief Specifies the day of the week.
 *
 * \sa DateTime::dayOfWeek()
 */
enum class DayOfWeek
{
    Monday, /**< Monday */
    Tuesday, /**< Tuesday */
    Wednesday, /**< Wednesday */
    Thursday, /**< Thursday */
    Friday, /**< Friday */
    Saturday, /**< Saturday */
    Sunday /**< Sunday */
};

/*!
 * \brief Specifies the date part.
 *
 * \sa DateTime::getDatePart()
 */
enum class DatePart
{
    Year, /**< year */
    Month, /**< month */
    DayOfYear, /**< day of year */
    Day /**< day */
};

class LIB_EXPORT DateTime
{
public:
    constexpr DateTime();
    constexpr DateTime(uint64 ticks);
    static DateTime fromDate(int year = 1, int month = 1, int day = 1);
    static DateTime fromTime(int hour = 0, int minute = 0, int second = 0, double millisecond = 0.0);
    static DateTime fromDateAndTime(int year = 1, int month = 1, int day = 1, int hour = 0, int minute = 0, int second = 0, double millisecond = 0.0);
    static DateTime fromString(const std::string &str);

    constexpr uint64 totalTicks() const;
    int year() const;
    int month() const;
    int day() const;
    int dayOfYear() const;
    constexpr DayOfWeek dayOfWeek() const;
    constexpr int hour() const;
    constexpr int minute() const;
    constexpr int second() const;
    constexpr int millisecond() const;
    constexpr bool isNull() const;
    constexpr TimeSpan timeOfDay() const;
    bool isLeapYear() const;
    constexpr bool isSameDay(const DateTime &other) const;
    std::string toString(DateTimeOutputFormat format = DateTimeOutputFormat::DateAndTime, bool noMilliseconds = false) const;
    void toString(std::string &result, DateTimeOutputFormat format = DateTimeOutputFormat::DateAndTime, bool noMilliseconds = false) const;
    static const char *printDayOfWeek(DayOfWeek dayOfWeek, bool abbreviation = false);

    static DateTime now();
    constexpr static bool isLeapYear(int year);
    static int daysInMonth(int year, int month);

    constexpr bool operator ==(const DateTime &other) const;
    constexpr bool operator !=(const DateTime &other) const;
    constexpr bool operator <(const DateTime &other) const;
    constexpr bool operator >(const DateTime &other) const;
    constexpr bool operator <=(const DateTime &other) const;
    constexpr bool operator >=(const DateTime &other) const;
    constexpr DateTime operator +(const TimeSpan &timeSpan) const;
    constexpr DateTime operator -(const TimeSpan &timeSpan) const;
    constexpr TimeSpan operator +(const DateTime &other) const;
    constexpr TimeSpan operator -(const DateTime &other) const;
    DateTime &operator +=(const TimeSpan &timeSpan);
    DateTime &operator -=(const TimeSpan &timeSpan);

private:
    static uint64 dateToTicks(int year, int month, int day);
    static uint64 timeToTicks(int hour, int minute, int second, double millisecond);
    int getDatePart(DatePart part) const;

    uint64 m_ticks;
    static const int m_daysPerYear;
    static const int m_daysPer4Years;
    static const int m_daysPer100Years;
    static const int m_daysPer400Years;
    static const int m_daysTo1601;
    static const int m_daysTo1899;
    static const int m_daysTo10000;
    static const int m_daysToMonth365[13];
    static const int m_daysToMonth366[13];
    static const int m_daysInMonth365[12];
    static const int m_daysInMonth366[12];
};

/*!
 * Constructs a DateTime.
 */
constexpr inline DateTime::DateTime() :
    m_ticks(0)
{}

/*!
 * Constructs a DateTime to a specified number of \a ticks.
 */
constexpr inline DateTime::DateTime(uint64 ticks) :
    m_ticks(ticks)
{}

/*!
 * Constructs a DateTime to the specified \a year, \a month, and \a day.
 */
inline DateTime DateTime::fromDate(int year, int month, int day)
{
    return DateTime(dateToTicks(year, month, day));
}

/*!
 * Constructs a DateTime to the specified \a hour, \a minute, \a second and \a millisecond.
 */
inline DateTime DateTime::fromTime(int hour, int minute, int second, double millisecond)
{
    return DateTime(timeToTicks(hour, minute, second, millisecond));
}

/*!
 * Constructs a DateTime to the specified \a year, \a month, \a day, \a hour, \a minute, \a second and \a millisecond.
 */
inline DateTime DateTime::fromDateAndTime(int year, int month, int day, int hour, int minute, int second, double millisecond)
{
    uint64 ticks = dateToTicks(year, month, day);
    if(ticks) {
        return DateTime(ticks + timeToTicks(hour, minute, second, millisecond));
    }
    return DateTime();
}

/*!
 * Gets the number of ticks that represent the value of the current DateTime class.
 */
constexpr inline uint64 DateTime::totalTicks() const
{
    return m_ticks;
}

/*!
 *Gets the year component of the date represented by this instance.
 */
inline int DateTime::year() const
{
    return getDatePart(DatePart::Year);
}

/*!
 * Gets the month component of the date represented by this instance.
 */
inline int DateTime::month() const
{
    return getDatePart(DatePart::Month);
}

/*!
 * Gets the day component of the date represented by this instance.
 */
inline int DateTime::day() const
{
    return getDatePart(DatePart::Day);
}

/*!
 * Gets the day of the year represented by this instance.
 */
inline int DateTime::dayOfYear() const
{
    return getDatePart(DatePart::DayOfYear);
}

/*!
 * Gets the day of the week represented by this instance.
 * \sa DayOfWeek
 */
constexpr inline DayOfWeek DateTime::dayOfWeek() const
{
    return static_cast<DayOfWeek>((m_ticks / TimeSpan::m_ticksPerDay) % 7l);
}

/*!
 * Gets the hour component of the date represented by this instance.
 */
constexpr inline int DateTime::hour() const
{
    return m_ticks / TimeSpan::m_ticksPerHour % 24ul;
}

/*!
 * Gets the minute component of the date represented by this instance.
 */
constexpr inline int DateTime::minute() const
{
    return m_ticks / TimeSpan::m_ticksPerMinute % 60ul;
}

/*!
 * Gets the second component of the date represented by this instance.
 */
constexpr inline int DateTime::second() const
{
    return m_ticks / TimeSpan::m_ticksPerSecond % 60ul;
}

/*!
 * Gets the millisecond component of the date represented by this instance.
 */
constexpr inline int DateTime::millisecond() const
{
    return m_ticks / TimeSpan::m_ticksPerMillisecond % 1000ul;
}

/*!
 * Returns ture if the date represented by the current DateTime class is null.
 * \sa DateTime
 */
constexpr inline bool DateTime::isNull() const
{
    return m_ticks == 0;
}

/*!
 * Gets the time of day as TimeSpan for this instance.
 */
constexpr inline TimeSpan DateTime::timeOfDay() const
{
    return TimeSpan(m_ticks % TimeSpan::m_ticksPerDay);
}

/*!
 * Returns an indication whether the year of the dae represented by this instance is a leap year.
 */
inline bool DateTime::isLeapYear() const
{
    return isLeapYear(year());
}

/*!
 * Returns an indication whether the specified \a year is a leap year.
 */
constexpr inline bool DateTime::isLeapYear(int year)
{
    return (year % 4 != 0)
            ? false
            : ((year % 100 == 0)
               ? (year % 400 == 0)
               : true);
}

/*!
 * Returns the number of days in the specified \a month and \a year.
 */
inline int DateTime::daysInMonth(int year, int month)
{
    return (month >= 1 && month <= 12)
            ? (isLeapYear(year)
               ? m_daysInMonth366[month - 1]
               : m_daysInMonth365[month - 1])
            : (0);
}

/*!
 * Returns and indication whether two DateTime instances represent the same day.
 */
constexpr inline bool DateTime::isSameDay(const DateTime &other) const
{
    return (m_ticks / TimeSpan::m_ticksPerDay) == (other.m_ticks / TimeSpan::m_ticksPerDay);
}

/*!
 * Indicates whether two DateTime instances are equal.
 */
constexpr inline bool DateTime::operator ==(const DateTime &other) const
{
    return m_ticks == other.m_ticks;
}

/*!
 * Indicates whether two DateTime instances are not equal.
 */
constexpr inline bool DateTime::operator !=(const DateTime &other) const
{
    return m_ticks != other.m_ticks;
}

/*!
 * Indicates whether a specified DateTime is less than another specified DateTime.
 */
constexpr inline bool DateTime::operator <(const DateTime &other) const
{
    return m_ticks < other.m_ticks;
}

/*!
 * Indicates whether a specified DateTime is greater than another specified DateTime.
 */
constexpr inline bool DateTime::operator >(const DateTime &other) const
{
    return m_ticks > other.m_ticks;
}

/*!
 * Indicates whether a specified DateTime is less or equal than another specified DateTime.
 */
constexpr inline bool DateTime::operator <=(const DateTime &other) const
{
    return m_ticks <= other.m_ticks;
}

/*!
 * Indicates whether a specified DateTime is greater or equal than another specified DateTime.
 */
constexpr inline bool DateTime::operator >=(const DateTime &other) const
{
    return m_ticks >= other.m_ticks;
}

/*!
 * Adds a TimeSpan.
 * \returns The result is another DateTime.
 */
constexpr inline DateTime DateTime::operator +(const TimeSpan &timeSpan) const
{
    return DateTime(m_ticks + timeSpan.m_ticks);
}

/*!
 * Substracts a TimeSpan.
 * \returns The result is another DateTime.
 */
constexpr inline DateTime DateTime::operator -(const TimeSpan &timeSpan) const
{
    return DateTime(m_ticks - timeSpan.m_ticks);
}

/*!
 * Adds two DateTime instances.
 * \returns The result is a TimeSpan.
 */
constexpr inline TimeSpan DateTime::operator +(const DateTime &other) const
{
    return TimeSpan(m_ticks + other.m_ticks);
}

/*!
 * Substracts two DateTime instances.
 * \returns The result is a TimeSpan.
 */
constexpr inline TimeSpan DateTime::operator -(const DateTime &other) const
{
    return TimeSpan(m_ticks - other.m_ticks);
}

/*!
 * Adds a TimeSpan to the current instance.
 */
inline DateTime &DateTime::operator +=(const TimeSpan &timeSpan)
{
    m_ticks += timeSpan.m_ticks;
    return *this;
}

/*!
 * Substracts a TimeSpan from the current instance.
 */
inline DateTime &DateTime::operator -=(const TimeSpan &timeSpan)
{
    m_ticks -= timeSpan.m_ticks;
    return *this;
}

}

#endif // DATETIME_H
