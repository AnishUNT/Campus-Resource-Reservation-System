#include "ReservationList.h"

#include <iostream>

ReservationList::ReservationList()
    : m_head(nullptr), m_tail(nullptr), m_size(0) {}

ReservationList::~ReservationList() {
    clear();
}

void ReservationList::insertReservation(const Reservation& reservation) {
    ReservationNode* node = new ReservationNode(reservation);

    if (m_head == nullptr) {
        m_head = node;
        m_tail = node;
    } else {
        m_tail->next = node;
        m_tail       = node;
    }
    ++m_size;
}

bool ReservationList::removeReservation(int reservationId, Reservation& removed) {
    ReservationNode* current  = m_head;
    ReservationNode* previous = nullptr;

    while (current != nullptr) {
        if (current->data.getReservationId() == reservationId) {
            if (previous == nullptr) {
                m_head = current->next;
            } else {
                previous->next = current->next;
            }
            if (current == m_tail) {
                m_tail = previous;
            }

            removed = current->data;
            delete current;
            --m_size;
            return true;
        }
        previous = current;
        current  = current->next;
    }
    return false;
}

bool ReservationList::removeReservation(int reservationId) {
    Reservation discarded;
    return removeReservation(reservationId, discarded);
}

const Reservation* ReservationList::findReservation(int reservationId) const {
    for (ReservationNode* current = m_head; current != nullptr; current = current->next) {
        if (current->data.getReservationId() == reservationId) {
            return &current->data;
        }
    }
    return nullptr;
}

bool ReservationList::hasConflict(int resourceId,
                                  const std::string& date,
                                  const std::string& startTime,
                                  const std::string& endTime,
                                  int ignoreReservationId) const {
    for (ReservationNode* current = m_head; current != nullptr; current = current->next) {
        const Reservation& existing = current->data;

        if (existing.getReservationId() == ignoreReservationId) {
            continue;
        }
        if (existing.getResourceId() != resourceId || existing.getDate() != date) {
            continue;
        }
        if (ReservationUtils::timesOverlap(startTime, endTime,
                                           existing.getStartTime(), existing.getEndTime())) {
            return true;
        }
    }
    return false;
}

bool ReservationList::hasConflict(const Reservation& candidate) const {
    return hasConflict(candidate.getResourceId(),
                       candidate.getDate(),
                       candidate.getStartTime(),
                       candidate.getEndTime(),
                       candidate.getReservationId());
}

void ReservationList::displayConflicts(int resourceId,
                                       const std::string& date,
                                       const std::string& startTime,
                                       const std::string& endTime) const {
    bool headerPrinted = false;

    for (ReservationNode* current = m_head; current != nullptr; current = current->next) {
        const Reservation& existing = current->data;

        if (existing.getResourceId() != resourceId || existing.getDate() != date) {
            continue;
        }
        if (!ReservationUtils::timesOverlap(startTime, endTime,
                                            existing.getStartTime(), existing.getEndTime())) {
            continue;
        }

        if (!headerPrinted) {
            std::cout << "Conflicting reservation(s):\n";
            Reservation::displayHeader();
            headerPrinted = true;
        }
        existing.display();
    }
}

void ReservationList::displayReservations() const {
    if (isEmpty()) {
        std::cout << "There are no active reservations.\n";
        return;
    }

    std::cout << "\nActive reservations (" << m_size << " total)\n";
    Reservation::displayHeader();
    for (ReservationNode* current = m_head; current != nullptr; current = current->next) {
        current->data.display();
    }
}

bool ReservationList::isEmpty() const { return m_head == nullptr; }

std::size_t ReservationList::size() const { return m_size; }

void ReservationList::clear() {
    ReservationNode* current = m_head;
    while (current != nullptr) {
        ReservationNode* next = current->next;
        delete current;
        current = next;
    }
    m_head = nullptr;
    m_tail = nullptr;
    m_size = 0;
}

std::size_t ReservationList::countForResource(int resourceId) const {
    std::size_t count = 0;
    ReservationNode* current = m_head;
    while (current != nullptr) {
        if (current->data.getResourceId() == resourceId) {
            count++;
        }
        current = current->next;
    }
    return count;
}
