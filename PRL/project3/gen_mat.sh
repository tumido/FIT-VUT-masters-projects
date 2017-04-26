#!/bin/bash

# $1.. filename
# $2.. dimension

echo $2 > $1
if [[ $1 == 'mat1' ]]; then
  rows=$2
  cols=$(($RANDOM % 10 + 2))
else # mat2
  cols=$2
  rows=$(($RANDOM % 10 + 2))
fi

for (( i = 0; i < $rows; i++ )); do
  for (( j = 0; j < $cols; j++ )); do
    [ $(($j+1)) -lt $cols ] && spacer=' ' || spacer='\n'
    echo -ne "$RANDOM$spacer" >> $1
  done
done
echo -ne '\n' >> $1
