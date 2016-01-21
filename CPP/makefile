CPP_FLAG=-std=c++11 -pthread
CC=g++

all: $(patsubst %.cc, %.tsk, $(wildcard *.cc))

%.tsk:
	${CC} $(subst .tsk,.cc, $@) -o $@ ${CPP_FLAG}

clean: 
	rm -f *.tsk 
