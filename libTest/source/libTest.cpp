#define BOOST_TEST_MODULE TestModule

#include "Test.h"
#include "Mesh.h"
#include "ThermalConduction.h"
#include "Boundary.h"
#include "LinearSystem.h"
#include "AnalyticalSolution.h"
#include "ControlVolume.h"

TestCase( MeshTest )
{

	/* -------------------------------------------------------------------
		MALHA TIPO CENTRAL - 3 NÓS E TAMANHO UNITÁRIO
	---------------------------------------------------------------------*/
	Mesh meshCenter(3,1.0,CENTER);

	// nó 0
	checkClose(meshCenter.centerPoint(0),       0.16666666667, 1e-8);								
	checkClose(meshCenter.eastPoint(0),         0.5,           1e-8);
	checkClose(meshCenter.eastDistance(0),      0.33333333333, 1e-8);
	checkClose(meshCenter.eastPlusDistance(0),  0.16666666667, 1e-8);
	checkClose(meshCenter.eastMinusDistance(0), 0.16666666667, 1e-8);

	// nó 1
	checkClose(meshCenter.westPoint(1),         0.16666666667, 1e-8);
	checkClose(meshCenter.centerPoint(1),       0.5,           1e-8);								
	checkClose(meshCenter.eastPoint(1),         0.83333333333, 1e-8);
	checkClose(meshCenter.eastDistance(1),      0.33333333333, 1e-8);
	checkClose(meshCenter.westDistance(1),      0.33333333333, 1e-8);
	checkClose(meshCenter.eastPlusDistance(1),  0.16666666667, 1e-8);
	checkClose(meshCenter.eastMinusDistance(1), 0.16666666667, 1e-8);
	checkClose(meshCenter.westPlusDistance(1),  0.16666666667, 1e-8);
	checkClose(meshCenter.westMinusDistance(1), 0.16666666667, 1e-8);

	//nó 2
	checkClose(meshCenter.westPoint(2),         0.5,           1e-8);
	checkClose(meshCenter.centerPoint(2),       0.83333333333, 1e-8);								
	checkClose(meshCenter.westDistance(2),      0.33333333333, 1e-8);
	checkClose(meshCenter.westPlusDistance(2),  0.16666666667, 1e-8);

	checkClose(meshCenter.westMinusDistance(2), 0.16666666667, 1e-8);
				
	/* -------------------------------------------------------------------
		MALHA TIPO WEST - 3 NÓS E TAMANHO UNITÁRIO
	---------------------------------------------------------------------*/
	Mesh meshWest(3,1.0,WEST);

	// nó 0
	checkClose(meshWest.centerPoint(0),       0,   1e-8);								
	checkClose(meshWest.eastPoint(0),         0.4, 1e-8);
	checkClose(meshWest.eastDistance(0),      0.4, 1e-8);
	checkClose(meshWest.eastPlusDistance(0),  0.2, 1e-8);
	checkClose(meshWest.eastMinusDistance(0), 0.2, 1e-8);

	// nó 1
	checkClose(meshWest.westPoint(1),         0,   1e-8);
	checkClose(meshWest.centerPoint(1),       0.4, 1e-8);								
	checkClose(meshWest.eastPoint(1),         0.8, 1e-8);
	checkClose(meshWest.eastDistance(1),      0.4, 1e-8);
	checkClose(meshWest.westDistance(1),      0.4, 1e-8);
	checkClose(meshWest.eastPlusDistance(1),  0.2, 1e-8);
	checkClose(meshWest.eastMinusDistance(1), 0.2, 1e-8);
	checkClose(meshWest.westPlusDistance(1),  0.2, 1e-8);
	checkClose(meshWest.westMinusDistance(1), 0.2, 1e-8);

	//nó 2
	checkClose(meshWest.westPoint(2),         0.4, 1e-8);
	checkClose(meshWest.centerPoint(2),       0.8, 1e-8);								
	checkClose(meshWest.westDistance(2),      0.4, 1e-8);
	checkClose(meshWest.westPlusDistance(2),  0.2, 1e-8);
	checkClose(meshWest.westMinusDistance(2), 0.2, 1e-8);

    /* -------------------------------------------------------------------
		MALHA TIPO EAST - 3 NÓS E TAMANHO UNITÁRIO
	---------------------------------------------------------------------*/
	Mesh meshEast(3,1.0,EAST);

	// nó 0
	checkClose(meshEast.centerPoint(0),       0.2, 1e-8);								
	checkClose(meshEast.eastPoint(0),         0.6, 1e-8);
	checkClose(meshEast.eastDistance(0),      0.4, 1e-8);
	checkClose(meshEast.eastPlusDistance(0),  0.2, 1e-8);
	checkClose(meshEast.eastMinusDistance(0), 0.2, 1e-8);

	// nó 1
	checkClose(meshEast.westPoint(1),         0.2, 1e-8);
	checkClose(meshEast.centerPoint(1),       0.6, 1e-8);								
	checkClose(meshEast.eastPoint(1),         1,   1e-8);
	checkClose(meshEast.eastDistance(1),      0.4, 1e-8);
	checkClose(meshEast.westDistance(1),      0.4, 1e-8);
	checkClose(meshEast.eastPlusDistance(1),  0.2, 1e-8);
	checkClose(meshEast.eastMinusDistance(1), 0.2, 1e-8);
	checkClose(meshEast.westPlusDistance(1),  0.2, 1e-8);
	checkClose(meshEast.westMinusDistance(1), 0.2, 1e-8);

	//nó 2
	checkClose(meshEast.westPoint(2),         0.6, 1e-8);
	checkClose(meshEast.centerPoint(2),       1,   1e-8);								
	checkClose(meshEast.westDistance(2),      0.4, 1e-8);
	checkClose(meshEast.westPlusDistance(2),  0.2, 1e-8);
	checkClose(meshEast.westMinusDistance(2), 0.2, 1e-8);

	/* -------------------------------------------------------------------
		MALHA TIPO BOTH - 3 NÓS E TAMANHO UNITÁRIO
	---------------------------------------------------------------------*/
	Mesh meshBoth(3,1.0,BOTH);

	// nó 0
	checkClose(meshBoth.centerPoint(0),       0,    1e-8);								
	checkClose(meshBoth.eastPoint(0),         0.5,  1e-8);
	checkClose(meshBoth.eastDistance(0),      0.5,  1e-8);
	checkClose(meshBoth.eastPlusDistance(0),  0.25, 1e-8);
	checkClose(meshBoth.eastMinusDistance(0), 0.25, 1e-8);

	// nó 1
	checkClose(meshBoth.westPoint(1),         0,    1e-8);
	checkClose(meshBoth.centerPoint(1),       0.5,  1e-8);								
	checkClose(meshBoth.eastPoint(1),         1,    1e-8);
	checkClose(meshBoth.eastDistance(1),      0.5,  1e-8);
	checkClose(meshBoth.westDistance(1),      0.5,  1e-8);
	checkClose(meshBoth.eastPlusDistance(1),  0.25, 1e-8);
	checkClose(meshBoth.eastMinusDistance(1), 0.25, 1e-8);
	checkClose(meshBoth.westPlusDistance(1),  0.25, 1e-8);
	checkClose(meshBoth.westMinusDistance(1), 0.25, 1e-8);

	//nó 2
	checkClose(meshBoth.westPoint(2),         0.5,  1e-8);
	checkClose(meshBoth.centerPoint(2),       1,    1e-8);								
	checkClose(meshBoth.westDistance(2),      0.5,  1e-8);
	checkClose(meshBoth.westPlusDistance(2),  0.25, 1e-8);
	checkClose(meshBoth.westMinusDistance(2), 0.25, 1e-8);
}

