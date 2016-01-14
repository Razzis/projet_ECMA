/*
 * maille.hpp
 *
 *  Created on: 9 janv. 2016
 *      Author: hassan
 */

#ifndef MAILLE_HPP_
#define MAILLE_HPP_

#include "common.hpp"
#include "util.hpp"

class Coordinate{
public:
	Coordinate(int x, int y);
	Coordinate();
	~Coordinate();

	double x;
	double y;


	bool operator <(const Coordinate &P2) const {
	    if(this->x*this->x + this->y*this->y < P2.x*P2.x + P2.y*P2.y)
	    	return false;
	    else
	    	return true;
	};
};



class Maille{

private:
	Coordinate coord;//coordonnée de la maille
	double Ca;// Coefficient correcteur d'altitude
	double Ha;// Handicap d'altitude
	double Cp;// Coefficient correcteur de pente
	double Hp;// Handicap de pente

public:
	//Constructeur
	Maille();//Par défaut
	Maille(Coordinate coord);
	Maille(int x, int y, int Ca, int Ha, int Cp, int Hp);
	Maille(const Maille& copy_maille);//constructeur par copie

	//destructeur
	~Maille();

	//affichage
	friend ostream & operator <<(ostream &os, Maille maille);

	//accesseur
	double get_Ca(){return Ca;}
	double& set_Ca(){return this->Ca;}

	double get_Cp(){return Cp;}
	double& set_Cp(){return this->Cp;}

	double get_Ha(){return Ha;}
	double& set_Ha(){return this->Ha;}

	double get_Hp(){return Hp;}
	double& set_Hp(){return this->Hp;}

	int get_coord_x(){return coord.x;}
	int get_coord_y(){return coord.y;}

};

#endif /* MAILLE_HPP_ */
