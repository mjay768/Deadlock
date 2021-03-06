CC = gcc
COMPILER_FLAGS =-g -std=gnu99
LINKER_FLAGS = -g -lpthread -lrt
BINARYOSS = oss
BINARYUSER = child
OBJSOSS = oss.o
OBJSUSER = child.o
HEADERS = deadlockmj.h data.h config.h
all: $(BINARYOSS) $(BINARYUSER)

$(BINARYOSS): $(OBJSOSS)
	$(CC) -o $(BINARYOSS) $(OBJSOSS) $(LINKER_FLAGS)

$(BINARYUSER): $(OBJSUSER)
	$(CC) -o $(BINARYUSER) $(OBJSUSER) $(LINKER_FLAGS)

%.o: %.c $(HEADERS)
	$(CC) $(COMPILER_FLAGS) -c $<

clean:
	/bin/rm $(OBJSOSS) $(OBJSUSER) $(BINARYOSS) $(BINARYUSER)
