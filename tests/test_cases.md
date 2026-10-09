# Manual Test Cases

For recorded automated results, see `docs/TestingResults.md`. These procedures describe
expected behavior and do not certify execution on UNT CELL.

All tests are run from the project root after building:

```bash
make            # or: g++ -std=c++17 -Wall -Wextra -pedantic src/*.cpp -Iinclude -o reservation_system
./reservation_system
```

Every test can be run interactively, or scripted by piping the keystrokes with `printf`.
The scripted form is given where it is convenient. Menu option numbers are shown in the
order they are typed.

Legend: **PASS** = observed behaviour matched the expected result during development.
Re-run and re-record these before submission.

---

## 1. Resource loading

### TC-01 — Successful resource-file loading
- **Steps:** `./reservation_system`, then menu `1`.
- **Expected:** `Loaded 6 resource(s).` followed by a formatted table of 6 resources with
  IDs 101, 102, 201, 202, 301, 302 and their type and capacity.
- **Status:** Not re-run individually for this update; see automated coverage in TestingResults.md.

### TC-02 — Missing resource file
- **Steps:** `./reservation_system data/does_not_exist.txt`, then menu `1`, `3`, `2`, `0`.
- **Expected:** An `ERROR: could not open resource file ...` message, then
  `Continuing with an empty resource catalog.` The program does **not** crash. Option 1
  reports `No resources are loaded.`; options 2 and 3 refuse politely; option 0 exits.
- **Status:** Not re-run individually for this update; see automated coverage in TestingResults.md.

### TC-03 — Malformed resource records
- **Steps:** `./reservation_system data/resources_malformed.txt`, then menu `1`, `0`.
- **Expected:** One `WARNING: line N skipped - ...` per bad record (missing separators,
  too few fields, non-numeric capacity, non-numeric id, empty name, duplicate id), then
  `Loaded 2 resource(s), skipped 6 malformed record(s).` Only the two valid resources are
  listed. No crash.
- **Status:** Not re-run individually for this update; see automated coverage in TestingResults.md.

---

## 2. Reservation creation and validation

### TC-04 — Create a valid reservation
- **Steps:** menu `3` → ID `1`, student `S001`, resource `101`, date `2026-09-21`,
  start `09:00`, end `11:00`. Then menu `5`.
- **Scripted:** `printf '3\n1\nS001\n101\n2026-09-21\n09:00\n11:00\n5\n0\n' | ./reservation_system`
- **Expected:** `Reservation created.` with the reservation echoed in a table; option 5
  shows `Active reservations (1 total)`.
- **Status:** Not re-run individually for this update; see automated coverage in TestingResults.md.

### TC-05 — Reject a duplicate reservation ID
- **Steps:** After TC-04, menu `3` → ID `1` again.
- **Expected:** `Reservation ID 1 is already in use.` and the prompt repeats. The first
  reservation is untouched.
- **Status:** Not re-run individually for this update; see automated coverage in TestingResults.md.

### TC-06 — Reject an invalid resource ID
- **Steps:** menu `3` → ID `2`, student `S002`, resource `999`.
- **Expected:** `No resource with ID 999. Use menu option 1 to list valid IDs.` and the
  resource prompt repeats. Also verify a non-numeric resource ID (`abc`) produces
  `Resource ID must be a positive whole number`.
- **Status:** Not re-run individually for this update; see automated coverage in TestingResults.md.

### TC-07 — Reject invalid HH:MM time input
- **Steps:** At a start-time prompt enter `9:00`, then `25:00`, then `09:61`, then `abc`,
  then `09:00`.
- **Expected:** Each invalid value produces
  `Invalid time. Use 24-hour HH:MM, for example 09:30 or 14:00.` and re-prompts. Only
  `09:00` is accepted. Also verify an invalid date (`2026-13-45`) is rejected with the
  date message.
- **Status:** Not re-run individually for this update; see automated coverage in TestingResults.md.

### TC-08 — Reject an end time earlier than the start time
- **Steps:** At the time prompts enter start `09:00`, end `08:00`.
- **Expected:** `The start time must be earlier than the end time. Please enter both times
  again.` and both time prompts repeat. Also check equal times (`09:00`/`09:00`) are
  rejected the same way.
- **Status:** Not re-run individually for this update; see automated coverage in TestingResults.md.

### TC-09 — Reject an empty student ID
- **Steps:** At the Student ID prompt press Enter on an empty line.
- **Expected:** `Student ID must not be empty (type 'cancel' to go back).` and re-prompt.
- **Status:** Not re-run individually for this update; see automated coverage in TestingResults.md.

---

## 3. Conflict detection

### TC-10 — Detect overlapping reservations
- **Steps:** With reservation 1 on resource 101, `2026-09-21`, `09:00`–`11:00` active,
  menu `3` → ID `2`, student `S002`, resource `101`, `2026-09-21`, `10:00`–`12:00`.
- **Expected:** `That resource is already reserved for an overlapping time.` plus the
  conflicting reservation printed, then the waiting-list offer. No second reservation is
  created.
