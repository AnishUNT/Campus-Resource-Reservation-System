#ifndef WAITING_LIST_H
#define WAITING_LIST_H

#include <cstddef>
#include <string>

struct WaitingRequest {
    std::string studentId;
    int         resourceId;
    std::string date;
    std::string startTime;
    std::string endTime;

    WaitingRequest();
    WaitingRequest(const std::string& studentId,
                   int resourceId,
                   const std::string& date,
                   const std::string& startTime,
                   const std::string& endTime);

    void        display(std::size_t position) const;
    static void displayHeader();
};

class WaitingList {
public:
    WaitingList();
    ~WaitingList();

    WaitingList(const WaitingList&)            = delete;
    WaitingList& operator=(const WaitingList&) = delete;

    void enqueue(const WaitingRequest& request);

    bool dequeue(WaitingRequest& removed);

    bool peek(WaitingRequest& front) const;

    bool peekAt(std::size_t index, WaitingRequest& out) const;
    bool removeAt(std::size_t index, WaitingRequest& removed);

    bool        isEmpty() const;
    std::size_t size() const;
    void        displayWaitingList() const;
    void        clear();
    std::size_t countForResource(int resourceId) const;

private:
    struct WaitingNode {
        WaitingRequest data;
        WaitingNode*   next;

        explicit WaitingNode(const WaitingRequest& request)
            : data(request), next(nullptr) {}
    };

    WaitingNode* m_front;
    WaitingNode* m_rear;
    std::size_t  m_size;
};

#endif
