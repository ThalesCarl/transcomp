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
	/*---------------------------------------------
		Tarefa sem respeitar divisão de materiais
	----------------------------------------------*/
	cout << "Running west_center" << endl;
	{
		DoublePlainWallInfo infoWithout;

		infoWithout.numberOfNodes1 = 5;
		infoWithout.numberOfNodes2 = 3;
		infoWithout.wallLength1 = 3.0;
		infoWithout.wallLength2 = 1.5;
		infoWithout.gridType1 = WEST;
		infoWithout.gridType2 = CENTER;
		infoWithout.thermalConduction1 = 80.0;
		infoWithout.thermalConduction2 = 20.0;
		infoWithout.beginBoundaryConditionType = PRESCRIBED_FLUX;
		infoWithout.endBoundaryConditionType = CONVECTION;
		infoWithout.beginBoundaryConditionInfo.push_back(300.0);
		infoWithout.endBoundaryConditionInfo.push_back(25.0);
		infoWithout.endBoundaryConditionInfo.push_back(298.0);
		
		infoWithout.interfaceOperation = EQUIVALENT_RESISTANCE;
		ControlVolume contVolEquivResistA(infoWithout);
		contVolEquivResistA.writeSolutionToCsv("../results/second_exercise/west_center", "division_not_respected_equivalent_resistance_VC");
		infoWithout.interfaceOperation = LINEAR_INTERPOLATION;
		ControlVolume  contVolLinInterpA(infoWithout);
		contVolLinInterpA.writeSolutionToCsv("../results/second_exercise/west_center", "division_not_linear_interpolation_resistance_VC");

		infoWithout.wallLength1 = 2.9;
		infoWithout.wallLength2 = 1.6;
		AnalyticalSolution analytSolA(infoWithout);
		analytSolA.writeSolutionToCsv("../results/second_exercise/west_center", "division_not_respected_AN");
		

	}
	/*---------------------------------------------
		Tarefa com o erro em cada posição
	----------------------------------------------*/
	{
		DoublePlainWallInfo infoEveryPosition;

		infoEveryPosition.numberOfNodes1 = 5;
		infoEveryPosition.numberOfNodes2 = 3;
		infoEveryPosition.wallLength1 = 3.0;
		infoEveryPosition.wallLength2 = 1.5;
		infoEveryPosition.gridType1 = WEST;
		infoEveryPosition.gridType2 = CENTER;
		infoEveryPosition.thermalConduction1 = 80.0;
		infoEveryPosition.thermalConduction2 = 20.0;
		infoEveryPosition.beginBoundaryConditionType = PRESCRIBED_FLUX;
		infoEveryPosition.endBoundaryConditionType = CONVECTION;
		infoEveryPosition.beginBoundaryConditionInfo.push_back(300.0);
		infoEveryPosition.endBoundaryConditionInfo.push_back(25.0);
		infoEveryPosition.endBoundaryConditionInfo.push_back(298.0);
		
		infoEveryPosition.interfaceOperation = EQUIVALENT_RESISTANCE;
		ControlVolume contVolEquivResistB(infoEveryPosition);

		infoEveryPosition.interfaceOperation = LINEAR_INTERPOLATION;
		ControlVolume  contVolLinInterpB(infoEveryPosition);
		
		AnalyticalSolution analytSolB(infoEveryPosition);	
		
		ofstream pFile;
		pFile.open("../results/second_exercise/west_center/error_in_every_position.csv",fstream::trunc);
		pFile << "position, error_equivalent_resistance, error_linear_interpolation" << endl;
		pFile << scientific;
		for (int i = 0; i < infoEveryPosition.numberOfNodes1 + infoEveryPosition.numberOfNodes2; i++)
		{
			double equivalentResistanceError = abs(contVolEquivResistB.getTemperature(i) - analytSolB[i]);
			double linearInterpolationError =  abs(contVolLinInterpB.getTemperature(i) - analytSolB[i]);
			pFile << contVolEquivResistB.getPosition(i) << ", " << equivalentResistanceError << ", " << linearInterpolationError << endl;
		}
		pFile.close();
		cout << "Error for positions values saved in ../results/second_exercise/west_center/error_in_every_position.csv" << endl;
	}

	/*---------------------------------------------
		Tarefa com o erro maximo em função do numero de volumes de controle
	----------------------------------------------*/
	{
		DoublePlainWallInfo infoMaximum;

		infoMaximum.wallLength1 = 3.0;
		infoMaximum.wallLength2 = 1.5;
		infoMaximum.gridType1 = WEST;
		infoMaximum.gridType2 = CENTER;
		infoMaximum.thermalConduction1 = 80.0;
		infoMaximum.thermalConduction2 = 20.0;
		infoMaximum.beginBoundaryConditionType = PRESCRIBED_FLUX;
		infoMaximum.endBoundaryConditionType = CONVECTION;
		infoMaximum.beginBoundaryConditionInfo.push_back(300.0);
		infoMaximum.endBoundaryConditionInfo.push_back(25.0);
		infoMaximum.endBoundaryConditionInfo.push_back(298.0);
		
		ofstream toFile;
		toFile.open("../results/second_exercise/west_center/maximum_error.csv",fstream::trunc);
		toFile << "number_of_nodes_first_material, number_of_nodes_second_material, maximum_error_equivalent_resistance, maximum_error_linear_interpolation" << endl;
		toFile << scientific;
		for (int i = 3; i < 10; ++i)
		{
			infoMaximum.numberOfNodes1 = i;
			for (int j = 3; j < 10; j++)
			{
				toFile << i << ", "  << j << ", ";
				infoMaximum.numberOfNodes2 = j;
				vector<double> errorsVec;

				

				infoMaximum.interfaceOperation = EQUIVALENT_RESISTANCE;
				ControlVolume contVolEquivResistC(infoMaximum);
				AnalyticalSolution analytSolC(infoMaximum);
				for(int k = 0; k < i + j; k++)
				{
					errorsVec.push_back(abs(contVolEquivResistC.getTemperature(k) - analytSolC[k]));
				}
				double maximumError = *max_element(errorsVec.begin(),errorsVec.end());
				toFile << maximumError << ", ";

				errorsVec.clear();
				infoMaximum.interfaceOperation = LINEAR_INTERPOLATION;
				ControlVolume  contVolLinInterpC(infoMaximum);
				for(int k = 0; k < infoMaximum.numberOfNodes1 + infoMaximum.numberOfNodes2; k++)
				{
					errorsVec.push_back(abs(contVolLinInterpC.getTemperature(k) - analytSolC[k]));
				}
				maximumError = *max_element(errorsVec.begin(),errorsVec.end());
				toFile << maximumError << ", " << endl;
			}
		}
		toFile.close();
		cout << "maximum Error valeus saved in ../results/second_exercise/west_center/maximum_error.csv" << endl;
	}

