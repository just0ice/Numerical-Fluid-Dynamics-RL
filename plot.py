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

# read the grid data from file into a 1d array. first index time step. second index grid idex
dim,U_1d = readfile("Ucc.tsv")
dim,V_1d = readfile("Vcc.tsv")
dim,P_1d = readfile("Pcc.tsv")

imax = int(dim[0])
jmax = int(dim[1])
xlength = float(dim[2])
ylength = float(dim[3])

# set up the grid and convert 2d arrays to 1d array for time step t_i
print("t_i = "+str(len(U_1d)-1))
t_i = len(U_1d)-1

# problem: he doesnt know how to interpret u date. make U to mesh
U = np.zeros((jmax,imax))
V = np.zeros((jmax,imax))
P = np.zeros((jmax,imax))

for j in range(0,jmax):
    for i in range(0,imax):
        U[j][i] = U_1d[t_i][i+jmax*j]
        V[j][i] = V_1d[t_i][i+jmax*j]
        P[j][i] = P_1d[t_i][i+jmax*j]
    

speed = np.sqrt(np.square(U) + np.square(V))

#fig = plt.figure(figsize=(7, 7))
#gs = gridspec.GridSpec(nrows=jmax, ncols=imax)


#  Varying line width along a streamline
#ax2 = fig.add_subplot(gs[0, 0])
dx = xlength/imax
dy = ylength/jmax
X, Y = np.meshgrid(dx/2 + np.arange(0,xlength,dx), dy/2 + np.arange(0,ylength,dy))
#X, Y = np.meshgrid(np.arange(0,1,1/imax), np.arange(0,1,1/jmax)) # square grid
#X, Y = np.mgrid[0:xlength:((imax)*1j), 0:ylength:((jmax)*1j)]
#print(Y)
fig = plt.figure(frameon=False)
if speed.max() != 0:
    lw = 5*speed / speed.max()
else: 
    lw = 0

extent = (0, xlength, 0, ylength)
plt.streamplot(X, Y, U, V, density=0.6, color='k', linewidth=lw)
plt.imshow(P, extent = extent, origin="lower", cmap="Blues")
#ax2.set_title('Varying Line Width')


#plt.tight_layout()
plt.colorbar(label=r"$P$")
plt.savefig("plot.pdf")
plt.show()