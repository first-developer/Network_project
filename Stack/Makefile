#
# Makefile for virtual stack executable
#

OBJS = netether.o stack.o
LIBS = -L ../Arrays -L ../Events -L ../NetTAP -larrays -levents -ltap
CFLAGS += -I ../Arrays -I ../Events -I ../NetTAP

#
# Global target
#

all: stack

#
# Cleaning target
#

clean: 
	rm -f core *.o stack

#
# Executable target
#

stack: $(OBJS)
	$(CC) -o stack $(OBJS) $(LIBS)

stack.o: stack.c stack.h
netether.o: netether.c netether.h

