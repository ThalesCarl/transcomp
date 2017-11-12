#include "LinearSystem.h"

LinearSystem::LinearSystem(int numberOfNodes): matrixOrder(numberOfNodes)
{	
	allocSolutionVector(numberOfNodes);
	allocIndependentValuesVector(numberOfNodes);
	allocMatrixOfCoeficients(numberOfNodes);
    
}

double LinearSystem::operator[](const int index)
{
	return this -> solution[index];

}

void LinearSystem::setValueToMatrix(int row, int col, double value)
{
	this -> matrixOfCoeficients[row][col] = value;

}
void LinearSystem::setValueToVector(int index, double value)
{
	this -> independentValues[index] = value;

}

void LinearSystem::solve()
{
    KSP            ksp;          /* linear solver context */
    Vec            x,b;          /* solution, residual vectors */
    Mat            A;            
    PetscInt n = this -> matrixOrder;
    PetscScalar value;      
    //PetscInt i = 0;

   
    /*----------------------------------------------------
        CRIAÇÃO DA MATRIZ A(COEFICIENTES LINEARES)
    -----------------------------------------------------*/
    MatCreate(PETSC_COMM_WORLD,&A);
    MatSetSizes(A,PETSC_DECIDE,PETSC_DECIDE,n,n);
    MatSetType(A,MATSEQAIJ);
    MatSetUp(A);
    
    for (int i = 0; i < n; i++)
    {
        for ( int j = 0; j < n; j++)
        {
            value = this -> matrixOfCoeficients[i][j];
            MatSetValues(A, 1, &i, 1, &j, &value, INSERT_VALUES);
        }
    }   
    MatAssemblyBegin(A, MAT_FINAL_ASSEMBLY);
    MatAssemblyEnd(A, MAT_FINAL_ASSEMBLY);
    
    /*---------------------------------------------
        CRIAÇÃO VETOR B (TERMOS INDEPENDENTES)
    ---------------------------------------------*/
    VecCreateSeq(PETSC_COMM_SELF, n, &b);
    for (int i = 0; i < n; i++)
    {
        value = this -> independentValues[i];
        VecSetValues(b,1,&i,&value,INSERT_VALUES);
    }
    VecAssemblyBegin(b);
    VecAssemblyEnd(b);

    /*---------------------------------------------
        CRIAÇÃO VETOR X (SOLUÇÃO)
    ---------------------------------------------*/
    VecCreateSeq(PETSC_COMM_WORLD, n, &x);
    VecSet(x,0);
    VecAssemblyBegin(b);
    VecAssemblyEnd(b);

    // MatView(A,PETSC_VIEWER_STDOUT_WORLD);
    // VecView(b,PETSC_VIEWER_STDOUT_WORLD);
    // VecView(x,PETSC_VIEWER_STDOUT_WORLD);

    /*---------------------------------------------
        CRIAÇÃO E EXECUÇÃO DO SOLVER
    ---------------------------------------------*/
    KSPCreate(PETSC_COMM_WORLD,&ksp);
    KSPSetOperators(ksp,A,A);
    KSPSetFromOptions(ksp);
    KSPSolve(ksp,b,x);
    
    for (int i = 0; i < n; i++)
    {
        VecGetValues(x,1,&i,&value);
        this -> solution[i] = value;
    }
    
    // VecView(x,PETSC_VIEWER_STDOUT_WORLD);
    // /*---------------------------------------------
    //     DESTRUIÇÃO DOS OBJETOS DA PETSC CRIADOS
    // ---------------------------------------------*/
    VecDestroy(&x);
    VecDestroy(&b);
    MatDestroy(&A);
    KSPDestroy(&ksp);
}

void LinearSystem::allocIndependentValuesVector(int vectorSize)
{
	this -> independentValues.resize(vectorSize);
    for(int i = 0; i < vectorSize; i++)
    {
        this -> independentValues[i]= 0.0;
    }
}

void LinearSystem::allocSolutionVector(int vectorSize)
{
	this -> solution.resize(vectorSize);
    for(int i = 0; i < vectorSize; i++)
    {
        this -> solution[i]= 0.0;
    }
}

void LinearSystem::allocMatrixOfCoeficients(int matrixSize)
{
	this -> matrixOfCoeficients.resize(matrixSize);
	for (int i = 0; i < matrixSize; i++)
	{
		matrixOfCoeficients[i].resize(matrixSize);
        for (int j = 0; j < matrixSize; j++)
        {
            this -> matrixOfCoeficients[i][j] = 0.0;
        }
	}
}

void LinearSystem::printValues()
{
    cout << "Matriz inserida:" << endl;
    for (int i = 0; i < this -> matrixOrder; i++)
    {
        for (int j = 0; j < this -> matrixOrder; j++)
        {
            
            cout << this -> matrixOfCoeficients[i][j] << " ";
        }
        cout << endl;
    }
    cout << "Termos independentes inseridos:" << endl;
    for (int i = 0; i < this -> matrixOrder; i++)
    {
        cout << independentValues[i] << endl;
    }
}

void LinearSystem::printSolution()
{
    cout << "Printing the solution" << endl;
    for(int i= 0; i < this -> matrixOrder; i++)
    {
        cout << this -> solution[i] << endl;
    }
}