
# Calculate dependencies
DEPS=`cpp -MM $1.cc -I/home/mike/projects/tools/include/OpenEXR | sed 's/.*.o: //g' | sed 's/\\\//g'`

redo-ifchange $DEPS

g++ -g -fPIC -Wall -c -o $3 $1.cc -I/home/mike/projects/tools/include/OpenEXR

