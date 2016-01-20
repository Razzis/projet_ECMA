/*
 * solution.hpp
 *
 *  Created on: 9 janv. 2016
 *      Author: hassan
 */

#ifndef SOLUTION_HPP_
#define SOLUTION_HPP_

#include "instance.hpp"

class Solution{
private:
	map <Coordinate,bool> Choix_maille;// vaut 1 si la maille correspondante est retenue dans la solution
	double Hp;
	double Ha;

	double cost;

	Instance inst;//  l'instance qui crée la solution

public:
	//constructeur
	Solution();
	Solution(Instance& inst);
	Solution(const Solution& copy_sol);
	Solution(const Instance& inst);
	const Solution& operator=(const Solution copy_sol);
	//destructeur
	~Solution();

	//accesseur
	double get_Ha(){return this->Ha;}
	double& set_Ha(){return this->Ha;}

	double get_Hp(){return this->Hp;}
	double& set_Hp(){return this->Hp;}

	double get_cost(){return this->cost;}
	double& set_cost(){return this->cost;}

	Instance get_inst(){return this->inst;}

	bool get_Choix_maille(Coordinate coord){return Choix_maille[coord];}
	bool& set_Choix_maille(Coordinate coord){return Choix_maille[coord];}

	//affichage
	friend ostream & operator <<(ostream &os, Solution Solution);

};


#endif /* SOLUTION_HPP_ */
