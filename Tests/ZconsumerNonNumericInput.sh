./start 5
sleep 1
./producer 1 5 &
./producer 2 5 &
./producer 3 5 &
./consumer a &
./consumer 2 &
