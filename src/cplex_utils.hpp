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
class DataNumMatrix;

class DataVarBoolMatrix{
private:
	int n;
	int m;
	IloArray<IloBoolVarArray> Matrix;
	string name;

public:
	DataVarBoolMatrix(string name, IloEnv env,int n, int m);
	~DataVarBoolMatrix();

	IloBoolVar& operator[](Coordinate coord){return Matrix[coord.x-1][coord.y-1];}// On part de 0 dans les vecteurs

	//Extrait la solution du plne et la recopie dans la variable solution
	DataNumMatrix ExtractSol(IloEnv env, IloCplex cplex, Solution& sol);
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
	DataNumMatrix ExtractSumK(IloEnv env, IloCplex cplex);

	//mise à jour des direction préférentile de recherche dans le branche and cut

	//void getDirections(IloCplex cplex, IloCplex::BranchDirectionArray dir) const;//

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

class DataNumMatrix{
private:
	int n;
	int m;
	IloArray<IloNumArray> Matrix;
	IloEnv env;

public:
	DataNumMatrix(IloEnv env,int n, int m);
	DataNumMatrix(){}//non implémenté car ne sert normalement jamais
	DataNumMatrix& operator=(const DataNumMatrix copy_sol);
	DataNumMatrix(const DataNumMatrix& copy_Matrix);
	~DataNumMatrix();

	void set_value(const double val);

	friend DataNumMatrix operator+(DataNumMatrix const& a, DataNumMatrix const& b);
	friend DataNumMatrix operator-(DataNumMatrix const& a, DataNumMatrix const& b);
	friend DataNumMatrix operator*(double const& coeff, DataNumMatrix const& a);
	friend ostream & operator <<(ostream &os, DataNumMatrix Matrix);

	IloNum& operator[](Coordinate coord) {return Matrix[coord.x-1][coord.y-1];}// On part de 0 dans les vecteurs
	IloNum get(Coordinate coord) const{return Matrix[coord.x-1][coord.y-1];}// On part de 0 dans les vecteurs
};

class DataExprMatrix{
private:
	int n;
	int m;
	IloEnv env;
	IloArray<IloExprArray> Matrix;

public:
	DataExprMatrix(IloEnv env,int n, int m);
	~DataExprMatrix();



	IloNumExpr& operator[](Coordinate coord){return Matrix[coord.x-1][coord.y-1];}// On part de 0 dans les vecteurs
};
/*
// A un certain moment du parcours de l'arbre, si un voisin d'un noeud déjà selectionné peut etre inséré en augmentant le handicap, on le fait sans réfléchir plus avant.
ILOUSERCUTCALLBACK6(CtCallback, DataVarBoolMatrix, x, Coordinate current_coord, Instance inst) {
	if(current_coord.y != 1){
		Coordinate coord_haut(current_coord.x, current_coord.y-1);


}

ILONODECALLBACK1(MySelect) {

}*/

#endif
