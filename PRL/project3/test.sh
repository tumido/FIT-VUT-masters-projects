#!/bin/bash

# Compute amount of used processors
mat1=$(head -n1 mat1)
mat2=$(head -n1 mat2)

cpus=$((mat1*mat2))

# If running on Fedora, export PATH
cat /etc/*-release | grep -q "fedora" && PATH=$PATH:/usr/lib64/openmpi/bin/

# If running on CentOS add prefix
cat /etc/*-release | grep -q "centos" && PREFIX="--prefix /usr/local/share/OpenMPI"

# Compile and run
mpic++ $PREFIX $DEFINE -o mm mm.cpp -std=c++0x
mpirun $PREFIX -np $cpus mm
rm -f mm
