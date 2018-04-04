#include <ConvergenceCriteria.h>

ConvergenceCriteria::ConvergenceCriteria(vector<double> tempField, vector<double> oldTempField, double tol)
{
	this -> tolerance = tol;
	this -> errorEveryPosition.resize(tempField.size());

	for (int i = 0; i < tempField.size(); ++i)
	{
		this -> errorEveryPosition = abs(tempField[i] - oldTempField[i]);
	}
	double maxError = *max_element(errorsVec.begin(),errorsVec.end());
	this -> maximumError = maxError;
}

ConvergenceCriteria::ConvergenceCriteria(vector<double> tempField, vector<double> oldTempField, double maxTemp, double minTemp, double tol)
{
	this -> tolerance = tol;
	this -> errorEveryPosition.resize(tempField.size());

	double deltaTemperature = maxTemp - minTemp;
	for (int i = 0; i < tempField.size(); ++i)
	{
		this -> errorEveryPosition = (abs(tempField[i] - oldTempField[i]))/deltaTemperature;
	}
	double maxError = *max_element(errorsVec.begin(),errorsVec.end());
	this -> maximumError = maxError;
}

ConvergenceCriteria::ConvergenceCriteria(vector<double> tempField, vector<double> oldTempField, int numbNod, bool quadratic, double tol)
{
	this -> tolerance = tol;
	

	if(quadratic
	{
		double sum = 0.0;
		for (int i = 0; i < tempField.size(); ++i)
		{
			double difference = tempField[i] - oldTempField[i];
			sum += difference*difference;
		}
		sum = sum/numbNod;
		sum = sqrt(sum);
		this -> maximumError = sum;
	} else
	{
		double sum = 0.0;
		for (int i = 0; i < tempField.size(); ++i)
		{
			double difference = tempField[i] - oldTempField[i];
			sum += abs(difference);
		}
		sum = sum/numbNod;
		this -> maximumError = sum;
	}

}

ConvergenceCriteria::ConvergenceCriteria(vector<double> tempField, vector<double> oldTempField, double maxTemp, double minTemp, int, numbNod, double tol)
{
	this -> tolerance = tol;
	
	double deltaTemperature;
	double sum = 0.0;
	for (int i = 0; i < tempField.size(); ++i)
	{
		double difference = tempField[i] - oldTempField[i];
		sum += abs(difference)/deltaTemperature;
	}
	sum = sum/numbNod;
	this -> maximumError = sum;
}

bool ConvergenceCriteria::doesItConverged()
{
	bool convergence = false
	if (this -> maximumError <= this -> tolerance)
		convergence = true;
	return convergence;
}

double ConvergenceCriteria::getMaximumError()
{
	return this -> maximumError;
}
