/*
 * instance.cpp
 *
 *  Created on: 9 janv. 2016
 *      Author: hassan
 */
#include "instance.hpp"



void Instance::read_data(){

	vector<string> lines = U::read_lines(this->filename);


	for(auto it_lines = lines.begin(); it_lines != lines.end(); ++it_lines){

		string line = it_lines;
		if(line.find("=") == 0){//declaration de variable
			unsigned long idx  = line.find_first_of("=");
			string key = line.substr(0, idx);

			string reste = line.substr(idx);
			string value = U::trim_string(reste," \t=:"," \t\r\n;,");

			if(value.find("[") == 0){//declaration d'une matrice
				map <Coordinate,double> matrice;//stocke la matrice
				while(it_lines != lines.end() || line.find("=") != 0){
					auto charactere = value[0];
					charactere++;//le premier charactère est  un "["

					string valeur;// stockera les nombres

					Coordinate current_pos;
					current_pos.x = -1;//on mettra a 0 dès le début
					current_pos.y = 0;

					while(charactere != value.end()){
						if(charactere == "["){
							matrice[current_pos] = stod(valeur);
							current_pos.x++;
							valeur.clear();
						}
						if(charactere == "]"){//on change de ligne
							matrice[current_pos] = stod(valeur);
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
		}


	}
}


