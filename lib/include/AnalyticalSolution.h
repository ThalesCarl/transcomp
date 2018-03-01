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
#include <biot_solver.h>
using namespace std;

class AnalyticalSolution
{
public:
	AnalyticalSolution(PlainWallInfo data);
	AnalyticalSolution(DoublePlainWallInfo data);
	AnalyticalSolution(TransientPlainWallInfo data);

	void writeSolutionToCsv(string directory, string fileName);
	void printSolutionOnTheScreen();
	double operator[](int index);

private:
	vector<double> temperatureField;
	vector< vector<double>> transientTemperatureField;

	Mesh mesh;
	Boundary boundaries;
	ThermalConduction vectorK;
	double evalFirstProblemTemperatureLaw(double position);
	double evalSecondProblemTemperatureLaw(double position, DoublePlainWallInfo data);
	

	void addToTemperatureField(int controlVolumeIndex, double value);
	
};

#endif