TestCase(ThermalConductionTest)
{
	int numberOfNodes = 3;
	double thermalConduction = 8.0; //[W/m*K]
	ThermalConduction vectorK(numberOfNodes, thermalConduction);
	for (int i = 0; i < numberOfNodes; i++)
	{
		checkClose(8.0, vectorK[i], 1e-8);
	}
}

TestCase(BoundaryConditionsTest)
{
	/*--------------------------------------------------
	Temperatura Prescrita nas duas fronteiras
	---------------------------------------------------*/
	BoundaryCondition beginType, endType;
	beginType = PRESCRIBED_TEMPERATURE;
	endType = PRESCRIBED_TEMPERATURE;
	double temperatureBegin = 100;
	double temperatureEnd = 20;
	vector<double> infoBegin(1, temperatureBegin);
	vector<double> infoEnd(1, temperatureEnd);
	Boundary bondariesWithPrescribedTemperature(beginType, endType, infoBegin, infoEnd);
	
	checkClose(bondariesWithPrescribedTemperature.getBeginBoundaryCondition(),temperatureBegin, 1e-8);
	checkClose(bondariesWithPrescribedTemperature.getEndBoundaryCondition(),  temperatureEnd,   1e-8);

	/*--------------------------------------------------
	Convecção nas duas fronteiras
	---------------------------------------------------*/
	beginType = CONVECTION;
	double infiniteTemperatureBegin = 110;
	double h_begin = 12; //[W/m^2*K]
	infoBegin.resize(2);
	infoBegin[0] = h_begin;
	infoBegin[1] = infiniteTemperatureBegin;

	endType = CONVECTION;
	double infiniteTemperatureEnd = 30;
	double h_end = 14; //[W/m^2*K]
	infoEnd.resize(2);
	infoEnd[0] = h_end;
	infoEnd[1] = infiniteTemperatureEnd;

	Boundary bondariesWithConvection(beginType, endType, infoBegin, infoEnd);
	
	checkClose(bondariesWithConvection.getBeginBoundaryCondition(),infiniteTemperatureBegin, 1e-8);
	checkClose(bondariesWithConvection.getEndBoundaryCondition(),  infiniteTemperatureEnd,   1e-8);
	checkClose(bondariesWithConvection.getBeginConvectionCoeficient(), h_begin, 1e-8);
	checkClose(bondariesWithConvection.getEndConvectionCoeficient(), h_end, 1e-8);

	/*--------------------------------------------------
	Convecção em uma fronteira e fluxo prescrito na outra
	---------------------------------------------------*/
	beginType = CONVECTION;
	infiniteTemperatureBegin = 125;
	h_begin = 15; //[W/m^2*K]
	infoBegin[0] = h_begin;
	infoBegin[1] = infiniteTemperatureBegin;

	endType = PRESCRIBED_FLUX;
	double endPrescribedFlux = 20; //[W]
	infoEnd.resize(1);
	infoEnd[0] = endPrescribedFlux;

	Boundary bondariesWithDiferentConditions(beginType, endType, infoBegin, infoEnd);
	
	checkClose(bondariesWithDiferentConditions.getBeginBoundaryCondition(),infiniteTemperatureBegin, 1e-8);
	checkClose(bondariesWithDiferentConditions.getBeginConvectionCoeficient(), h_begin, 1e-8);
	checkClose(bondariesWithDiferentConditions.getEndBoundaryCondition(),  endPrescribedFlux, 1e-8);
	
}

