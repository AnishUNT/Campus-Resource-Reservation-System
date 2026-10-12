#include "Reservation.h"

#include <cctype>
#include <cstdlib>
#include <iomanip>
#include <iostream>

Reservation::Reservation()
    : m_reservationId(0), m_studentId(""), m_resourceId(0),
      m_date(""), m_startTime(""), m_endTime("") {}

Reservation::Reservation(int reservationId,
                         const std::string& studentId,
                         int resourceId,
                         const std::string& date,
                         const std::string& startTime,
                         const std::string& endTime)
    : m_reservationId(reservationId), m_studentId(studentId), m_resourceId(resourceId),
      m_date(date), m_startTime(startTime), m_endTime(endTime) {}

int Reservation::getReservationId() const { return m_reservationId; }

const std::string& Reservation::getStudentId() const { return m_studentId; }

int Reservation::getResourceId() const { return m_resourceId; }

const std::string& Reservation::getDate() const { return m_date; }

const std::string& Reservation::getStartTime() const { return m_startTime; }

const std::string& Reservation::getEndTime() const { return m_endTime; }

void Reservation::setReservationId(int reservationId) { m_reservationId = reservationId; }

void Reservation::setStudentId(const std::string& studentId) { m_studentId = studentId; }

void Reservation::setResourceId(int resourceId) { m_resourceId = resourceId; }

void Reservation::setDate(const std::string& date) { m_date = date; }

void Reservation::setStartTime(const std::string& startTime) { m_startTime = startTime; }

void Reservation::setEndTime(const std::string& endTime) { m_endTime = endTime; }

bool Reservation::overlapsWith(const Reservation& other) const {
    if (m_resourceId != other.m_resourceId) {
        return false;
    }
    if (m_date != other.m_date) {
        return false;
    }
    return ReservationUtils::timesOverlap(m_startTime, m_endTime,
                                          other.m_startTime, other.m_endTime);
}

void Reservation::displayHeader() {
    std::cout << std::left
              << std::setw(8)  << "Res.ID"
              << std::setw(14) << "Student"
              << std::setw(10) << "Resource"
              << std::setw(14) << "Date"
              << std::setw(9)  << "Start"
              << std::setw(9)  << "End" << "\n";
    std::cout << std::string(64, '-') << "\n";
}

void Reservation::display() const {
    std::cout << std::left
              << std::setw(8)  << m_reservationId
              << std::setw(14) << m_studentId
              << std::setw(10) << m_resourceId
              << std::setw(14) << m_date
              << std::setw(9)  << m_startTime
              << std::setw(9)  << m_endTime << "\n";
}

namespace ReservationUtils {

bool isValidTimeFormat(const std::string& time) {

    if (time.size() != 5 || time[2] != ':') {
        return false;
    }
    for (std::size_t i = 0; i < time.size(); ++i) {
        if (i == 2) {
            continue;
        }
        if (!std::isdigit(static_cast<unsigned char>(time[i]))) {
            return false;
        }
    }

    const int hours   = (time[0] - '0') * 10 + (time[1] - '0');
    const int minutes = (time[3] - '0') * 10 + (time[4] - '0');
    return hours >= 0 && hours <= 23 && minutes >= 0 && minutes <= 59;
}

bool isValidDateFormat(const std::string& date) {

    if (date.size() != 10 || date[4] != '-' || date[7] != '-') {
        return false;
    }
    for (std::size_t i = 0; i < date.size(); ++i) {
        if (i == 4 || i == 7) {
            continue;
        }
        if (!std::isdigit(static_cast<unsigned char>(date[i]))) {
            return false;
        }
    }

    const int month = (date[5] - '0') * 10 + (date[6] - '0');
    const int day   = (date[8] - '0') * 10 + (date[9] - '0');
    const int year = (date[0]-'0')*1000 + (date[1]-'0')*100 +
                     (date[2]-'0')*10 + (date[3]-'0');
    if (year == 0 || month < 1 || month > 12 || day < 1) return false;
    const int days[] = {31,28,31,30,31,30,31,31,30,31,30,31};
    int limit = days[month - 1];
    if (month == 2 && year % 4 == 0 && (year % 100 != 0 || year % 400 == 0))
        limit = 29;
    return day <= limit;
}

// Back-to-back reservations are allowed because touching endpoints do not overlap.
bool timesOverlap(const std::string& startA, const std::string& endA,
                  const std::string& startB, const std::string& endB) {

    return startA < endB && startB < endA;
}

std::string trim(const std::string& text) {
    std::size_t first = 0;
    while (first < text.size() &&
           std::isspace(static_cast<unsigned char>(text[first]))) {
        ++first;
    }

    std::size_t last = text.size();
    while (last > first &&
           std::isspace(static_cast<unsigned char>(text[last - 1]))) {
        --last;
    }

    return text.substr(first, last - first);
}

bool parseInt(const std::string& text, int& outValue) {
    const std::string trimmed = trim(text);
    if (trimmed.empty()) {
        return false;
    }

    std::size_t index = 0;
    if (trimmed[0] == '+' || trimmed[0] == '-') {
        index = 1;
        if (trimmed.size() == 1) {
            return false;
        }
    }
    for (; index < trimmed.size(); ++index) {
        if (!std::isdigit(static_cast<unsigned char>(trimmed[index]))) {
            return false;
        }
    }

    const long long parsed = std::strtoll(trimmed.c_str(), nullptr, 10);
    if (parsed > 2147483647LL || parsed < -2147483648LL) {
        return false;
    }

    outValue = static_cast<int>(parsed);
    return true;
}

}
