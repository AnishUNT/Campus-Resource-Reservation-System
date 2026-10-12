#include <cctype>
#include <iostream>
#include <string>

#include "CancellationStack.h"
#include "Reservation.h"
#include "ReservationList.h"
#include "ResourceManager.h"
#include "WaitingList.h"
#include "SystemReports.h"

namespace {

const int AUTO_RESERVATION_ID_BASE = 9000;

const char* const DEFAULT_RESOURCE_FILE = "data/resources.txt";

enum class InputStatus { Ok, Cancelled, EndOfInput };

std::string toLower(const std::string& text) {
    std::string lowered = text;
    for (std::size_t i = 0; i < lowered.size(); ++i) {
        lowered[i] = static_cast<char>(std::tolower(static_cast<unsigned char>(lowered[i])));
    }
    return lowered;
}

// Read a whole line so invalid input does not break later prompts.
InputStatus promptLine(const std::string& prompt, std::string& out) {
    std::cout << prompt;
    std::cout.flush();

    std::string raw;
    if (!std::getline(std::cin, raw)) {
        std::cout << "\nInput ended.\n";
        return InputStatus::EndOfInput;
    }

    out = ReservationUtils::trim(raw);
    if (toLower(out) == "cancel") {
        std::cout << "  Operation cancelled.\n";
        return InputStatus::Cancelled;
    }
    return InputStatus::Ok;
}

InputStatus promptNonEmpty(const std::string& prompt,
                           const std::string& label,
                           std::string& out) {
    while (true) {
        const InputStatus status = promptLine(prompt, out);
        if (status != InputStatus::Ok) {
            return status;
        }
        if (!out.empty()) {
            return InputStatus::Ok;
        }
        std::cout << "  " << label << " must not be empty (type 'cancel' to go back).\n";
    }
}

InputStatus promptPositiveInt(const std::string& prompt,
                              const std::string& label,
                              int& out) {
    while (true) {
        std::string text;
        const InputStatus status = promptLine(prompt, text);
        if (status != InputStatus::Ok) {
            return status;
        }

        int parsed = 0;
        if (ReservationUtils::parseInt(text, parsed) && parsed > 0) {
            out = parsed;
            return InputStatus::Ok;
        }
        std::cout << "  " << label << " must be a positive whole number "
                  << "(type 'cancel' to go back).\n";
    }
}

InputStatus promptTime(const std::string& prompt, std::string& out) {
    while (true) {
        const InputStatus status = promptLine(prompt, out);
        if (status != InputStatus::Ok) {
            return status;
        }
        if (ReservationUtils::isValidTimeFormat(out)) {
            return InputStatus::Ok;
        }
        std::cout << "  Invalid time. Use 24-hour HH:MM, for example 09:30 or 14:00.\n";
    }
}

InputStatus promptDate(const std::string& prompt, std::string& out) {
    while (true) {
        const InputStatus status = promptLine(prompt, out);
        if (status != InputStatus::Ok) {
            return status;
        }
        if (ReservationUtils::isValidDateFormat(out)) {
            return InputStatus::Ok;
        }
        std::cout << "  Invalid date. Use YYYY-MM-DD, for example 2026-09-21.\n";
    }
}

InputStatus promptYesNo(const std::string& prompt, bool& out) {
    while (true) {
        std::string answer;
        const InputStatus status = promptLine(prompt, answer);
        if (status != InputStatus::Ok) {
            return status;
        }

        const std::string lowered = toLower(answer);
        if (lowered == "y" || lowered == "yes") {
            out = true;
            return InputStatus::Ok;
        }
        if (lowered == "n" || lowered == "no") {
            out = false;
            return InputStatus::Ok;
        }
        std::cout << "  Please answer y or n.\n";
    }
}

InputStatus promptExistingResourceId(const ResourceManager& manager, int& out) {
    while (true) {
        int resourceId = 0;
        const InputStatus status =
            promptPositiveInt("Resource ID: ", "Resource ID", resourceId);
        if (status != InputStatus::Ok) {
            return status;
        }
        if (manager.resourceExists(resourceId)) {
            out = resourceId;
            return InputStatus::Ok;
        }
        std::cout << "  No resource with ID " << resourceId
                  << ". Use menu option 1 to list valid IDs.\n";
    }
}

InputStatus promptTimeWindow(std::string& date, std::string& startTime, std::string& endTime) {
    InputStatus status = promptDate("Date (YYYY-MM-DD): ", date);
    if (status != InputStatus::Ok) {
        return status;
    }

    while (true) {
        status = promptTime("Start time (HH:MM): ", startTime);
        if (status != InputStatus::Ok) {
            return status;
        }
        status = promptTime("End time   (HH:MM): ", endTime);
        if (status != InputStatus::Ok) {
            return status;
        }
        if (startTime < endTime) {
            return InputStatus::Ok;
        }
        std::cout << "  The start time must be earlier than the end time. "
                  << "Please enter both times again.\n";
    }
}

int generateReservationId(const ReservationList& activeReservations) {
    int candidate = AUTO_RESERVATION_ID_BASE;
    while (activeReservations.findReservation(candidate) != nullptr) {
        ++candidate;
    }
    return candidate;
}

// Promote the first waiting request that fits the freed resource and date.
bool promoteFromWaitingList(int resourceId,
                            const std::string& date,
                            ReservationList& activeReservations,
                            WaitingList& waitingList) {
    for (std::size_t index = 0; index < waitingList.size(); ++index) {
        WaitingRequest request;
        if (!waitingList.peekAt(index, request)) {
            break;
        }

        if (request.resourceId != resourceId || request.date != date) {
            continue;
        }
        if (activeReservations.hasConflict(request.resourceId, request.date,
                                           request.startTime, request.endTime)) {
            continue;
        }

        WaitingRequest promoted;
        if (!waitingList.removeAt(index, promoted)) {
            break;
        }

        const Reservation newReservation(generateReservationId(activeReservations),
                                         promoted.studentId,
                                         promoted.resourceId,
                                         promoted.date,
                                         promoted.startTime,
                                         promoted.endTime);
        activeReservations.insertReservation(newReservation);

        std::cout << "\nWaiting list: student " << promoted.studentId
                  << " was promoted into the freed slot.\n";
        Reservation::displayHeader();
        newReservation.display();
        return true;
    }
    return false;
}

bool handleDisplayResources(const ResourceManager& manager) {
    manager.displayAllResources();
    return true;
}

bool handleCheckAvailability(const ResourceManager& manager,
                             const ReservationList& activeReservations) {
    if (manager.getResourceCount() == 0) {
        std::cout << "No resources are loaded, so availability cannot be checked.\n";
        return true;
    }

    std::cout << "\n-- Check resource availability --\n";

    int resourceId = 0;
    InputStatus status = promptExistingResourceId(manager, resourceId);
    if (status == InputStatus::EndOfInput) {
        return false;
    }
    if (status == InputStatus::Cancelled) {
        return true;
    }

    std::string date;
    std::string startTime;
    std::string endTime;
    status = promptTimeWindow(date, startTime, endTime);
    if (status == InputStatus::EndOfInput) {
        return false;
    }
    if (status == InputStatus::Cancelled) {
        return true;
    }

    const Resource* resource = manager.findResourceById(resourceId);
    std::cout << "\nResource " << resourceId << " (" << resource->getName() << ") on "
              << date << " from " << startTime << " to " << endTime << ":\n";

    if (activeReservations.hasConflict(resourceId, date, startTime, endTime)) {
        std::cout << "  NOT AVAILABLE - the slot overlaps an existing reservation.\n";
        activeReservations.displayConflicts(resourceId, date, startTime, endTime);
    } else {
        std::cout << "  AVAILABLE - no active reservation overlaps this window.\n";
    }
    return true;
}

bool handleCreateReservation(const ResourceManager& manager,
                             ReservationList& activeReservations,
                             WaitingList& waitingList,
                             SystemReports& reports) {
    if (manager.getResourceCount() == 0) {
        std::cout << "No resources are loaded, so reservations cannot be created.\n";
        return true;
    }

    std::cout << "\n-- Create a reservation -- (type 'cancel' at any prompt to go back)\n";

    int reservationId = 0;
    while (true) {
        const InputStatus status =
            promptPositiveInt("Reservation ID: ", "Reservation ID", reservationId);
        if (status == InputStatus::EndOfInput) {
            return false;
        }
        if (status == InputStatus::Cancelled) {
            return true;
        }
        if (activeReservations.findReservation(reservationId) == nullptr) {
            break;
        }
        std::cout << "  Reservation ID " << reservationId << " is already in use.\n";
    }

    std::string studentId;
    InputStatus status = promptNonEmpty("Student ID: ", "Student ID", studentId);
    if (status == InputStatus::EndOfInput) {
        return false;
    }
    if (status == InputStatus::Cancelled) {
        return true;
    }

    int resourceId = 0;
    status = promptExistingResourceId(manager, resourceId);
    if (status == InputStatus::EndOfInput) {
        return false;
    }
    if (status == InputStatus::Cancelled) {
        return true;
    }

    std::string date;
    std::string startTime;
    std::string endTime;
    status = promptTimeWindow(date, startTime, endTime);
    if (status == InputStatus::EndOfInput) {
        return false;
    }
    if (status == InputStatus::Cancelled) {
        return true;
    }

    if (activeReservations.hasConflict(resourceId, date, startTime, endTime)) {
        std::cout << "\nThat resource is already reserved for an overlapping time.\n";
        activeReservations.displayConflicts(resourceId, date, startTime, endTime);

        bool joinQueue = false;
        status = promptYesNo("Join the waiting list for this slot? (y/n): ", joinQueue);
        if (status == InputStatus::EndOfInput) {
            return false;
        }
        if (status == InputStatus::Ok && joinQueue) {
            waitingList.enqueue(WaitingRequest(studentId, resourceId, date, startTime, endTime));
            reports.recordRequest(resourceId);
            std::cout << "Added to the waiting list at position " << waitingList.size() << ".\n";
        } else {
            std::cout << "No reservation was created.\n";
        }
        return true;
    }

    const Reservation reservation(reservationId, studentId, resourceId, date, startTime, endTime);
    activeReservations.insertReservation(reservation);
    reports.recordRequest(resourceId);

    std::cout << "\nReservation created.\n";
    Reservation::displayHeader();
    reservation.display();
    return true;
}

bool handleCancelReservation(ReservationList& activeReservations,
                             WaitingList& waitingList,
                             CancellationStack& cancellationHistory) {
    if (activeReservations.isEmpty()) {
        std::cout << "There are no active reservations to cancel.\n";
        return true;
    }

    std::cout << "\n-- Cancel a reservation --\n";

    int reservationId = 0;
    const InputStatus status =
        promptPositiveInt("Reservation ID to cancel: ", "Reservation ID", reservationId);
    if (status == InputStatus::EndOfInput) {
        return false;
    }
    if (status == InputStatus::Cancelled) {
        return true;
    }

    Reservation cancelled;
    if (!activeReservations.removeReservation(reservationId, cancelled)) {
        std::cout << "No active reservation has ID " << reservationId << ".\n";
        return true;
    }

    cancellationHistory.push(cancelled);

    std::cout << "\nReservation cancelled and pushed onto the cancellation history.\n";
    Reservation::displayHeader();
    cancelled.display();

    if (!promoteFromWaitingList(cancelled.getResourceId(), cancelled.getDate(),
                                activeReservations, waitingList)) {
        std::cout << "No waiting student could take the freed slot.\n";
    }
    return true;
}

bool handleDisplayReservations(const ReservationList& activeReservations) {
    activeReservations.displayReservations();
    return true;
}

bool handleJoinWaitingList(const ResourceManager& manager,
                           const ReservationList& activeReservations,
                           WaitingList& waitingList,
                           SystemReports& reports) {
    if (manager.getResourceCount() == 0) {
        std::cout << "No resources are loaded, so the waiting list cannot be used.\n";
        return true;
    }

    std::cout << "\n-- Add a student to the waiting list --\n";

    std::string studentId;
    InputStatus status = promptNonEmpty("Student ID: ", "Student ID", studentId);
    if (status == InputStatus::EndOfInput) {
        return false;
    }
    if (status == InputStatus::Cancelled) {
        return true;
    }

    int resourceId = 0;
    status = promptExistingResourceId(manager, resourceId);
    if (status == InputStatus::EndOfInput) {
        return false;
    }
    if (status == InputStatus::Cancelled) {
        return true;
    }

    std::string date;
    std::string startTime;
    std::string endTime;
    status = promptTimeWindow(date, startTime, endTime);
    if (status == InputStatus::EndOfInput) {
        return false;
    }
    if (status == InputStatus::Cancelled) {
        return true;
    }

    if (!activeReservations.hasConflict(resourceId, date, startTime, endTime)) {
        std::cout << "\nNote: that slot is currently free - menu option 3 would reserve it now.\n";

        bool joinAnyway = false;
        status = promptYesNo("Add to the waiting list anyway? (y/n): ", joinAnyway);
        if (status == InputStatus::EndOfInput) {
            return false;
        }
        if (status != InputStatus::Ok || !joinAnyway) {
            std::cout << "Nothing was added to the waiting list.\n";
            return true;
        }
    }

    waitingList.enqueue(WaitingRequest(studentId, resourceId, date, startTime, endTime));
    reports.recordRequest(resourceId);
    std::cout << "Student " << studentId << " joined the waiting list at position "
              << waitingList.size() << ".\n";
    return true;
}

bool handleDisplayWaitingList(const WaitingList& waitingList) {
    waitingList.displayWaitingList();
    return true;
}

bool handleDisplayCancellationHistory(const CancellationStack& cancellationHistory) {
    cancellationHistory.displayCancellationHistory();
    return true;
}

bool handleUndoCancellation(ReservationList& activeReservations,
                            CancellationStack& cancellationHistory) {
    Reservation mostRecent;
    if (!cancellationHistory.peek(mostRecent)) {
        std::cout << "The cancellation history is empty - there is nothing to undo.\n";
        return true;
    }

    std::cout << "\nMost recent cancellation:\n";
    Reservation::displayHeader();
    mostRecent.display();

    if (activeReservations.findReservation(mostRecent.getReservationId()) != nullptr) {
        std::cout << "\nCannot restore: reservation ID " << mostRecent.getReservationId()
                  << " has been reused by an active reservation.\n"
                  << "It stays on the cancellation history.\n";
        return true;
    }

    if (activeReservations.hasConflict(mostRecent.getResourceId(), mostRecent.getDate(),
                                       mostRecent.getStartTime(), mostRecent.getEndTime())) {
        std::cout << "\nCannot restore: the slot has been taken by another reservation.\n";
        activeReservations.displayConflicts(mostRecent.getResourceId(), mostRecent.getDate(),
                                            mostRecent.getStartTime(), mostRecent.getEndTime());
        std::cout << "It stays on the cancellation history.\n";
        return true;
    }

    // Keep the cancellation on the stack if restoring it would cause a conflict.
    Reservation restored;
    cancellationHistory.pop(restored);
    activeReservations.insertReservation(restored);

    std::cout << "\nReservation restored to the active list.\n";
    Reservation::displayHeader();
    restored.display();
    return true;
}

bool handleRemoveWaiting(WaitingList& waiting) {
    if (waiting.isEmpty()) { std::cout << "The waiting list is empty.\n"; return true; }
    waiting.displayWaitingList();
    int position = 0;
    const InputStatus status = promptPositiveInt("Position to remove: ", "Position", position);
    if (status == InputStatus::EndOfInput) return false;
    if (status == InputStatus::Cancelled) return true;
    WaitingRequest removed;
    if (waiting.removeAt(static_cast<std::size_t>(position - 1), removed))
        std::cout << "Removed waiting student " << removed.studentId << ".\n";
    else std::cout << "No waiting request at that position.\n";
    return true;
}
bool handleSearchResource(const ResourceManager& manager) {
    int id = 0;
    const InputStatus status = promptPositiveInt("Resource ID to search: ", "Resource ID", id);
    if (status == InputStatus::EndOfInput) return false;
    if (status == InputStatus::Cancelled) return true;
    const Resource* resource = manager.findResourceById(id);
    if (resource) { Resource::displayHeader(); resource->display(); }
    else std::cout << "No resource with ID " << id << ".\n";
    return true;
}

void displayMenu() {
    std::cout << "\n==========================================\n"
              << "  Campus Resource Reservation System\n"
              << "==========================================\n"
              << "  1. Display all resources\n"
              << "  2. Display resource availability\n"
              << "  3. Create reservation\n"
              << "  4. Cancel reservation\n"
              << "  5. Display active reservations\n"
              << "  6. Add student to waiting list\n"
              << "  7. Display waiting list\n"
              << "  8. Display cancellation history\n"
              << "  9. Undo most recent cancellation\n"
              << " 10. Remove student from waiting list\n"
              << " 11. Search resource by ID (linear search)\n"
              << " 12. Display resources sorted by name (merge sort)\n"
              << " 13. Generate system reports\n"
              << "  0. Exit\n"
              << "------------------------------------------\n";
}

}