TestCase(LinearSystemTest)
{
	LinearSystem solver(3);
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if(i==j)
				solver.setValueToMatrix(i,j, double((i+1.3)*(j-1.2)));
			else
				solver.setValueToMatrix(i,j, double((i+1.5)*(j-1.1)));

		}
	}
	for (int i = 0; i < 3; i++)
	{
		solver.setValueToVector(i,double(i+0.3));
	}
	
	PetscInitialize(NULL,NULL,NULL,NULL);
	solver.solve();
	
	
	vector<double> exactSolution(3);
	exactSolution[0] = -1.973309906291830;
	exactSolution[1] = -2.400016733601070;
	exactSolution[2] = -2.324715528781790;

	for (int i = 0; i < 3; i++)
	{
		checkClose(solver[i], exactSolution[i], 1e-8);
	}
}

TestCase(LinearSystemTestTDMA)
{
	LinearSystem solver(3);
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if(i==j)
				solver.setValueToMatrix(i,j, double((i+1.3)*(j-1.2)));
			else
				solver.setValueToMatrix(i,j, double((i+1.5)*(j-1.1)));

		}
	}
	for (int i = 0; i < 3; i++)
	{
		solver.setValueToVector(i,double(i+0.3));
	}
	
	solver.printValues();
	solver.solveTDMA();
	
	
	vector<double> exactSolution(3);
	exactSolution[0] = -1.973309906291830;
	exactSolution[1] = -2.400016733601070;
	exactSolution[2] = -2.324715528781790;

	for (int i = 0; i < 3; i++)
	{
		checkClose(solver[i], exactSolution[i], 1e-8);
	}
}

