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
	{
		PlainWallNonLinearInfo info;

		info.numberOfNodes = 4;
		info.wallLength = 0.1;
		info.tolerance = 1e-3;
		info.gridType = BOTH;
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
	}

	{
		PlainWallNonLinearInfo infoSecondGraph;


		infoSecondGraph.wallLength = 0.1;
		infoSecondGraph.tolerance = 1e-10;
		infoSecondGraph.gridType = BOTH;
		infoSecondGraph.beginBoundaryConditionType = PRESCRIBED_TEMPERATURE;
		infoSecondGraph.endBoundaryConditionType = PRESCRIBED_TEMPERATURE;
		infoSecondGraph.beginBoundaryConditionInfo.push_back(100);
		infoSecondGraph.endBoundaryConditionInfo.push_back(20);
		infoSecondGraph.thermalConductionCoefficients.push_back(1.6);
		infoSecondGraph.thermalConductionCoefficients.push_back(-0.01);

		
		ofstream pFile;
		pFile.open("../results/second_task/counter.csv",fstream::trunc);
		pFile << "numberOfNodes, counter_equivalent_resistance, counter_linear_interpolation" << endl;
		for (int i = 0; i < 6; ++i)
		{
			infoSecondGraph.numberOfNodes = 5*pow(2,i);
			pFile << infoSecondGraph.numberOfNodes << ", ";

			infoSecondGraph.interfaceOperation = EQUIVALENT_RESISTANCE;
			ControlVolume controlVolumeEquiv(infoSecondGraph);
			pFile << controlVolumeEquiv.getIterationCounter() << ", ";


			infoSecondGraph.interfaceOperation = LINEAR_INTERPOLATION;
			ControlVolume controlVolumeLinear(infoSecondGraph);
			pFile << controlVolumeLinear.getIterationCounter() << "\n";
		}

		pFile.close();
		cout << "Counter saved in ../results/second_task/counter.csv" << endl;
	}


	
	PetscFinalize();

		
}
