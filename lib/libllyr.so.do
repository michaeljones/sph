DEPS="../src/Simulator.o"

HEADER_DEPS="../src/*.hh"

redo-ifchange $DEPS $HEADER_DEPS

g++ -shared -o $3 $DEPS

