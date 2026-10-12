# Testing results — updated 2026-10-11

## Development environment

- Clean C++17 build with `g++ -Wall -Wextra -pedantic`: passed, with no compiler warnings.
- Unit tests and all **16 CLI integration checks passed**.
- AddressSanitizer and UndefinedBehaviorSanitizer checks passed in the development environment with no reported address or undefined-behavior errors.
- LeakSanitizer could not run because of an environment restriction. Leak detection is not claimed.
- Anish also ran `make clean` and `make test` on his Mac. Unit tests and all 16 integration checks passed.

## Automated coverage

| Area | Checks | Result |
| --- | --- | --- |
| File input | Normal catalog, missing file, malformed lines, extra trailing delimiter | Pass |
| Linear search | Existing and missing resource IDs; search menu integration | Pass |
| Merge sort | Alphabetical order, tied names ordered by ID, empty catalog, preservation of stored order | Pass |
| Linked list | Insert, lookup, count, head/middle/tail/singleton removal, reuse after empty | Pass |
| Validation | Leap years, invalid dates and times, integer overflow, blank student ID, nonexistent resource, start/end order | Pass |
| Reservations | Creation, back-to-back time windows, overlapping requests | Pass |
| Queue | FIFO dequeue, middle/tail removal, enqueue after empty or tail removal, waiting-list promotion | Pass |
| Stack and undo | Empty operations, LIFO pop, successful restore, conflict and reused-ID refusal | Pass |
| Reports | Empty data, active/waiting/demand counts, no double counting after undo | Pass |
| Interface | Invalid menu input, cancel prompt, end-of-input handling | Pass |

Automated commands:

```bash
make clean
make test
```

`tests/test_cases.md` contains additional manual procedures. Expected results in that file do not establish that every scenario was personally run by a team member.

## UNT CELL verification — passed

Anish Sandaka tested the project on `cell01-cse` on October 11, 2026, at approximately 10:22 PM Central Time.

Commands run from `~/campus-final`:

```bash
make clean
make test
./reservation_system
```

| CELL evidence | Actual result |
| --- | --- |
| Student who ran checks | Anish Sandaka, EUID as3713 |
| Date/time | October 11, 2026, approximately 10:22 PM Central Time |
| CELL hostname | cell01-cse |
| Operating system | Ubuntu 22.04.5 LTS |
| Build | C++17 build completed with no compiler warnings |
| Compiler version | Exact version not recorded; build used g++ |
| Unit tests | Passed |
| Integration tests | All 16 checks passed |
| Program startup | Loaded six resources and displayed the main menu |
| Memory checking on CELL | Not performed |

The missing-file error and malformed-record warning were expected output from tests checking invalid file input. They did not indicate a build failure.

CELL verification covers the clean build, automated tests, and program startup shown in the recorded terminal output. Additional manual menu testing and memory checking on CELL are not claimed.