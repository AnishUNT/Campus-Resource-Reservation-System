# User documentation

Build with `make` and run `./reservation_system` from the project root. The default
catalog is `data/resources.txt`; optionally pass another file path. No installation or
external C++ libraries are required.

| Option | Action |
| --- | --- |
| 1 | Display resource IDs, names, types, and capacities |
| 2 | Check one resource's availability for a date/time window |
| 3 | Create a reservation; offer the queue when the window conflicts |
| 4 | Cancel a reservation by ID and attempt one FIFO-compatible promotion |
| 5 | Display active reservations |
| 6 | Add a waiting request; confirm when the requested window is already free |
| 7 | Display queue entries in order with their positions |
| 8 | Display cancellation history, newest first |
| 9 | Undo the latest cancellation if no ID or time conflict exists |
| 10 | Remove a waiting request by its displayed position (starting at 1) |
| 11 | Search the resource catalog by ID using linear search |
| 12 | Display resources alphabetically using handwritten merge sort |
| 13 | Generate all four system reports |
| 0 | Exit |

## Inputs and errors

Reservation and resource IDs are positive integers. Dates are `YYYY-MM-DD` and must
exist on the calendar. Times are zero-padded 24-hour `HH:MM`; start must be earlier than
end on the same day. The program rejects overlapping windows for the same resource/date.
An ending time equal to another booking's starting time is allowed.

Use `cancel` at any operation prompt to return to the menu. EOF (Ctrl+D on Linux) exits.
An unknown resource ID during booking is rejected and re-prompted. An unknown ID in
option 11 reports no match. Missing files and malformed lines are reported without a crash.

## Demonstration

1. Use option 1 to see the six sample resources.
2. Use option 3: ID `1`, student `S001`, resource `101`, date `2026-10-11`,
   start `09:00`, end `10:00`. This creates the booking.
3. Repeat option 3 with ID `2`, student `S002`, and the same resource/date/window.
   Answer `y` to join the queue. Option 7 shows S002 waiting.
4. Use option 13. Resource 101 has one active booking, one waiting request, two requests.
5. Use option 4 to cancel ID 1. S002 is promoted, usually with ID 9000.
6. Use option 9. Undo of ID 1 is refused because the promoted booking occupies its window.
   The cancellation stays on the stack.
7. Demonstrate a successful undo on an independent booking for resource 102: create ID 3,
   cancel ID 3, then option 9. It restores ID 3 because no waiting student took its slot.
8. Use option 11 for resource 201 and for missing ID 999; show hit/miss results.
9. Use option 12 to demonstrate alphabetical ordering and explain recursive split/merge.
10. Use option 13 again and explain that historical request counts survive cancellations.

A promotion and undo reuse an existing request, so they do not increase demand counts.
All session data resets when you exit. Save report output by redirecting the program's
stdout when desired; menu option 13 prints reports rather than writing a report file.
