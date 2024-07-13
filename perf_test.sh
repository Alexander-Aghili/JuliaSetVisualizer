#!/bin/bash

# Create a function to run the julia_set_visualizer with given parameters
run_visualizer() {
    local c=$1
    local p=$2
    echo "Running: ./julia_set_visualizer -p $p -c $c" 
    ./julia_set_visualizer -p "$p" -c "$c" > /dev/null
}

export -f run_visualizer

# Use GNU parallel to run the commands in parallel
#parallel --verbose run_visualizer ::: {3..5096} ::: {1..5}
for i in {3..5096}; do
    for j in {1..5}; do
        run_visualizer $i $j
    done
done

