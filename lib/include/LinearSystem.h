#ifndef LINEAR_SYSTEM_H
#define LINEAR_SYSTEM_H

#include <iostream>
#include <vector>
#include <petscksp.h>
using namespace std;
using vector_double_2d = vector<vector<double>>;

class LinearSystem
{
public:
	//Constructor
	LinearSystem(int numberOfNodes);
	LinearSystem(void){};

	//get functions
	double operator[](const int index);
	//void solve();

	void setValueToMatrix(int row, int col, double value);
	void setValueToVector(int index, double Value);

	void solve();
	void solveTDMA();

	void printValues();
	void printSolution();
	
	
private:
	vector_double_2d matrixOfCoeficients;
	vector<double> independentValues;
	vector<double> solution;
	int matrixOrder;

	void allocIndependentValuesVector(int vectorSize);
	void allocSolutionVector(int vectorSize);
	void allocMatrixOfCoeficients(int matrixSize);
};

#endif