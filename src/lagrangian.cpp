/*
 * lagrangian.cpp
 *
 *  Created on: 12 janv. 2016
 *      Author: hassan
 */
#include "solver.hpp"

Lagrangian::Lagrangian(Instance inst){
	this->name = "Lagrangian";
	this->sol = Solution(inst);
	this->inst = inst;
}

Lagrangian::~Lagrangian(){
	Env.end();
}

bool Lagrangian::solve(){
	IloEnv env = this->Env;
	logn5("Lagrangian::solve START");

	DataNumMatrix lagrange_multiplier(this->Env, this->inst.get_n(), this->inst.get_m());
	lagrange_multiplier.set_value(0);
	DataNumMatrix xij(this->Env, this->inst.get_n(), this->inst.get_m());
	DataNumMatrix S_ijk(this->Env, this->inst.get_n(), this->inst.get_m());
	//initialisation du multiplicateur




	//int dist_max = floor(this->inst.get_n()*this->inst.get_m()/2);//distance maximal entre deux points pour la connexité
	PLNE_lag_x pl_solver_x(inst, env);//declaration du solveur PLNE
	PLNE_lag_S pl_solver_S(inst, env);//declaration du solveur PLNE
	bool stop = false;

	Solution current_sol(this->inst);
	double last_cost = 10000;//stocke le dernier cout calculé pour calculer le critère d'arret
	double current_cost = 1;
	double eps = 0.005;//variation du cout accepté
	double rho = 10;//coefficient pour mettre à jour lambda

	int iteration_num = 1;



	logn5("Lagrangian::solve variables definies");
	while(!stop && iteration_num < 100){


		pl_solver_x.solve(lagrange_multiplier, xij);// mise à jour de la solution courante
		pl_solver_S.solve(lagrange_multiplier, S_ijk);// mise à jour de la solution courante
		current_sol = pl_solver_x.get_solution();
		current_cost = pl_solver_x.get_solution().get_cost() + pl_solver_S.get_solution().get_cost();

		//DataNumMatrix S_ijk = pl_solver.get_sum_k_S_ijk();
		//DataNumMatrix xij =
		//pl_solver.get_x_ij();





		lagrange_multiplier = lagrange_multiplier + rho*(1/double(iteration_num))*(S_ijk-xij);

		//cout << lagrange_multiplier << endl;

		iteration_num += 1;
		if(abs(current_cost - last_cost) < eps)
			stop = true;

		cout << "-------it : "<< iteration_num-1 << "---------" << endl;
		cout << "pente : " << (S_ijk-xij) << endl;
		cout << "delta : " << abs(current_cost - last_cost) << endl;
		cout << "----------------" << endl;
		last_cost = current_cost;// Mise à jour du cout

	}


	cout << current_sol << endl;

	this->sol = current_sol;






	return true;
}
