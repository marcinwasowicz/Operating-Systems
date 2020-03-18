CC=gcc
CFLAGS= -Wall

task1library:
	$(CC) -fPIC -shared QuickSortLibrary.c -o qsortlib.so
task1program:
	$(CC) $(CFLAGS) task1.c  -L. -Wl,-rpath=. qsortlib.so -o task1
cleanTask1:
	rm *.so
	rm task1
Find:
	$(CC) $(CFLAGS) FindDirent.c -o findDirent
	$(CC) $(CFLAGS) FindNftw.c -o findNftw
cleanFind:
	rm findDirent
	rm findNftw
SystemTest:
	./task1 sys generate test.txt $(count) $(rlen)
	./task1 sys sort test.txt $(count) $(rlen)
	touch testCopy.txt 
	./task1 sys copy test.txt $(count) $(rlen) testCopy.txt
	make cleanAfterTest
LibraryTest:
	./task1 lib generate test.txt $(count) $(rlen)
	./task1 lib sort test.txt $(count) $(rlen) 
	touch testCopy.txt 
	./task1 lib copy test.txt $(count) $(rlen) testCopy.txt
	make cleanAfterTest
Test:
	make SystemTest count=$(count) rlen=$(rlen)
	make LibraryTest count=$(count) rlen=$(rlen)
TestList:
	make Test count=$(count) rlen=1
	make Test count=$(count) rlen=4
	make Test count=$(count) rlen=512
	make Test count=$(count) rlen=1024
	make Test count=$(count) rlen=4096
	make Test count=$(count) rlen=8192
FullTests:
	make TestList count=500
	make TestList count=1000
	make TestList count=2000
Task1:
	make task1library
	make task1program
	make FullTests
	make cleanTask1
cleanAfterTest:
	rm test.txt
	rm testCopy.txt
