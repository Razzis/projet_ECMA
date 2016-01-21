/*
 * plne.cpp
 *
 *  Created on: 12 janv. 2016
 *      Author: hassan
 */
#include "solver.hpp"



//GreedySolver::GreedySolver(Instance* inst) : Solver::Solver(inst)


PLNE::PLNE(Instance inst) : Solver::Solver(){
	this->name = "PLNE";
	this->sol = Solution(inst);
	this->inst = inst;
}

PLNE::~PLNE(){

}


bool PLNE::solve(){


	//typedef IloArray<IloNumArray> DataMatrix;
	//typedef IloArray<IloBoolVarArray> DataVarBoolMatrix;

	//definition de l'environnement cplex
	IloEnv env;
	IloModel model(env);

	// definition des variables
	DataVarBoolMatrix x("x",env,this->inst.get_n(),this->inst.get_m());
	int dist_max = floor(this->inst.get_n()*this->inst.get_m()/2);//distance maximal entre deux points pour la connexité


	DataVarBoolTriMatrix S("S", env, this->inst.get_n(),this->inst.get_m(), dist_max);

	DataVarNumMatrix A("a",inst.get_Ba(),env,this->inst.get_n(),this->inst.get_m());
	DataVarNumMatrix P("p",inst.get_Bp(),env,this->inst.get_n(),this->inst.get_m());

	//DataExprMatrix A_expr(env,this->inst.get_n(),this->inst.get_m());
	//DataExprMatrix P_expr(env,this->inst.get_n(),this->inst.get_m());
	IloNumVar a(env,0,inst.get_Ba(),"a");
	IloNumVar p(env,0,inst.get_Bp(),"p");

	//IloExpr a_expr(env);
	//IloExpr p_expr(env);
	IloExpr handicap_a(env);
	IloExpr handicap_p(env);

	IloExpr Cout(env);


	IloExpr Sum_S_ij0(env);


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

			string Sum_Sk_cons_name = "Sum_Sk(";
			Sum_Sk_cons_name += U::to_s(i);
			Sum_Sk_cons_name += ",";
			Sum_Sk_cons_name += U::to_s(j);
			Sum_Sk_cons_name += ")";

			IloConstraint Sum_Sk_cons(Sum_Sk == x[coord]);
			Sum_Sk_cons.setName(Sum_Sk_cons_name.c_str());
			model.add(Sum_Sk_cons);



			//a_expr += x[coord] * this->inst.get_grille(coord).get_Ca();
			//p_expr += x[coord] * this->inst.get_grille(coord).get_Cp();

			Cout += x[coord];
			handicap_a += this->inst.get_grille(coord).get_Ha() * this->inst.get_grille(coord).get_Ca() * A[coord]
			         - this->inst.get_grille(coord).get_Ca() * A[coord];

			handicap_p += this->inst.get_grille(coord).get_Hp() * this->inst.get_grille(coord).get_Cp() * x[coord]
						         - this->inst.get_grille(coord).get_Cp() * P[coord];



		}
	}


	string Sum_S_ij0_cons_name = "Sum_Sij0";

	IloConstraint Sum_S_ij0_cons(Sum_S_ij0 == 1);
	Sum_S_ij0_cons.setName(Sum_S_ij0_cons_name.c_str());
	model.add(Sum_S_ij0_cons);

	/*IloConstraint a_def(a == a_expr);
	a_def.setName("a_def");
	model.add(a_def);

	IloConstraint p_def(p == p_expr);
	p_def.setName("p_def");
	model.add(p_def);*/

	IloConstraint handicap_a_const(handicap_a == 0);
	handicap_a_const.setName("handicap_a");
	model.add(handicap_a_const);

	IloConstraint handicap_p_const(handicap_p == 0);
	handicap_p_const.setName("handicap_p");
	model.add(handicap_p_const);

	IloConstraint handicap_const(p + a >= 2);
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

			//if(inst.get_grille(coord).get_Ca() == 0 && inst.get_grille(coord).get_Cp() == 0)
				//model.add(x[coord] == 0);



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
		/*for(int i = 1; i <= this->inst.get_n(); i++){
			for(int j = 1; j <= this->inst.get_m(); j++){
				for(int k = 0; k < dist_max; k++){

					cplex.setDirection(S.get(i,j,k), IloCplex::BranchDown);//les Sijk sont presque toute nulle, donc en branch vers le bas de pref
				}
				Coordinate coord(i,j);
				if((1 <= inst.get_grille(coord).get_Ha() && 1 <= inst.get_grille(coord).get_Hp()) ||
						(0 == inst.get_grille(coord).get_Ca() && 0 == inst.get_grille(coord).get_Cp()))
					cplex.setDirection(x[coord], IloCplex::BranchUp);// Si Ca et Cp sont nul, ajouter x ne change pas l'admissibilité (sauf eventuellemnt la connexité) et augmente le cout
			}
		}*/



		size_t pos = this->inst.get_name().find(".");
		string lp_name = this->inst.get_name().substr (0,pos);//enlève le .dat à la fin
		pos = lp_name.find("/");
		lp_name = lp_name.substr (pos+1);//enleve de data/ au début
		lp_name += ".lp";
		cout << lp_name << endl;
		cplex.exportModel(lp_name.c_str());

		std::cout << "lp generated" << endl;




		if(cplex.solve()){
			logn5("Résolution succeed ! ");

			env.out() << "Solution status = " << cplex.getStatus() << endl;
			env.out() << "Solution value  = " << cplex.getObjValue() << endl;

			x.ExtractSol(env, cplex, this->set_solution());
			//env.out() << "Values        = " << x_sol << endl;
			//cplex.getSlacks(x_sol, con);
			//env.out() << "Slacks        = " << vals << endl;


			//S.ExtractSol(env,cplex);

			a.end();
			p.end();
			handicap_a.end();
			handicap_p.end();
			Cout.end();






			return true;
		}else{


			a.end();
			p.end();
			handicap_a.end();
			handicap_p.end();
			Cout.end();
			env.end();
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
