"""End-to-end CLI checks; uses Python's standard library only."""
import subprocess
import re
import os

def run(lines, catalog=None):
    args = [os.environ.get('RESERVATION_BINARY', './reservation_system')] + ([catalog] if catalog else [])
    result = subprocess.run(args, input='\n'.join(map(str, lines))+'\n', text=True,
                            capture_output=True, timeout=10, check=True)
    return result.stdout

def book(id, student='S1', resource=101, start='09:00', end='10:00', date='2026-10-11'):
    return [3,id,student,resource,date,start,end]

checks = 0

def check(name, condition):
    global checks
    assert condition, name
    checks += 1
    print('PASS:', name)

out=run([11,101,11,999,12,13,0])
check('load resources and search hit/miss', 'Loaded 6 resource(s)' in out and 'No resource with ID 999' in out)
section=out.split('Resources sorted by name (merge sort):')[1].split('==========================================')[0]
ids=re.findall(r'^([0-9]+)\s', section, re.M)
check('merge sort integrated in menu', ids == ['201','202','301','302','101','102'])
check('empty reports', 'No requests recorded.' in out and 'Total active: 0; total waiting: 0' in out)
out=run(book(1)+[4,1,8,9,5,13,0])
check('cancel and undo restore', 'Reservation restored to the active list.' in out and 'Total active: 1; total waiting: 0' in out)
check('undo does not double count demand', re.search(r'^101\s+Willis Library Study Room A\s+1\s+0\s+1$',out,re.M))
out=run(book(1)+book(2,'S2')+['y']+book(3,'S3')+['y',4,1,9,13,0])
check('FIFO promotion', 'student S2 was promoted' in out and 'student S3 was promoted' not in out)
check('blocked undo preserves stack', 'It stays on the cancellation history.' in out)
check('report utilization/waiting/demand', re.search(r'^101\s+Willis Library Study Room A\s+1\s+1\s+3$',out,re.M))
out=run(book(1)+book(2,'S2')+['y',10,99,10,1,7,0])
check('waiting removal and invalid position', 'No waiting request at that position.' in out and 'Removed waiting student S2.' in out and 'The waiting list is empty.' in out)
out=run(book(1)+book(2,'S2',start='10:00',end='11:00')+[5,0])
check('back-to-back reservations accepted', 'Active reservations (2 total)' in out)
out=run(['garbage',3,'bad',1,'', 'S1',999,101,'2026-02-31','2026-10-11','24:00','09:00','08:00','09:00','10:00',0])
check('invalid menu/integer/student/resource/date/time/window', all(x in out for x in ['not a valid option','positive whole number','must not be empty','No resource with ID 999','Invalid date','Invalid time','start time must be earlier','Reservation created.']))
out=run([3,1,'cancel',9,0])
check('cancel prompt and empty undo', 'nothing to undo' in out and 'Reservation created.' not in out)
check('missing file handled', 'Continuing with an empty resource catalog' in run([3,0],'data/no_such_file.txt'))
check('malformed file handled', 'skipped' in run([1,0],'data/resources_malformed.txt'))
check('EOF at prompt exits', 'Reservation created.' not in run([3,1]))
out=run(book(1)+[4,1]+book(1,'S2',resource=102)+[9,0])
check('reused reservation ID blocks undo', 'has been reused by an active reservation' in out)
print(f'{checks} integration checks passed.')
