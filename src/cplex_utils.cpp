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
	this->name = name;
	this->n = n;
	this->m = m;
}


DataVarBoolMatrix::~DataVarBoolMatrix(){
	for(int i = 0; i < this->n; i++){
		this->Matrix[i].end();
	}
	this->Matrix.end();

}


DataNumMatrix DataVarBoolMatrix::ExtractSol(IloEnv env, IloCplex cplex, Solution& sol){
	DataNumMatrix x_ij(env,sol.get_inst().get_n(),sol.get_inst().get_m());
	double Ca_x = 0;
	double Cp_x = 0;

	double CaHa_x = 0;
	double CpHp_x = 0;

	//int Cost = 0;


	for(int i = 1; i <= sol.get_inst().get_n(); i++){
		for(int j = 1; j <= sol.get_inst().get_m(); j++){
			Coordinate coord(i,j);
			//if(cplex.getValue(this->Matrix[i-1][j-1]) == 1)
			//	Cost++;
			Ca_x += sol.get_inst().get_grille(coord).get_Ca()*cplex.getValue(this->Matrix[i-1][j-1]);
			Cp_x += sol.get_inst().get_grille(coord).get_Cp()*cplex.getValue(this->Matrix[i-1][j-1]);

			CaHa_x += sol.get_inst().get_grille(coord).get_Ha()*sol.get_inst().get_grille(coord).get_Ca()*cplex.getValue(this->Matrix[i-1][j-1]);
			CpHp_x += sol.get_inst().get_grille(coord).get_Hp()*sol.get_inst().get_grille(coord).get_Cp()*cplex.getValue(this->Matrix[i-1][j-1]);

			sol.set_Choix_maille(coord) = cplex.getValue(this->Matrix[i-1][j-1]);
			x_ij[coord] = cplex.getValue(this->Matrix[i-1][j-1]);
		}
	}

	//sol.set_cost() = Cost;

	sol.set_Ha() = CaHa_x/Ca_x;
	sol.set_Hp() = CpHp_x/Cp_x;



	return x_ij;
}





