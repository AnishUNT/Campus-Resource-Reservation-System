#ifndef RESERVATION_LIST_H
#define RESERVATION_LIST_H

#include <cstddef>
#include <string>

#include "Reservation.h"

class ReservationList {
public:
    ReservationList();
    ~ReservationList();

    ReservationList(const ReservationList&)            = delete;
    ReservationList& operator=(const ReservationList&) = delete;

    void insertReservation(const Reservation& reservation);

    bool removeReservation(int reservationId, Reservation& removed);

    bool removeReservation(int reservationId);

    const Reservation* findReservation(int reservationId) const;

    bool hasConflict(int resourceId,
                     const std::string& date,
                     const std::string& startTime,
                     const std::string& endTime,
                     int ignoreReservationId = -1) const;

    bool hasConflict(const Reservation& candidate) const;

    void displayConflicts(int resourceId,
                          const std::string& date,
                          const std::string& startTime,
                          const std::string& endTime) const;

    void        displayReservations() const;
    bool        isEmpty() const;
    std::size_t size() const;

    void clear();
    std::size_t countForResource(int resourceId) const;

private:
    struct ReservationNode {
        Reservation      data;
        ReservationNode* next;

        explicit ReservationNode(const Reservation& reservation)
            : data(reservation), next(nullptr) {}
    };

    ReservationNode* m_head;
    ReservationNode* m_tail;
    std::size_t      m_size;
};

#endif
