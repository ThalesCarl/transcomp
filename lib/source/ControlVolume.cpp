#include <ControlVolume.h>

ControlVolume::ControlVolume(PlainWallInfo data):	
		mesh(data.numberOfNodes, data.wallLength, data.gridType),
		vectorK(data.numberOfNodes, data.thermalConduction),
		boundaries(data.beginBoundaryConditionType, data.endBoundaryConditionType, data.beginBoundaryConditionInfo, data.endBoundaryConditionInfo), 
		solver(data.numberOfNodes),
		interfaceOperation(data.interfaceOperation)
{
	int n = this -> mesh.getNumberOfNodes();
	beginProcessor();
	for (int i = 1; i < n - 1; i++)
	{
		double aw , ae, ap;
		aw = vectorK.getWestInterface(mesh,i, this -> interfaceOperation)/this -> mesh.westDistance(i);
		ae = vectorK.getEastInterface(mesh,i,this -> interfaceOperation)/this -> mesh.eastDistance(i);
		ap = (aw + ae);
		solver.setValueToMatrix(i,i-1,(-1)*aw);
		solver.setValueToMatrix(i,i+1,(-1)*ae);
		solver.setValueToMatrix(i,i,ap);	
	}
	endProcessor();
	
	this -> solver.solve();

	this -> temperatureField.resize(n);
	for (int i = 0; i < n; i++)
	{
		this -> temperatureField[i] = this -> solver[i];
	}
}

ControlVolume::ControlVolume(DoublePlainWallInfo data):	
		mesh(data.numberOfNodes1, data.wallLength1, data.gridType1),
		vectorK(data.numberOfNodes1, data.thermalConduction1, data.numberOfNodes2,data.thermalConduction2),
		boundaries(data.beginBoundaryConditionType, data.endBoundaryConditionType, data.beginBoundaryConditionInfo, data.endBoundaryConditionInfo), 
		solver(data.numberOfNodes1 + data.numberOfNodes2),
		interfaceOperation(data.interfaceOperation)
{
	mesh.addPlainWall(data.numberOfNodes2,data.wallLength2,data.gridType2);

	int n = this -> mesh.getNumberOfNodes();
	beginProcessor();
	for (int i = 1; i < n - 1; i++)
	{
		double aw , ae, ap;
		aw = vectorK.getWestInterface(mesh,i, this -> interfaceOperation)/this -> mesh.westDistance(i);
		ae = vectorK.getEastInterface(mesh,i,this -> interfaceOperation)/this -> mesh.eastDistance(i);
		ap = (aw + ae);
		solver.setValueToMatrix(i,i-1,(-1)*aw);
		solver.setValueToMatrix(i,i+1,(-1)*ae);
		solver.setValueToMatrix(i,i,ap);	
	}
	endProcessor();
	this -> solver.solve();

	this -> temperatureField.resize(n);
	for (int i = 0; i < n; i++)
	{
		this -> temperatureField[i] = this -> solver[i];
	}
}

ControlVolume::ControlVolume(PlainWallNonLinearInfo data):	
		mesh(data.numberOfNodes, data.wallLength, data.gridType),
		vectorK(data.numberOfNodes, 0.0),
		boundaries(data.beginBoundaryConditionType, data.endBoundaryConditionType, data.beginBoundaryConditionInfo, data.endBoundaryConditionInfo), 
		solver(data.numberOfNodes),
		interfaceOperation(data.interfaceOperation)
{
	int n = this -> mesh.getNumberOfNodes();
	this -> temperatureField.resize(n);

	//First guess for the temperature field
	for (int i = 0; i < n; ++i)
	{
		double a = data.beginBoundaryConditionInfo.back();
		double b = data.endBoundaryConditionInfo.back();
		double L = data.wallLength;
		double x = mesh.centerPoint(i);
		this -> temperatureField[i] = a + ((b-a)*x)/(L);
	}

	

	this -> iterationCounter = 0;
	double maximumError = 1e6;
	double deltaTemperature = boundaries.getBeginBoundaryCondition() - boundaries.getEndBoundaryCondition();
	while((maximumError > data.tolerance) && (this -> iterationCounter < 1000))
	{
		vectorK.setNonLinearProblem(data.thermalConductionCoefficients, this ->temperatureField);

		beginProcessor();
		for (int i = 1; i < n - 1; i++)
		{
			double aw , ae, ap;
			aw = vectorK.getWestInterface(mesh,i, this -> interfaceOperation)/this -> mesh.westDistance(i);
			ae = vectorK.getEastInterface(mesh,i,this -> interfaceOperation)/this -> mesh.eastDistance(i);
			ap = (aw + ae);
			solver.setValueToMatrix(i,i-1,(-1)*aw);
			solver.setValueToMatrix(i,i+1,(-1)*ae);
			solver.setValueToMatrix(i,i,ap);	
		}
		endProcessor();
		
		this -> solver.solve();

		this -> oldTemperatureField = this -> temperatureField;
		for (int i = 0; i < n; i++)
		{
			this -> oldTemperatureField[i] = this -> temperatureField[i];
			this -> temperatureField[i] = this -> solver[i];
		}
		vector<double> errorsVec;
		for (int i = 0; i < n; ++i)
		{
			errorsVec.push_back((abs(temperatureField[i]-oldTemperatureField[i]))/deltaTemperature);
		}
		maximumError = *max_element(errorsVec.begin(),errorsVec.end());
		this -> iterationCounter++;
	}
}

