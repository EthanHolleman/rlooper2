//
// Created by Robert Stolz on 10/5/17.
//

#include <iostream>
#include "simulation.h"
#include <string.h>

using namespace std;

int main(int argc, char* argv[]) {

    //initialize new simulation
    Simulation sim;
    Rloop_equilibrium_model model;
    //print usage statement
    if (argc == 1){
        cout << "Usage: expected_length sequence_file output_file superhelicity_lower_bound superhelicity_upper_bound --options" << endl;
        return 0;
    }
    //process command line arguments and set simulation parameters
    sim.set_infile(argv[1]);
    sim.set_outfile(argv[2]);
    float supercoiling_lower_bound = atof(argv[3]);
    float supercoiling_upper_bound = atof(argv[4]);
    for (int i=5; i<argc; i++) {
        if (!strcmp(argv[i], "--a")) {
            model.seta(atof(argv[i + 1]));
            i++;
        }
        else if  (!strcmp(argv[i], "--sigma")) {
            model.set_superhelicity(atof(argv[i+1]));
            i++;
        }
        else if (!strcmp(argv[i], "--N")) {
            if (!strcmp(argv[i+1],"auto")){
                sim.set_auto_domain_size(true);
            }
            else{
                model.setN(atoi(argv[i+1]));
            }
            i++;
        }
        else if (!strcmp(argv[i], "--minlength")) {
            sim.set_minlength(atoi(argv[i + 1]));
            i++;
        }
        else if (!strcmp(argv[i], "--reverse")) {
            sim.reverse_input();
        }
        else if (!strcmp(argv[i], "--complement")) {
            sim.complement_input();
        }
        else if (!strcmp(argv[i], "--invert")) {
            sim.complement_input();
            sim.reverse_input();
        }
        else if (!strcmp(argv[i], "--homopolymer")) {
            model.set_bp_energy_override(atof(argv[i+1]));
            i++;
        }
        else if (!strcmp(argv[i], "--top")) {
            sim.set_top(atoi(argv[i+1]));
            i++;
        }
        else{
            cout << "Unrecognized command line option: " << argv[i] << endl;
            exit(1);
        }
    }
    //run simulation
    sim.add_model(model);
    ofstream outfile(argv[2],ios::out);
    for (float superhelicity=supercoiling_lower_bound; superhelicity <= supercoiling_upper_bound+0.0001; superhelicity += 0.01){
        sim.simulation_C(superhelicity,outfile);
    }
    outfile.close();
    return 0;
}