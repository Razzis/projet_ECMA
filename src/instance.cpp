/*
 * instance.cpp
 *
 *  Created on: 9 janv. 2016
 *      Author: hassan
 */
#include "instance.hpp"


Instance::~Instance(){

}

Instance::Instance(){

}

Instance::Instance(const Instance& copy_inst){
	this->filename = copy_inst.filename;


	this->n = copy_inst.n;
	this->m = copy_inst.m;
	this->Ba = copy_inst.Ba;
	this->Bp = copy_inst.Bp;

	this->grille = copy_inst.grille;

}
const Instance& Instance::operator=(const Instance copy_inst){
	this->filename = copy_inst.filename;


	this->n = copy_inst.n;
	this->m = copy_inst.m;
	this->Ba = copy_inst.Ba;
	this->Bp = copy_inst.Bp;

	this->grille = copy_inst.grille;
	return *this;
}


Instance::Instance(string filename){
	this->filename = filename;

	// Les attributs qui suivent sont chang par la suite
	this->n = -1;
	this->m = -1;


	//this->grille = map <Coordinate,Maille>();

	this->read_data();


	this->Ba = 0;
	this->Bp = 0;

	for(int i = 1; i<=this->n; i++){
		for(int j = 1; j<=this->m; j++){
			Coordinate coord(i,j);
			this->Ba += this->grille[coord].get_Ca();// TODO le choix de la borne peut être amélioré
			this->Bp += this->grille[coord].get_Cp();
		}
	}


	if(this->n == -1 ||
			this->m == -1 ||
			this->Ba == -1 ||
			this->Bp == -1)
		U::die("Instance::Instance problème à la lecture");
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
			//cout << "value : " << value << endl;
			if(value.at(0) == '['){//declaration d'une matrice
					string valeur;// stockera les nombres
					Coordinate current_pos(1,1);//on part du coin supérieur gauche

					bool entre_crochet = false;//indique si on se trouve dans un bloc et qu'on lit des nombre
					for(string::iterator it_charactere = value.begin(); it_charactere != value.end()--; ++it_charactere){//le dernier charactère est  un "]"
						char charactere = *it_charactere;
						//cout << "Instance::read_data charactere : " << charactere << endl;
						if(charactere == ']'){
							//cout << "key : " << key << " valeur : " << valeur << endl;
							if(key == "Ca"){
								this->grille[current_pos].set_Ca() = stod(valeur);
							}
							else if(key == "Cp")
								this->grille[current_pos].set_Cp()=  stod(valeur);
							else if(key == "Ha"){
								this->grille[current_pos].set_Ha() = stod(valeur);
							}
							else if(key == "Hp")
								this->grille[current_pos].set_Hp() = stod(valeur);
							current_pos.x = 1;
							current_pos.y++;
							++it_charactere;//le charactere suivant est ,
							entre_crochet = false;
							valeur.clear();

						}
						else if(charactere == ',' && entre_crochet){//il y a des virgule entre blocs, qu'on ne veut pas rendre en compte
							if(key == "Ca"){
								this->grille[current_pos].set_Ca() = stod(valeur);
							}
							else if(key == "Cp")
								this->grille[current_pos].set_Cp() =  stod(valeur);
							else if(key == "Ha"){
								this->grille[current_pos].set_Ha() = stod(valeur);
							}
							else if(key == "Hp")
								this->grille[current_pos].set_Hp() = stod(valeur);

							current_pos.x++;
							valeur.clear();
						}
						else if(charactere == '['){
							//rien a faire
						}
						else{
							valeur.push_back(charactere);
							entre_crochet = true;
						}
						//charactere++;
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
							//cout << "go" << endl;

							//Maille Current_maille(i,j);
							//cout << "go" << endl;
							grille[coord] = Maille(i,j);
							logn5("Creation of Maille ("+U::to_s(grille[coord].get_coord_x())+","+U::to_s(grille[coord].get_coord_y())+")");

						}
					}
					//cout << *this << endl;
					//U::die("TEST");
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


ostream &operator <<(ostream &os, Instance inst){
	os << "######################" << endl;
	os << "affichage de l'instance : " << inst.filename << endl;
	os << "grille de taille nxm : " << inst.n << "x" << inst.m << endl;
	os << "Ba : " << inst.Ba << endl;
	os << "Bp : " << inst.Bp << endl;
	os << "----------------------" << endl;
	os << "Description des mailles : " << endl;


	cout << "Ha : " << endl;
	for(int j = 1; j<= inst.m; j++){
			for(int i = 1; i <= inst.n; i++){
				Coordinate coord(i,j);
				os << setiosflags(ios::fixed) << std::setprecision(3) << inst.grille[coord].get_Ha() << "|";
			}
			cout << endl;
		}
	os << "----------------------" << endl;
	cout << "Ca : " << endl;
	for(int j = 1; j<= inst.m; j++){
			for(int i = 1; i <= inst.n; i++){
				Coordinate coord(i,j);
				os << std::setprecision(3) << inst.grille[coord].get_Ca() << "|";
			}
			cout << endl;
		}
	os << "----------------------" << endl;
	cout << "Hp : " << endl;
	for(int j = 1; j<= inst.m; j++){
			for(int i = 1; i <= inst.n; i++){
				Coordinate coord(i,j);
				os << std::setprecision(3) << inst.grille[coord].get_Hp() << "|";
			}
			cout << endl;
		}
	os << "----------------------" << endl;
	cout << "Cp : " << endl;
	for(int j = 1; j<= inst.m; j++){
			for(int i = 1; i <= inst.n; i++){
				Coordinate coord(i,j);
				os << std::setprecision(3) << inst.grille[coord].get_Cp() << "|";
			}
			cout << endl;
		}


	return os;

}