DataVarBoolTriMatrix::DataVarBoolTriMatrix(string name, IloEnv env,int n, int m, int K){
	this->Matrix = IloArray< IloArray<IloBoolVarArray> > (env,n);
	for(int i = 0; i < n; i++){
		this->Matrix[i] = IloArray<IloBoolVarArray>(env,m);
		for(int j = 0; j < m; j++){
			this->Matrix[i][j] = IloBoolVarArray(env,K);
			for(int k = 0; k < K; k++){
				string var_name = name;
				var_name += "(";
				var_name += U::to_s(i+1);
				var_name += ",";
				var_name += U::to_s(j+1);
				var_name += ",";
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
DataVarBoolTriMatrix::~DataVarBoolTriMatrix(){
	for(int i = 0; i < this->n; i++){
		for(int j = 0; j < this->m; j++){
			this->Matrix[i][j].end();
		}
		this->Matrix[i].end();
	}
	this->Matrix.end();
}

DataNumMatrix DataVarBoolTriMatrix::ExtractSumK(IloEnv env, IloCplex cplex){
	DataNumMatrix res(env,this->n, this->m);

	for(int j = 1; j <= this->m; j++){
		for(int i = 1; i <= this->n; i++){
			Coordinate coord(i,j);
			res[coord] = 0;
			for(int l = 0; l < this->k; l++){
				res[coord] += cplex.getValue(this->get(i,j,l));

			}
		}
	}
	return res;
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
DataVarNumMatrix::~DataVarNumMatrix(){
	for(int i = 0; i < this->n; i++){
		this->Matrix[i].end();
	}

}




DataBoolMatrix::DataBoolMatrix(IloEnv env,int n, int m){

	this->Matrix = IloArray<IloBoolArray> (env,n);
	for(int i = 0; i < n; i++){
		this->Matrix[i] = IloBoolArray(env,m);
	}

	this->n = n;
	this->m = m;
}
DataBoolMatrix::~DataBoolMatrix(){
	for(int i = 0; i < this->n; i++){
		this->Matrix[i].end();
	}
	this->Matrix.end();
}

DataNumMatrix::DataNumMatrix(IloEnv env,int n, int m){
	this->Matrix = IloArray<IloNumArray> (env,n);
	for(int i = 0; i < n; i++){
		this->Matrix[i] = IloNumArray(env,m);
	}
	this->n = n;
	this->m = m;
	this->env = env;

}
DataNumMatrix::~DataNumMatrix(){
	for(int i = 0; i < this->n; i++){
		this->Matrix[i].end();
	}
	this->Matrix.end();

}

void DataNumMatrix::set_value(const double val){
	for(int i = 0; i < this->n; i++){
		for(int j = 0; j < this->m; j++){
			this->Matrix[i][j] = val;
		}
	}

}
DataNumMatrix::DataNumMatrix(const DataNumMatrix& copy_Matrix){
	this->n  = copy_Matrix.n;
	this->m = copy_Matrix.m;
	this->env = copy_Matrix.env;

	this->Matrix = IloArray<IloNumArray> (this->env,this->n);
	for(int i = 0; i < this->n; i++){
		this->Matrix[i] = IloNumArray(this->env,this->m);
	}

	for(int i = 1; i <= this->n; i++){
		for(int j = 1; j <= this->m; j++){
			Coordinate coord(i,j);
			this->Matrix[i-1][j-1]= copy_Matrix.get(coord);
		}
	}

}


DataNumMatrix& DataNumMatrix::operator=(const DataNumMatrix copy_Matrix){
	this->n  = copy_Matrix.n;
	this->m = copy_Matrix.m;
	this->env = copy_Matrix.env;

	this->Matrix = IloArray<IloNumArray> (this->env,this->n);
	for(int i = 0; i < this->n; i++){
		this->Matrix[i] = IloNumArray(this->env,this->m);
	}

	for(int i = 1; i <= this->n; i++){
		for(int j = 1; j <= this->m; j++){
			Coordinate coord(i,j);
			this->Matrix[i-1][j-1]= copy_Matrix.get(coord);
		}
	}

	return *this;
}

DataNumMatrix operator+(DataNumMatrix const& a, DataNumMatrix const& b){
	if(a.n != b.n || a.m != b.m)
		U::die("DataExprMatrix::DataExprMatrix operator+ : dimension mismatch");
	DataNumMatrix res(a.env,a.n,a.m);
	for(int i = 1; i <= a.n; i++){
		for(int j = 1; j <= a.m; j++){
			Coordinate coord(i,j);
			res[coord] = a.get(coord) + b.get(coord);
		}
	}

	return res;
}

DataNumMatrix operator-(DataNumMatrix const& a, DataNumMatrix const& b){
	if(a.n != b.n || a.m != b.m)
		U::die("DataExprMatrix::DataExprMatrix operator- : dimension mismatch");
	DataNumMatrix res(a.env,a.n,a.m);
	for(int i = 1; i <= a.n; i++){
		for(int j = 1; j <= a.m; j++){
			Coordinate coord(i,j);
			res[coord] = a.get(coord) - b.get(coord);
		}
	}

	return res;
}

DataNumMatrix operator*(double const& coeff, DataNumMatrix const& a){

	DataNumMatrix res(a.env,a.n,a.m);
	for(int i = 1; i <= a.n; i++){
		for(int j = 1; j <= a.m; j++){
			Coordinate coord(i,j);
			res[coord] = coeff*a.get(coord);
		}
	}

	return res;
}

ostream & operator <<(ostream &os, DataNumMatrix Matrix){
	os << "----------------------------------" << endl;
	for(int j = 1; j <= Matrix.m; j++){
		for(int i = 1; i <= Matrix.n; i++){
			Coordinate coord(i,j);
			os << Matrix.get(coord) << "|";
		}
		os << endl;
	}
	os << "----------------------------------" << endl;
	return os;
}


DataExprMatrix::DataExprMatrix(IloEnv env,int n, int m){
	this->Matrix = IloArray<IloExprArray> (env,n);
	for(int i = 0; i < n; i++){
		this->Matrix[i] = IloExprArray(env,m);
	}
	this->env = env;
	this->n = n;
	this->m = m;
}

DataExprMatrix::~DataExprMatrix(){
	for(int i = 0; i < this->n; i++){
		this->Matrix[i].end();

	}
	this->Matrix.end();
}




