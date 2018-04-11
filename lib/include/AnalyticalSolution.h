#ifndef ANALYTICAL_H
#define ANALYTICAL_H

#include <iostream>
#include <vector>
#include <string>
#include <Mesh.h>
#include <Boundary.h>
#include <fstream>
#include <iomanip>
#include <ThermalConduction.h>
#include <petscksp.h>
#include <petscsnes.h>
using namespace std;

class AnalyticalSolution
{
public:
	AnalyticalSolution(PlainWallInfo data);
	AnalyticalSolution(DoublePlainWallInfo data);
	AnalyticalSolution(TransientPlainWallInfo data);
	AnalyticalSolution(PlainWallNonLinearInfo data);

	void writeSolutionToCsv(string directory, string fileName);
	void printSolutionOnTheScreen();
	void printTransientSolutionOnTheScreen();
	double operator[](int index);

private:
	vector<double> temperatureField;
	vector< vector<double>> transientTemperatureField;

	Mesh mesh;
	Boundary boundaries;
	ThermalConduction vectorK;
	double evalFirstProblemTemperatureLaw(double position);
	double evalSecondProblemTemperatureLaw(double position, DoublePlainWallInfo data);
	double evalNonLinearTemperatureLaw(double position);
	

	void addToTemperatureField(int controlVolumeIndex, double value);
	void getZetaNumbers(double biotNumber, vector<double> &zetaNumbers);

	double calculateFunction(double x, double biotNumber);
	double calculateDerivate(double x);
	PetscErrorCode function(SNES snes, Vec x, Vec f, void *ctx);
	PetscErrorCode dfunction(SNES snes, Vec x, Mat A, Mat B, void* ctx);
	double solve(double initialGuess);
	
			
};

#endif
