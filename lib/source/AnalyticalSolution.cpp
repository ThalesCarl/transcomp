#include <AnalyticalSolution.h>

AnalyticalSolution::AnalyticalSolution(PlainWallInfo data): 
	mesh(data.numberOfNodes, data.wallLength, data.gridType),
	boundaries(data.beginBoundaryConditionType, data.endBoundaryConditionType, data.beginBoundaryConditionInfo, data.endBoundaryConditionInfo)		
{
	this -> temperatureField.resize(this -> mesh.getNumberOfNodes());
	double temporaryTemperatureValue;
	for (int node = 0; node < mesh.getNumberOfNodes(); node++)
	{
		temporaryTemperatureValue = evalTemperatureLaw(mesh.centerPoint(node));
		addToTemperatureField(node,temporaryTemperatureValue);
	}
}

void AnalyticalSolution::writeSolutionToCsv(string directory, string fileName)
{
	string outputFileNameAndDirectory;
	outputFileNameAndDirectory = directory + "/" + fileName + ".csv";
	
	ofstream pFile;
	pFile.open(outputFileNameAndDirectory);

	pFile << "nodesPosition, " << "nodesTemperature" << endl;
	for (int i = 0; i < this -> mesh.getNumberOfNodes(); i++)
	{
		pFile << mesh.centerPoint(i) << ", " << temperatureField[i] << endl;
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

double AnalyticalSolution::evalTemperatureLaw(double position)
{
	double beginPrescTemp = boundaries.getBeginBoundaryCondition();
	double endPrescTemp = boundaries.getEndBoundaryCondition();
	double wallLength = mesh.getWallLength();

	return beginPrescTemp + (endPrescTemp - beginPrescTemp)*(position/wallLength);
}