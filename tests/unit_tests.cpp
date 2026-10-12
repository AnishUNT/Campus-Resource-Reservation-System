#include <cassert>
#include <fstream>
#include <iostream>
#include "ResourceManager.h"
#include "ReservationList.h"
#include "WaitingList.h"
#include "CancellationStack.h"
#include "SystemReports.h"

int main() {
    using namespace ReservationUtils;
    assert(isValidDateFormat("2028-02-29"));
    assert(!isValidDateFormat("2026-02-29"));
    assert(!isValidDateFormat("1900-02-29"));
    assert(isValidDateFormat("2000-02-29"));
    assert(!isValidDateFormat("2026-04-31"));
    assert(!isValidDateFormat("0000-01-01"));
    assert(isValidTimeFormat("23:59") && !isValidTimeFormat("24:00"));
    int value = 0;
    assert(!parseInt("999999999999999999999999999", value));
    assert(!parseInt("12abc", value));
    assert(parseInt(" 42 ", value) && value == 42);
    Reservation a(1,"A",101,"2026-10-11","09:00","10:00");
    Reservation b(2,"B",101,"2026-10-11","10:00","11:00");
    Reservation c(3,"C",101,"2026-10-11","09:30","10:30");
    ReservationList list;
    assert(!list.removeReservation(1));
    list.insertReservation(a); list.insertReservation(b); list.insertReservation(c);
    assert(!a.overlapsWith(b) && a.overlapsWith(c));
    assert(list.size()==3 && list.countForResource(101)==3);
    assert(list.findReservation(2) && !list.findReservation(99));
    assert(list.removeReservation(2)); // middle
    assert(list.removeReservation(3)); // tail
    assert(list.removeReservation(1)); // singleton
    list.insertReservation(b); assert(list.size()==1); list.clear();
    assert(list.isEmpty());
    WaitingList queue;
    WaitingRequest out;
    assert(!queue.dequeue(out) && !queue.peek(out) && !queue.removeAt(0,out));
    queue.enqueue(WaitingRequest("A",101,"2026-10-11","09:00","10:00"));
    queue.enqueue(WaitingRequest("B",102,"2026-10-11","09:00","10:00"));
    queue.enqueue(WaitingRequest("C",101,"2026-10-11","09:00","10:00"));
    assert(queue.countForResource(101)==2);
    assert(queue.removeAt(1,out) && out.studentId=="B");
    assert(queue.removeAt(1,out) && out.studentId=="C");
    queue.enqueue(WaitingRequest("D",101,"2026-10-11","09:00","10:00"));
    assert(queue.dequeue(out) && out.studentId=="A");
    assert(queue.dequeue(out) && out.studentId=="D");
    queue.enqueue(WaitingRequest("E",101,"2026-10-11","09:00","10:00"));
    assert(queue.peek(out) && out.studentId=="E" && queue.size()==1);
    queue.clear(); assert(queue.isEmpty());
    CancellationStack stack;
    Reservation removed;
    assert(!stack.pop(removed) && !stack.peek(removed));
    stack.push(a); stack.push(b);
    assert(stack.peek(removed) && removed.getReservationId()==2 && stack.size()==2);
    assert(stack.pop(removed) && removed.getReservationId()==2);
    assert(stack.pop(removed) && removed.getReservationId()==1);
    stack.push(c); stack.clear(); assert(stack.isEmpty());
    ResourceManager manager;
    assert(manager.loadFromFile("data/resources.txt") && manager.getResourceCount()==6);
    assert(manager.findResourceById(101) && !manager.findResourceById(999));
    auto sorted=manager.resourcesSortedByName();
    for (std::size_t i=1;i<sorted.size();++i) assert(sorted[i-1].getName()<=sorted[i].getName());
    assert(manager.getResources()[0].getResourceId()==101); // sort preserves catalog
    const char* fixture="build/sort_fixture.txt";
    { std::ofstream file(fixture); file << "3|Zulu|Lab|1\n2|Alpha|Lab|1\n1|Alpha|Lab|1\n4|Bad|Lab|1|\n"; }
    assert(manager.loadFromFile(fixture) && manager.getResourceCount()==3);
    sorted=manager.resourcesSortedByName();
    assert(sorted[0].getResourceId()==1 && sorted[1].getResourceId()==2 && sorted[2].getResourceId()==3);
    assert(manager.getSkippedRecordCount()==1);
    assert(!manager.loadFromFile("data/does_not_exist.txt"));
    assert(manager.resourcesSortedByName().empty());
    SystemReports reports;
    assert(reports.requestCount(101)==0);
    reports.recordRequest(101); reports.recordRequest(101); reports.recordRequest(102);
    assert(reports.requestCount(101)==2 && reports.requestCount(102)==1);
    std::cout << "Unit tests passed.\n";
}
