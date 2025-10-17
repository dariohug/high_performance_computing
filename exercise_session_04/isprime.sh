#!/bin/bash

# Safety checks 
if [ $# -ne 1 ]; then
    echo "Usage: $0 <number>"
    exit 1
fi

num="$1"

# Check if it's a number
if ! [[ "$num" =~ ^[0-9]+$ ]]; then
    if [[ "$num" =~ ^- ]]; then
        echo "ERROR: The given input is not a positive integer: $num"
    else
        echo "ERROR: The given input is not a number: $num"
    fi
    exit 1
fi

# Check positive
if [ "$num" -le 0 ]; then
    echo "ERROR: The given input is not a positive integer: $num"
    exit 1
fi

# Prime check using factor 
factors=$(factor "$num" | wc -w)

if [ "$factors" -eq 2 ]; then
    echo "$num is prime"
else
    echo "$num is composite"
fi
