ServerV:
	gcc Server.c -o ServerV
ClientV:
	gcc Client.c -o ClientV
ServerPOSIX:
	gcc ServerPOSIX.c -o sposix -lrt
ClientPOSIX:
	gcc ClientPOSIX.c -o cposix -lrt
V:
	make ServerV
	make ClientV
POSIX:
	make ServerPOSIX
	make ClientPOSIX
cleanV:
	rm ServerV
	rm ClientV
cleanPOSIX:
	rm sposix
	rm cposix
all:
	make allV
	make allPOSIX
clean:
	make cleanV
	make cleanPOSIX