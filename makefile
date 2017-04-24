all:
	g++ -std=c++11  -O3 -o solver solver.cpp -fopenmp -lgsl -lgslcblas 

clean:
	$(RM) solver
