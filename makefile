# `make all` for generating binary, `make clean` for deleting.

CC     := g++
CFLAGS := -pthread

OS     := $(shell uname -s)
LIBS   := 
ifeq ($(OS),Linux)
	LIBS += -pthread
endif

SRCS   := main.cpp \

OBJS   := ${SRCS:cpp=o}
PROGS  := ${SRCS:.cpp=}

.PHONY: all
all: ${PROGS}

${PROGS} : % : %.o Makefile
	${CC} $< -o $@ ${LIBS}

clean:
	rm -f ${PROGS} ${OBJS}

%.o: %.c Makefile
	${CC} ${CFLAGS} -c $<