ControlVolume::ControlVolume(TransientPlainWallInfo data):
	mesh(data.numberOfNodes, data.wallLength, data.gridType),
	vectorK(data.numberOfNodes, data.thermalConduction),
	boundaries(data.beginBoundaryConditionType, data.endBoundaryConditionType, data.beginBoundaryConditionInfo, data.endBoundaryConditionInfo), 
	solver(data.numberOfNodes),
	interfaceOperation(data.interfaceOperation)
{
	int n = this -> mesh.getNumberOfNodes();
	this -> temperatureField.resize(n);
	this -> oldTemperatureField.resize(n);
	this -> transientTemperatureField.resize(1000);
	for (int i = 0; i < oldTemperatureField.size(); ++i)
	{
		this -> oldTemperatureField[i] = data.initialTemperature;
		this -> temperatureField[i] = data.initialTemperature;
	}

	double diff = abs(oldTemperatureField[0] - this -> boundaries.getEndBoundaryCondition());

	int count = 0;
	double deltaT = data.timeStep;
	while ((diff>1e-3)&&(count < 10))
	{
		
		beginProcessorTransient(data,deltaT);
		for (int i = 1; i < n - 1; i++)
		{
			double ro = data.density;
			double cp = data.cp;
			double area = data.transversalArea;
			double deltaX = mesh.getDelta();				
			double aw = vectorK.getWestInterface(mesh,i, this -> interfaceOperation)/this -> mesh.westDistance(i);
			double ae = this -> vectorK.getEastInterface(this -> mesh, i, interfaceOperation)/mesh.eastDistance(i);
			double ap0 = (ro*cp*area*deltaX)/deltaT;

			solver.setValueToMatrix(i,i,ap0);
			double tE0 = this -> oldTemperatureField[i+1];
			double tW0 = this -> oldTemperatureField[i-1];
			double tP0 = this -> oldTemperatureField[0];
			double aux = ae*tE0 + aw*tW0 + tP0*(ap0 - ae - aw);
			solver.setValueToVector(i,ae*tE0 + aw*tW0 + tP0*(ap0 - ae - aw));	
		}
		endProcessorTransient(data,deltaT);
		
		this -> solver.solve();


		for (int i = 0; i < n; i++)
		{
			this -> oldTemperatureField[i] = this -> temperatureField[i];
			this -> temperatureField[i] = solver[i];
			cout << temperatureField[i] << endl;
		}
		diff = abs(temperatureField[0] - this -> boundaries.getEndBoundaryCondition());
		++count;
	}
	
	
}


void ControlVolume::writeSolutionToCsv(string directory, string fileName)
{
	
	string outputFileNameAndDirectory;
	outputFileNameAndDirectory = directory + "/" + fileName + ".csv";
	cout << "Finite Volume results saved in " << outputFileNameAndDirectory << endl;
	
	ofstream pFile;
	pFile.open(outputFileNameAndDirectory,fstream::trunc);

	pFile << "nodesPosition, " << "nodesTemperature" << endl;
	pFile << fixed;
	for (int i = 0; i < this -> mesh.getNumberOfNodes(); i++)
	{
		pFile << mesh.centerPoint(i) << ", " << setprecision(20) << this -> temperatureField[i] << endl;
	}
	pFile.close();
}

void ControlVolume::printSolutionOnTheScreen()
{
	cout << "Solution with finite volume method" << endl;
	cout << setw(21) << "Position of the nodes, " << setw(24) << "Temperature of the Nodes" << endl;
	for (int i = 0; i < this -> mesh.getNumberOfNodes(); i++)
	{
		cout << setw(21) << mesh.centerPoint(i) << ", " << setw(24) << this -> temperatureField[i] << endl;
	}
}

