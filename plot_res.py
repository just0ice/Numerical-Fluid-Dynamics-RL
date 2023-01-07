import numpy as np
import matplotlib.pyplot as plt
import matplotlib.gridspec as gridspec
import csv

def readfile(fname):
    data = []
    with open(fname) as file:
        reader = csv.reader(file, delimiter="\t")
        line = next(reader)
        dim = line
        for line in reader:
            data.append(np.asarray(line, dtype=float))
    return dim,data

dim,res = readfile("res.tsv")

res = res[0]
i = range(len(res))

plt.plot(i, res)
plt.yscale("log")
plt.savefig("res.pdf")
plt.show()