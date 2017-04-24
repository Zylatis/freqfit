#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_multimin.h>	

/////////////////////////////////////////////////////////////////////
// least squares cost function
double compute_SSR(const gsl_vector *v,  void * data){

	int fit_par_n = v->size;
	double SSR = 0.;
	
	double * local_dat = (double * ) data;
	int size = local_dat[0];
	vector<double> pars;
	pars.resize(fit_par_n);
	
	// fill par vector
	for(int c = 0; c<fit_par_n; c++){
		pars[c] =  gsl_vector_get(v, c);
	}

	for(int c = 0; c < size; c++){
		SSR = SSR + pow(fit_fn( c+1 , pars ) - local_dat[ c + 1 ], 2.) ;
	}	
	//~ cout<<SSR<<"\t"<< pars[0]<<"\t"<<pars[1]<<endl;
	//~ exit(0);
	return SSR;
	
}

/////////////////////////////////////////////////////////////////////
// fit function currently global - ideal to have in class but meh.
vector<double> do_fit( double * data, vector<double> v0 ){
	// Using non-rando Nelder-Mead without analytic Jacobian
	const gsl_multimin_fminimizer_type *T = gsl_multimin_fminimizer_nmsimplex2;
	gsl_multimin_fminimizer *s = NULL;
	gsl_vector *ss, *x;
	gsl_multimin_function minex_func;
	
	int nn = v0.size();
	size_t iter = 0;
	int status;
	double size;
	/* Starting point */
	x = gsl_vector_alloc(nn);
	ss = gsl_vector_alloc(nn);
	for(int c = 0; c<nn;c++){
		gsl_vector_set( x, c, v0[c] );	
		gsl_vector_set( ss, c, 10. );
	}

	
	
	//~ /* Initialize method and iterate */
	minex_func.n = nn;
	minex_func.f = compute_SSR;
	minex_func.params = data;
	
	s = gsl_multimin_fminimizer_alloc(T, nn);

	gsl_multimin_fminimizer_set(s, &minex_func, x, ss);

	do
	{
	  iter++;
	  status = gsl_multimin_fminimizer_iterate(s);
	  
	  if (status) 
		break;

	  size = gsl_multimin_fminimizer_size(s);
	  status = gsl_multimin_test_size(size, 1e-10);

	} while (status == GSL_CONTINUE && iter < pow(10.,3));
	
	
	vector<double> pars;
	pars.resize( nn + 1);
	pars[0] = s->fval;
	for(int c = 1; c<nn;c++){
		pars[c] = gsl_vector_get(s->x, c);
	}

	gsl_vector_free(x);
	gsl_vector_free(ss);
	gsl_multimin_fminimizer_free (s);

	return pars;
}
