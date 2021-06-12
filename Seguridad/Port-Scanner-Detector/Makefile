all: detector

detector:
	gcc -c -IIP2Location-C-Library-master detector.c
	gcc -Wall -o detector detector.c -L. -lpcap -lIP2Location

clean:
	@echo "Cleaning up..."
	@rm detector detector.o report.txt