/*------------------------------------------------------------------------------------------------------*/
	cout << endl <<"Running west_east" << endl;
	{
		DoublePlainWallInfo infoWithout;

		infoWithout.numberOfNodes1 = 5;
		infoWithout.numberOfNodes2 = 3;
		infoWithout.wallLength1 = 3.0;
		infoWithout.wallLength2 = 1.5;
		infoWithout.gridType1 = WEST;
		infoWithout.gridType2 = EAST;
		infoWithout.thermalConduction1 = 80.0;
		infoWithout.thermalConduction2 = 20.0;
		infoWithout.beginBoundaryConditionType = PRESCRIBED_FLUX;
		infoWithout.endBoundaryConditionType = CONVECTION;
		infoWithout.beginBoundaryConditionInfo.push_back(300.0);
		infoWithout.endBoundaryConditionInfo.push_back(25.0);
		infoWithout.endBoundaryConditionInfo.push_back(298.0);
		
		infoWithout.interfaceOperation = EQUIVALENT_RESISTANCE;
		ControlVolume contVolEquivResistA(infoWithout);
		contVolEquivResistA.writeSolutionToCsv("../results/second_exercise/west_east", "division_not_respected_equivalent_resistance_VC");
		infoWithout.interfaceOperation = LINEAR_INTERPOLATION;
		ControlVolume  contVolLinInterpA(infoWithout);
		contVolLinInterpA.writeSolutionToCsv("../results/second_exercise/west_east", "division_not_linear_interpolation_resistance_VC");

		infoWithout.wallLength1 = 2.9;
		infoWithout.wallLength2 = 1.6;
		AnalyticalSolution analytSolA(infoWithout);
		analytSolA.writeSolutionToCsv("../results/second_exercise/west_east", "division_not_respected_AN");
		

	}
	/*---------------------------------------------
		Tarefa com o erro em cada posição
	----------------------------------------------*/
	{
		DoublePlainWallInfo infoEveryPosition;

		infoEveryPosition.numberOfNodes1 = 5;
		infoEveryPosition.numberOfNodes2 = 3;
		infoEveryPosition.wallLength1 = 3.0;
		infoEveryPosition.wallLength2 = 1.5;
		infoEveryPosition.gridType1 = WEST;
		infoEveryPosition.gridType2 = EAST;
		infoEveryPosition.thermalConduction1 = 80.0;
		infoEveryPosition.thermalConduction2 = 20.0;
		infoEveryPosition.beginBoundaryConditionType = PRESCRIBED_FLUX;
		infoEveryPosition.endBoundaryConditionType = CONVECTION;
		infoEveryPosition.beginBoundaryConditionInfo.push_back(300.0);
		infoEveryPosition.endBoundaryConditionInfo.push_back(25.0);
		infoEveryPosition.endBoundaryConditionInfo.push_back(298.0);
		
		infoEveryPosition.interfaceOperation = EQUIVALENT_RESISTANCE;
		ControlVolume contVolEquivResistB(infoEveryPosition);

		infoEveryPosition.interfaceOperation = LINEAR_INTERPOLATION;
		ControlVolume  contVolLinInterpB(infoEveryPosition);
		
		AnalyticalSolution analytSolB(infoEveryPosition);	
		
		ofstream pFile;
		pFile.open("../results/second_exercise/west_east/error_in_every_position.csv",fstream::trunc);
		pFile << "position, error_equivalent_resistance, error_linear_interpolation" << endl;
		pFile << scientific;
		for (int i = 0; i < infoEveryPosition.numberOfNodes1 + infoEveryPosition.numberOfNodes2; i++)
		{
			double equivalentResistanceError = abs(contVolEquivResistB.getTemperature(i) - analytSolB[i]);
			double linearInterpolationError =  abs(contVolLinInterpB.getTemperature(i) - analytSolB[i]);
			pFile << contVolEquivResistB.getPosition(i) << ", " << equivalentResistanceError << ", " << linearInterpolationError << endl;
		}
		pFile.close();
		cout << "Error for positions values saved in ../results/second_exercise/west_east/error_in_every_position.csv" << endl;
	}

	/*---------------------------------------------
		Tarefa com o erro maximo em função do numero de volumes de controle
	----------------------------------------------*/
	{
		DoublePlainWallInfo infoMaximum;

		infoMaximum.wallLength1 = 3.0;
		infoMaximum.wallLength2 = 1.5;
		infoMaximum.gridType1 = WEST;
		infoMaximum.gridType2 = EAST;
		infoMaximum.thermalConduction1 = 80.0;
		infoMaximum.thermalConduction2 = 20.0;
		infoMaximum.beginBoundaryConditionType = PRESCRIBED_FLUX;
		infoMaximum.endBoundaryConditionType = CONVECTION;
		infoMaximum.beginBoundaryConditionInfo.push_back(300.0);
		infoMaximum.endBoundaryConditionInfo.push_back(25.0);
		infoMaximum.endBoundaryConditionInfo.push_back(298.0);
		
		ofstream toFile;
		toFile.open("../results/second_exercise/west_east/maximum_error.csv",fstream::trunc);
		toFile << "number_of_nodes_first_material, number_of_nodes_second_material, maximum_error_equivalent_resistance, maximum_error_linear_interpolation" << endl;
		toFile << scientific;
		for (int i = 3; i < 10; ++i)
		{
			infoMaximum.numberOfNodes1 = i;
			for (int j = 3; j < 10; j++)
			{
				toFile << i << ", "  << j << ", ";
				infoMaximum.numberOfNodes2 = j;
				vector<double> errorsVec;

				

				infoMaximum.interfaceOperation = EQUIVALENT_RESISTANCE;
				ControlVolume contVolEquivResistC(infoMaximum);
				AnalyticalSolution analytSolC(infoMaximum);
				for(int k = 0; k < i + j; k++)
				{
					errorsVec.push_back(abs(contVolEquivResistC.getTemperature(k) - analytSolC[k]));
				}
				double maximumError = *max_element(errorsVec.begin(),errorsVec.end());
				toFile << maximumError << ", ";

				errorsVec.clear();
				infoMaximum.interfaceOperation = LINEAR_INTERPOLATION;
				ControlVolume  contVolLinInterpC(infoMaximum);
				for(int k = 0; k < infoMaximum.numberOfNodes1 + infoMaximum.numberOfNodes2; k++)
				{
					errorsVec.push_back(abs(contVolLinInterpC.getTemperature(k) - analytSolC[k]));
				}
				maximumError = *max_element(errorsVec.begin(),errorsVec.end());
				toFile << maximumError << ", " << endl;
			}
		}
		toFile.close();
		cout << "maximum Error valeus saved in ../results/second_exercise/west_east/maximum_error.csv" << endl;
	}

