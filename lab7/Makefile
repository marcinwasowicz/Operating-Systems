CreatorV:
	gcc Creator.c -o Creator
WorkersV:
	gcc Worker1.c -o Worker1
	gcc Worker2.c -o Worker2
	gcc Worker3.c -o Worker3
SystemV:
	make CreatorV
	make WorkersV
CreatorPOSIX:
	gcc CreatorPOSIX.c -o cposix -lpthread -lrt
WorkersPOSIX:
	gcc WorkerPOSIX1.c -o wposix1 -lpthread -lrt
	gcc WorkerPOSIX2.c -o wposix2 -lpthread -lrt
	gcc WorkerPOSIX3.c -o wposix3 -lpthread -lrt
POSIX:
	make CreatorPOSIX
	make WorkersPOSIX
cleanV:
	rm Creator
	rm Worker1
	rm Worker2
	rm Worker3
cleanPOSIX:
	rm cposix
	rm wposix1
	rm wposix2
	rm wposix3
