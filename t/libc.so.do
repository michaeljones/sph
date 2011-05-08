
DEPS=`cpp -MM libc.cc -I../src -I/home/mike/projects/tools/include/OpenEXR | sed 's/.*.o: //g' | sed 's/\\\//g'`

redo-ifchange $DEPS

g++ -shared -fPIC -Wall -g -o $3 libc.cc -I../src -I/home/mike/projects/tools/include/OpenEXR -L../lib -lllyr

