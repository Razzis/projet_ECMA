/*
 * plne_lag_x.cpp
 *
 *  Created on: 12 janv. 2016
 *      Author: hassan
 */
#include "solver.hpp"



PLNE_lag_x::PLNE_lag_x(Instance inst, IloEnv env) : Solver::Solver(){
	cout << "PLNE_lag::PLNE_lag(Instance inst, IloEnv env)" << endl;
	this->name = "PLNE";
	this->sol = Solution(inst);
	this->inst = inst;
	this->env = env;
	/*this->x_ij = DataNumMatrix(env, inst.get_n(), inst.get_m());
	this->sum_k_S_ijk = DataNumMatrix(env, inst.get_n(), inst.get_m());*/
}

PLNE_lag_x::~PLNE_lag_x(){

}


bool PLNE_lag_x::solve(DataNumMatrix const& lagrange_multiplier, DataNumMatrix& x_ij){

	IloEnv env = this->env;

	//definition de l'environnement cplex

	IloModel model(env);

	logn5("PLNE_lag::solve environement cplex definit");
	// definition des variables
	DataVarBoolMatrix x("x",env,this->inst.get_n(),this->inst.get_m());


	DataVarNumMatrix A("a",inst.get_Ba(),env,this->inst.get_n(),this->inst.get_m());
	DataVarNumMatrix P("p",inst.get_Bp(),env,this->inst.get_n(),this->inst.get_m());

	DataExprMatrix A_expr(env,this->inst.get_n(),this->inst.get_m());
	DataExprMatrix P_expr(env,this->inst.get_n(),this->inst.get_m());
	IloNumVar a(env,0,inst.get_Ba(),"a");
	IloNumVar p(env,0,inst.get_Bp(),"p");

	IloExpr a_expr(env);
	IloExpr p_expr(env);
	IloExpr handicap(env);
	IloExpr Cout(env);




	logn5("PLNE_lag::solve variables definies");

	for(int i = 1; i <= this->inst.get_n(); i++){
		for(int j = 1; j <= this->inst.get_m(); j++){
			Coordinate coord(i,j);

			a_expr += x[coord] * this->inst.get_grille(coord).get_Ca();
			p_expr += x[coord] * this->inst.get_grille(coord).get_Cp();

			Cout += (1-lagrange_multiplier.get(coord))*x[coord];
			handicap += this->inst.get_grille(coord).get_Hp() * this->inst.get_grille(coord).get_Cp() * A[coord]
			         + this->inst.get_grille(coord).get_Ha() * this->inst.get_grille(coord).get_Ca() * P[coord]
			         - 2 * this->inst.get_grille(coord).get_Ca() * P[coord];



		}
	}



	IloConstraint a_def(a == a_expr);
	a_def.setName("a_def");
	model.add(a_def);

	IloConstraint p_def(p == p_expr);
	p_def.setName("p_def");
	model.add(p_def);

	IloConstraint handicap_const(handicap >= 0);
	handicap_const.setName("handicap");
	model.add(handicap_const);
	//model.add(p > 0);
	//model.add(a > 0);
	// définition des contraintes

	for(int i = 1; i <= this->inst.get_n(); i++){
		for(int j = 1; j <= this->inst.get_m(); j++){
			Coordinate coord(i,j);

			/* definition des noms des contraintes dans le lp */
			string const_A = "A_def";//attribut à nom de la forme A(i,j) à la variable
			const_A += "(";
			const_A += U::to_s(i);
			const_A += ",";
			const_A += U::to_s(j);
			const_A += ")";


			string const_P = "P_def";//attribut à nom de la forme P(i,j) à la variable
			const_P += "(";
			const_P += U::to_s(i);
			const_P += ",";
			const_P += U::to_s(j);
			const_P += ")";



			IloConstraint A_cons1(A[coord] <= inst.get_Ba()*x[coord]);
			IloConstraint A_cons2(A[coord] <= a);
			IloConstraint A_cons3(A[coord] >= (x[coord] - 1) * this->inst.get_Ba() + a);
			IloConstraint A_cons4(A[coord] >= 0);

			IloConstraint P_cons1(P[coord] <= inst.get_Bp()*x[coord]);
			IloConstraint P_cons2(P[coord] <= p);
			IloConstraint P_cons3(P[coord] >= (x[coord] - 1) * this->inst.get_Bp() + p);
			IloConstraint P_cons4(P[coord] >= 0);

			A_cons1.setName(const_A.c_str());
			A_cons2.setName(const_A.c_str());
			A_cons3.setName(const_A.c_str());
			A_cons4.setName(const_A.c_str());

			P_cons1.setName(const_P.c_str());
			P_cons2.setName(const_P.c_str());
			P_cons3.setName(const_P.c_str());
			P_cons4.setName(const_P.c_str());

			model.add(A_cons1);
			model.add(A_cons2);
			model.add(A_cons3);
			model.add(A_cons4);

			model.add(P_cons1);
			model.add(P_cons2);
			model.add(P_cons3);
			model.add(P_cons4);


		}
	}


	// définition fonction objectif
	model.add(IloMaximize(env, Cout));


	// Résolution

	try{

		IloCplex cplex(model);

		//parametre de la résolution
		for(int i = 1; i <= this->inst.get_n(); i++){
			for(int j = 1; j <= this->inst.get_m(); j++){

				Coordinate coord(i,j);
				if((1 <= inst.get_grille(coord).get_Ha() && 1 <= inst.get_grille(coord).get_Hp()) ||
						(0 == inst.get_grille(coord).get_Ca() && 0 == inst.get_grille(coord).get_Cp()))
					cplex.setDirection(x[coord], IloCplex::BranchUp);// Si Ca et Cp sont nul, ajouter x ne change pas l'admissibilité (sauf eventuellemnt la connexité) et augmente le cout
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
			x_ij = x.ExtractSol(env, cplex, this->set_solution());

			//env.out() << "Values        = " << x_sol << endl;
			//cplex.getSlacks(x_sol, con);
			//env.out() << "Slacks        = " << vals << endl;


			//S.ExtractSol(env,cplex);

			a.end();
			p.end();
			handicap.end();
			Cout.end();
			cplex.end();
			model.end();





			return true;
		}else{

			a.end();
			p.end();
			handicap.end();
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
