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

AnalyticalSolution::AnalyticalSolution(TransientPlainWallInfo data):
	mesh(data.numberOfNodes, data.wallLength, data.gridType),
	vectorK(data.numberOfNodes,data.thermalConduction),
	boundaries(data.beginBoundaryConditionType, data.endBoundaryConditionType, data.beginBoundaryConditionInfo, data.endBoundaryConditionInfo)
{
	
	double Bi = 2.05;
	vector<double> csi;
	vector<double> Cn;
	double crit = 1e-13;
	int numerodetermos = 10;
	double k = 0.7;
	double cp = 700;
	double L = 0.4;
	double ro = 2000;
	double To = 20;
	double Tinf = 100;
	double h = Bi*k/L;
	double DELTAt=0;
	double alpha;
	alpha = k/(ro*cp);
	double wallLength = mesh.getWallLength();
	
	vector<double> Tinicial;
	vector<double>thetas;
	thetas.resize(mesh.getNumberOfNodes());
	vector<double> Tanalit;
	Tanalit.resize(mesh.getNumberOfNodes());

	ofstream pFile;
	pFile.open("../results/third_task/analytical_solution.csv");

	for (int i = 0; i < mesh.getNumberOfNodes(); ++i)
	{
		pFile << mesh.centerPoint(i);
		if(i!=mesh.getNumberOfNodes()-1)
			pFile << ", ";
		else		
			pFile << endl;
	}
		
	for(int i = 0; i < 872; i++)
	{
		DELTAt += data.timeStep;
		pFile << DELTAt << ", " << endl;
		
		double Fo = alpha*DELTAt/pow(wallLength,2);
		if (i%100 == 0)
		{
			for(int j=0; j<mesh.getNumberOfNodes(); j++)
			{
				Tinicial.push_back(To);
				thetas[j] = getSolucaoAnalitica(Bi, Fo, mesh.centerPoint(j)/wallLength, crit);
				
				Tanalit[j] = thetas[j]*(To-Tinf)+Tinf;
				pFile << Tanalit[j];
				if(j!=mesh.getNumberOfNodes()-1)
					pFile << ", ";
				else		
					pFile << endl;
			}
		}
		
	}
	pFile.close();
}

AnalyticalSolution::AnalyticalSolution(PlainWallNonLinearInfo data):
	mesh(data.numberOfNodes, data.wallLength, data.gridType),
	vectorK(data.numberOfNodes,0.0),
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

void AnalyticalSolution::printTransientSolutionOnTheScreen()
{
	cout << "Analytical Solution" << endl;
	//cout << setw(21) << << "Position of the nodes, " << setw(24) << "Temperature of the Nodes" << endl;
	for (int i = 0; i < this -> transientTemperatureField[0].size(); i++)
	{
		for (int j = 0; j < this -> transientTemperatureField.size(); ++j)
		{
			cout << i << ", " << mesh.centerPoint(j) << ", " << setw(24) << this -> transientTemperatureField[i][j] << endl;
		}
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

double AnalyticalSolution::evalNonLinearTemperatureLaw(double x)
{
	//the output of this function is expressed in degrees celsius and the input must be in meters.
	double value;
	value = 160 - sqrt(3600 +160000 * x);
}

double AnalyticalSolution::getZetaNumber(double initialGuess)
{
	double xi = initialGuess;
	double diff = 1000;
	int count = 0;
	double x = 0;
	while((diff>1e-3)&& (count < 100))
	{
		double aux = calculateFunction(xi)/calculateDerivate(xi);
		x = xi - aux;
		diff =abs(aux);
		xi = x;
		count++;
	}
	return x;
}





double AnalyticalSolution::calculateFunction(double x)
{
	return x*tan(x) - 2.05;
}

double AnalyticalSolution::calculateDerivate(double x)
{
	double sec, secSquare;
	sec = 1 / cos(x);
	secSquare = sec*sec;
	return tan(x) + x*secSquare;
}

double AnalyticalSolution::getcsi(double Bi, double xo, double crit)
{
	double x;
	int cont = 0;
	int iteracaomaxima=100;
	double erro = 1;
	while(crit<erro && iteracaomaxima>cont)
	{
		cont++;
		x = xo-(Bi-xo*tan(xo))/(-(tan(xo)+xo*pow(1/cos(xo),2)));
		erro = fabs(x - xo);
		xo = x;
	}
	return (x);
}
double AnalyticalSolution::getCn(double csi)
{
	double Cn=(4*sin(csi)/(2*csi+sin(2*csi)));
	return(Cn);
}
double AnalyticalSolution::getSolucaoAnalitica(double Bi, double Fo, double xc, double crit)
{
	double pi = 3.14159265359;
	double theta = 0;
	int cont = 0;
	int iteracaomaxima=100;
	double soma = crit+1;
	double xo=1;
	double Cn;
	double csi;
	while((crit<soma || soma<-crit) && iteracaomaxima>cont)
	{
		csi = getcsi(Bi, xo, crit);
		Cn = getCn(csi);
		soma = Cn*exp(-(pow(csi,2)*Fo))*cos(csi*xc);
		//cout<<endl<<endl<<"	"<<soma<<"	"<<cont<<endl<<endl;
		theta = theta + soma;
		cont ++;
		xo = cont*pi;
	}
	return (theta);
}