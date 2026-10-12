#include "SystemReports.h"
#include <iomanip>
#include <iostream>

void SystemReports::recordRequest(int id) {
    for (auto& entry : m_requests) {
        if (entry.first == id) { ++entry.second; return; }
    }
    m_requests.push_back({id, 1});
}
std::size_t SystemReports::requestCount(int id) const {
    for (const auto& entry : m_requests)
        if (entry.first == id) return entry.second;
    return 0;
}
void SystemReports::display(const ResourceManager& manager,
                            const ReservationList& active,
                            const WaitingList& waiting) const {
    std::cout << "\n-- System reports --\n";
    active.displayReservations();
    std::cout << "\nResource utilization and waiting-list statistics\n"
              << "Counts reflect current active reservations and waiting requests.\n"
              << std::left << std::setw(10) << "ID" << std::setw(40) << "Resource"
              << std::setw(12) << "Active" << std::setw(12) << "Waiting"
              << "Requests\n";
    std::size_t maximum = 0;
    for (const Resource& resource : manager.getResources()) {
        const int id = resource.getResourceId();
        const std::size_t demand = requestCount(id);
        if (demand > maximum) maximum = demand;
        std::cout << std::setw(10) << id << std::setw(40) << resource.getName()
                  << std::setw(12) << active.countForResource(id)
                  << std::setw(12) << waiting.countForResource(id) << demand << "\n";
    }
    std::cout << "Total active: " << active.size() << "; total waiting: "
              << waiting.size() << "\n\nMost requested resources\n"
              << "Requests = accepted reservations plus waiting-list entries this run.\n";
    if (maximum == 0) { std::cout << "No requests recorded.\n"; return; }
    for (const Resource& resource : manager.getResources()) {
        if (requestCount(resource.getResourceId()) == maximum)
            std::cout << resource.getResourceId() << " | " << resource.getName()
                      << " | " << maximum << " request(s)\n";
    }
}
