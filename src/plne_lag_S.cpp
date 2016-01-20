/*
 * plne_lag_S.cpp
 *
 *  Created on: 12 janv. 2016
 *      Author: hassan
 */
#include "solver.hpp"



PLNE_lag_S::PLNE_lag_S(Instance inst, IloEnv env) : Solver::Solver(){
	cout << "PLNE_lag::PLNE_lag(Instance inst, IloEnv env)" << endl;
	this->name = "PLNE";
	this->sol = Solution(inst);
	this->inst = inst;
	this->env = env;
	/*this->x_ij = DataNumMatrix(env, inst.get_n(), inst.get_m());
	this->sum_k_S_ijk = DataNumMatrix(env, inst.get_n(), inst.get_m());*/
}

PLNE_lag_S::~PLNE_lag_S(){

}


bool PLNE_lag_S::solve(DataNumMatrix const& lagrange_multiplier, DataNumMatrix& sum_k_S_ijk){

	IloEnv env = this->env;

	//definition de l'environnement cplex

	IloModel model(env);

	logn5("PLNE_lag::solve environement cplex definit");
	// definition des variables
	int dist_max = floor(this->inst.get_n()*this->inst.get_m()/2);//distance maximal entre deux points pour la connexité


	DataVarBoolTriMatrix S("S", env, this->inst.get_n(),this->inst.get_m(), dist_max);

	IloExpr Cout(env);


	IloExpr Sum_S_ij0(env);

	logn5("PLNE_lag::solve variables definies");

	for(int i = 1; i <= this->inst.get_n(); i++){
		for(int j = 1; j <= this->inst.get_m(); j++){
			Coordinate coord(i,j);
			Sum_S_ij0 += S.get(i,j,0);
			IloExpr Sum_Sk(env);


			for(int k = 0; k < dist_max; k++){

				Sum_Sk += S.get(i,j,k);

				if(k != 0){
					string S_voisinage_cons_name = "S_voisinage";//attribut à nom de la forme A(i,j) à la variable

					S_voisinage_cons_name += "(";
					S_voisinage_cons_name += U::to_s(i);
					S_voisinage_cons_name += ",";
					S_voisinage_cons_name += U::to_s(j);
					S_voisinage_cons_name += ",";
					S_voisinage_cons_name += U::to_s(k);
					S_voisinage_cons_name += ")";


					//la contrainte de connexité doit prendre en compte tout les effets de bords ...

					if(i != 1 && j != 1 && i != this->inst.get_n() && j != this->inst.get_m()){
						IloConstraint S_voisinage_cons(S.get(i,j,k) <= S.get(i-1,j,k-1) + S.get(i+1,j,k-1) + S.get(i,j-1,k-1) + S.get(i,j+1,k-1));
						S_voisinage_cons.setName(S_voisinage_cons_name.c_str());
						model.add(S_voisinage_cons);
					}


					else if(i == 1 && j != 1 && i != this->inst.get_n() && j != this->inst.get_m()){
						IloConstraint S_voisinage_cons(S.get(i,j,k) <= S.get(i+1,j,k-1) + S.get(i,j-1,k-1) + S.get(i,j+1,k-1));
						S_voisinage_cons.setName(S_voisinage_cons_name.c_str());
						model.add(S_voisinage_cons);
					}else if(i != 1 && j == 1 && i != this->inst.get_n() && j != this->inst.get_m()){
						IloConstraint S_voisinage_cons(S.get(i,j,k) <= S.get(i-1,j,k-1) + S.get(i+1,j,k-1) + S.get(i,j+1,k-1));
						S_voisinage_cons.setName(S_voisinage_cons_name.c_str());
						model.add(S_voisinage_cons);
					}else if(i != 1 && j != 1 && i == this->inst.get_n() && j != this->inst.get_m()){
						IloConstraint S_voisinage_cons(S.get(i,j,k) <= S.get(i-1,j,k-1) + S.get(i,j-1,k-1) + S.get(i,j+1,k-1));
						S_voisinage_cons.setName(S_voisinage_cons_name.c_str());
						model.add(S_voisinage_cons);
					}else if(i != 1 && j != 1 && i != this->inst.get_n() && j == this->inst.get_m()){
						IloConstraint S_voisinage_cons(S.get(i,j,k) <= S.get(i-1,j,k-1) + S.get(i,j-1,k-1) + S.get(i+1,j,k-1));
						S_voisinage_cons.setName(S_voisinage_cons_name.c_str());
						model.add(S_voisinage_cons);
					}


					else if(i == 1 && j == 1 && i != this->inst.get_n() && j != this->inst.get_m()){
						IloConstraint S_voisinage_cons(S.get(i,j,k) <= S.get(i+1,j,k-1) + S.get(i,j+1,k-1));
						S_voisinage_cons.setName(S_voisinage_cons_name.c_str());
						model.add(S_voisinage_cons);
					}else if(i == 1 && j != 1 && i == this->inst.get_n() && j != this->inst.get_m()){
						IloConstraint S_voisinage_cons(S.get(i,j,k) <=  S.get(i,j-1,k-1) + S.get(i,j+1,k-1));
						S_voisinage_cons.setName(S_voisinage_cons_name.c_str());
						model.add(S_voisinage_cons);
					}else if(i == 1 && j != 1 && i != this->inst.get_n() && j == this->inst.get_m()){
						IloConstraint S_voisinage_cons(S.get(i,j,k) <= S.get(i+1,j,k-1) + S.get(i,j-1,k-1));
						S_voisinage_cons.setName(S_voisinage_cons_name.c_str());
						model.add(S_voisinage_cons);
					}else if(i != 1 && j == 1 && i == this->inst.get_n() && j != this->inst.get_m()){
						IloConstraint S_voisinage_cons(S.get(i,j,k) <= S.get(i-1,j,k-1) + S.get(i,j+1,k-1));
						S_voisinage_cons.setName(S_voisinage_cons_name.c_str());
						model.add(S_voisinage_cons);
					}else if(i != 1 && j != 1 && i == this->inst.get_n() && j == this->inst.get_m()){
						IloConstraint S_voisinage_cons(S.get(i,j,k) <=  S.get(i,j-1,k-1) + S.get(i-1,j,k-1));
						S_voisinage_cons.setName(S_voisinage_cons_name.c_str());
						model.add(S_voisinage_cons);
					}else if(i != 1 && j == 1 && i != this->inst.get_n() && j == this->inst.get_m()){
						IloConstraint S_voisinage_cons(S.get(i,j,k) <= S.get(i-1,j,k-1) + S.get(i+1,j,k-1));
						S_voisinage_cons.setName(S_voisinage_cons_name.c_str());
						model.add(S_voisinage_cons);
					}


					else if(i == 1 && j == 1 && i == this->inst.get_n() && j != this->inst.get_m()){
						IloConstraint S_voisinage_cons(S.get(i,j,k) <= S.get(i,j+1,k-1));
						S_voisinage_cons.setName(S_voisinage_cons_name.c_str());
						model.add(S_voisinage_cons);
					}else if(i == 1 && j == 1 && i != this->inst.get_n() && j == this->inst.get_m()){
						IloConstraint S_voisinage_cons(S.get(i,j,k) <= S.get(i+1,j,k-1));
						S_voisinage_cons.setName(S_voisinage_cons_name.c_str());
						model.add(S_voisinage_cons);
					}
					else if(i == 1 && j != 1 && i == this->inst.get_n() && j == this->inst.get_m()){
						IloConstraint S_voisinage_cons(S.get(i,j,k) <= S.get(i,j-1,k-1));
						S_voisinage_cons.setName(S_voisinage_cons_name.c_str());
						model.add(S_voisinage_cons);
					}
					else if(i != 1 && j == 1 && i == this->inst.get_n() && j == this->inst.get_m()){
						IloConstraint S_voisinage_cons(S.get(i,j,k) <= S.get(i-1,j,k-1));
						S_voisinage_cons.setName(S_voisinage_cons_name.c_str());
						model.add(S_voisinage_cons);
					}
				}
			}






			Cout += lagrange_multiplier.get(coord)*Sum_Sk;




		}
	}

	string Sum_S_ij0_cons_name = "Sum_Sij0";

	IloConstraint Sum_S_ij0_cons(Sum_S_ij0 == 1);
	Sum_S_ij0_cons.setName(Sum_S_ij0_cons_name.c_str());
	model.add(Sum_S_ij0_cons);


	// définition des contraintes



	// définition fonction objectif
	model.add(IloMaximize(env, Cout));


	// Résolution

	try{

		IloCplex cplex(model);

		//parametre de la résolution
		for(int i = 1; i <= this->inst.get_n(); i++){
			for(int j = 1; j <= this->inst.get_m(); j++){
				for(int k = 0; k < dist_max; k++){

					cplex.setDirection(S.get(i,j,k), IloCplex::BranchDown);//les Sijk sont presque toute nulle, donc en branch vers le bas de pref
				}

			}
		}
		// empêche l'affichage des solution au fur a mesure des iteration du plne
		cplex.setOut(env.getNullStream());
		//cplex.setParam(IloCplex::MIPDisplay,0);

		/*size_t pos = this->inst.get_name().find(".");
		string lp_name = this->inst.get_name().substr (0,pos);//enlève le .dat à la fin
		pos = lp_name.find("/");
		lp_name = lp_name.substr (pos+1);//enleve de data/ au début
		lp_name += ".lp";
		cplex.exportModel(lp_name.c_str());*/






		if(cplex.solve()){
			logn5("Résolution succeed ! ");

			//env.out() << "Solution status = " << cplex.getStatus() << endl;
			//env.out() << "Solution value  = " << cplex.getObjValue() << endl;

			this->sol.set_cost() = cplex.getObjValue();

			sum_k_S_ijk = S.ExtractSumK(env, cplex);
			//env.out() << "Values        = " << x_sol << endl;
			//cplex.getSlacks(x_sol, con);
			//env.out() << "Slacks        = " << vals << endl;


			//S.ExtractSol(env,cplex);


			Cout.end();
			cplex.end();
			model.end();





			return true;
		}else{


			Cout.end();

			U::die("PLNE::Solve Erreur dans la résultion du plne");
			return false;
		}
	}
	catch (IloException& e) {
	   cerr << "Concert exception caught: " << e << endl;
	   return false;
	}
	catch (...) {
	   cerr << "Unknown exception caught" << endl;
	   return false;
	}

}
