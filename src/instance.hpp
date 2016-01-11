/*
 * instance.hpp
 *
 *  Created on: 9 janv. 2016
 *      Author: hassan
 */

#ifndef INSTANCE_HPP_
#define INSTANCE_HPP_

#include "common.hpp"
#include "maille.hpp"
#include "util.hpp"

class Instance{

private:
	int n;// longueur de la grille
	int m;// largeur de la grille

	double Ba;// Borne sup pour a
	double Bp;// Borne sup pour p

	map <Coordinate,Maille> grille;

	//lecture des données
	string filename;
	void read_data();
public:

	//accesseurs

	int get_n(){return n;}
	void set_n(int n){this->n = n;}

	int get_m(){return m;}
	void set_m(int m){this->m = m;}

	double get_Ba(){return Ba;}
	void set_Ba(double Ba){this->Ba = Ba;}

	double get_Bp(){return Bp;}
	void set_Bp(double Bp){this->Bp = Bp;}

	Maille get_grille(Coordinate coord){return grille[coord];}
	void set_grille(Coordinate coord, Maille maille){grille[coord] = maille;}





};



#endif /* INSTANCE_HPP_ */