/*----------------------------------------------------------------------------------------------------*/
		cout << endl << "Running center_center" << endl;
	{
		DoublePlainWallInfo infoWithout;

		infoWithout.numberOfNodes1 = 5;
		infoWithout.numberOfNodes2 = 3;
		infoWithout.wallLength1 = 3.0;
		infoWithout.wallLength2 = 1.5;
		infoWithout.gridType1 = CENTER;
		infoWithout.gridType2 = CENTER;
		infoWithout.thermalConduction1 = 80.0;
		infoWithout.thermalConduction2 = 20.0;
		infoWithout.beginBoundaryConditionType = PRESCRIBED_FLUX;
		infoWithout.endBoundaryConditionType = CONVECTION;
		infoWithout.beginBoundaryConditionInfo.push_back(300.0);
		infoWithout.endBoundaryConditionInfo.push_back(25.0);
		infoWithout.endBoundaryConditionInfo.push_back(298.0);
		
		infoWithout.interfaceOperation = EQUIVALENT_RESISTANCE;
		ControlVolume contVolEquivResistA(infoWithout);
		contVolEquivResistA.writeSolutionToCsv("../results/second_exercise/center_center", "division_not_respected_equivalent_resistance_VC");
		infoWithout.interfaceOperation = LINEAR_INTERPOLATION;
		ControlVolume  contVolLinInterpA(infoWithout);
		contVolLinInterpA.writeSolutionToCsv("../results/second_exercise/center_center", "division_not_linear_interpolation_resistance_VC");

		infoWithout.wallLength1 = 2.9;
		infoWithout.wallLength2 = 1.6;
		AnalyticalSolution analytSolA(infoWithout);
		analytSolA.writeSolutionToCsv("../results/second_exercise/center_center", "division_not_respected_AN");
		

	}
	/*---------------------------------------------
		Tarefa com o erro em cada posição
	----------------------------------------------*/
	{
		DoublePlainWallInfo infoEveryPosition;

		infoEveryPosition.numberOfNodes1 = 5;
		infoEveryPosition.numberOfNodes2 = 3;
		infoEveryPosition.wallLength1 = 3.0;
		infoEveryPosition.wallLength2 = 1.5;
		infoEveryPosition.gridType1 = CENTER;
		infoEveryPosition.gridType2 = CENTER;
		infoEveryPosition.thermalConduction1 = 80.0;
		infoEveryPosition.thermalConduction2 = 20.0;
		infoEveryPosition.beginBoundaryConditionType = PRESCRIBED_FLUX;
		infoEveryPosition.endBoundaryConditionType = CONVECTION;
		infoEveryPosition.beginBoundaryConditionInfo.push_back(300.0);
		infoEveryPosition.endBoundaryConditionInfo.push_back(25.0);
		infoEveryPosition.endBoundaryConditionInfo.push_back(298.0);
		
		infoEveryPosition.interfaceOperation = EQUIVALENT_RESISTANCE;
		ControlVolume contVolEquivResistB(infoEveryPosition);

		infoEveryPosition.interfaceOperation = LINEAR_INTERPOLATION;
		ControlVolume  contVolLinInterpB(infoEveryPosition);
		
		AnalyticalSolution analytSolB(infoEveryPosition);	
		
		ofstream pFile;
		pFile.open("../results/second_exercise/center_center/error_in_every_position.csv",fstream::trunc);
		pFile << "position, error_equivalent_resistance, error_linear_interpolation" << endl;
		pFile << scientific;
		for (int i = 0; i < infoEveryPosition.numberOfNodes1 + infoEveryPosition.numberOfNodes2; i++)
		{
			double equivalentResistanceError = abs(contVolEquivResistB.getTemperature(i) - analytSolB[i]);
			double linearInterpolationError =  abs(contVolLinInterpB.getTemperature(i) - analytSolB[i]);
			pFile << contVolEquivResistB.getPosition(i) << ", " << equivalentResistanceError << ", " << linearInterpolationError << endl;
		}
		pFile.close();
		cout << "Error for positions values saved in ../results/second_exercise/center_center/error_in_every_position.csv" << endl;
	}

	/*---------------------------------------------
		Tarefa com o erro maximo em função do numero de volumes de controle
	----------------------------------------------*/
	{
		DoublePlainWallInfo infoMaximum;

		infoMaximum.wallLength1 = 3.0;
		infoMaximum.wallLength2 = 1.5;
		infoMaximum.gridType1 = CENTER;
		infoMaximum.gridType2 = CENTER;
		infoMaximum.thermalConduction1 = 80.0;
		infoMaximum.thermalConduction2 = 20.0;
		infoMaximum.beginBoundaryConditionType = PRESCRIBED_FLUX;
		infoMaximum.endBoundaryConditionType = CONVECTION;
		infoMaximum.beginBoundaryConditionInfo.push_back(300.0);
		infoMaximum.endBoundaryConditionInfo.push_back(25.0);
		infoMaximum.endBoundaryConditionInfo.push_back(298.0);
		
		ofstream toFile;
		toFile.open("../results/second_exercise/center_center/maximum_error.csv",fstream::trunc);
		toFile << "number_of_nodes_first_material, number_of_nodes_second_material, maximum_error_equivalent_resistance, maximum_error_linear_interpolation" << endl;
		toFile << scientific;
		for (int i = 3; i < 10; ++i)
		{
			infoMaximum.numberOfNodes1 = i;
			for (int j = 3; j < 10; j++)
			{
				toFile << i << ", "  << j << ", ";
				infoMaximum.numberOfNodes2 = j;
				vector<double> errorsVec;

				

				infoMaximum.interfaceOperation = EQUIVALENT_RESISTANCE;
				ControlVolume contVolEquivResistC(infoMaximum);
				AnalyticalSolution analytSolC(infoMaximum);
				for(int k = 0; k < i + j; k++)
				{
					errorsVec.push_back(abs(contVolEquivResistC.getTemperature(k) - analytSolC[k]));
				}
				double maximumError = *max_element(errorsVec.begin(),errorsVec.end());
				toFile << maximumError << ", ";

				errorsVec.clear();
				infoMaximum.interfaceOperation = LINEAR_INTERPOLATION;
				ControlVolume  contVolLinInterpC(infoMaximum);
				for(int k = 0; k < infoMaximum.numberOfNodes1 + infoMaximum.numberOfNodes2; k++)
				{
					errorsVec.push_back(abs(contVolLinInterpC.getTemperature(k) - analytSolC[k]));
				}
				maximumError = *max_element(errorsVec.begin(),errorsVec.end());
				toFile << maximumError << ", " << endl;
			}
		}
		toFile.close();
		cout << "maximum Error valeus saved in ../results/second_exercise/center_center/maximum_error.csv" << endl;
	}

