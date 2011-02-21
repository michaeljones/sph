
DEPS=`cpp -MM $1.cc -I/home/mike/projects/tools/include/OpenEXR | sed 's/.*.o: //g' | sed 's/\\\//g'`

redo-ifchange $DEPS

g++ -g -o $3 viewer-bin.cc -L../lib -I../src -I/home/mike/projects/tools/include/OpenEXR -lllyr -lglut -lGL -lGLU

