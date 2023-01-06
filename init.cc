// READ_PARAMETER, COMP_DELT(), INIT_UVP
#include "grid.h"



int grid::READ_PARAMETER(string inputfile = "settings.in"){
    // read data from settings.in . every value is expected to be in one row in the following order:
    // xlength, ylength, imax, jmax, t_end, tau, itermax, eps, omg, gamma, Re, GX, GY, UI, VI, PI, wW, wE, wN, wS 
    // as in 3.3.2 p. 42

    string line;
    std::ifstream file;
    vector<string> data;

    file.open (inputfile, ios::out);
    while (getline (file, line)){
        if (line[0] != '#')
            data.push_back(line);
    }
    file.close();

    // write data to class variables
    xlength = stod(data[0]);
    ylength = stod(data[1]);
    imax = stoul(data[2]);
    jmax = stoul(data[3]);
    t_end = stod(data[4]);
    tau = stod(data[5]);
    itermax = stoul(data[6]);
    eps = stod(data[7]);
    omg = stod(data[8]);
    gamma = stod(data[9]);
    Re = stod(data[10]);
    GX = stod(data[11]);
    GY = stod(data[12]);
    UI = stod(data[13]);
    VI = stod(data[14]);
    PI = stod(data[15]);
    wW = stoul(data[16]);
    wE = stoul(data[17]);
    wN = stoul(data[18]);
    wS = stoul(data[19]);

    cout << "Data read from " << inputfile << endl;
    cout << "xlength " << xlength << ", ylength " << ylength << ", imax " << imax << ", jmax " << jmax << endl;
    cout << "t_end " << t_end << ", tau " << tau << ", itermax " << itermax << ", eps " << eps << ", omg " << omg << endl;
    cout << "gamma " << gamma << ", Re " << Re << ", GX " << GX << ", GY " << GY << ", UI " << UI << endl;
    cout << "VI " << VI << ", PI " << PI << ", wW " << wW << ", wE " << wE << ", wN " << wN << ", wS " << wS << endl;


    // compute delx, dely
    // physical grid spacing according to 3.1.1 p. 22. ghost zones are outside xlength x ylength!
    delx = xlength / imax;
    dely = xlength / jmax;
    cout << "delx = " << delx << ", dely = " << dely << endl;

    return 0;
}

void grid::INIT_UVP(){
    // taking a 1D vector. Pseudo 2D conversion is i = x + width*y, x = i % width; y = i / width.
    // 1 block width boundary. Whole array dimension [0,imax+1]x[0,jmax+1]. Without boundary [1,imax]x[1,jmax]

    U = vector<double>((imax + 2)*(jmax + 2),UI); 
    V = vector<double>((imax + 2)*(jmax + 2),VI);
    P = vector<double>((imax + 2)*(jmax + 2),PI);

    // Spatial derivatives
    // Initialize at same size as UVP although not all elements will be used. This is to keep indexing consistent with UVP
    // This might very will kill my memory
    du2_dx = vector<double>((imax + 2)*(jmax + 2),0); 
    duv_dy = vector<double>((imax + 2)*(jmax + 2),0); 
    d2u_dx2 = vector<double>((imax + 2)*(jmax + 2),0); 
    d2u_dy2 = vector<double>((imax + 2)*(jmax + 2),0); 
    dp_dx = vector<double>((imax + 2)*(jmax + 2),0); 

    duv_dx = vector<double>((imax + 2)*(jmax + 2),0); 
    dv2_dy = vector<double>((imax + 2)*(jmax + 2),0); 
    d2v_dx2 = vector<double>((imax + 2)*(jmax + 2),0); 
    d2v_dy2 = vector<double>((imax + 2)*(jmax + 2),0); 
    dp_dy = vector<double>((imax + 2)*(jmax + 2),0); 

    F = vector<double>((imax + 2)*(jmax + 2),0); 
    G = vector<double>((imax + 2)*(jmax + 2),0); 

    RHS = vector<double>((imax + 2)*(jmax + 2),0); 
}   

double abs_max(vector<double> X){
    double max = 0;
    for (auto i = X.cbegin(); i != X.cend(); ++i){
        if (abs(*i) > max) max = abs(*i); 
    }

    return max;
}

void grid::COMP_DELT(){
    // according to 3.50
    double Umax = abs_max(U);
    double Vmax = abs_max(V);

    //cout << "Umax = " << Umax << ", Vmax = " << Vmax << endl;
    if (Umax == 0){
        if (Vmax == 0)
            delt = tau * Re/2 * 1/( 1/pow(delx,2) + 1/pow(dely, 2) );
        else
            delt = tau * min( Re/2 * 1/( 1/pow(delx,2) + 1/pow(dely, 2) ), dely/Vmax );
    } 
    else {
        if (Vmax == 0)
            delt = tau * min( Re/2 * 1/( 1/pow(delx,2) + 1/pow(dely, 2) ), delx/Umax );
        else
            delt = tau * min( {Re/2 * 1/( 1/pow(delx,2) + 1/pow(dely, 2) ), delx/Umax , dely/Vmax});
    }

    //delt = min(delx, dely);
    //cout << "delt = " << delt << endl;

    // check if delt negative
    if (delt < 0)
        cout << "WARNING! delt is smaller than 0!" << endl;

}