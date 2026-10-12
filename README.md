# Campus Resource Reservation System — Final Project

C++17 console application for CS 2100. Active reservations use a custom singly linked list, waiting requests use a custom FIFO queue, and cancelled reservations use a custom LIFO stack. Each container frees its nodes when the program exits.

## Build and run

Run from the project root:

```bash
make
./reservation_system
```

Without Make:

```bash
g++ -std=c++17 -Wall -Wextra -pedantic src/*.cpp -Iinclude -o reservation_system
./reservation_system
```

To use a different resource file:

```bash
./reservation_system data/resources_malformed.txt
```

Run automated tests:

```bash
make clean
make test
```

Python 3 is required for the integration tests. `make clean` removes the executable and build directory.

## CELL verification

Anish Sandaka compiled and tested the project on UNT's `cell01-cse` server running Ubuntu 22.04.5 LTS on October 11, 2026.

- Clean C++17 build completed with no compiler warnings.
- Unit tests passed.
- All 16 integration checks passed.
- The program loaded six resources and displayed the main menu.

The missing-file error and malformed-record warning printed during testing are expected results from invalid-input tests. See `docs/TestingResults.md` for details.

## Features

- Load resources from a pipe-delimited file, store them in memory, and display them.
- Check resource availability for a date and time window.
- Validate positive IDs, nonempty student IDs, existing resources, calendar dates, 24-hour times, start/end order, and reservation conflicts.
- Create, cancel, and display active reservations.
- Add, remove, and display waiting-list entries.
- After cancellation, promote the earliest compatible waiting request for that resource and date.
- Store cancellation history and restore the most recently cancelled reservation when its ID and time window are available.
- Preserve cancellation history when an undo attempt fails.
- Search resources by ID using a manually implemented linear search in menu option 11.
- Display resources sorted by name using a manually implemented merge sort in menu option 12. Matching names are ordered by resource ID.
- Generate active reservation, resource utilization, most requested resource, and waiting-list reports.
- Handle invalid input, allow `cancel` at reservation-entry prompts, and exit cleanly at end of input.

The required search and sorting algorithms do not use library search or sorting functions.

## Resource file format

Each resource record uses this format:

```text
resourceId|name|type|capacity
```

Example:

```text
101|Willis Library Study Room A|StudyRoom|6
```

Resource IDs and capacities must be positive integers. Resource IDs must be unique, and names and types must not be empty.

Blank lines and lines beginning with `#` are ignored. Malformed records are reported and skipped. A missing file produces an error and leaves an empty catalog.

## Project layout

| Path | Purpose |
| --- | --- |
| `include/Resource.h`, `src/Resource.cpp` | Resource data and display formatting |
| `include/ResourceManager.h`, `src/ResourceManager.cpp` | File loading, linear search, and merge sort |
| `include/Reservation.h`, `src/Reservation.cpp` | Reservation data and validation helpers |
| `include/ReservationList.h`, `src/ReservationList.cpp` | Custom linked list for active reservations |
| `include/WaitingList.h`, `src/WaitingList.cpp` | Custom queue for waiting requests |
| `include/CancellationStack.h`, `src/CancellationStack.cpp` | Custom stack for cancellation history |
| `include/SystemReports.h`, `src/SystemReports.cpp` | Request counts and system reports |
| `src/main.cpp` | Menu, input handling, cancellation, promotion, and undo |
| `data/` | Resource catalog and malformed sample input |
| `tests/unit_tests.cpp` | Automated component tests |
| `tests/integration_tests.py` | Automated console integration tests |
| `tests/test_cases.md` | Additional manual test procedures |
| `docs/UserDocumentation.md` | Menu guide and usage examples |
| `docs/ComplexityAnalysis.md` | Big-O analysis |
| `docs/TestingResults.md` | Development and CELL test results |
| `docs/GroupContributionReport.md` | Individual contributions and GitHub activity |
| `repository_url.txt` | GitHub repository URL |
| `Makefile` | Build, test, and cleanup commands |

## Reports and program limits

Resource utilization shows the current number of active reservations for each resource. Waiting-list statistics show the number of waiting requests for each resource. Multiple requests from the same student count as separate entries.

Demand counts each accepted reservation or waiting-list request once during the current run. Cancellation does not remove its historical demand count. Promotion and undo do not count the request again. Invalid requests and declined waiting-list offers do not count.

The most requested resources report displays all resources tied for the highest nonzero demand count.

Reservations, waiting requests, cancellation history, and demand counts are stored in memory and reset when the program restarts. The resource catalog is loaded from disk.

Each cancellation promotes at most one compatible waiting request. Back-to-back reservations are allowed. Reservation windows must start and end on the same date and cannot cross midnight.

## Team

- Anish Sandaka — GitHub: `AnishUNT`
- Jacob Casilio — GitHub: `Jakecas1209`

The Group Contribution Report describes each member's actual work, testing, and GitHub contributions. 

## Repository

https://github.com/AnishUNT/Campus-Resource-Reservation-System