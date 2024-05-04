# Makefile for building embedded application.
# by Brian Fraser

# Edit this file to compile extra C files into their own programs.
TARGETS= updown
SRCS= io.c


PUBDIR = $(HOME)/shared
OUTDIR = $(PUBDIR)
CROSS_TOOL = arm-linux-gnueabihf-
CC_CPP = $(CROSS_TOOL)g++
CC_C = $(CROSS_TOOL)gcc

CFLAGS = -Wall -g -std=c99 -D _POSIX_C_SOURCE=200809L -Werror 
# -pg for supporting gprof profiling.
CFLAGS += -pg



# Convert the targets to full-path executables to delete.
OUTPUTS= $(addprefix $(OUTDIR)/, $(TARGETS))
OBJECTS=$(SRCS:.c=.o)


all: $(TARGETS)

updown: io.o
	$(CC_C) $(CFLAGS) $@.c $(OUTDIR)/$< -o $(OUTDIR)/$@ -lpthread

$(OBJECTS): %.o: %.c
	$(CC_C) $(CFLAGS) -c $< -o $(OUTDIR)/$@

clean:
	rm -f $(OUTPUTS)
