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

# for Ucc the left and for Vcc the bottom row is not in the Array because of averaging over a staggered grid!
dim,U_1d = readfile("Ucc.tsv")
dim,V_1d = readfile("Vcc.tsv")
dim,P_1d = readfile("P.tsv")

imax = int(dim[0])
jmax = int(dim[1])
xlength = float(dim[2])
ylength = float(dim[3])

Y, X = np.mgrid[0:(int(dim[0])+3), 0:(int(dim[1])+3)]

# problem: he doesnt know how to interpret u date. make U to mesh
# U = -1 - X**2 + Y
# V = 1 + X - Y**2
#speed = np.sqrt(np.square(U))

#fig = plt.figure(figsize=(7, 7))
#gs = gridspec.GridSpec(nrows=1, ncols=1, height_ratios=[1])


#  Varying line width along a streamline
#ax2 = fig.add_subplot(gs[0, 0])
#lw = 5*speed / speed.max()
#ax2.streamplot(X, Y, U, V, density=0.6, color='k', linewidth=lw)
#ax2.set_title('Varying Line Width')


#plt.tight_layout()
#plt.show()