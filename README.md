# Campus Resource Reservation System — Milestone 1

C++17 console program. Resources load from a file; active reservations use a custom
singly linked list, waiting requests a custom FIFO queue, and cancellations a custom
LIFO stack. The code uses nodes and pointers rather than std::list, std::queue, or std::stack.

## Build and test

From the project root:

```bash
make
./reservation_system
make test
```

Without Make:

```bash
g++ -std=c++17 -Wall -Wextra -pedantic src/*.cpp -Iinclude -o reservation_system
```

Python 3 is needed only for the automated CLI tests. Run on UNT CELL before submission.
Local testing is documented in docs/TestingResults.md; CELL verification is pending.

## Menu

| Option | Action |
| --- | --- |
| 1 | Display resources |
| 2 | Check availability for a date and time |
| 3 | Create a reservation or offer the waiting list for a conflicting slot |
| 4 | Cancel a reservation and promote the earliest compatible waiting request |
| 5 | Display active reservations |
| 6 | Add a waiting request |
| 7 | Display waiting list |
| 8 | Display cancellation history, newest first |
| 9 | Undo latest cancellation if its ID and slot are free |
| 10 | Remove a waiting entry by its displayed position |
| 0 | Exit |

Resource file format: resourceId|name|type|capacity. Positive integer IDs and capacities,
unique resource IDs, and nonempty name/type are required. Comments beginning with # and
blank lines are ignored. Invalid rows are reported and skipped. An alternative catalog
can be passed as an argument, e.g. ./reservation_system data/resources_malformed.txt.

Dates must be real calendar dates in YYYY-MM-DD form, including leap years. Times use
HH:MM in 24-hour format; start must precede end. Back-to-back bookings are allowed. Past
dates are accepted. Type cancel at an operation prompt to return to the menu; EOF exits.
All booking/history/queue data is in memory and resets on exit. Each cancellation promotes
at most one compatible waiting request; automatic booking IDs begin at 9000.

## Files and responsibilities

Anish's assigned components: Resource.h/.cpp, ResourceManager.h/.cpp, Reservation.h/.cpp,
ReservationList.h/.cpp and their tests. Jacob's assigned components: WaitingList.h/.cpp,
CancellationStack.h/.cpp and queue/stack tests. main.cpp, integration, documentation,
contribution report, and CELL verification are shared responsibilities. The contribution
report must reflect actual completed work, not just assignments.

This package contains Milestone 1 features. Sorting and final reports are held for the
final project. docs/ComplexityAnalysis.md explains the operation costs. The tests cover
file input, validation, linked-list operations, queue behavior, cancellation, and undo.
