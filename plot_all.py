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

for problem in ["Cavity1","Cavity2","Cavity5","Cavity10","Cavity","Evangelion1","Evangelion20","Disc1","Disc20","Disc100"]:
    print(problem)

    y_factor = 1
    broken_streamlines = True
    density=[0.5, 1]
    cbar_location = "right"

    folder = problem + "/"
    if problem == "":
        folder = ""
    if "Cavity" in problem:
        plt.figure(figsize=(16, 16), dpi=100)
        broken_streamlines = True
        cbar_location = "right"
    if problem == "Cavity":
        folder = "Cavity/"
    if problem == "Cavity1":
        folder = "Cavity/1/"
    if problem == "Cavity2":
        folder = "Cavity/2/"
    if problem == "Cavity5":
        folder = "Cavity/5/"
    if problem == "Cavity10":
        folder = "Cavity/10/"
    if "Step" in problem:
        folder = "Step/"
        density=[4, 0.25]
        broken_streamlines = False
        cbar_location = "bottom"
        plt.figure(figsize=(32, 8), dpi=100)
        plt.xlim(5, 20) 
        #y_factor = 3
    if "Disc" in problem:
        broken_streamlines = False
        cbar_location = "bottom"
        plt.figure(figsize=(32, 8), dpi=100)
    if problem == "Disc1":
        folder = "Disc/1.000000/"
    if problem == "Disc20":
        folder = "Disc/20.000000/"
    if problem == "Disc100":
        folder = "Disc/100.000000/"
    
    if "Evangelion" in problem:
        density=[0.5, 1]
        cbar_location = "right"
        broken_streamlines = True
    
    if problem == "Evangelion1":
        folder = "Evangelion/1.000000/"
    if problem == "Evangelion20":
        folder = "Evangelion/20.000000/"
    if problem == "Evangelion100":
        folder = "Evangelion/100.000000/"

    # read the grid data from file into a 1d array. first index time step. second index grid idex
    dim,U_1d = readfile(folder+"Ucc.tsv")
    dim,V_1d = readfile(folder+"Vcc.tsv")
    dim,P_1d = readfile(folder+"Pcc.tsv")
    dim,flag_1d = readfile(folder+"flag.tsv")



    imax = int(dim[0])
    jmax = int(dim[1])
    xlength = float(dim[2])
    ylength = float(dim[3])#*y_factor
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
    if "Disc" in problem:
        streamlines = 10
        y_seed = np.linspace(1,3,streamlines)
        x_seed = dx*np.ones(streamlines)
        seed_points = np.array([x_seed,y_seed])
        #seed_points = np.array([[dx/2], [0.5]])
        plt.streamplot(X, Y, U, V, density=density, broken_streamlines=broken_streamlines, start_points = seed_points.T, color ="black")
    else:
        plt.streamplot(X, Y, U, V, density=density, broken_streamlines=broken_streamlines, color ="black")
    #plt.streamplot(X, Y, U, V, density=[0.5, 1], broken_streamlines=False)

    plt.imshow(P, extent = extent, origin="lower", cmap="Blues")
    if problem == "Cavity":
        cb = plt.colorbar(location=cbar_location, shrink = 0.8)
        cb.set_label(label=r"$P$",weight='bold', size=15)
    if "Evangelion" in problem:
        cb = plt.colorbar(location=cbar_location)
        cb.set_label(label=r"$P$",weight='bold', size=40)
        cb.ax.tick_params(labelsize=40)


    #
    # make a color map of fixed colors
    border = (flag >= 2.).astype(float)
    cmap = colors.ListedColormap(['black'])
    bounds=[0,1]
    norm = colors.BoundaryNorm(bounds, cmap.N)

    # Create a mask
    plt.imshow(mask, alpha=mask, extent=extent, origin="lower", cmap=colors.ListedColormap(['lightgrey']), norm=norm)
    plt.imshow(border, alpha=border, extent=extent, origin="lower", cmap=cmap, norm=norm)
    
    #plt.axis('off')
    if problem != "Cavity":
        plt.tick_params(axis='both', which='both', bottom=False, top=False, labelbottom=False, right=False, left=False, labelleft=False)
    plt.savefig("plots/"+problem+".pdf", bbox_inches='tight')
    plt.clf()
    print("\n")