- **Status:** Not re-run individually for this update; see automated coverage in TestingResults.md.

### TC-11 — Allow adjacent non-overlapping reservations
- **Steps:** With reservation 1 occupying `09:00`–`11:00`, create ID `3`, student `S003`,
  resource `101`, `2026-09-21`, `11:00`–`12:00`.
- **Expected:** `Reservation created.` Touching end points are not an overlap
  (`newStart < existingEnd AND existingStart < newEnd`). Option 5 then shows 2 active
  reservations.
- **Status:** Not re-run individually for this update; see automated coverage in TestingResults.md.

### TC-12 — Same time window on a different resource or date
- **Steps:** Create a reservation for resource `102` at `2026-09-21` `09:00`–`11:00`, and
  another for resource `101` at `2026-09-22` `09:00`–`11:00`.
- **Expected:** Both succeed — conflicts are scoped to the same resource **and** the same
  date.
- **Status:** Not re-run individually for this update; see automated coverage in TestingResults.md.

### TC-13 — Availability check reflects date and time, not a permanent flag
- **Steps:** menu `2` → resource `101`, `2026-09-21`, `09:30`–`10:00`. Then menu `2` →
  resource `101`, `2026-09-21`, `13:00`–`14:00`.
- **Expected:** The first reports `NOT AVAILABLE` and lists the conflicting reservation;
  the second reports `AVAILABLE`. A resource with one booking is not marked unavailable
  for the whole day.
- **Status:** Not re-run individually for this update; see automated coverage in TestingResults.md.

---

## 4. Waiting-list queue

### TC-14 — Add multiple students to the queue
- **Steps:** Decline-then-accept the waiting-list offer from TC-10 with `y` for `S002`.
  Then menu `6` → student `S003`, resource `201`, matching date/time that conflicts.
- **Expected:** Each returns `joined the waiting list at position N` with N increasing.
- **Status:** Not re-run individually for this update; see automated coverage in TestingResults.md.

### TC-15 — Confirm FIFO order
- **Steps:** After TC-14, menu `7`.
- **Scripted:** `printf '3\n10\nS001\n201\n2026-09-22\n09:00\n11:00\n3\n11\nS002\n201\n2026-09-22\n10:00\n11:00\ny\n6\nS003\n201\n2026-09-22\n09:00\n10:00\n7\n0\n' | ./reservation_system`
- **Expected:** `Waiting list in FIFO order (2 waiting)` with position 1 = `S002` (added
  first) and position 2 = `S003`. Order is insertion order, not sorted by time.
- **Status:** Not re-run individually for this update; see automated coverage in TestingResults.md.

### TC-16 — Empty waiting list
- **Steps:** On a fresh run, menu `7`.
- **Expected:** `The waiting list is empty.` No crash, no garbage output.
- **Status:** Not re-run individually for this update; see automated coverage in TestingResults.md.

### TC-17 — Waiting-list entry for an already-free slot
- **Steps:** menu `6` with a date/time that has no conflict.
- **Expected:** A note that the slot is currently free and option 3 would reserve it now,
  followed by `Add to the waiting list anyway? (y/n):`. Answering `n` adds nothing.
- **Status:** Not re-run individually for this update; see automated coverage in TestingResults.md.

---

## 5. Cancellation and the stack

### TC-18 — Cancel a reservation
- **Steps:** With reservation 1 active, menu `4` → `1`. Then menu `5`.
- **Expected:** `Reservation cancelled and pushed onto the cancellation history.` and the
  reservation no longer appears in the active list.
- **Status:** Not re-run individually for this update; see automated coverage in TestingResults.md.

### TC-19 — Cancel a nonexistent reservation
- **Steps:** menu `4` → an ID that does not exist (e.g. `4242`).
- **Expected:** `No active reservation has ID 4242.` Nothing is pushed onto the stack.
- **Status:** Not re-run individually for this update; see automated coverage in TestingResults.md.

### TC-20 — Cancel with an empty active list
- **Steps:** On a fresh run, menu `4`.
- **Expected:** `There are no active reservations to cancel.` No prompt, no crash.
- **Status:** Not re-run individually for this update; see automated coverage in TestingResults.md.

### TC-21 — Promotion from the waiting list on cancellation
- **Steps:** Use the TC-15 script, then cancel reservation `10`.
- **Scripted:** `printf '3\n10\nS001\n201\n2026-09-22\n09:00\n11:00\n3\n11\nS002\n201\n2026-09-22\n10:00\n11:00\ny\n6\nS003\n201\n2026-09-22\n09:00\n10:00\n7\n4\n10\n7\n5\n0\n' | ./reservation_system`
- **Expected:** `Waiting list: student S002 was promoted into the freed slot.` with a new
  auto-generated reservation ID (9000). The waiting list drops to 1 entry (`S003`, who
  keeps their place), and the active list shows the promoted reservation.
