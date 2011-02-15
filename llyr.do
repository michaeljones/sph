DEPS="src/main.o src/Simulator.o"

HEADER_DEPS="src/*.hh"

redo-ifchange $DEPS $HEADER_DEPS

g++ -o $3 $DEPS

