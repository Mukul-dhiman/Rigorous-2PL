#! /bin/bash

i=0
while read line
do
    array[ $i ]="$line"        
    (( i++ ))
done < <(ls input)

g++ main.cpp -pthread -o main
prefix="in"
suffix=""
for i in "${array[@]}"
do
    j=`perl -pe "s/${prefix}((?:(?!(${suffix})).)*)${suffix}/\1/" <<< $i`
	./main input/$i output/out$j
    
done