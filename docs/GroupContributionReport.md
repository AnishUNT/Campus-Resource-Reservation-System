# Group Contribution Report — Milestone 1

**Team:** Anish Sandaka (@AnishUNT), Jacob Casilio (@Jakecas1209)
**Status:** Draft; verify actual work and fill pending fields before submission.

The existing responsibility split assigns resource/reservation code to Anish and queue,
stack, and undo work to Jacob. An assignment is not evidence of completed work. Existing
main at commit `cd5f601` contained queue/stack stubs. This completion update was prepared
with assistant help; copying or uploading its files does not establish original authorship.
Record each student's actual implementation, review, changes, tests, and documentation.

| Team Member | Component(s) | Specific Tasks Completed | Testing/Debugging | GitHub Contributions |
| --- | --- | --- | --- | --- |
| Anish Sandaka | Resource catalog, reservation model/list; final integration responsibilities to confirm | Existing commits record resource/reservation work; confirm exact personal functions and work on the Milestone 1 update | Pending: personally run tests, fixes, CELL verification | Existing main: `ce6a266`, `a74d5e9`, `cd5f601`; Milestone 1 branch/commit/PR pending |
| Jacob Casilio | Assigned waiting queue, cancellation stack, restoration | Pending: actual functions implemented or revised and reviewed; completed reference files available for review | Pending: personally run queue/stack/undo tests and record fixes | Remote `feature/waitlist-cancellations` exists; personal commits/PR and merge pending |

## Anish Sandaka

Pending: confirm the specific personal work in `ResourceManager::loadFromFile`,
`findResourceById`, `ReservationUtils` validation, and `ReservationList` insertion,
removal, lookup, and overlap checks. Describe actual work on menu integration, tests, or documentation for this update, including assistant assistance as
required by course policy. List personally executed tests and actual Milestone 1 commit/PR URLs.

## Jacob Casilio

Pending: document actual implementation/revision of `WaitingList::enqueue`, `dequeue`,
`peekAt`, `removeAt`, `clear`, and `CancellationStack::push`, `pop`, `peek`, and `clear`.
Specify review or debugging of cancellation promotion and undo only if performed. List
personally executed tests, fixes, and actual commit/PR URLs. Do not claim supplied
reference implementations as work independently written by Jacob.

## Completion update inventory

Prepared with assistant help on 2026-10-08: completed queue/stack implementations,
waiting-list removal menu, calendar validation, component and CLI tests, README,
complexity corrections, and Milestone 1 packaging.
Local automated results are in `TestingResults.md`; they are not personal CELL evidence.

## Final details to complete

Record final implementation ownership, review/debugging changes, individual test runs,
CELL results, final commit hashes, feature branch, pull request URL, and merge commit.
Both team members should verify the finished report accurately describes their own work.

## Observed testing by Anish on 2026-10-08

Anish provided Mac terminal output demonstrating compilation, resource loading,
reservation creation, invalid resource/time rejection, availability conflicts, cancellation,
successful undo, waiting-list enqueue/display, automatic promotion, an empty queue after
promotion, and undo refusal for an occupied slot. Anish also ran the full-project automated
suite: unit tests and 16 integration checks passed. The isolated Milestone 1 suite must still
be run by Anish on his machine; Jacob's personal tests and both members' CELL results are pending.
