# Complexity analysis

Let `n` be active reservations, `w` waiting requests, `c` cancelled reservations, and
`r` resources. String lengths are treated as bounded for these counts.

| Operation | Big-O time | Explanation |
| --- | --- | --- |
| Reservation list insertion | O(1) | Append using the tail pointer |
| Complete reservation creation | O(n + r) | Scan IDs/conflicts and search the catalog before inserting |
| Reservation list removal | O(n) | Walk to the target, then unlink in constant time |
| Reservation lookup / conflict check | O(n) | Handwritten linked-list traversal |
| Waiting enqueue / dequeue / peek | O(1) | Rear/front pointers avoid traversal |
| Waiting positional access / removal | O(w) | Walk to the indexed node |
| Waiting-list display / per-resource count | O(w) | Traverse every queue node once |
| Stack push / pop / peek | O(1) | Change or read only the top node |
| Undo cancellation | O(n) | Check ID and time conflict before O(1) pop and insertion |
| Cancellation history display | O(c) | Traverse the stack from newest to oldest |
| Catalog loading | O(Lr), worst O(r²) for valid rows | Each of L input lines may check duplicate ID by linear search |

## Waiting-list processing after cancellation

The current promotion routine checks queue positions with `peekAt(index)`. Repeated
positional walks total O(w²) in the worst case. Each candidate's conflict check may
scan `n` active reservations, adding O(wn). Removing the chosen queue entry costs O(w).
The routine preserves FIFO order by promoting the earliest compatible request.

Automatic IDs start at 9000 and probe until unused. Each probe is an O(n) lookup;
up to n+1 probes gives O(n²) worst-case ID generation. Thus a complete cancellation,
including removal, compatible promotion, and ID generation, has worst-case time
O(w² + wn + n² + n). If no promotion occurs there is no ID-generation cost.
This is distinct from O(1) queue enqueue/dequeue and O(n) list removal.

## Undo and space

Undo first peeks at the stack in O(1), then scans the active list for a reused ID and
an overlapping window in O(n). Only successful validation pops and inserts in O(1).
A refused undo keeps the original history entry. Total time is O(n).

Each linked structure owns one node per entry: O(n), O(w), and O(c) space.
and O(log r) recursion depth; it sorts a copy so stored resource order remains unchanged.
