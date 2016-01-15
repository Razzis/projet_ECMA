/*
 * maille.cpp
 *
 *  Created on: 12 janv. 2016
 *      Author: hassan
 */
#include "maille.hpp"

Coordinate::Coordinate(int x, int y){
	this->x = x;
	this->y = y;
}


Coordinate::Coordinate(){
	this->x = -1;
	this->y = -1;
}

Coordinate::Coordinate(const Coordinate& copy_coordinate){
	this->x = copy_coordinate.x;
	this->y = copy_coordinate.y;
}

Coordinate::~Coordinate(){
}

Maille::Maille(){
	//U::die("STOP");
	this->coord = Coordinate(-1,-1);
	this->Ca = -1;
	this->Ha = -1;
	this->Cp = -1;
	this->Hp = -1;
	//U::die("STOP");
}

Maille::Maille(const Maille& copy_maille){
	this->coord = Coordinate(copy_maille.get_coord_x(),copy_maille.get_coord_y());
	//this->coord.x = copy_maille.get_coord_x();
	//this->coord.y = copy_maille.get_coord_y();
	//this->coord.x = 1;//copy_maille.coord.x;
	//this->coord.y = 1;//copy_maille.coord.y;
	this->Ca = copy_maille.Ca;
	this->Ha = copy_maille.Ha;
	this->Cp = copy_maille.Cp;
	this->Hp = copy_maille.Hp;
}

Maille::Maille(Coordinate coord){
	this->coord = Coordinate(coord.x,coord.y);
	this->Ca = -1;
	this->Ha = -1;
	this->Cp = -1;
	this->Hp = -1;
}
Maille::Maille(int x, int y){
	this->coord = Coordinate(x,y);
	this->Ca = -1;
	this->Ha = -1;
	this->Cp = -1;
	this->Hp = -1;
}
Maille::Maille(int x, int y, int Ca, int Ha, int Cp, int Hp){

	this->coord = Coordinate(x,y);
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
