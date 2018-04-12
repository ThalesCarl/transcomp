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
	TransientPlainWallInfo info;

	info.numberOfNodes = 7;
	info.gridType = BOTH;
	info.wallLength = 0.4;
	info.thermalConduction = 0.7;
	info.density = 2000;
	info.cp = 700;
	info.biotNumber = 2.05;
	info.timeStep = 800;
	info.initialTemperature = 20;
	info.transversalArea = 1;
	info.beginBoundaryConditionType = PRESCRIBED_FLUX;
	info.endBoundaryConditionType = CONVECTION;
	info.beginBoundaryConditionInfo.push_back(0);
	info.endBoundaryConditionInfo.push_back(3.5875);
	info.endBoundaryConditionInfo.push_back(100);
	info.interfaceOperation = EQUIVALENT_RESISTANCE;

	AnalyticalSolution analyt(info);

	ControlVolume control(info);
		
	PetscFinalize();

		
}
