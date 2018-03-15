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
		DoublePlainWallInfo infoWithout;

		infoWithout.numberOfNodes1 = 5;
		infoWithout.numberOfNodes2 = 3;
		infoWithout.wallLength1 = 0.083;
		infoWithout.wallLength2 = 0.027;
		infoWithout.gridType1 = WEST;
		infoWithout.gridType2 = EAST;
		infoWithout.thermalConduction1 = 10.0;
		infoWithout.thermalConduction2 = 1;
		infoWithout.beginBoundaryConditionType = PRESCRIBED_FLUX;
		infoWithout.endBoundaryConditionType = CONVECTION;
		infoWithout.beginBoundaryConditionInfo.push_back(5000.0);
		infoWithout.endBoundaryConditionInfo.push_back(70.0);
		infoWithout.endBoundaryConditionInfo.push_back(30);

		infoWithout.interfaceOperation = EQUIVALENT_RESISTANCE;
		ControlVolume contVolEquivResist(infoWithout);
		contVolEquivResist.writeSolutionToCsv("../results", "equivalent_resistance_VC");
		
		infoWithout.interfaceOperation = LINEAR_INTERPOLATION;
		ControlVolume  contVolLinInterp(infoWithout);
		contVolLinInterp.writeSolutionToCsv("../results", "interpolation_VC");

		AnalyticalSolution analytSol(infoWithout);
		analytSol.writeSolutionToCsv("../results", "AnalyticalSolution");

	}
	
	{
		DoublePlainWallInfo infoEveryPosition;

		infoEveryPosition.numberOfNodes1 = 5;
		infoEveryPosition.numberOfNodes2 = 3;
		infoEveryPosition.wallLength1 = 0.083;
		infoEveryPosition.wallLength2 = 0.027;
		infoEveryPosition.gridType1 = WEST;
		infoEveryPosition.gridType2 = EAST;
		infoEveryPosition.thermalConduction1 = 10.0;
		infoEveryPosition.thermalConduction2 = 1;
		infoEveryPosition.beginBoundaryConditionType = PRESCRIBED_FLUX;
		infoEveryPosition.endBoundaryConditionType = CONVECTION;
		infoEveryPosition.beginBoundaryConditionInfo.push_back(5000.0);
		infoEveryPosition.endBoundaryConditionInfo.push_back(70.0);
		infoEveryPosition.endBoundaryConditionInfo.push_back(30);

		infoEveryPosition.interfaceOperation = EQUIVALENT_RESISTANCE;
		ControlVolume contVolEquivResist(infoEveryPosition);

		infoEveryPosition.interfaceOperation = LINEAR_INTERPOLATION;
		ControlVolume contVolLinInterp(infoEveryPosition);

		AnalyticalSolution analytSol(infoEveryPosition);

		ofstream pFile;
		pFile.open("../results/error_in_every_position.csv",fstream::trunc);
		pFile << "position, error_equivalent_resistance, error_linear_interpolation" << endl;
		pFile << scientific;
		for (int i = 0; i < infoEveryPosition.numberOfNodes1 + infoEveryPosition.numberOfNodes2; i++)
		{
			double equivalentResistanceError = abs(contVolEquivResist.getTemperature(i) - analytSol[i]);
			double linearInterpolationError =  abs(contVolLinInterp.getTemperature(i) - analytSol[i]);
			pFile << contVolEquivResist.getPosition(i) << ", " << equivalentResistanceError << ", " << linearInterpolationError << endl;
		}
		pFile.close();
		cout << "Error for positions values saved in ../results/error_in_every_position.csv" << endl;
	}

	{
		DoublePlainWallInfo infoMaximumError;

		infoMaximumError.wallLength1 = 0.083;
		infoMaximumError.wallLength2 = 0.027;
		infoMaximumError.gridType1 = WEST;
		infoMaximumError.gridType2 = EAST;
		infoMaximumError.thermalConduction1 = 10.0;
		infoMaximumError.thermalConduction2 = 1;
		infoMaximumError.beginBoundaryConditionType = PRESCRIBED_FLUX;
		infoMaximumError.endBoundaryConditionType = CONVECTION;
		infoMaximumError.beginBoundaryConditionInfo.push_back(5000.0);
		infoMaximumError.endBoundaryConditionInfo.push_back(70.0);
		infoMaximumError.endBoundaryConditionInfo.push_back(30);

		ofstream toFile;
		toFile.open("../results/maximum_error.csv",fstream::trunc);
		toFile << "numberOfNodes, maximum_error_equivalent_resistance, maximum_error_linear_interpolation" << endl;

		for (int i = 0; i < 6; ++i)
		{
			infoMaximumError.numberOfNodes1 = 5*pow(2,i);
			infoMaximumError.numberOfNodes2 = 5*pow(2,i);

			vector<double> errorsVec;

			toFile << infoMaximumError.numberOfNodes1 << ", ";
			infoMaximumError.interfaceOperation = EQUIVALENT_RESISTANCE;
			ControlVolume contVolMaxEquivResist(infoMaximumError);
			AnalyticalSolution analytSolMax(infoMaximumError);

			for(int k = 0; k < infoMaximumError.numberOfNodes1 + infoMaximumError.numberOfNodes2; k++)
			{
				errorsVec.push_back(abs(contVolMaxEquivResist.getTemperature(k) - analytSolMax[k]));
			}
			double maximumError = *max_element(errorsVec.begin(),errorsVec.end());
			toFile << maximumError << ", ";

			errorsVec.clear();
			infoMaximumError.interfaceOperation = LINEAR_INTERPOLATION;
			ControlVolume  contVolMaxLinInterp(infoMaximumError);
			for(int k = 0; k < infoMaximumError.numberOfNodes1 + infoMaximumError.numberOfNodes2; k++)
			{
				errorsVec.push_back(abs(contVolMaxLinInterp.getTemperature(k) - analytSolMax[k]));
			}
			maximumError = *max_element(errorsVec.begin(),errorsVec.end());
			toFile << maximumError << ", " << endl;
		}
		toFile.close();
		cout << "maximum Error values saved at ../results/maximum_error.csv" << endl;
	}
	
	PetscFinalize();	
}
