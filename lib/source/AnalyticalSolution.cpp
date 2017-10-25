#include <AnalyticalSolution.h>

AnalyticalSolution::AnalyticalSolution(PlainWallInfo data): 
	mesh(data.numberOfNodes, data.wallLength, data.gridType),
	vecotorK(data.numberOfNodes,data.thermalConduction),
	boundaries(data.beginBoundaryConditionType, data.endBoundaryConditionType, data.beginBoundaryConditionInfo, data.endBoundaryConditionInfo)		
{
	this -> temperatureField.resize(this -> mesh.getNumberOfNodes());
	double temporaryTemperatureValue;
	for (int node = 0; node < mesh.getNumberOfNodes(); node++)
	{
		temporaryTemperatureValue = evalFirstProblemTemperatureLaw(mesh.centerPoint(node));
		addToTemperatureField(node,temporaryTemperatureValue);
	}
}

AnalyticalSolution::AnalyticalSolution(DoublePlainWallInfo data): 
	mesh(data.numberOfNodes1, data.wallLength1, data.gridType1),
	vecotorK(data.numberOfNodes1,data.thermalConduction1,data.numberOfNodes2,data.thermalConduction2),
	boundaries(data.beginBoundaryConditionType, data.endBoundaryConditionType, data.beginBoundaryConditionInfo, data.endBoundaryConditionInfo)		
{
	double temporaryTemperatureValue;
	
	for (int node = 0; node < mesh.getNumberOfNodes(); node++)
	{
		temporaryTemperatureValue = evalSecondProblemTemperatureLawSecondMaterial(mesh.centerPoint(node));
		addToTemperatureField(node,temporaryTemperatureValue);
	}

	this -> mesh.addPlainWall(data.numberOfNodes2, data.wallLength2, data.gridType2);
	this -> temperatureField.resize(this -> mesh.getNumberOfNodes());

	for (int node = 0; node < mesh.getNumberOfNodes(); node++)
	{
		temporaryTemperatureValue = evalSecondProblemTemperatureLawFirstMaterial(mesh.centerPoint(node));
		addToTemperatureField(node,temporaryTemperatureValue);
	}
}


void AnalyticalSolution::writeSolutionToCsv(string directory, string fileName)
{
	string outputFileNameAndDirectory;
	outputFileNameAndDirectory = directory + "/" + fileName + ".csv";
	
	cout << "Analytical Solution results saved in " << outputFileNameAndDirectory << endl;
	ofstream pFile;
	pFile.open(outputFileNameAndDirectory,fstream::trunc);

	pFile << "nodesPosition, " << "nodesTemperature" << endl;
	pFile << fixed;
	for (int i = 0; i < this -> mesh.getNumberOfNodes(); i++)
	{
		pFile << mesh.centerPoint(i) << ", " << setprecision(20) << temperatureField[i] << endl;
	}
}
void AnalyticalSolution::printSolutionOnTheScreen()
{
	cout << "Analytical Solution" << endl;
	cout << setw(21) << "Position of the nodes, " << setw(24) << "Temperature of the Nodes" << endl;
	for (int i = 0; i < this -> mesh.getNumberOfNodes(); i++)
	{
		cout << setw(21) << mesh.centerPoint(i) << ", " << setw(24) << this -> temperatureField[i] << endl;
	}
}

double AnalyticalSolution::operator[](int index)
{
	return this -> temperatureField[index];

}

void AnalyticalSolution::addToTemperatureField(int index, double value)
{
	this -> temperatureField[index] = value;
}

double AnalyticalSolution::evalFirstProblemTemperatureLaw(double position)
{
	double beginPrescTemp = boundaries.getBeginBoundaryCondition();
	double endPrescTemp = boundaries.getEndBoundaryCondition();
	double wallLength = mesh.getWallLength();

	return beginPrescTemp + (endPrescTemp - beginPrescTemp)*(position/wallLength);
}

double AnalyticalSolution::evalSecondProblemTemperatureLawFirstMaterial(double position)
{
	double Tinf = bondaries.getEndBoundaryCondition();
	double q = bondaries.getBeginBoundaryCondition();
	double LA = mesh.getWallLength();
	double LA

}

double AnalyticalSolution::evalSecondProblemTemperatureLawSecondMaterial(double position)
{

}