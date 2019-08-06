//
// Created by Robert Stolz on 7/21/19.
//

#ifndef RLOOPER2_RLOOP_DYNAMIC_MODEL_H
#define RLOOPER2_RLOOP_DYNAMIC_MODEL_H

#include "model.h"
#include <vector>


class Rloop_dynamic_model : public Model{
protected:
    //model parameters
    //equilibrium energetics parameters
    int N; //experimentally determined length of the (-) sc domain adter the transcription machinery
    double A; //turns/bp of the B-form double helix
    double C; //tortional stiffness of ssDNA winding. (Could be 3.6 for ds or 1.8 for ss winding)
    double k; //Hooke's law coefficient: (2200*ideal_gas_constant in kcal/mol*absolute_temp_in_kelvin)/N
    double T; //temparature
    double a; //Neucleation Free Energy (junction energy) in Kcals (~3-10.2kCals) 5000
    double sigma; //measurement of energy upstream of replication domain. Moved to up the Model object.
    double alpha; //linking difference: topological parameter. refers to the percent of twists difference.
    //energy difference for the described RNA/DNA dinucleotide pairs (RNA/DNA - DNA/DNA) in kcal/mol as described in Huppert et. al. 2008
    //note, energies are for the RNA (non-template/sense DNA) strand in the 5' to 3' direction
    double rGG_dCC;
    double rGC_dCG;
    double rGA_dCT;
    double rGU_dCA;
    double rCG_dGC;
    double rCC_dGG;
    double rCA_dGT;
    double rCU_dGA;
    double rAG_dTC;
    double rAC_dTG;
    double rAA_dTT;
    double rAU_dTA;
    double rUG_dAC;
    double rUC_dAG;
    double rUA_dAT;
    double rUU_dAA;
    bool homopolymer_override;
    bool unconstrained;
    double override_energy;

    //dynamic model specific parameters
    char* raw_sequence;
    double current_bp_energy;
    double current_junction_energy;
    double current_superhelical_energy;
    double total_energy;
    double proposed_bp_energy;
    double proposed_junction_energy;
    double proposed_superhelical_energy;
    int n_rloops;
    int current_pos;

public:
    //constructors
    Rloop_dynamic_model();
    //need a special constructor that lets your specify some or all of these parameters

    bool in_rloop;

    //getters and setters
    int getN() const;
    double getA() const;
    double getC() const;
    double getK() const;
    double getT() const;
    double geta() const;
    double getSigma() const;
    double getAlpha() const;
    int getCurrentPos() const;
    void setN(int N);
    void setA(double A);
    void setC(double C);
    void setK(double k);
    void setT(double T);
    void seta(double a);
    void set_superhelicity(double sigma);
    void set_unconstrained(bool value);
    void setAlpha(double alpha);
    void set_bp_energy_override(double energy);

    //member functions
    void reset_model();
    void step_forward_initiation(int n);
    void step_forward_elongation(int n);

    int find_distance(vector<char>& sequence,const vector<char>::iterator& first, const vector<char>::iterator& second, Structure& structure);
    double step_forward_bps(const vector<char>::iterator& first, const vector<char>::iterator& second);
    double compute_bps_interval(const char &first, const char &second);
    void compute_structure(vector<char>& sequence, const std::vector<char>::iterator &start, const std::vector<char>::iterator &stop, Structure& structure);
    void compute_external_structure(Structure& structure, Structure& rloop, Peak& external);
    void compute_residuals(Structure& structure);
    void ground_state_residuals(double& twist, double& writhe);
    long double ground_state_factor();
    long double ground_state_energy();
};


#endif //RLOOPER2_RLOOP_DYNAMIC_MODEL_H