#include <AnalyticalSolution.h>

AnalyticalSolution::AnalyticalSolution(PlainWallInfo data): 
	mesh(data.numberOfNodes, data.wallLength, data.gridType),
	vectorK(data.numberOfNodes,data.thermalConduction),
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
	vectorK(data.numberOfNodes1,data.thermalConduction1,data.numberOfNodes2,data.thermalConduction2),
	boundaries(data.beginBoundaryConditionType, data.endBoundaryConditionType, data.beginBoundaryConditionInfo, data.endBoundaryConditionInfo)		
{
	double temporaryTemperatureValue;	
	this -> temperatureField.resize(mesh.getNumberOfNodes());
	int numberOfNodes1 = mesh.getNumberOfNodes();
	for (int node = 0; node < numberOfNodes1; node++)
	{
		temporaryTemperatureValue = evalSecondProblemTemperatureLaw(mesh.centerPoint(node),data);
		addToTemperatureField(node,temporaryTemperatureValue);
	}
	this -> mesh.addPlainWall(data.numberOfNodes2, data.wallLength2, data.gridType2);
	this -> temperatureField.resize(this -> mesh.getNumberOfNodes());

	for (int node = numberOfNodes1; node < mesh.getNumberOfNodes(); node++)
	{
		temporaryTemperatureValue = evalSecondProblemTemperatureLaw(mesh.centerPoint(node),data);
		addToTemperatureField(node,temporaryTemperatureValue);
	}
}

// AnalyticalSolution(TransientPlainWallInfo data):
// 	mesh(data.numberOfNodes, data.wallLength, data.gridType),
// 	vectorK(data.numberOfNodes,data.thermalConduction),
// 	boundaries(data.beginBoundaryConditionType, data.endBoundaryConditionType, data.beginBoundaryConditionInfo, data.endBoundaryConditionInfo)
// {
// 	this -> transientTemperatureField[0].resize(mesh.getNumberOfNodes());
// 	for (int i = 0; i < mesh.getNumberOfNodes(); ++i)
// 	{
// 		this -> transientTemperatureField[0][i] = data.initialTemperature;
// 	}
	
// 	vector<double> zetaNumbers.(50);
// 	getZetaNumbers(data.biotNumber, zetaNumbers)
	
// 	double timePosition = 0;
// 	double Tinf = this -> boundaries.getEndBoundaryCondition()
// 	double diff = abs(this -> transientTemperatureField[timePosition][0] - Tinf;
// 	while (diff > 1)
// 	{
// 		double alpha = data.thermalConduction/(data.density*data.cp)
// 		double fourierNumber = (alpha*timePosition)/(data.wallLength*data.wallLength);
// 		sum = 0;
// 		for (int i = 0; i < zetaNumbers.size(); ++i)
// 		{
// 			double zeta = zetaNumbers[i]
// 			double cn = (4*sin(zeta))/(2*zeta+sin(2*zeta));
// 		}
		
// 		diff = abs(this -> transientTemperatureField[timePosition][0] - Tinf);
// 		timePosition += data.timeStep;
// 	}

AnalyticalSolution(PlainWallNonLinearInfo data):
	mesh(data.numberOfNodes, data.wallLength, data.gridType),
	vectorK(data.numberOfNodes,data.thermalConduction),
	boundaries(data.beginBoundaryConditionType, data.endBoundaryConditionType, data.beginBoundaryConditionInfo, data.endBoundaryConditionInfo)		
{
	this -> temperatureField.resize(this -> mesh.getNumberOfNodes());
	double temporaryTemperatureValue;
	for (int node = 0; node < mesh.getNumberOfNodes(); node++)
	{
		temporaryTemperatureValue = evalNonLinearTemperatureLaw(mesh.centerPoint(node));
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

double AnalyticalSolution::evalSecondProblemTemperatureLaw(double position, DoublePlainWallInfo data)
{
	double Tinf = boundaries.getEndBoundaryCondition();
	double q = boundaries.getBeginBoundaryCondition();
	double h = boundaries.getEndConvectionCoeficient();
	double LA = data.wallLength1;
	double LB = data.wallLength2;
	double KA = data.thermalConduction1;
	double KB = data.thermalConduction2;
	double R = (LA/KA)+(LB/KB)+(1/h);
	double T0 = Tinf + q * R;
	if(position <= LA)
		return T0 - (q/KA)*position;
	else if ((position > LA) && (position <= LA + (1.01)*LB))
	{
		double TLA = T0 - (q/KA)*LA;
		R = (LA/KA) + (LB/KB);
		double TL = T0 - q * R;
		double d1 = -(h/KB) * (TL - Tinf);
		double d2 = TLA + (h/KB)*(TL - Tinf)*LA;
		return d2 + d1 * position;
	}
	else
	{
		cout << "Invalid position _ AN" << endl;
		return -100;
	}
}

double evalNonLinearTemperatureLaw(double x)
{
	//the output of this function is expressed in degrees celsius and the input must be in meters.
	double value;
	value = 160 - sqrt(3600 +160000 * x);
}