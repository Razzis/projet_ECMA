/*
 * solver.hpp
 *
 *  Created on: 9 janv. 2016
 *      Author: hassan
 */

#ifndef SOLVER_HPP_
#define SOLVER_HPP_

#include "instance.hpp"
#include "solution.hpp"

#include "cplex_utils.hpp"
ILOSTLBEGIN

class Solver{
protected:
	Solution sol;// Solution qui sera remplie
	Instance inst; // Référence vers l'instance concernée
	string name; // Nom de la méthode de résolution


public:
	// Constructeur
	Solver();

	// Destructeur
	virtual ~Solver();



	//solve
	virtual bool solve(){return false;} // sera implémenté dans chaque type de solveur

	// Accesseur
	Solution get_solution(){return this->sol;}
	Solution& set_solution(){return this->sol;}

	//affichage
	friend ostream & operator <<(ostream &os, Solver Solver);


};

class PLNE : public Solver {
public:
	//constructeur
	PLNE(Instance inst);

	// Destructeur
	virtual ~PLNE();




	//solve
	virtual bool solve();

};

class PLNE_non_connexe : public Solver {
public:
	//constructeur
	PLNE_non_connexe(Instance inst);

	// Destructeur
	virtual ~PLNE_non_connexe();




	//solve
	virtual bool solve();

};


class PLNE_lag_x : public Solver {
	/*DataNumMatrix sum_k_S_ijk;//somme sur k des S_ijk
	DataNumMatrix x_ij;//valeur des Sijk*/
	IloEnv env;
public:
	//constructeur
	PLNE_lag_x(Instance inst, IloEnv env);
	// Destructeur
	virtual ~PLNE_lag_x();

	/*DataNumMatrix get_x_ij() const{return this->x_ij;}
	DataNumMatrix& set_x_ij(){return this->x_ij;}

	DataNumMatrix get_sum_k_S_ijk() const{return this->sum_k_S_ijk;}
	DataNumMatrix& set_sum_k_S_ijk(){return this->sum_k_S_ijk;}*/

	//solve
	virtual bool solve(DataNumMatrix const& lagrange_multiplier, DataNumMatrix& xij);

};

class PLNE_lag_S : public Solver {
	/*DataNumMatrix sum_k_S_ijk;//somme sur k des S_ijk
	DataNumMatrix x_ij;//valeur des Sijk*/
	IloEnv env;
public:
	//constructeur
	PLNE_lag_S(Instance inst, IloEnv env);
	// Destructeur
	virtual ~PLNE_lag_S();

	/*DataNumMatrix get_x_ij() const{return this->x_ij;}
	DataNumMatrix& set_x_ij(){return this->x_ij;}

	DataNumMatrix get_sum_k_S_ijk() const{return this->sum_k_S_ijk;}
	DataNumMatrix& set_sum_k_S_ijk(){return this->sum_k_S_ijk;}*/

	//solve
	virtual bool solve(DataNumMatrix const& lagrange_multiplier, DataNumMatrix& sum_k_S_ijk);

};
class Lagrangian : public Solver {
private:
	IloEnv Env;
public:
	//constructeur
	Lagrangian(Instance inst);

	// Destructeur
	virtual ~Lagrangian();




	//solve
	virtual bool solve();

};



#endif /* SOLVER_HPP_ */
