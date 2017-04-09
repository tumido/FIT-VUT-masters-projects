#!/bin/sh
# Define source filename
NAME="es"
# Show Metrics?
# DEFINE="-DMETRICS" # Comment out if metrics should be disabled
SAMPLES=100

# Pass me numbers count as $1
[ $# -lt 1 ] && num=10 || num=$1

# If running on Fedora, export PATH
cat /etc/*-release | grep -q "fedora" && PATH=$PATH:/usr/lib64/openmpi/bin/

# If running on CentOS add prefix
cat /etc/*-release | grep -q "centos" && PREFIX="--prefix /usr/local/share/OpenMPI"

# Compile
mpic++ $PREFIX $DEFINE -o $NAME $NAME.cpp

# Create input file
dd if=/dev/random bs=1 count=$num of=numbers status=none

# Run
if [ -z $DEFINE ]; then
  mpirun $PREFIX -np $(($num + 1)) $NAME
else
  metrics=''
  for ((i=0; i<$SAMPLES; i++)); do
    metrics+=$(mpirun $PREFIX -np $(($num + 1)) $NAME)"\n"
  done
  echo -e -n $metrics | awk 'BEGIN{x=1000}{if ($0<x) x=$0}END{print x}'
fi

# Cleanup
rm -f $NAME numbers
