#! /bin/bash

make openmp
# Run the program without openmp and mpi, the source file is openmp.c
echo "Run without openmp and mpi"
STARTTIME=$(date +%s)
./openmp
ENDTIME=$(date +%s)
timez=$(( $ENDTIME - $STARTTIME ))

# Run the program with openmp, the source file is openmp.c and the excute file is openmps
echo "Run with openmp"

gcc-6  -fopenmp openmp.c -o openmps
export OMP_NUM_THREADS=12
STARTTIME=$(date +%s)
./openmps
ENDTIME=$(date +%s)
time0=$(( $ENDTIME - $STARTTIME ))


#Run the program with MPI & OPENMP, the source file is mpi.c
echo "Run with mpi"

# Generate the excute file mpi
mpicc -fopenmp -o mpi mpi.c 

#syscCluster

# Then run the outcome with different number of codes, this piece can be modified with different number of
# nodes by modifying mpirun -np * --hostfile host mpi
echo "Run with 2 nodes"
STARTTIME=$(date +%s)
mpiexec -np 3  mpi
ENDTIME=$(date +%s)
time2=$(( $ENDTIME - $STARTTIME ))


echo "Run with 4 nodes"
STARTTIME=$(date +%s)
mpirun -np 4  mpi
ENDTIME=$(date +%s)
time3=$(( $ENDTIME - $STARTTIME ))

echo "Run with 8 nodes"
STARTTIME=$(date +%s)
mpirun -np 8 mpi
ENDTIME=$(date +%s)
time4=$(( $ENDTIME - $STARTTIME ))

echo "Run with 16 nodes"
STARTTIME=$(date +%s)
mpirun -np 16 mpi
ENDTIME=$(date +%s)
time5=$(( $ENDTIME - $STARTTIME ))

echo "Run with 28 nodes"
STARTTIME=$(date +%s)
mpirun -np 28  mpi
ENDTIME=$(date +%s)
time6=$(( $ENDTIME - $STARTTIME ))


#echo the time spent to do the job

echo "It takes $timez seconds without mpi and openmp"
echo "It takes $time0 seconds with openmp"

echo "It takes $time2 seconds with 2 nodes"

echo "It takes $time3 seconds with 4 nodes"
echo "It takes $time4 seconds with 8 nodes"
echo "It takes $time5 seconds with 16 nodes"
echo "It takes $time6 seconds with 28 nodes"

# output the time into a html with google chart, so we can see a graph when we open Graph.html with web browser.
cat > Graph.html <<EOF
<html>
  <head>
    <script type="text/javascript" src="https://www.gstatic.com/charts/loader.js"></script>
    <script type="text/javascript">
      google.charts.load('current', {'packages':['bar']});
      google.charts.setOnLoadCallback(drawChart);
      function drawChart() {
        var data = google.visualization.arrayToDataTable([
          ['Config', 'Time'],


          ['Without', $timez],
          ['Openmp', $time0],
          ['MPI-2', $time2],
          ['MPI-4', $time3],
          ['MPI-8', $time4],
          ['MPI-16', $time5],
          ['MPI-28', $time6]

 ]);
        var options = {
          chart: {
            title: 'Time spent to calculate with different configure',
           
          }
        };

        var chart = new google.charts.Bar(document.getElementById('columnchart_material'));

        chart.draw(data, options);
      }
    </script>
  </head>
  <body>
    <div id="columnchart_material" style="width: 900px; height: 500px;"></div>
  </body>
</html>
EOF




