/*
 * cplex_utils.cpp
 *
 *  Created on: 9 janv. 2016
 *      Author: hassan
 */
#include "cplex_utils.hpp"


DataVarBoolMatrix::DataVarBoolMatrix(string name, IloEnv env,int n, int m){
	this->Matrix = IloArray<IloBoolVarArray> (env,n);
	for(int i = 0; i < n; i++){
		this->Matrix[i] = IloBoolVarArray(env,m);
		for(int j = 0; j < m; j++){
			string var_name = name;//attribut à nom de la forme x(i,j) à la variable
			var_name += "(";
			var_name += U::to_s(i+1);
			var_name += ",";
			var_name += U::to_s(j+1);
			var_name += ")";

			this->Matrix[i][j] = IloBoolVar(env,var_name.c_str());
		}
	}

	this->n = n;
	this->m = m;
}

DataVarBoolTriMatrix::DataVarBoolTriMatrix(string name, IloEnv env,int n, int m, int K){
	this->Matrix = IloArray< IloArray<IloBoolVarArray> > (env,n);
	for(int i = 0; i < n; i++){
		string tmp_name = name;
		tmp_name += "(";
		tmp_name += U::to_s(i+1);
		tmp_name += ",";
		this->Matrix[i] = IloArray<IloBoolVarArray>(env,m);
		for(int j = 0; j < m; j++){
			string var_name = name;//attribut à nom de la forme x(i,j) à la variable
			var_name += U::to_s(j+1);
			var_name += ",";
			this->Matrix[i][j] = IloBoolVarArray(env,K);
			for(int k = 0; k < K; k++){
				var_name += U::to_s(k);
				var_name += ")";

				this->Matrix[i][j][k] = IloBoolVar(env,var_name.c_str());
			}
		}
	}

	this->n = n;
	this->m = m;
	this->k = K;
}
DataVarNumMatrix::DataVarNumMatrix(string name, double borne, IloEnv env,int n, int m){
	this->Matrix = IloArray<IloNumVarArray> (env,n);
	for(int i = 0; i < n; i++){
		this->Matrix[i] = IloNumVarArray(env,m,0,borne);
		for(int j = 0; j < m; j++){
			string var_name = name;//attribut à nom de la forme x(i,j) à la variable
			var_name += "(";
			var_name += U::to_s(i+1);
			var_name += ",";
			var_name += U::to_s(j+1);
			var_name += ")";


			this->Matrix[i][j] = IloNumVar(env,0,borne,var_name.c_str());

		}
	}

	this->n = n;
	this->m = m;
}

DataBoolMatrix::DataBoolMatrix(IloEnv env,int n, int m){
	this->Matrix = IloArray<IloBoolArray> (env,n);
	for(int i = 0; i < n; i++){
		this->Matrix[i] = IloBoolArray(env,m);
	}

	this->n = n;
	this->m = m;
}

DataExprMatrix::DataExprMatrix(IloEnv env,int n, int m){
	this->Matrix = IloArray<IloExprArray> (env,n);
	for(int i = 0; i < n; i++){
		this->Matrix[i] = IloExprArray(env,m);
	}

	this->n = n;
	this->m = m;
}

DataVarBoolTriMatrix::~DataVarBoolTriMatrix(){

}

DataBoolMatrix::~DataBoolMatrix(){

}

DataVarBoolMatrix::~DataVarBoolMatrix(){

}
DataVarNumMatrix::~DataVarNumMatrix(){

}
DataExprMatrix::~DataExprMatrix(){

}


bool DataVarBoolMatrix::ExtractSol(IloEnv env, IloCplex cplex, Solution& sol, DataVarBoolMatrix x_var){
	for(int i = 1; i <= sol.get_inst().get_n(); i++){
		for(int j = 1; j <= sol.get_inst().get_m(); j++){
			Coordinate coord(i,j);
			sol.set_Choix_maille(coord) = cplex.getValue(x_var[coord]);
		}
	}
	return true;
}

bool DataVarBoolTriMatrix::ExtractSol(IloEnv env, IloCplex cplex){
	for(int l = 0; l < this->k; l++){
		cout << "S : " << l << endl;
		cout << "----------------------" << endl;
 		for(int j = 1; j <= this->m; j++){
			for(int i = 1; i <= this->n; i++){

					cout << int(cplex.getValue(this->Matrix[i-1][j-1][l])) ;
			}
			cout << endl;
		}
 		cout << "----------------------" << endl;
	}
	return true;
}


