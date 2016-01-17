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
	cout << name << endl;

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
	DataVarBoolTriMatrix S("S", env, this->inst.get_n(),this->inst.get_m(), this->inst.get_n()*this->inst.get_m());

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
	IloExpr Sum_S_ij0(env);
	std::cout << "define variables" << endl;

	for(int i = 1; i <= this->inst.get_n(); i++){
		for(int j = 1; j <= this->inst.get_m(); j++){
			Coordinate coord(i,j);
			Sum_S_ij0 += S.get(i,j,0);


			IloExpr Sum_Sk(env);
			for(int k = 0; k < this->inst.get_n()*this->inst.get_m(); k++){

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



			a_expr += x[coord] * this->inst.get_grille(coord).get_Ca();
			p_expr += x[coord] * this->inst.get_grille(coord).get_Cp();

			Cout += x[coord];
			handicap += this->inst.get_grille(coord).get_Hp() * this->inst.get_grille(coord).get_Cp() * A[coord]
			         + this->inst.get_grille(coord).get_Ha() * this->inst.get_grille(coord).get_Ca() * P[coord]
			         - 2 * this->inst.get_grille(coord).get_Ca() * P[coord];



		}
	}

	string Sum_S_ij0_cons_name = "Sum_Sij0";

	IloConstraint Sum_S_ij0_cons(Sum_S_ij0 == 1);
	Sum_S_ij0_cons.setName(Sum_S_ij0_cons_name.c_str());
	model.add(Sum_S_ij0_cons);

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
		string lp_name = this->inst.get_name() + ".lp";
		cplex.exportModel("pl.lp");//lp_name.c_str());

		std::cout << "lp generated" << endl;




		if(cplex.solve()){
			logn5("Résolution succeed ! ");

			env.out() << "Solution status = " << cplex.getStatus() << endl;
			env.out() << "Solution value  = " << cplex.getObjValue() << endl;

			x.ExtractSol(env, cplex, this->set_solution(), x);
			//env.out() << "Values        = " << x_sol << endl;
			//cplex.getSlacks(x_sol, con);
			//env.out() << "Slacks        = " << vals << endl;


			//S.ExtractSol(env,cplex);

			a.end();
			p.end();
			handicap.end();
			Cout.end();

			env.end();




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
