#include <iostream>
#include <string>
#include <iomanip>
#include <vector>
#include <info.h>
#include <ControlVolume.h>
#include <AnalyticalSolution.h>
#include <algorithm>
#include <petscksp.h>
#include <cmath>
using namespace std;

int main()
{
	PlainWallNonLinearInfo info;

	info.numberOfNodes = 5;
	info.wallLength = 0.1;
	info.tolerance = 1e-6;
	info.gridType = BOTH;
	info.convergenceCriteriaType = FIRST;
	info.beginBoundaryConditionType = PRESCRIBED_TEMPERATURE;
	info.endBoundaryConditionType = PRESCRIBED_TEMPERATURE;
	info.beginBoundaryConditionInfo.push_back(100);
	info.endBoundaryConditionInfo.push_back(20);
	info.interfaceOperation = EQUIVALENT_RESISTANCE;
	info.thermalConductionCoefficients.push_back(1.6);
	info.thermalConductionCoefficients.push_back(-0.01);

	AnalyticalSolution analytic(info);

	for (int i = 0; i < info.numberOfNodes; ++i)
	{
		info.analyticalSolution.push_back(analytic[i])
	}

	ControlVolume controlVolume(info);

		
}
