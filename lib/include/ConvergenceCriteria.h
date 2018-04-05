#ifndef CONVERGENCE_H
#define CONVERGENCE_H

#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
using namespace std;

class ConvergenceCriteria
{
public:
	ConvergenceCriteria(vector<double> temperatureField, vector<double> oldTemperatureField); //first type of convergence criteria
	ConvergenceCriteria(vector<double> temperatureField, vector<double> oldTemperatureField, double maxTemperature, double minTemperature); //second type of convergence criteria
	ConvergenceCriteria(vector<double> temperatureField, vector<double> oldTemperatureField, int numberOfNodes, bool quadratic); //third and fourth type of convergence criteria
	ConvergenceCriteria(vector<double> temperatureField, vector<double> oldTemperatureField, double maxTemperature, double minTemperature, int numberOfNodes); //second type of convergence criteria
	

	bool doesItConverged(double tolerance);
	double getMaximumError();
	vector<double> getErrorInEveryPosition();


private:
	vector<double> errorEveryPosition;
	double maximumError;
	
};

#endif