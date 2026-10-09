# Push Milestone 1

Keep the full-project folder separately for Sunday. Use this Milestone 1 folder for today's
build and submission. Its menu is 0-10. It contains no SystemReports or merge sort.

Assigned file groups:
- Anish: Resource, ResourceManager, Reservation, ReservationList headers and source.
- Jacob: WaitingList and CancellationStack headers and source.
- Shared: main.cpp, Makefile, data, README, docs, tests, repository_url.txt.

Each student reviews, tests, and commits actual changes using their own account. Record
accurate implementation/review/testing work and assistant help in the contribution report.

Copy these files into the existing Git checkout on a new Milestone 1 branch, preserving
its .git directory. If the checkout has the final-project additions, leave those on their
separate branch; start today's branch from main instead. Run make test and check git diff
before committing. If final changes have not yet been committed, do not switch branches
until they have been saved separately or safely stashed.

Anish's staging group:

git add include/Resource.h src/Resource.cpp include/ResourceManager.h src/ResourceManager.cpp include/Reservation.h src/Reservation.cpp include/ReservationList.h src/ReservationList.cpp

Jacob's staging group:

git add include/WaitingList.h src/WaitingList.cpp include/CancellationStack.h src/CancellationStack.cpp

Commit shared integration/docs/tests once, after agreeing who handles them. Commit
messages should describe the actual changes made. Push each feature branch, open pull
requests into main, review and merge. Test the merged main on CELL. Update contributions
and CELL results, then create the submission ZIP from that same repository revision.

Do not claim the prepared reference implementations were independently authored by a
student merely because that student uploads them. Assigned responsibilities and actual
work should both be documented accurately.