TestCase(ThermalConductionAndMeshTest)
{
	Mesh newMesh(4,1.0,CENTER);
	vector<double> k;
	k.push_back(2);
	k.push_back(9);
	k.push_back(7);
	k.push_back(3);
	ThermalConduction newVectorK(k);

	//Primeira interface
	checkClose(newVectorK.getEastInterface(newMesh, 0, EQUIVALENT_RESISTANCE),3.2727272727,1e-8);
	checkClose(newVectorK.getEastInterface(newMesh, 0, LINEAR_INTERPOLATION), 5.5,         1e-8);
	checkClose(newVectorK.getWestInterface(newMesh, 1, EQUIVALENT_RESISTANCE),3.2727272727,1e-8);
	checkClose(newVectorK.getWestInterface(newMesh, 1, LINEAR_INTERPOLATION), 5.5,         1e-8);

	//Segunda interface
	checkClose(newVectorK.getEastInterface(newMesh, 1, EQUIVALENT_RESISTANCE),7.875, 1e-8);
	checkClose(newVectorK.getEastInterface(newMesh, 1, LINEAR_INTERPOLATION), 8,     1e-8);
	checkClose(newVectorK.getWestInterface(newMesh, 2, EQUIVALENT_RESISTANCE),7.875, 1e-8);
	checkClose(newVectorK.getWestInterface(newMesh, 2, LINEAR_INTERPOLATION), 8,     1e-8);	
}

TestCase(AnalyticalSolutionTest)
{
	PlainWallInfo  analyticalSolutionData;

	analyticalSolutionData.numberOfNodes = 10;
	analyticalSolutionData.wallLength = 13;
	analyticalSolutionData.gridType = CENTER;
	analyticalSolutionData.thermalConduction = 13;
	analyticalSolutionData.beginBoundaryConditionType = PRESCRIBED_TEMPERATURE;
	analyticalSolutionData.endBoundaryConditionType = PRESCRIBED_TEMPERATURE;
	analyticalSolutionData.beginBoundaryConditionInfo.push_back(150);
	analyticalSolutionData.endBoundaryConditionInfo.push_back(320);

	AnalyticalSolution solution(analyticalSolutionData);
	Mesh mesh_AN_Test(10,13.0,CENTER);
	vector<double> exactValues;
	exactValues.push_back(158.5);
	exactValues.push_back(175.5);
	exactValues.push_back(192.5);
	exactValues.push_back(209.5);
	exactValues.push_back(226.5);
	exactValues.push_back(243.5);
	exactValues.push_back(260.5);
	exactValues.push_back(277.5);
	exactValues.push_back(294.5);
	exactValues.push_back(311.5);	
	
	for(int i = 0; i < mesh_AN_Test.getNumberOfNodes(); i++)
	{
		checkClose(solution[i], exactValues[i], 1e-8);
	}
}

