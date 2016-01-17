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



#endif /* SOLVER_HPP_ */
