BIN_FILES  = server	client

CC = gcc


CCGLAGS =	-Wall  -g

LDFLAGS = -L$(INSTALL_PATH)/lib/
LDLIBS = -lpthread 


all: CFLAGS=$(CCGLAGS)
all: $(BIN_FILES)
.PHONY : all

client:
	javac client.java

server: server.o lines.o operations.o
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@ -l sqlite3 -Ilib/sqlite

%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $<

clean:
	@echo "Cleaning up..."
	rm -f $(BIN_FILES) *.o
	rm -f $(BIN_FILES) *.class

.SUFFIXES:
.PHONY : clean
