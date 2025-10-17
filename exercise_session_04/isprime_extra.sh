#!/bin/bash

if [ $# -eq 0 ]; then
    echo "Usage: $0 <num1> [num2 num3 ...]"
    exit 1
fi

while [ $# -gt 0 ]; do
    num="$1"

    # Safety checks
    if ! [[ "$num" =~ ^[0-9]+$ ]]; then
        if [[ "$num" =~ ^- ]]; then
            echo "ERROR: The given input is not a positive integer: $num"
        else
            echo "ERROR: The given input is not a number: $num"
        fi
        shift
        continue
    fi

    if [ "$num" -le 0 ]; then
        echo "ERROR: The given input is not a positive integer: $num"
        shift
        continue
    fi

    # Prime check using factor 
    factors=$(factor "$num" | wc -w)

    if [ "$factors" -eq 2 ]; then
        echo "$num is prime"
    else
        echo "$num is composite"
    fi

    shift
done