TestCase(AnalyticalSolution_ContVol_Test)
{
	PlainWallInfo  firstExerciseInfo;

	firstExerciseInfo.numberOfNodes = 10;
	firstExerciseInfo.wallLength = 13;
	firstExerciseInfo.gridType = BOTH;
	firstExerciseInfo.thermalConduction = 13;
	firstExerciseInfo.beginBoundaryConditionType = PRESCRIBED_TEMPERATURE;
	firstExerciseInfo.endBoundaryConditionType = PRESCRIBED_TEMPERATURE;
	firstExerciseInfo.beginBoundaryConditionInfo.push_back(150);
	firstExerciseInfo.endBoundaryConditionInfo.push_back(320);
	firstExerciseInfo.interfaceOperation = EQUIVALENT_RESISTANCE;

	AnalyticalSolution analyticalSolution(firstExerciseInfo);

	
	ControlVolume firstExerciseControlVolume(firstExerciseInfo);
	PetscFinalize();
	for (int i = 0; i < 10; ++i)
	{
		checkClose(analyticalSolution[i], firstExerciseControlVolume.getTemperature(i),1e-8);
	}
}

TestCase(MeshAdderTest)
{
	Mesh mesh(3,1,WEST);	
	mesh.addPlainWall(2,1,CENTER);
	mesh.addPlainWall(3,1,EAST);

	vector<double> meshPositionsChecker;
	meshPositionsChecker.push_back(0.00);
	meshPositionsChecker.push_back(0.40);
	meshPositionsChecker.push_back(0.80);
	meshPositionsChecker.push_back(1.25);
	meshPositionsChecker.push_back(1.75);
	meshPositionsChecker.push_back(2.20);
	meshPositionsChecker.push_back(2.60);
	meshPositionsChecker.push_back(3.00);

	vector<double> meshSurfaceChecker;
	meshSurfaceChecker.push_back(0.0);
	meshSurfaceChecker.push_back(0.2);
	meshSurfaceChecker.push_back(0.6);
	meshSurfaceChecker.push_back(1.0);
	meshSurfaceChecker.push_back(1.5);
	meshSurfaceChecker.push_back(2.0);
	meshSurfaceChecker.push_back(2.4);
	meshSurfaceChecker.push_back(2.8);
	meshSurfaceChecker.push_back(3.0);

	for (int i = 0; i < meshPositionsChecker.size(); ++i)
	{
		checkClose(meshPositionsChecker[i], mesh.centerPoint(i), 1e-8);
	}

	for (int i = 0; i < meshSurfaceChecker.size() - 1 ; ++i)
	{
		checkClose(meshSurfaceChecker[i], mesh.westFrontier(i), 1e-8);
	}
	int lastFrontierIndex = meshSurfaceChecker.size() - 1;
	checkClose(meshSurfaceChecker[lastFrontierIndex], mesh.eastFrontier(lastFrontierIndex - 1),1e-8);
}

TestCase(DoubleWallAnalyticalSolutionTest)
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

	AnalyticalSolution doubleWallAN(info);
	vector<double> exactTemperatureField;
	exactTemperatureField.push_back(343.75);
	exactTemperatureField.push_back(341.25);
	exactTemperatureField.push_back(338.75);
	exactTemperatureField.push_back(336.25);
	exactTemperatureField.push_back(333.75);
	exactTemperatureField.push_back(328.75);
	exactTemperatureField.push_back(321.25);
	exactTemperatureField.push_back(313.75);
	
	for (int i = 0; i < (info.numberOfNodes1+info.numberOfNodes2); ++i)
	{
		checkClose(exactTemperatureField[i],doubleWallAN[i],1e-8);
	}
}