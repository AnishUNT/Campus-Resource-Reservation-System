#ifndef RESERVATION_H
#define RESERVATION_H

#include <string>

class Reservation {
public:
    Reservation();
    Reservation(int reservationId,
                const std::string& studentId,
                int resourceId,
                const std::string& date,
                const std::string& startTime,
                const std::string& endTime);

    int                getReservationId() const;
    const std::string& getStudentId() const;
    int                getResourceId() const;
    const std::string& getDate() const;
    const std::string& getStartTime() const;
    const std::string& getEndTime() const;

    void setReservationId(int reservationId);
    void setStudentId(const std::string& studentId);
    void setResourceId(int resourceId);
    void setDate(const std::string& date);
    void setStartTime(const std::string& startTime);
    void setEndTime(const std::string& endTime);

    bool overlapsWith(const Reservation& other) const;

    void        display() const;
    static void displayHeader();

private:
    int         m_reservationId;
    std::string m_studentId;
    int         m_resourceId;
    std::string m_date;
    std::string m_startTime;
    std::string m_endTime;
};

namespace ReservationUtils {

bool isValidTimeFormat(const std::string& time);

bool isValidDateFormat(const std::string& date);

bool timesOverlap(const std::string& startA, const std::string& endA,
                  const std::string& startB, const std::string& endB);

std::string trim(const std::string& text);

bool parseInt(const std::string& text, int& outValue);

}

#endif
