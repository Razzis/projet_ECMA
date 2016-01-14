/*
 * instance.cpp
 *
 *  Created on: 9 janv. 2016
 *      Author: hassan
 */
#include "instance.hpp"

Instance::Instance(string filename){
	this->filename = filename;

	// Les attributs qui suivent sont chang par la suite
	this->n = -1;
	this->m = -1;
	this->Ba = -1;
	this->Bp = -1;

	this->grille = map <Coordinate,Maille>();

	this->read_data();


	if(this->n == -1 ||
			this->m == -1 ||
			this->Ba == -1 ||
			this->Bp == -1)
		U::die("Instance::Instance problème à la lecture");
}

Instance::~Instance(){

}



void Instance::read_data(){
	logn5("Instance::read_data START");
	vector<string> lines = U::read_lines(this->filename);
	logn5("Lecture de l'instance : " +this->filename);

	for(unsigned int it_lines = 0; it_lines != lines.size(); ++it_lines){

		string line = lines.at(it_lines);
		logn7("Lecture de la ligne : " +line);
		if(line.find("=") != string::npos){//declaration de variable
			unsigned long idx  = line.find_first_of("=");//on trouve la séparation entre le nom de la variable et la valeur
			string key = line.substr(0, idx);//on découpe
			string reste = line.substr(idx);
			string value = U::trim_string(reste," \t=:"," \t\r\n;,");//on nettoit les retour à la ligne et cie

			if(value.at(0) == '['){//declaration d'une matrice
				while(it_lines != lines.size() && line.find("=") == string::npos){//tant qu'on est pas à la fin du fichier, ou qu'on ne déclare pas une nouvelles variable



					string valeur;// stockera les nombres

					Coordinate current_pos(-1,0);

					for(string::iterator it_charactere = ++value.begin(); it_charactere != value.end(); ++it_charactere){//le premier charactère est  un "["
						char charactere = *it_charactere;
						if(charactere == '['){
							if(key == "Ca")
								this->grille[current_pos].set_Ca() = stod(valeur);
							else if(key == "Cp")
								this->grille[current_pos].set_Cp()=  stod(valeur);
							else if(key == "Ha")
								this->grille[current_pos].set_Ha() = stod(valeur);
							else if(key == "Hp")
								this->grille[current_pos].set_Hp() = stod(valeur);
							current_pos.x++;
							valeur.clear();
						}
						if(charactere == ']'){//on change de ligne
							if(key == "Ca")
								this->grille[current_pos].set_Ca() = stod(valeur);
							else if(key == "Cp")
								this->grille[current_pos].set_Cp() = stod(valeur);
							else if(key == "Ha")
								this->grille[current_pos].set_Ha() = stod(valeur);
							else if(key == "Hp")
								this->grille[current_pos].set_Hp() = stod(valeur);
							current_pos.x = 0;
							current_pos.y++;
							valeur.clear();
						}
						else{
							valeur.push_back(charactere);
						}
						charactere++;
					}

					it_lines++;
				}

			}
			else{//stockage d'une variable scalaire
				if(key == "n")
					this->set_n() = stoi(value);
				else if(key == "m"){
					this->set_m() = stoi(value);
					for(int i = 1; i<=n; i++){
						for(int j = 1; j<=m; j++){
							Coordinate coord(i,j);
							grille[coord] = Maille(coord);
						}
					}
				}
				if(key == "Ba")
					this->set_Ba() = stod(value);
				if(key == "Bp")
					this->set_Bp() = stod(value);
			}
		}


	}
	logn5("Instance::read_data END");
}


ostream & operator <<(ostream &os, Instance inst){
	os << "######################" << endl;
	os << "affichage de l'instance : " << inst.filename;
	os << "grille de taille nxm : " << inst.n << "x" << inst.m << endl;
	os << "Ba : " << inst.Ba << endl;
	os << "Bp : " << inst.Bp << endl;
	os << "----------------------" << endl;
	os << "Description des mailles : " << endl;
	for(int i = 1; i <= inst.n; i++){
		for(int j = 1; j <= inst.n; j++){
			Maille current_maille = inst.grille[Coordinate(i,j)];
			os << current_maille << endl;
		}
	}
	return os;

}

