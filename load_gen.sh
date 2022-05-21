#!/bin/bash

left_limit=$1
right_limit=$2
step=$3

function usage {
    echo "Usage: <program> <left_limit> <step> <right_limit>"
    exit 1
}

if [ -z "$left_limit" ]; then
    usage
elif [ -z "$step" ]; then
    usage
elif [ -z "$right_limit" ]; then
    usage
fi  

pid=0
sleep_time=.1s
# Low to high processing times
for ms in $(seq "$left_limit" "$step" "$right_limit"); do
    echo "$pid $ms"
    pid=$((pid + 1))
done

# Hopefully the task queue empties here
sleep 10s

# High to low processing times
for ms in $(seq "$right_limit" "-$step" "$left_limit"); do
    echo "$pid $ms"
    pid=$((pid + 1))
done
