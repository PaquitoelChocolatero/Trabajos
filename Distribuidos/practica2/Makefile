BIN_FILES  = server

CC = gcc


CCGLAGS =	-Wall  -g

LDFLAGS = -L$(INSTALL_PATH)/lib/
LDLIBS = -lpthread


all: CFLAGS=$(CCGLAGS)
all: $(BIN_FILES)
.PHONY : all

server: server.o lines.o operations.o
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@ -l sqlite3

%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $<

clean:
	@echo "Cleaning up..."
	rm -f $(BIN_FILES) *.o

.SUFFIXES:
.PHONY : clean
