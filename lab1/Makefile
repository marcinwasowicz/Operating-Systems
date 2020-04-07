CC=gcc
CFLAGS= -Wall

staticLibraries:
	$(CC) -c $(CFLAGS) DiffOperationsLibrary.c -o DiffOperationsLibrary.o 
	ar rcs libDiff.a DiffOperationsLibrary.o
	$(CC) $(CFLAGS) SmallTests.c -static -L. libDiff.a -o Small -O${op}
	$(CC) $(CFLAGS) MediumTests.c -static -L. libDiff.a -o Medium -O${op}
	$(CC) $(CFLAGS) LargeTests.c -static -L. libDiff.a -o Large -O${op}
sharedLibraries:
	$(CC) -fPIC -shared DiffOperationsLibrary.c -o libDiff.so
	$(CC) $(CFLAGS) SmallTests.c  -L. -Wl,-rpath=. libDiff.so -o Small -O${op}
	$(CC) $(CFLAGS) MediumTests.c  -L. -Wl,-rpath=. libDiff.so -o Medium -O${op}
	$(CC) $(CFLAGS) LargeTests.c  -L. -Wl,-rpath=. libDiff.so -o Large -O${op}
dynamicLibraries:
	$(CC) -fPIC -shared DiffOperationsLibrary.c -o libDiff.so
	$(CC) $(CFLAGS) SmallTestsDynamic.c -ldl -o Small -O${op}
	$(CC) $(CFLAGS) MediumTestsDynamic.c -ldl -o Medium -O${op}
	$(CC) $(CFLAGS) LargeTestsDynamic.c -ldl -o Large -O${op}
clean:
	rm -f *.o
	rm -f *.a
	rm -f *.so
	rm -f Small
	rm -f Medium
	rm -f Large
zadanieStatic:
	make clean
	make staticLibraries
	make test
	make clean
zadanieShared:
	make clean
	make sharedLibraries
	make test
	make clean
zadanieDynamic:
	make clean
	make dynamicLibraries
	make test
	make clean
Static_0:
	make zadanieStatic op=0
Static_1:
	make zadanieStatic op=1
Static_2:
	make zadanieStatic op=2
Static_3:
	make zadanieStatic op=3
Static_s:
	make zadanieStatic op=s
Shared_0:
	make zadanieShared op=0
Shared_1:
	make zadanieShared op=1
Shared_2:
	make zadanieShared op=2
Shared_3:
	make zadanieShared op=3
Shared_s:
	make zadanieShared op=s
Dynamic_0:
	make zadanieDynamic op=0
Dynamic_1:
	make zadanieDynamic op=1
Dynamic_2:
	make zadanieDynamic op=2
Dynamic_3:
	make zadanieDynamic op=3
Dynamic_s:
	make zadanieDynamic op=s
Static:
	make Static_0
	make Static_1
	make Static_2
	make Static_3
	make Static_s
Shared:
	make Shared_0
	make Shared_1
	make Shared_2
	make Shared_3
	make Shared_s
Dynamic:
	make Dynamic_0
	make Dynamic_1
	make Dynamic_2
	make Dynamic_3
	make Dynamic_s
Whole:
	make Static
	make Shared
	make Dynamic
test:
	./Small  
	./Medium  
	./Large 
