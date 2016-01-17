/*
* solution.cpp
 *
 *  Created on: 9 janv. 2016
 *      Author: hassan
 */
#include "solution.hpp"
Solution::Solution(){
}
Solution::Solution(Instance& inst){
	for(int i = 1; i <= inst.get_n(); i++){
		for(int j = 1; j <= inst.get_m(); j++){
			Coordinate coord(i,j);
			this->Choix_maille[coord] = false;
		}
	}



	this->Ha = 0;
	this->Hp = 0;

	this->inst = inst;
}

Solution::Solution(const Instance& inst){
	for(int i = 1; i <= inst.get_n(); i++){
		for(int j = 1; j <= inst.get_m(); j++){
			Coordinate coord(i,j);
			this->Choix_maille[coord] = false;
		}
	}

	this->Ha = 0;
	this->Hp = 0;

	this->inst = inst;
}

Solution::Solution(const Solution& copy_sol){
	this->Ha = copy_sol.Ha;
	this->Hp = copy_sol.Hp;
	this->inst = copy_sol.inst;

	this->Choix_maille = copy_sol.Choix_maille;


}
const Solution& Solution::operator=(const Solution copy_sol){

	this->Choix_maille = copy_sol.Choix_maille;

	this->inst = copy_sol.inst;
	this->Ha = copy_sol.Ha;
	this->Hp = copy_sol.Hp;

	return *this;
}



Solution::~Solution(){

}

ostream & operator <<(ostream &os, Solution Solution){
	os << "###################################" << endl;
	os << "#### Affichage de la solution #####" << endl;
	os << "Ha(Z) : " << Solution.Ha << ", Hp(Z) : " << Solution.Hp << endl;
	os << "Afichage de la grille nxm : (" << Solution.inst.get_n() << "," << Solution.inst.get_m() << ")" << endl;
	for(int j = 1; j <= Solution.inst.get_m(); j++){
		for(int i = 1; i <= Solution.inst.get_n(); i++){
			Coordinate coord(i,j);
			os << int(Solution.Choix_maille[coord]);
		}
		os << "//" << endl;
	}
	os << "###################################" << endl;
	return os;

}
