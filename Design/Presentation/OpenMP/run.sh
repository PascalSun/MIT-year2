gcc-6 -fopenmp openmp.c 
echo "Run with openmp"
STARTTIME=$(date +%s)
./a.out
ENDTIME=$(date +%s)
timez=$(( $ENDTIME - $STARTTIME ))
gcc-6 openmp.c -o openmp
echo "Run without openmp"
STARTTIME=$(date +%s)
./openmp
ENDTIME=$(date +%s)
timeo=$(( $ENDTIME - $STARTTIME ))
echo "It takes $timez seconds with openmp"
echo "It takes $timeo seconds without openmp"
