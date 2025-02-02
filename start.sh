source .venv/bin/activate
cd sniffles/mitm
./mitmdump -s sniff.py > mtitmlog.txt &
