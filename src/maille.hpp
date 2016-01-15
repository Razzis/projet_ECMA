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
	Coordinate(const Coordinate& copy_coordinate);//constructeur par copie
	~Coordinate();

	int x;
	int y;

	bool operator ==(const Coordinate &P2) const {//la distance n'a pas de sens, mais il faut que deux Coordonee egale renvois false dans les deux sens
	    if(this->x != P2.x || this->y != P2.y)
	    	return false;
	    else
	    	return true;
	}
/* il faut un ordre total, et que deux coordonnée différente ne puissent avoir la même distance.
 * le coeff devant y sert a ça, il doit etre grand devant x_max*y_max
 *
 */
	bool operator <(const Coordinate &P2) const {//la distance n'a pas de sens, mais il faut que deux Coordonee egale renvois false dans les deux sens
	    if(this->x + 1000000*this->y < P2.x + 1000000*P2.y)
	    	return true;
	    else
	    	return false;
	}
};



class Maille{

private:

	double Ca;// Coefficient correcteur d'altitude
	double Ha;// Handicap d'altitude
	double Cp;// Coefficient correcteur de pente
	double Hp;// Handicap de pente
	Coordinate coord;//coordonnée de la maille
public:


	//Constructeur
	Maille();//Par défaut
	Maille(Coordinate coord);
	Maille(int x, int y);
	Maille(int x, int y, int Ca, int Ha, int Cp, int Hp);
	Maille(const Maille& copy_maille);//constructeur par copie

	//destructeur
	~Maille();

	//affichage
	friend ostream & operator <<(ostream &os, Maille maille);

	//accesseur
	double get_Ca() const {return this->Ca;}
	double& set_Ca(){return this->Ca;}

	double get_Cp() const {return this->Cp;}
	double& set_Cp(){return this->Cp;}

	double get_Ha()const {return this->Ha;}
	double& set_Ha(){return this->Ha;}

	double get_Hp() const {return this->Hp;}
	double& set_Hp(){return this->Hp;}

	int get_coord_x() const {return this->coord.x;}
	int get_coord_y() const {return this->coord.y;}

	int& set_coord_x() {return this->coord.x;}
	int& set_coord_y() {return this->coord.y;}

};

#endif /* MAILLE_HPP_ */
