/*
 * maille.cpp
 *
 *  Created on: 12 janv. 2016
 *      Author: hassan
 */
#include "maille.hpp"

Coordinate::Coordinate(int x, int y){
	this->x = double(x);
	this->y = double(y);
}

Coordinate::Coordinate(){
	this->x = double(-1);
	this->y = double(-1);
}

Coordinate::~Coordinate(){

}

Maille::Maille(){
	this->coord.x = -1;
	this->coord.y = -1;
	this->Ca = -1;
	this->Ha = -1;
	this->Cp = -1;
	this->Hp = -1;
}

Maille::Maille(const Maille& copy_maille){
	this->coord.x = copy_maille.coord.x;
	this->coord.y = copy_maille.coord.y;
	this->Ca = copy_maille.Ca;
	this->Ha = copy_maille.Ha;
	this->Cp = copy_maille.Cp;
	this->Hp = copy_maille.Hp;
}

Maille::Maille(Coordinate coord){

	this->coord.x = coord.x;
	this->coord.y = coord.y;
	this->Ca = -1;
	this->Ha = -1;
	this->Cp = -1;
	this->Hp = -1;
}

Maille::Maille(int x, int y, int Ca, int Ha, int Cp, int Hp){

	this->coord.x = x;
	this->coord.y = y;
	this->Ca = Ca;
	this->Ha = Ha;
	this->Cp = Cp;
	this->Hp = Hp;
}

Maille::~Maille(){

}

ostream & operator <<(ostream &os, Maille maille){
	os << "affichage de la maille (" << maille.coord.x << "," << maille.coord.y << ")" << endl;
	os << "(Ca,Ha) : (" << maille.Ca << "," << maille.Ha << ')' << endl;
	os << "(Cp,Hp) : (" << maille.Cp << "," << maille.Hp << ')' << endl;
	return os;
}