/*--------------------------------------------------------------------------------------------------*/
		cout << endl << "Running center_east" << endl;
	{
		DoublePlainWallInfo infoWithout;

		infoWithout.numberOfNodes1 = 5;
		infoWithout.numberOfNodes2 = 3;
		infoWithout.wallLength1 = 3.0;
		infoWithout.wallLength2 = 1.5;
		infoWithout.gridType1 = CENTER;
		infoWithout.gridType2 = EAST;
		infoWithout.thermalConduction1 = 80.0;
		infoWithout.thermalConduction2 = 20.0;
		infoWithout.beginBoundaryConditionType = PRESCRIBED_FLUX;
		infoWithout.endBoundaryConditionType = CONVECTION;
		infoWithout.beginBoundaryConditionInfo.push_back(300.0);
		infoWithout.endBoundaryConditionInfo.push_back(25.0);
		infoWithout.endBoundaryConditionInfo.push_back(298.0);
		
		infoWithout.interfaceOperation = EQUIVALENT_RESISTANCE;
		ControlVolume contVolEquivResistA(infoWithout);
		contVolEquivResistA.writeSolutionToCsv("../results/second_exercise/center_east", "division_not_respected_equivalent_resistance_VC");
		infoWithout.interfaceOperation = LINEAR_INTERPOLATION;
		ControlVolume  contVolLinInterpA(infoWithout);
		contVolLinInterpA.writeSolutionToCsv("../results/second_exercise/center_east", "division_not_linear_interpolation_resistance_VC");

		infoWithout.wallLength1 = 2.9;
		infoWithout.wallLength2 = 1.6;
		AnalyticalSolution analytSolA(infoWithout);
		analytSolA.writeSolutionToCsv("../results/second_exercise/center_east", "division_not_respected_AN");
		

	}
	/*---------------------------------------------
		Tarefa com o erro em cada posição
	----------------------------------------------*/
	{
		DoublePlainWallInfo infoEveryPosition;

		infoEveryPosition.numberOfNodes1 = 5;
		infoEveryPosition.numberOfNodes2 = 3;
		infoEveryPosition.wallLength1 = 3.0;
		infoEveryPosition.wallLength2 = 1.5;
		infoEveryPosition.gridType1 = CENTER;
		infoEveryPosition.gridType2 = EAST;
		infoEveryPosition.thermalConduction1 = 80.0;
		infoEveryPosition.thermalConduction2 = 20.0;
		infoEveryPosition.beginBoundaryConditionType = PRESCRIBED_FLUX;
		infoEveryPosition.endBoundaryConditionType = CONVECTION;
		infoEveryPosition.beginBoundaryConditionInfo.push_back(300.0);
		infoEveryPosition.endBoundaryConditionInfo.push_back(25.0);
		infoEveryPosition.endBoundaryConditionInfo.push_back(298.0);
		
		infoEveryPosition.interfaceOperation = EQUIVALENT_RESISTANCE;
		ControlVolume contVolEquivResistB(infoEveryPosition);

		infoEveryPosition.interfaceOperation = LINEAR_INTERPOLATION;
		ControlVolume  contVolLinInterpB(infoEveryPosition);
		
		AnalyticalSolution analytSolB(infoEveryPosition);	
		
		ofstream pFile;
		pFile.open("../results/second_exercise/center_east/error_in_every_position.csv",fstream::trunc);
		pFile << "position, error_equivalent_resistance, error_linear_interpolation" << endl;
		pFile << scientific;
		for (int i = 0; i < infoEveryPosition.numberOfNodes1 + infoEveryPosition.numberOfNodes2; i++)
		{
			double equivalentResistanceError = abs(contVolEquivResistB.getTemperature(i) - analytSolB[i]);
			double linearInterpolationError =  abs(contVolLinInterpB.getTemperature(i) - analytSolB[i]);
			pFile << contVolEquivResistB.getPosition(i) << ", " << equivalentResistanceError << ", " << linearInterpolationError << endl;
		}
		pFile.close();
		cout << "Error for positions values saved in ../results/second_exercise/center_east/error_in_every_position.csv" << endl;
	}

	/*---------------------------------------------
		Tarefa com o erro maximo em função do numero de volumes de controle
	----------------------------------------------*/
	{
		DoublePlainWallInfo infoMaximum;

		infoMaximum.wallLength1 = 3.0;
		infoMaximum.wallLength2 = 1.5;
		infoMaximum.gridType1 = CENTER;
		infoMaximum.gridType2 = EAST;
		infoMaximum.thermalConduction1 = 80.0;
		infoMaximum.thermalConduction2 = 20.0;
		infoMaximum.beginBoundaryConditionType = PRESCRIBED_FLUX;
		infoMaximum.endBoundaryConditionType = CONVECTION;
		infoMaximum.beginBoundaryConditionInfo.push_back(300.0);
		infoMaximum.endBoundaryConditionInfo.push_back(25.0);
		infoMaximum.endBoundaryConditionInfo.push_back(298.0);
		
		ofstream toFile;
		toFile.open("../results/second_exercise/center_east/maximum_error.csv",fstream::trunc);
		toFile << "number_of_nodes_first_material, number_of_nodes_second_material, maximum_error_equivalent_resistance, maximum_error_linear_interpolation" << endl;
		toFile << scientific;
		for (int i = 3; i < 10; ++i)
		{
			infoMaximum.numberOfNodes1 = i;
			for (int j = 3; j < 10; j++)
			{
				toFile << i << ", "  << j << ", ";
				infoMaximum.numberOfNodes2 = j;
				vector<double> errorsVec;

				

				infoMaximum.interfaceOperation = EQUIVALENT_RESISTANCE;
				ControlVolume contVolEquivResistC(infoMaximum);
				AnalyticalSolution analytSolC(infoMaximum);
				for(int k = 0; k < i + j; k++)
				{
					errorsVec.push_back(abs(contVolEquivResistC.getTemperature(k) - analytSolC[k]));
				}
				double maximumError = *max_element(errorsVec.begin(),errorsVec.end());
				toFile << maximumError << ", ";

				errorsVec.clear();
				infoMaximum.interfaceOperation = LINEAR_INTERPOLATION;
				ControlVolume  contVolLinInterpC(infoMaximum);
				for(int k = 0; k < infoMaximum.numberOfNodes1 + infoMaximum.numberOfNodes2; k++)
				{
					errorsVec.push_back(abs(contVolLinInterpC.getTemperature(k) - analytSolC[k]));
				}
				maximumError = *max_element(errorsVec.begin(),errorsVec.end());
				toFile << maximumError << ", " << endl;
			}
		}
		toFile.close();
		cout << "maximum Error valeus saved in ../results/second_exercise/center_east/maximum_error.csv" << endl;
	}

	
	PetscFinalize();
	return 0;
}