void ControlVolume::beginProcessor()
{
	
	FrontierType beginFrontierType = this -> mesh.getBeginFrontierType();
	BoundaryCondition beginBoundaryConditionType = this -> boundaries.getTypeBegin();
	
	if ((beginFrontierType == CONNECTED) && (beginBoundaryConditionType == PRESCRIBED_TEMPERATURE))
	{
		solver.setValueToMatrix(0,0,1);
		solver.setValueToVector(0, this -> boundaries.getBeginBoundaryCondition());
	}

	if ((beginFrontierType == CONNECTED) && (beginBoundaryConditionType == CONVECTION))
	{
		double ae = this -> vectorK.getEastInterface(this -> mesh, 0, interfaceOperation)/mesh.eastDistance(0);
		double ap = ae + this -> boundaries.getBeginConvectionCoeficient();
		solver.setValueToMatrix(0,0,ap);
		solver.setValueToMatrix(0,1,(-1)*ae);
		solver.setValueToVector(0, this -> boundaries.getBeginBoundaryCondition()* this -> boundaries.getBeginConvectionCoeficient());
	}
	
	if (beginBoundaryConditionType == PRESCRIBED_FLUX)
	{
		double ae = this -> vectorK.getEastInterface(this -> mesh, 0, this -> interfaceOperation)/this -> mesh.eastDistance(0);
		double ap = ae;
		solver.setValueToMatrix(0,0,ap);
		solver.setValueToMatrix(0,1,(-1)*ae);
		solver.setValueToVector(0, this -> boundaries.getBeginBoundaryCondition());
	}
	
	if ((beginFrontierType == UNCONNECTED) && (beginBoundaryConditionType == PRESCRIBED_TEMPERATURE))
	{
		double ae = this -> vectorK.getEastInterface(mesh, 0, this -> interfaceOperation)/this -> mesh.eastDistance(0);
		double aPre = this -> vectorK[0]/this -> mesh.westPlusDistance(0);
		double ap = ae + aPre;
		solver.setValueToMatrix(0,0,ap);
		solver.setValueToMatrix(0,1,(-1)*ae);
		solver.setValueToVector(0,aPre* (this -> boundaries.getBeginBoundaryCondition()));
	}
	
	if ((beginFrontierType == UNCONNECTED) && (beginBoundaryConditionType == CONVECTION))
	{
		double R;
		R = 1 / (this -> boundaries.getBeginConvectionCoeficient());
		R += this -> mesh.westPlusDistance(0)/this -> vectorK[0];
		double ae = this -> vectorK.getEastInterface(this -> mesh, 0, this -> interfaceOperation)/this -> mesh.eastDistance(0);
		double ap = ae +(1/R);
		solver.setValueToMatrix(0,0,ap);
		solver.setValueToMatrix(0,1,(-1)*ae);
		solver.setValueToVector(0,this -> boundaries.getBeginBoundaryCondition()/R);
	}
}

