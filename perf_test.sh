#!/bin/bash

# Create a function to run the julia_set_visualizer with given parameters
run_visualizer() {
    local c=$1
    local p=$2
    echo "Running: ./julia_set_visualizer -p $p -c $c" > 2
    ./julia_set_visualizer -p "$p" -c "$c" 
}

export -f run_visualizer

# Use GNU parallel to run the commands in parallel
parallel --verbose run_visualizer ::: {1..5096} ::: {1..30}
