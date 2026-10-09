"""Milestone 1 CLI tests using Python's standard library."""
import subprocess
import os

def run(lines, catalog=None):
    args = [os.environ.get('RESERVATION_BINARY', './reservation_system')]
    if catalog:
        args.append(catalog)
    result = subprocess.run(args, input='\n'.join(map(str, lines))+'\n',
                            text=True, capture_output=True, timeout=10, check=True)
    return result.stdout

def book(id, student='S1', resource=101, start='09:00', end='10:00'):
    return [3,id,student,resource,'2026-10-11',start,end]

checks = 0

def check(name, condition):
    global checks
    assert condition, name
    checks += 1
    print('PASS:', name)

out=run([1,0])
check('resource file loading and display', 'Loaded 6 resource(s)' in out and 'Willis Library Study Room A' in out)
out=run(book(1)+[2,101,'2026-10-11','09:00','10:00',5,0])
check('create, display, and occupied availability', 'NOT AVAILABLE' in out and 'Active reservations (1 total)' in out)
out=run([2,101,'2026-10-11','09:00','10:00',0])
check('free availability', 'AVAILABLE - no active reservation' in out)
out=run(book(1)+[4,1,8,9,5,0])
check('cancellation history and successful undo', 'Reservation restored to the active list.' in out and 'Active reservations (1 total)' in out)
out=run(book(1)+book(2,'S2')+['y']+book(3,'S3')+['y',4,1,7,9,0])
check('FIFO waiting promotion', 'student S2 was promoted' in out and 'student S3 was promoted' not in out and 'S3' in out)
check('blocked undo preserves history', 'It stays on the cancellation history.' in out)
out=run(book(1)+book(2,'S2')+['y',10,99,10,1,7,0])
check('waiting removal and invalid position', 'No waiting request at that position.' in out and 'Removed waiting student S2.' in out and 'The waiting list is empty.' in out)
out=run(book(1)+book(2,'S2',start='10:00',end='11:00')+[5,0])
check('back-to-back reservations', 'Active reservations (2 total)' in out)
out=run(['bad',3,'bad',1,'','S1',999,101,'2026-02-31','2026-10-11','24:00','09:00','08:00','09:00','10:00',0])
check('invalid input handled', all(x in out for x in ['not a valid option','positive whole number','must not be empty','No resource with ID 999','Invalid date','Invalid time','start time must be earlier','Reservation created.']))
check('missing file handled', 'Continuing with an empty resource catalog' in run([3,0],'data/no_such_file.txt'))
check('malformed input file handled', 'skipped' in run([1,0],'data/resources_malformed.txt'))
check('cancel and empty undo', 'nothing to undo' in run([3,1,'cancel',9,0]))
check('EOF at prompt exits', 'Reservation created.' not in run([3,1]))
out=run(book(1)+[4,1]+book(1,'S2',resource=102)+[9,0])
check('reused ID blocks undo', 'has been reused by an active reservation' in out)
print(f'{checks} integration checks passed.')