void ControlVolume::endProcessor()
{
	int n = this -> mesh.getNumberOfNodes();
	FrontierType endFrontierType = this -> mesh.getEndFrontierType();
	BoundaryCondition endBoundaryConditionType = this -> boundaries.getTypeEnd();
	
	if ((endFrontierType == CONNECTED) && (endBoundaryConditionType == PRESCRIBED_TEMPERATURE))
	{
		solver.setValueToMatrix(n-1,n-1,1);
		double seila = this -> boundaries.getEndBoundaryCondition();
		solver.setValueToVector(n-1, this -> boundaries.getEndBoundaryCondition());
	}

	if ((endFrontierType == CONNECTED) && (endBoundaryConditionType == CONVECTION))
	{
		double aw = this -> vectorK.getWestInterface(this -> mesh, n-1, interfaceOperation)/mesh.westDistance(n-1);
		double ap = aw + this -> boundaries.getEndConvectionCoeficient();
		solver.setValueToMatrix(n-1,n-1,ap);
		solver.setValueToMatrix(n-1,n-2,(-1)*aw);
		solver.setValueToVector(n-1, this -> boundaries.getEndBoundaryCondition()* this -> boundaries.getEndConvectionCoeficient());
	}
	
	if (endBoundaryConditionType == PRESCRIBED_FLUX)
	{
		double aw = this -> vectorK.getWestInterface(this -> mesh, n-1, this -> interfaceOperation)/this -> mesh.westDistance(n-1);
		double ap = aw;
		solver.setValueToMatrix(n-1,n-1,ap);
		solver.setValueToMatrix(n-1,1,(-1)*aw);
		solver.setValueToVector(n-1, this -> boundaries.getEndBoundaryCondition());
	}
	
	if ((endFrontierType == UNCONNECTED) && (endBoundaryConditionType == PRESCRIBED_TEMPERATURE))
	{
		double aw = this -> vectorK.getWestInterface(mesh, n-1, this -> interfaceOperation)/this -> mesh.westDistance(n-1);
		double aPre = this -> vectorK[n-1]/this -> mesh.eastMinusDistance(n-1);
		double ap = aw + aPre;
		solver.setValueToMatrix(n-1,n-1,ap);
		solver.setValueToMatrix(n-1,n-2,(-1)*aw);
		solver.setValueToVector(n-1,aPre* (this -> boundaries.getEndBoundaryCondition()));
	}
	
	if ((endFrontierType == UNCONNECTED) && (endBoundaryConditionType == CONVECTION))
	{
		double R;
		R = 1 / (this -> boundaries.getEndConvectionCoeficient());
		R += this -> mesh.eastMinusDistance(n-1)/this -> vectorK[n-1];
		double aw = this -> vectorK.getWestInterface(this -> mesh, n-1, this -> interfaceOperation)/this -> mesh.westDistance(n-1);
		double ap = aw +(1/R);
		solver.setValueToMatrix(n-1,n-1,ap);
		solver.setValueToMatrix(n-1,n-2,(-1)*aw);
		solver.setValueToVector(n-1,this -> boundaries.getEndBoundaryCondition()/R);
	}	
}

void ControlVolume::beginProcessorTransient(TransientPlainWallInfo data, double deltaT)
{
	FrontierType beginFrontierType = this -> mesh.getBeginFrontierType();
	BoundaryCondition beginBoundaryConditionType = this -> boundaries.getTypeBegin();

	if((beginFrontierType == CONNECTED)&&(beginBoundaryConditionType == PRESCRIBED_FLUX))
	{
		double q = this -> boundaries.getBeginBoundaryCondition();
		double ro = data.density;
		double cp = data.cp;
		double area = data.transversalArea;
		double deltaX = 0.5 * mesh.getDeltaBegin();
		double ke = this -> vectorK.getEastInterface(this -> mesh, 0, interfaceOperation);
		double dxe = mesh.eastDistance(0);
		double ae = area * ke/dxe;
		double ap0 = (ro*cp*area*deltaX)/deltaT;
		solver.setValueToMatrix(0,0,ap0);
		double tE0 = this -> oldTemperatureField[1];
		double tP0 = this -> oldTemperatureField[0];

		this -> solver.setValueToVector(0,ae*tE0+tP0*(ap0 - ae)-q);
	}

	//eventualmente implementar as outras possibilidades, or not.
}

void ControlVolume::endProcessorTransient(TransientPlainWallInfo data, double deltaT)
{
	int n = this -> mesh.getNumberOfNodes();
	FrontierType endFrontierType = this -> mesh.getEndFrontierType();
	BoundaryCondition endBoundaryConditionType = this -> boundaries.getTypeEnd();

	if((endFrontierType == CONNECTED)&&(endBoundaryConditionType == CONVECTION))
	{
		double ro = data.density;
		double cp = data.cp;
		double area = data.transversalArea;
		double deltaX = 0.5*mesh.getDeltaBegin();
		
		double aw = area * this -> vectorK.getWestInterface(this -> mesh, n-1, interfaceOperation)/mesh.westDistance(n-1);
		double ap0 = (ro*cp*area*deltaX)/deltaT;
		
		

		solver.setValueToMatrix(n-1,n-1,ap0);
		double tW0 = this -> oldTemperatureField[n-2];
		double tP0 = this -> oldTemperatureField[n-1];
		double h = boundaries.getEndConvectionCoeficient();
		double tInf = boundaries.getEndBoundaryCondition();
		double aux = aw*tW0 +tInf*h*area + tP0*(-h*area + ap0 - aw);
		solver.setValueToVector(n-1,aux);
	}
}
double ControlVolume::getTemperature(int ControlVolumeIndex)
{
	return this -> temperatureField[ControlVolumeIndex];
}

double ControlVolume::getPosition(int ControlVolumeIndex)
{
	return this -> mesh.centerPoint(ControlVolumeIndex);
}


int ControlVolume::getIterationCounter()
{
	return this -> iterationCounter;
}


