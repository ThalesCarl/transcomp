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
	PetscInitialize(NULL,NULL,NULL,NULL);

	PlainWallNonLinearInfo info;

	info.numberOfNodes = 5;
	info.wallLength = 0.1;
	info.tolerance = 1e-3;
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
	analytic.writeSolutionToCsv("../results/second_task", "analytic");	

	ControlVolume controlVolumeEquiv(info);
	controlVolumeEquiv.writeSolutionToCsv("../results/second_task", "equivalent");
	cout << "Equivalent counter = " << controlVolumeEquiv.getIterationCounter() << endl;

	info.interfaceOperation = LINEAR_INTERPOLATION;
	ControlVolume controlVolumeLinear(info);
	controlVolumeLinear.writeSolutionToCsv("../results/second_task", "linear");
	cout << "Linear Counter = " << controlVolumeLinear.getIterationCounter() << endl;

	PetscFinalize();

		
}
