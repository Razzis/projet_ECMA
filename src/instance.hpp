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
	//constructeur
	Instance(string filename);

	//destructeur
	~Instance();

	//affichage
	friend ostream & operator <<(ostream &os, Instance inst);
	//accesseurs

	int get_n(){return n;}
	int& set_n(){return this->n;}

	int get_m(){return m;}
	int& set_m(){return this->m;}

	double get_Ba(){return Ba;}
	double& set_Ba(){return this->Ba;}

	double get_Bp(){return Bp;}
	double& set_Bp(){return this->Bp;}

	Maille get_grille(Coordinate coord){return grille[coord];}
	void set_grille(Coordinate coord, Maille maille){grille[coord] = maille;}





};



#endif /* INSTANCE_HPP_ */
