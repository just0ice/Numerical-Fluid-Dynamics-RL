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

dim,res2 = readfile("res_2.tsv")
res2 = res2[0]
j = range(len(res2))
plt.plot(j, res2)

plt.yscale("log")
plt.xlabel("iteration")
plt.ylabel("residual")
plt.savefig("res.pdf")
plt.show()