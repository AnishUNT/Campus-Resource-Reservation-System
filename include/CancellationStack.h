#ifndef CANCELLATION_STACK_H
#define CANCELLATION_STACK_H

#include <cstddef>

#include "Reservation.h"

class CancellationStack {
public:
    CancellationStack();
    ~CancellationStack();

    CancellationStack(const CancellationStack&)            = delete;
    CancellationStack& operator=(const CancellationStack&) = delete;

    void push(const Reservation& reservation);

    bool pop(Reservation& removed);

    bool peek(Reservation& top) const;

    bool        isEmpty() const;
    std::size_t size() const;

    void displayCancellationHistory() const;

    void clear();

private:
    struct CancellationNode {
        Reservation       data;
        CancellationNode* next;

        explicit CancellationNode(const Reservation& reservation)
            : data(reservation), next(nullptr) {}
    };

    CancellationNode* m_top;
    std::size_t       m_size;
};

#endif
