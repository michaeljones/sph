
DEPS=`cpp -MM libv.cc -I/home/mike/projects/tools/include/OpenEXR | sed 's/.*.o: //g' | sed 's/\\\//g'`

redo-ifchange $DEPS

g++ -shared -fPIC -g -o $3 libv.cc -I../src -I/home/mike/projects/tools/include/OpenEXR -L../lib -lllyr -lglut -lGL -lGLU

