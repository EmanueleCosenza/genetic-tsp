import numpy as np
from matplotlib import pyplot as plt
import matplotlib
import sys
import math
import argparse
import os
import itertools

parser = argparse.ArgumentParser()
parser.add_argument('dir', type=str)
parser.add_argument('--type', default='scalability')
parser.add_argument('--title', default='')
parser.add_argument('--latex', default=False, action='store_true')
args = parser.parse_args()

if args.latex:
    matplotlib.use("pgf")
    matplotlib.rcParams.update({
        "pgf.texsystem": "pdflatex",
        'font.family': 'serif',
        'text.usetex': True,
        'pgf.rcfonts': False,
    })

if args.type == 'speedup':
    path = os.path.join(args.dir, 'seq.csv')
    if not os.path.isfile(os.path.join(args.dir, 'seq.csv')):
        sys.exit('Missing seq.csv with sequential data.')

    seq = np.genfromtxt(path)
    best_seq = np.min(seq, axis=0)[1]

if args.title != '':
    plt.title(args.title)
else:
    if args.type == 'speedup':
        plt.title("Speedup")
    else:
        plt.title("Scalability")
plt.xlabel('Parallelism degree')

marker = itertools.cycle(('P', 'D', 'o', 'X'))
max_nw = 0

for filename in os.listdir(args.dir):
    if filename != 'seq.csv':
        print(filename)
        path = os.path.join(args.dir, filename)
        data = np.genfromtxt(path)
        nws = np.unique(data[:, 0])
        max_nw = int(max(nws[-1], max_nw))

        # Compute runs
        n = data[0, 0]
        for i in range(0, data.shape[0]):
            if data[i, 0] != n:
                break
        runs = i

        avg_times = []
        for i in range(0, data.shape[0], runs):
            avg_times.append(np.mean(data[i:i+runs, :], axis=0)[1])

        # Compute speedups or scalability
        if args.type == 'speedup':
            num = best_seq
        else:
            num = avg_times[0]
        y = num / avg_times

        if filename == 'blockbar.csv':
            label = 'Blocking barrier'
        elif filename == 'spinbar.csv':
            label = 'Spin barrier'
        elif filename == 'parfor.csv':
            label = 'ParallelFor'
        elif filename == 'pool.csv':
            label = 'poolEvolution'

        print("Workers:\n", end="")
        for a in nws:
            print("%d\t" % a, end="")
        print()
        print("Sp/sc:\n", end="")
        for a in y:
            print("%.2f\t" % a, end="")
        print()
        print()

        plt.plot(nws, y, marker=next(marker), label=label)
        plt.xticks(nws)



plt.plot(np.linspace(0, max_nw, max_nw), np.linspace(0, max_nw, max_nw),
         '--', label="Ideal")
plt.xlim(0)
plt.ylim(0)
plt.legend(fontsize = 'small')
plt.grid()
if args.latex:
    fig = matplotlib.pyplot.gcf()
    fig.set_size_inches(5.39, 4)
    plt.savefig('plot.pgf')
else:
    plt.show()
