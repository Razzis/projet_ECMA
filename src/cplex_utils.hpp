/*
 * cplex_utils.hpp
 *
 *  Created on: 9 janv. 2016
 *      Author: hassan
 */

#ifndef CPLEX_UTILS_HPP_
#define CPLEX_UTILS

#include "instance.hpp"
#include "solution.hpp"
#include <ilcplex/ilocplex.h>

ILOSTLBEGIN


// Classe utilitaire pour cplex
class DataVarBoolMatrix{
private:
	int n;
	int m;
	IloArray<IloBoolVarArray> Matrix;

public:
	DataVarBoolMatrix(string name, IloEnv env,int n, int m);
	~DataVarBoolMatrix();

	IloBoolVar& operator[](Coordinate coord){return Matrix[coord.x-1][coord.y-1];}// On part de 0 dans les vecteurs

	//Extrait la solution du plne et la recopie dans la variable solution
	bool ExtractSol(IloEnv env, IloCplex cplex, Solution& sol, DataVarBoolMatrix x_var);
};

class DataVarBoolTriMatrix{
private:
	int n;
	int m;
	int k;
	IloArray < IloArray<IloBoolVarArray> > Matrix;

public:
	DataVarBoolTriMatrix(string name, IloEnv env,int n, int m, int k);
	~DataVarBoolTriMatrix();

	IloBoolVar& get(int i, int j, int k){return Matrix[i-1][j-1][k];}// On part de 0 dans les vecteurs
	bool ExtractSol(IloEnv env, IloCplex cplex);
	//Extrait la solution du plne et la recopie dans la variable solution
	//bool ExtractSol(IloEnv env, IloCplex cplex, Solution& sol, DataVarBoolMatrix x_var);
};

class DataVarNumMatrix{
private:
	int n;
	int m;
	IloArray<IloNumVarArray> Matrix;

public:
	DataVarNumMatrix(string name, double borne, IloEnv env,int n, int m);
	~DataVarNumMatrix();

	IloNumVar& operator[](Coordinate coord){return Matrix[coord.x-1][coord.y-1];}// On part de 0 dans les vecteurs

	//Extrait la solution du plne et la recopie dans la variable solution
	//bool ExtractSol(IloEnv env, IloCplex cplex, Solution& sol, DataVarNumMatrix x_var);
};

class DataBoolMatrix{
private:
	int n;
	int m;
	IloArray<IloBoolArray> Matrix;

public:
	DataBoolMatrix(IloEnv env,int n, int m);
	~DataBoolMatrix();

	IloBool& operator[](Coordinate coord){return Matrix[coord.x-1][coord.y-1];}// On part de 0 dans les vecteurs
};

class DataExprMatrix{
private:
	int n;
	int m;
	IloArray<IloExprArray> Matrix;

public:
	DataExprMatrix(IloEnv env,int n, int m);
	~DataExprMatrix();

	IloNumExpr& operator[](Coordinate coord){return Matrix[coord.x-1][coord.y-1];}// On part de 0 dans les vecteurs
};



#endif
