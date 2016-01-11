#include "common.hpp"



#include "options.hpp"

// je dois définir cette variable statique en dehors de la classe Options
Options* Options::args = NULL;

// TODO : Remplacer classe Solver par un interface purement abstraite
//        et réorganiser ce fichier
//
int main(int argc, char *argv[]) {

    // Analyse et extraction des arguments de la ligne de commande
    Options::args =  new Options(argc, argv);
    Options* args = Options::args;


    // Exemple d'affichage de quelques options

    if(args->solver ==2){
    	cout << "filename : " << args->filename << endl;
    }





    // exit(0);


    return 0;
}
//./
