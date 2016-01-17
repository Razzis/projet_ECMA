/*
 * solveur.cpp
 *
 *  Created on: 12 janv. 2016
 *      Author: hassan
 */
#include "solver.hpp"

Solver::Solver(){
	this->name = "non spécifié";
}

Solver::~Solver(){

}

ostream & operator <<(ostream &os, Solver Solver){
	os << "------- Solver : " << Solver.name << "------------------" << endl;
	os << "Solution : " << endl;
	os << Solver.sol << endl;
	os << "---------------------------------" << endl;

	return os;
}
