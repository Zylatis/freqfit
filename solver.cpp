#include <iostream>                                                             
#include <fstream>    
#include <ctime>
#include <numeric>
#include <vector>
#include <string>
#include <math.h>
#include <sstream>
#include <set>
#include <sys/time.h>
#include <complex>
#include <cstdlib>
#include <functional>
#include <iomanip>

using namespace std;
double fit_fn( double c, vector<double> pars ){
	double dx = .1;
	double out  = sin(pars[0]*c*dx) + sin(pars[1]*c*dx );// + sin(pars[2]*c*dx ) + sin(pars[3]*c*dx );
	return out;
}

#include "nelder_fit.h"

/////////////////////////////////////////////////////////////////////////////////////
vector<double>  FileRead1D( string filename){
	vector<double> v;
    int i = 0;
    string line;
	double val;
    fstream in(filename.c_str());
	while (std::getline(in, line))
    {
		in >> val;
		v.push_back( val );
    }
    //~ cout<<" Done."<<endl;
    return v;
}

// pinched from intertubes
double fRand(double fMin, double fMax)
{
    double f = (double)rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}

int main (){
	vector<double> time_series = FileRead1D( "data.dat" );
	int n = time_series.size();
	double *fit_data = new double [ n + 1 ]; 
		
	fit_data[0] = n;
	for(int c = 1; c<n+1; c++){
		fit_data[c] = time_series[c-1];
	}
	
	//////////////////////////////////////////
	// parameter search space stuff
	//////////////////////////////////////////
	
	
	vector<double> min, max;
	int n_pars = 2;
	int n_runs = 1;
	vector< vector< double > > par_table;
	vector<double> row;
	row.resize( n_runs );
	par_table.resize( n_pars + 1 );
	for(int c = 0; c< n_pars; c++){
		par_table[c] = row;
	}
	
	min.resize( n_pars );
	max.resize( n_pars );
	
	srand(time(NULL));
	fill(min.begin(), min.end(), 0.);
	fill(max.begin(), max.end(), .4);
	
	// initializer loop
	for(int i = 0; i < n_runs; i++){
		//~ cout<<"Start "<<i;
		vector<double> init;
		init.resize( n_pars );
		for(int c = 0; c < n_pars; c++ ){
			init[c] = fRand(min[c],max[c]);
		}
		
		vector<double> pars = do_fit( fit_data, init );
		cout<<pars[0]<<endl;
		//~ for(int c = 0; c < n_pars; c++ ){
			//~ par_table[c][i] = pars[c];
		//~ }
		cout<<" - done"<<endl;
		//~ cout<<pars[0]<<"\t"<<pars[1]<<endl;
	}
	
	//////////////////////////////////////////
	// do fitting and report pars
	//////////////////////////////////////////
	
	cout<<"-------------"<<endl;
	//~ cout<<"Pars:"<<endl;
	//~ for(int i = 0; i < n_runs; i++){
		//~ cout<<par_table[0][i]<<"\t"<<par_table[1][i]<<endl;
	//~ }
	
	

return 0;
}
