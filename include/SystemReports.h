#ifndef SYSTEM_REPORTS_H
#define SYSTEM_REPORTS_H
#include <cstddef>
#include <utility>
#include <vector>
#include "ResourceManager.h"
#include "ReservationList.h"
#include "WaitingList.h"

// Count each accepted request once; promotion and undo do not add demand.
class SystemReports {
public:
    void recordRequest(int resourceId);
    std::size_t requestCount(int resourceId) const;
    void display(const ResourceManager& manager, const ReservationList& active,
                 const WaitingList& waiting) const;
private:
    std::vector<std::pair<int, std::size_t>> m_requests;
};
#endif