int main(int argc, char* argv[]) {

    const std::string resourceFile = (argc > 1) ? argv[1] : DEFAULT_RESOURCE_FILE;

    ResourceManager   manager;
    ReservationList   activeReservations;
    WaitingList       waitingList;
    CancellationStack cancellationHistory;
    SystemReports     reports;

    std::cout << "Loading resources from \"" << resourceFile << "\"...\n";
    if (manager.loadFromFile(resourceFile)) {
        std::cout << "Loaded " << manager.getResourceCount() << " resource(s)";
        if (manager.getSkippedRecordCount() > 0) {
            std::cout << ", skipped " << manager.getSkippedRecordCount() << " malformed record(s)";
        }
        std::cout << ".\n";
    } else {
        std::cout << "Continuing with an empty resource catalog. "
                  << "Reservation options will be unavailable.\n";
    }

    bool running = true;
    while (running) {
        displayMenu();

        std::string choice;
        if (promptLine("Select an option (0-13): ", choice) == InputStatus::EndOfInput) {
            break;
        }

        if (choice == "1") {
            running = handleDisplayResources(manager);
        } else if (choice == "2") {
            running = handleCheckAvailability(manager, activeReservations);
        } else if (choice == "3") {
            running = handleCreateReservation(manager, activeReservations, waitingList, reports);
        } else if (choice == "4") {
            running = handleCancelReservation(activeReservations, waitingList, cancellationHistory);
        } else if (choice == "5") {
            running = handleDisplayReservations(activeReservations);
        } else if (choice == "6") {
            running = handleJoinWaitingList(manager, activeReservations, waitingList, reports);
        } else if (choice == "7") {
            running = handleDisplayWaitingList(waitingList);
        } else if (choice == "8") {
            running = handleDisplayCancellationHistory(cancellationHistory);
        } else if (choice == "9") {
            running = handleUndoCancellation(activeReservations, cancellationHistory);
        } else if (choice == "10") {
            running = handleRemoveWaiting(waitingList);
        } else if (choice == "11") {
            running = handleSearchResource(manager);
        } else if (choice == "12") {
            manager.displayResourcesSortedByName();
        } else if (choice == "13") {
            reports.display(manager, activeReservations, waitingList);
        } else if (choice == "0") {
            std::cout << "Exiting. Goodbye!\n";
            running = false;
        } else {

            std::cout << "\"" << choice << "\" is not a valid option. "
                      << "Please enter a number from 0 to 13.\n";
        }
    }

    return 0;
}