- **Status:** Not re-run individually for this update; see automated coverage in TestingResults.md.

### TC-22 — No compatible waiting student
- **Steps:** With `S002` waiting for `10:00`–`12:00` on resource 101 and reservations 1
  (`09:00`–`11:00`) and 3 (`11:00`–`12:00`) active, cancel reservation `1`.
- **Expected:** `No waiting student could take the freed slot.` — reservation 3 still
  overlaps the waiting request, so `S002` stays queued.
- **Status:** Not re-run individually for this update; see automated coverage in TestingResults.md.

### TC-23 — Confirm LIFO cancellation history
- **Steps:** Cancel reservation `1`, then cancel reservation `3`, then menu `8`.
- **Expected:** `Cancellation history, most recent first` listing reservation `3` above
  reservation `1`.
- **Status:** Not re-run individually for this update; see automated coverage in TestingResults.md.

### TC-24 — Empty cancellation stack display
- **Steps:** On a fresh run, menu `8`.
- **Expected:** `No reservations have been cancelled yet.` No crash.
- **Status:** Not re-run individually for this update; see automated coverage in TestingResults.md.

---

## 6. Undo

### TC-25 — Undo the latest cancellation (success)
- **Steps:** Create reservation `20` on resource `302`, `2026-09-23`, `13:00`–`14:00`;
  cancel it; menu `9`; then menu `5` and `8`.
- **Scripted:** `printf '3\n20\nS004\n302\n2026-09-23\n13:00\n14:00\n4\n20\n8\n9\n5\n8\n0\n' | ./reservation_system`
- **Expected:** `Reservation restored to the active list.` Option 5 shows reservation 20
  again; option 8 shows the history is now empty (the entry was popped only after a
  successful restore).
- **Status:** Not re-run individually for this update; see automated coverage in TestingResults.md.

### TC-26 — Prevent an invalid restoration
- **Steps:** Use the promotion scenario so the freed slot is taken by a promoted student,
  then menu `9`.
- **Expected:** `Cannot restore: the slot has been taken by another reservation.` with the
  conflicting reservation listed, then `It stays on the cancellation history.` Running
  option `9` again repeats the same refusal — the stack is unchanged.
- **Status:** Not re-run individually for this update; see automated coverage in TestingResults.md.

### TC-27 — Undo with an empty cancellation stack
- **Steps:** On a fresh run, menu `9`.
- **Expected:** `The cancellation history is empty - there is nothing to undo.` No crash.
- **Status:** Not re-run individually for this update; see automated coverage in TestingResults.md.

### TC-28 — Undo refused because the ID was reused
- **Steps:** Create reservation `30`, cancel it, create a *new* reservation also using ID
  `30` at a non-conflicting time, then menu `9`.
- **Expected:** `Cannot restore: reservation ID 30 has been reused by an active
  reservation.` and the entry stays on the stack.
- **Status:** Not re-run individually for this update; see automated coverage in TestingResults.md.

---

## 7. Input robustness

### TC-29 — Letters entered at the numeric menu prompt
- **Steps:** At the main menu type `abc`, then `hello world`, then an empty line, then
  `99`, then `-3`, then `0`.
- **Scripted:** `printf 'abc\nhello world\n\n99\n-3\n0\n' | ./reservation_system`
- **Expected:** Each invalid entry produces
  `"<input>" is not a valid option. Please enter a number from 0 to 10.` and the menu
  redisplays. The input stream is never left in a failed state (input is read with
  `std::getline`, never `cin >> int`), and the program exits normally on `0`.
- **Status:** Not re-run individually for this update; see automated coverage in TestingResults.md.

### TC-30 — `cancel` aborts an operation
- **Steps:** menu `3`, then type `cancel` at the Reservation ID prompt.
- **Expected:** `Operation cancelled.` and a return to the main menu with nothing created.
- **Status:** Not re-run individually for this update; see automated coverage in TestingResults.md.

### TC-31 — End of input (Ctrl+D / piped script ends)
- **Steps:** `printf '3\n' | ./reservation_system`
- **Expected:** `Input ended.` and a clean exit with status 0 — no infinite loop.
- **Status:** Not re-run individually for this update; see automated coverage in TestingResults.md.

### TC-32 — Empty linked list, queue, and stack together
- **Steps:** On a fresh run: `5`, `4`, `7`, `8`, `9`, `0`.
- **Expected:** Each reports its own empty-state message and the program exits cleanly.
  This exercises the null-pointer guards in all three structures.
- **Status:** Not re-run individually for this update; see automated coverage in TestingResults.md.

---

## 8. Memory

### TC-33 — No memory leaks
- **Steps (if Valgrind is available on the CELL machines):**
  ```bash
  valgrind --leak-check=full ./reservation_system < input_script.txt
  ```
- **Expected:** `All heap blocks were freed -- no leaks are possible`. Every node allocated
  by the list, queue, and stack is released by its destructor / `clear()`.
- **Status:** Leak detection not verified for this update; CELL/Valgrind check pending.
