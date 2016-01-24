CPP_FLAG=-std=c++11 -pthread
CC=g++

#all: $(patsubst %.cc, %.tsk, $(wildcard *.cc))
#all: concurrency5.tsk

#%.tsk:
#	${CC} $(subst .tsk,.cc, $@) -o $@ ${CPP_FLAG}

concurrency7.tsk:
	${CC} concurrency7.cc -o $@ ${CPP_FLAG} -lboost_system -lboost_filesystem

#concurrency5.tsk:
#	${CC} concurrency5.cc -o $@ ${CPP_FLAG} 

clean: 
	rm -f *.tsk 
