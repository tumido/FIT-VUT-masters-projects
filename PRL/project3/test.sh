#!/bin/bash

# Uncomment if a time measurement should be run
METRICS_TEST=true

# If running on Fedora, export PATH
cat /etc/*-release | grep -q "fedora" && PATH=$PATH:/usr/lib64/openmpi/bin/

# If running on CentOS add prefix
cat /etc/*-release | grep -q "centos" && PREFIX="--prefix /usr/local/share/OpenMPI"

# Compile
if [ "$METRICS_TEST" = true ]; then DEFINE="-DMETRICS"; fi
mpic++ $PREFIX $DEFINE -o mm mm.cpp -std=c++0x

# Prepare and run
if [ "$METRICS_TEST" = true ]; then
  # Set up test environment
  RUNS=10
  DIMENSION_1=$1
  DIMENSION_2=$2

  for (( i = 0; i < $RUNS; i++ )); do
    bash gen_mat.sh mat1 $DIMENSION_1
    bash gen_mat.sh mat2 $DIMENSION_2
    avg="$avg + `mpirun $PREFIX -np $(($DIMENSION_1*$DIMENSION_2)) mm`"
  done
  awk "BEGIN{print ($avg)/$RUNS}"
else
  # Compute amount of used processors
  mat1=$(head -n1 mat1)
  mat2=$(head -n1 mat2)
  cpus=$((mat1*mat2))

  # Run
  mpirun $PREFIX -np $cpus mm
fi

# Cleanup
rm -f mm
