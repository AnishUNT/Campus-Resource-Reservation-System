#include "WaitingList.h"

#include <iomanip>
#include <iostream>

WaitingRequest::WaitingRequest()
    : studentId(""), resourceId(0), date(""), startTime(""), endTime("") {}

WaitingRequest::WaitingRequest(const std::string& studentId,
                               int resourceId,
                               const std::string& date,
                               const std::string& startTime,
                               const std::string& endTime)
    : studentId(studentId), resourceId(resourceId), date(date),
      startTime(startTime), endTime(endTime) {}

void WaitingRequest::displayHeader() {
    std::cout << std::left
              << std::setw(8)  << "Pos."
              << std::setw(14) << "Student"
              << std::setw(10) << "Resource"
              << std::setw(14) << "Date"
              << std::setw(9)  << "Start"
              << std::setw(9)  << "End" << "\n";
    std::cout << std::string(64, '-') << "\n";
}

void WaitingRequest::display(std::size_t position) const {
    std::cout << std::left
              << std::setw(8)  << position
              << std::setw(14) << studentId
              << std::setw(10) << resourceId
              << std::setw(14) << date
              << std::setw(9)  << startTime
              << std::setw(9)  << endTime << "\n";
}

WaitingList::WaitingList() : m_front(nullptr), m_rear(nullptr), m_size(0) {}
WaitingList::~WaitingList() { clear(); }

void WaitingList::enqueue(const WaitingRequest& request) {
    WaitingNode* node = new WaitingNode(request);
    if (m_rear) m_rear->next = node;
    else m_front = node;
    m_rear = node;
    ++m_size;
}
bool WaitingList::dequeue(WaitingRequest& removed) {
    if (!m_front) return false;
    WaitingNode* node = m_front;
    removed = node->data;
    m_front = node->next;
    if (!m_front) m_rear = nullptr;
    delete node;
    --m_size;
    return true;
}
bool WaitingList::peek(WaitingRequest& front) const { return peekAt(0, front); }
bool WaitingList::peekAt(std::size_t index, WaitingRequest& out) const {
    if (index >= m_size) return false;
    WaitingNode* node = m_front;
    for (std::size_t i = 0; i < index; ++i) node = node->next;
    out = node->data;
    return true;
}
bool WaitingList::removeAt(std::size_t index, WaitingRequest& removed) {
    if (index >= m_size) return false;
    if (index == 0) return dequeue(removed);
    WaitingNode* previous = m_front;
    for (std::size_t i = 1; i < index; ++i) previous = previous->next;
    WaitingNode* node = previous->next;
    removed = node->data;
    previous->next = node->next;
    if (node == m_rear) m_rear = previous;
    delete node;
    --m_size;
    return true;
}
bool WaitingList::isEmpty() const { return m_front == nullptr; }
std::size_t WaitingList::size() const { return m_size; }
std::size_t WaitingList::countForResource(int id) const {
    std::size_t count = 0;
    for (WaitingNode* node = m_front; node; node = node->next)
        if (node->data.resourceId == id) ++count;
    return count;
}
void WaitingList::displayWaitingList() const {
    if (isEmpty()) { std::cout << "The waiting list is empty.\n"; return; }
    WaitingRequest::displayHeader();
    std::size_t position = 1;
    for (WaitingNode* node = m_front; node; node = node->next)
        node->data.display(position++);
}
void WaitingList::clear() {
    while (m_front) {
        WaitingNode* next = m_front->next;
        delete m_front;
        m_front = next;
    }
    m_rear = nullptr;
    m_size = 0;
}
