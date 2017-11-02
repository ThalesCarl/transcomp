#include <iostream>
#include <string>
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
	/*---------------------------------------------
		Tarefa sem respeitar divisão de materiais
	----------------------------------------------*/
	{
		DoublePlainWallInfo info;

		info.numberOfNodes1 = 5;
		info.numberOfNodes2 = 3;
		info.wallLength1 = 3.0;
		info.wallLength2 = 1.5;
		info.gridType1 = WEST;
		info.gridType2 = CENTER;
		info.thermalConduction1 = 80.0;
		info.thermalConduction2 = 20.0;
		info.beginBoundaryConditionType = PRESCRIBED_FLUX;
		info.endBoundaryConditionType = CONVECTION;
		info.beginBoundaryConditionInfo.push_back(300.0);
		info.endBoundaryConditionInfo.push_back(25.0);
		info.endBoundaryConditionInfo.push_back(298.0);
		info.interfaceOperation = EQUIVALENT_RESISTANCE;

		ControlVolume contVolEquivResist(info);
		contVolEquivResist.writeSolutionToCsv("../results", "secondExerciseDivisionNotRespectedEquivalentResistanceVC");

		info.interfaceOperation = LINEAR_INTERPOLATION;
		ControlVolume  contVolLinInterp(info);
		contVolLinInterp.writeSolutionToCsv("../results", "secondExerciseDivisionNotRespectedLinearInterpolationVC");

		info.wallLength1 = 2.9;
		info.wallLength2 = 1.6;
		AnalyticalSolution analytSol(info);
		analytSol.writeSolutionToCsv("../results", "secondExerciseDivisionNotRespectedAN");
	}
	/*---------------------------------------------
		Tarefa com o erro em cada posição
	----------------------------------------------*/
	{
		DoublePlainWallInfo info;

		info.numberOfNodes1 = 5;
		info.numberOfNodes2 = 3;
		info.wallLength1 = 3.0;
		info.wallLength2 = 1.5;
		info.gridType1 = WEST;
		info.gridType2 = CENTER;
		info.thermalConduction1 = 80.0;
		info.thermalConduction2 = 20.0;
		info.beginBoundaryConditionType = PRESCRIBED_FLUX;
		info.endBoundaryConditionType = CONVECTION;
		info.beginBoundaryConditionInfo.push_back(300.0);
		info.endBoundaryConditionInfo.push_back(25.0);
		info.endBoundaryConditionInfo.push_back(298.0);
		
		info.interfaceOperation = EQUIVALENT_RESISTANCE;
		ControlVolume contVolEquivResist(info);

		info.interfaceOperation = LINEAR_INTERPOLATION;
		ControlVolume  contVolLinInterp(info);
		
		AnalyticalSolution analytSol(info);	
		
		ofstream pFile;
		pFile.open("../results/second_exercise_error_in_every_position.csv",fstream::trunc);
		pFile << "position, error_equivalent_resistance, error_linear_interpolation" << endl;
		pFile << scientific;
		for (int i = 0; i < info.numberOfNodes1 + info.numberOfNodes2; ++i)
		{
			double equivalentResistanceError = abs(contVolEquivResist.getTemperature(i) - analytSol[i]);
			double linearInterpolationError =  abs(contVolLinInterp.getTemperature(i) - analytSol[i]);
			pFile << contVolEquivResist.getPosition(i) << ", " << equivalentResistanceError << ", " << linearInterpolationError << endl;
		}
		pFile.close();
		cout << "Error for positions values saved in ../results/second_exercise_error_in_every_position.csv" << endl;
	}

	/*---------------------------------------------
		Tarefa com o erro maximo em função do numero de volumes de controle
	----------------------------------------------*/
	{
		DoublePlainWallInfo info;

		info.wallLength1 = 3.0;
		info.wallLength2 = 1.5;
		info.gridType1 = WEST;
		info.gridType2 = CENTER;
		info.thermalConduction1 = 80.0;
		info.thermalConduction2 = 20.0;
		info.beginBoundaryConditionType = PRESCRIBED_FLUX;
		info.endBoundaryConditionType = CONVECTION;
		info.beginBoundaryConditionInfo.push_back(300.0);
		info.endBoundaryConditionInfo.push_back(25.0);
		info.endBoundaryConditionInfo.push_back(298.0);
		
		ofstream toFile;
		toFile.open("../results/second_exercise_maximum_error.csv",fstream::trunc);
		toFile << "number_of_nodes_first_material, number_of_nodes_second_material, maximum_error_equivalent_resistance, maximum_error_linear_interpolation" << endl;
		toFile << scientific;
		for (int i = 3; i < 10; ++i)
		{
			info.numberOfNodes1 = i;
			for (int j = 3; j < 10; j++)
			{
				toFile << i << ", "  << j << ", ";
				info.numberOfNodes2 = j;
				vector<double> errorsVec;

				

				info.interfaceOperation = EQUIVALENT_RESISTANCE;
				ControlVolume contVolEquivResist(info);
				AnalyticalSolution analytSol(info);
				for(int k = 0; k < i + j; k++)
				{
					errorsVec.push_back(abs(contVolEquivResist.getTemperature(k) - analytSol[k]));
				}
				double maximumError = *max_element(errorsVec.begin(),errorsVec.end());
				toFile << maximumError << ", ";

				errorsVec.clear();
				info.interfaceOperation = LINEAR_INTERPOLATION;
				ControlVolume  contVolLinInterp(info);
				for(int k = 0; k < info.numberOfNodes1 + info.numberOfNodes2; k++)
				{
					errorsVec.push_back(abs(contVolLinInterp.getTemperature(k) - analytSol[k]));
				}
				maximumError = *max_element(errorsVec.begin(),errorsVec.end());
				toFile << maximumError << ", " << endl;
			}
		}
		toFile.close();
		cout << "maximum Error valeus saved in ../results/second_exercise_maximum_error.csv" << endl;
	}



	PetscFinalize();
	return 0;
}