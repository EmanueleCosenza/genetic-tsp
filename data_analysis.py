import numpy as np
from matplotlib import pyplot as plt
import sys
import math


# Read data
data = np.genfromtxt('test_data.csv')

runs = np.where((data[:,0]==0))
runs = runs[0].shape[0]

# Get sequential times and compute minimum
seq = data[0:runs, :]
best_seq = np.min(seq, axis=0)[1]

max_nw = int(data[-1, 0])

# Number of rows per parallel experiment
par_rows = runs*(1+math.floor(math.log2(max_nw)))

# Get parallel data
par_th = data[runs:runs+par_rows, :]
par_th_block = data[runs+par_rows:runs+2*par_rows, :]
par_ff = data[runs+2*par_rows:runs+3*par_rows, :]

# Get parallel times
par_th_times = []
par_th_block_times = []
par_ff_times = []
for i in range(0, par_rows, runs):
    par_th_times.append(np.mean(par_th[i:i+runs, :], axis=0)[1])
    par_th_block_times.append(np.mean(par_th_block[i:i+runs, :], axis=0)[1])
    par_ff_times.append(np.mean(par_ff[i:i+runs, :], axis=0)[1])

nws = np.unique(par_th[:, 0])

print()
print("--- C++ threads parallel version with active wait barrier ---")

speedups = best_seq / par_th_times
scal = par_th_times[0] / par_th_times

print("Workers:\t", end="")
for a in nws:
    print("%d\t" % a, end="")
print()
print("Speedup:\t", end="")
for a in speedups:
    print("%.2f\t" % a, end="")
print()
print("Scalability:\t", end="")
for a in scal:
    print("%.2f\t" % a, end="")
print()
print()

# Speedup and scalability plot
plt.plot(nws, speedups, marker='.', label="Speedup")
plt.plot(nws, scal, marker='.', label="Scalability")
plt.plot(np.linspace(0, max_nw, max_nw), np.linspace(0, max_nw, max_nw),
         '--', label="Ideal")
plt.xlabel('Parallelism degree')
plt.title("C++ threads (active wait barrier) version")
plt.xticks(nws)
plt.xlim(0)
plt.ylim(0)
plt.legend()
plt.show()

print("--- C++ threads parallel version with blocking barrier ---")

speedups = best_seq / par_th_block_times
scal = par_th_block_times[0] / par_th_block_times

print("Workers:\t", end="")
for a in nws:
    print("%d\t" % a, end="")
print()
print("Speedup:\t", end="")
for a in speedups:
    print("%.2f\t" % a, end="")
print()
print("Scalability:\t", end="")
for a in scal:
    print("%.2f\t" % a, end="")
print()
print()

plt.plot(nws, speedups, marker='.', label="Speedup")
plt.plot(nws, scal, marker='.', label="Scalability")
plt.plot(np.linspace(0, max_nw, max_nw), np.linspace(0, max_nw, max_nw),
         '--', label="Ideal")
plt.xlabel('Parallelism degree')
plt.title("C++ threads (blocking barrier) version")
plt.xticks(nws)
plt.xlim(0)
plt.ylim(0)
plt.legend()
plt.show()

print("--- FastFlow parallel version with blocking barrier ---")

speedups = best_seq / par_ff_times
scal = par_ff_times[0] / par_ff_times

print("Workers:\t", end="")
for a in nws:
    print("%d\t" % a, end="")
print()
print("Speedup:\t", end="")
for a in speedups:
    print("%.2f\t" % a, end="")
print()
print("Scalability:\t", end="")
for a in scal:
    print("%.2f\t" % a, end="")
print()
print()

plt.plot(nws, speedups, marker='.', label="Speedup")
plt.plot(nws, scal, marker='.', label="Scalability")
plt.plot(np.linspace(0, max_nw, max_nw), np.linspace(0, max_nw, max_nw),
         '--', label="Ideal")
plt.xlabel('Parallelism degree')
plt.title("FastFlow version")
plt.xticks(nws)
plt.xlim(0)
plt.ylim(0)
plt.legend()
plt.show()
