import numpy as np
import matplotlib.pyplot as plt
import matplotlib.gridspec as gridspec
from matplotlib import colors
import csv
from matplotlib.colors import Normalize

def readfile(fname):
    data = []
    with open(fname) as file:
        reader = csv.reader(file, delimiter="\t")
        line = next(reader)
        dim = line
        for line in reader:
            data.append(np.asarray(line, dtype=float))
    return dim,data

# plotting parameters set by problem
y_factor = 1
broken_streamlines = True
density=[0.5, 1]


problem = 4
if problem == 0:
    folder = ""
if problem == 1:
    folder = "Lid-Driven Cavity/"
if problem == 2:
    print("Step")
    folder = "Step/"
    broken_streamlines = False
    plt.xlim(5, 20) 
    y_factor = 3
if problem == 3:
    folder = "Evangelion/"
if problem == 4:
    print("Disc")
    folder = "Disc/"
    broken_streamlines = False

# read the grid data from file into a 1d array. first index time step. second index grid idex
dim,U_1d = readfile(folder+"Ucc.tsv")
dim,V_1d = readfile(folder+"Vcc.tsv")
dim,P_1d = readfile(folder+"Pcc.tsv")
dim,flag_1d = readfile(folder+"flag.tsv")



imax = int(dim[0])
jmax = int(dim[1])
xlength = float(dim[2])
ylength = float(dim[3])*y_factor
print("imax = ",imax,", jmax = ",jmax,", xlength = ",xlength,", ylength = ",ylength)

# set up the grid and convert 2d arrays to 1d array for time step t_i
t_i = len(U_1d)-1
print("t_i = "+str(t_i))

# problem: he doesnt know how to interpret u date. make U to mesh
U = np.zeros((jmax,imax))
V = np.zeros((jmax,imax))
P = np.zeros((jmax,imax))
flag = np.zeros((jmax,imax))
#temp = np.zeros((jmax,imax))


for j in range(0,jmax):
    for i in range(0,imax):
        U[j][i] = U_1d[t_i][i+imax*j]
        V[j][i] = V_1d[t_i][i+imax*j]
        P[j][i] = P_1d[t_i][i+imax*j]
        flag[j][i] = flag_1d[t_i][i+imax*j]
        #temp[j][i] = temp_1d[t_i][i+imax*j]
    
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
#print("U")
#print(U)
#print("V")
#print(V)
#fig = plt.figure(frameon=False)
if speed.max() != 0:
    lw = 5*speed / speed.max()
else: 
    lw = 0

extent = (0, xlength, 0, ylength)
mask = (flag != 0.).astype(float)
U = np.ma.array(U, mask=mask)
V = np.ma.array(V, mask=mask)
P = np.ma.array(P, mask=mask)

# normalize p because of wrong solutions
pmin = np.amin(P)
print("pmin =", pmin)
P = P - pmin

#temp = np.ma.array(temp)
#plt.streamplot(X, Y, U, V, density=0.6, color='k', linewidth=lw)
if problem == 4:
    streamlines = 20
    y_seed = np.linspace(1,3,streamlines)
    x_seed = dx*np.ones(streamlines)
    seed_points = np.array([x_seed,y_seed])
    #seed_points = np.array([[dx/2], [0.5]])
    plt.streamplot(X, Y, U, V, density=density, broken_streamlines=broken_streamlines, start_points = seed_points.T)
else:
    plt.streamplot(X, Y, U, V, density=density, broken_streamlines=broken_streamlines)
#plt.streamplot(X, Y, U, V, density=[0.5, 1], broken_streamlines=False)

plt.imshow(P, extent = extent, origin="lower", cmap="Blues")
plt.colorbar(label=r"$P$")


#
# make a color map of fixed colors
border = (flag >= 2.).astype(float)
cmap = colors.ListedColormap(['black'])
bounds=[0,1]
norm = colors.BoundaryNorm(bounds, cmap.N)

# Create a mask
plt.imshow(border, alpha=border, extent=extent, origin="lower", cmap=cmap, norm=norm)

plt.savefig("plot.pdf")
plt.show()

