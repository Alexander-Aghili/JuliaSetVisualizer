import os
import re
from collections import defaultdict
from glob import glob
import matplotlib.pyplot as plt


# Function to parse the content of each file
def parse_file(file_path):
    data = defaultdict(list)
    with open(file_path, 'r') as file:
        for line in file:
            match = re.match(r"\((\-?\d+\.\d+),\s*(\-?\d+\.\d+)\):\s*(\d+\.\d+)\s*ms\.", line)
            if match:
                coord = (float(match.group(1)), float(match.group(2)))
                time = float(match.group(3))
                data[coord].append(time)
    return data

# Directory containing the files
directory = 'perf2'

# Dictionary to hold the averaged data
average_times = defaultdict(lambda: defaultdict(list))

# Reading all files and aggregating the data
for thread_count in range(1, 105):
    for iteration in range(1, 6):
        file_path = os.path.join(directory, f'perf_{thread_count}_{iteration}.out')
        if os.path.isfile(file_path):
            data = parse_file(file_path)
            for coord, times in data.items():
                average_times[thread_count][coord].extend(times)

# Compute the averages
averaged_data = defaultdict(dict)
for thread_count, coords in average_times.items():
    for coord, times in coords.items():
        averaged_data[thread_count][coord] = sum(times) / len(times)

# Print the result (or process further as needed)
for thread_count, coords in averaged_data.items():
    print(f"Thread Count: {thread_count}")
    for coord, avg_time in coords.items():
        print(f"  Coordinate: {coord}, Average Time: {avg_time:.10f} ms")


# Calculate the sum of average times for all coordinates for each thread count
sum_of_averages = {thread_count: sum(times.values()) for thread_count, times in averaged_data.items()}

# Plot the results
x = list(sum_of_averages.keys())
y = list(sum_of_averages.values())

plt.figure(figsize=(10, 6))
plt.plot(x, y, marker='o')
plt.xlabel('Thread Count')
plt.ylabel('Sum of Average Times (ms)')
plt.title('Sum of Average Times for All Coordinates vs. Thread Count')
plt.grid(True)
plt.show()
