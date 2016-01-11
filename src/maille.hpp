/*
 * maille.hpp
 *
 *  Created on: 9 janv. 2016
 *      Author: hassan
 */

#ifndef MAILLE_HPP_
#define MAILLE_HPP_

typedef struct Coordinate{
	int x;
	int y;
};


class Maille{

private:
	double Ca;// Coefficient correcteur d'altitude
	double Ha;// Handicap d'altitude
	double Cp;// Coefficient correcteur de pente
	double Hp;// Handicap de pente

public:
	double get_Ca(){return Ca;}
	void set_Ca(int Ca){this->Ca = Ca;}

	double get_Cp(){return Cp;}
	void set_Cp(int Ca){this->Cp = Cp;}

	double get_Ha(){return Ha;}
	void set_Ha(int Ca){this->Ha = Ha;}

	double get_Hp(){return Hp;}
	void set_Hp(int Ca){this->Hp = Hp;}

};



#endif /* MAILLE_HPP_ */
