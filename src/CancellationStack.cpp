#include "CancellationStack.h"
#include <iostream>

CancellationStack::CancellationStack() : m_top(nullptr), m_size(0) {}
CancellationStack::~CancellationStack() { clear(); }
void CancellationStack::push(const Reservation& reservation) {
    CancellationNode* node = new CancellationNode(reservation);
    node->next = m_top;
    m_top = node;
    ++m_size;
}
bool CancellationStack::pop(Reservation& removed) {
    if (!m_top) return false;
    CancellationNode* node = m_top;
    removed = node->data;
    m_top = node->next;
    delete node;
    --m_size;
    return true;
}
bool CancellationStack::peek(Reservation& top) const {
    if (!m_top) return false;
    top = m_top->data;
    return true;
}
bool CancellationStack::isEmpty() const { return m_top == nullptr; }
std::size_t CancellationStack::size() const { return m_size; }
void CancellationStack::displayCancellationHistory() const {
    if (isEmpty()) { std::cout << "No reservations have been cancelled yet.\n"; return; }
    Reservation::displayHeader();
    for (CancellationNode* node = m_top; node; node = node->next) node->data.display();
}
void CancellationStack::clear() {
    while (m_top) {
        CancellationNode* next = m_top->next;
        delete m_top;
        m_top = next;
    }
    m_size = 0;
}
