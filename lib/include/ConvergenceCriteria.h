#ifndef CONVERGENCE_H
#define CONVERGENCE_H

#include <iostream>
#include <vector>
using namespace std;

class ConvergenceCriteria
{
public:
	ConvergenceCriteria(vector<double> temperatureField, vector<double> oldTemperatureField, double tolerance); //first type of convergence criteria
	ConvergenceCriteria(vector<double> temperatureField, vector<double> oldTemperatureField, double maxTemperature, double minTemperature, double tolerance); //second type of convergence criteria
	ConvergenceCriteria(vector<double> temperatureField, vector<double> oldTemperatureField, int numberOfNodes, bool quadraditc, double tolerance); //third and fourth type of convergence criteria
	ConvergenceCriteria(vector<double> temperatureField, vector<double> oldTemperatureField, double maxTemperature, double minTemperature, int numberOfNodes double tolerance); //second type of convergence criteria
	

	bool doesItConverged();
	double getMaximumError();
	vector<double> getErrorInEveryPosition();


private:
	vector<double> errorEveryPosition;
	double maximumError;
	double tolerance;
};

#endif