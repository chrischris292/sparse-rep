//
//  main.cpp
//  Research
//
//  Created by Christopher Chan on 1/6/16.
//  Copyright © 2016 Christopher Chan. All rights reserved.
//
#include <iostream>
#include "Dependencies/cnpy.h"
#include"Voigt.hpp"
#include<complex>
#include "Testing.hpp"
#include "Dependencies/Eigen/Dense"


using namespace std;

class Sparse_Representation {
public:
    Sparse_Representation();
    void load_file();
    int area() {return 0;}//return width*height;}
    void prepareDictionary(double dz,int numGalaxy,int pdfSize,vector<vector<double> >& pdfs,vector<double>& z);
    void create_voigt_dict(vector<double> &zfine, tuple<double,double> mu, int Nmu, tuple<double,double> sigma, int Nsigma, int Nv);
private:
    string fname = "Data/CFHTLens_sample.P.npy";
    vector<vector<double> >pdfs;
    vector< double> linspace( double a,  double b, int n);
};
Sparse_Representation::Sparse_Representation(){
    //load_file();
}
void Sparse_Representation::load_file () {
    cnpy::NpyArray arr = cnpy::npy_load(fname);
    int numGalaxy = arr.shape[0]-1; //for redshift
    int redshiftpos = arr.shape[0];
    int pdfsize = arr.shape[1];
    double* loaded_data = reinterpret_cast<double*>(arr.data);
    int curr = 0;
    cout << loaded_data[201];
    for(int i = 0;i<numGalaxy;i++){
        vector<double> row;
        for(int i =0;i<pdfsize;i++){
            row.push_back(loaded_data[curr]);
            curr++;
        }
        pdfs.push_back(row);
    }
    vector<double> z;
    for(int i =0;i<pdfsize;i++){
        z.push_back(loaded_data[curr]);
        curr++;
    }
    double dz = z[1]-loaded_data[0];
    arr.destruct();
    prepareDictionary(dz, numGalaxy, pdfsize,pdfs,z);
    
}
void Sparse_Representation::prepareDictionary(double dz,int numGalaxy,int pdfSize,vector<vector<double> >& pdfs,vector<double>& z){
    //cout << dz;
    auto minelemz = std::min_element(std::begin(z),std::end(z));
    auto maxelemz = std::max_element(std::begin(z),std::end(z));
    tuple<double,double> mu = make_tuple(*minelemz,*maxelemz);
    int Nmu = 250;
    double max_sig = (*maxelemz-*minelemz)/12;
    double min_sig = dz/6;
    tuple<double,double> sig = make_tuple(min_sig,max_sig);
    int Nv = 3;
    int Nsig = 80;
    int Na = Nmu*Nsig*Nv;
    cout << "Nmu, Nsig, Nv = "<< "["<< Nmu<< ","<< Nsig<< ","<<Nv<< "]"<<endl;
    cout << "Total bases in dictionary: "<< Na<<endl;
    create_voigt_dict(z, mu, Nmu, sig, Nsig, Nv);
}
void Sparse_Representation::create_voigt_dict(vector<double>& zfine, tuple<double,double> mu, int Nmu, tuple<double,double> sigma, int Nsigma, int Nv){
    vector< double> zmid = linspace(get<0>(mu),get<1>(mu),Nmu);
    vector< double>sig = linspace(get<0>(sigma), get<1>(sigma), Nsigma);
    vector< double>gamma = linspace(0,.5,Nv);
    int NA = Nmu*Nsigma*Nv;
    int Npdf = zfine.size();
    

    
}

//Helper functions
//http://stackoverflow.com/questions/11734322/matlab-type-arrays-in-c
vector< double> Sparse_Representation::linspace( double a,  double b, int n) {
    vector< double> array;
     double step = (b-a) / (n-1);
    
    while(a <= b) {
        array.push_back(a);
        a += step;           // could recode to better handle rounding errors
    }
    return array;
}

vector<vector<double>> Sparse_Representation::zeroes(int r, int c){
    vector<vector<int> > matrix(r);
    for ( int i = 0 ; i < r ; i++ )
       matrix[i].resize(c);
}
vector<vector<double>> Sparse_representation::norm(){
    
}
int main () {
    Sparse_Representation init;
    init.load_file();
    Voigt temp;
    return 0;
}
