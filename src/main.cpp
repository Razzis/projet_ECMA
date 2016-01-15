#include "common.hpp"
#include "instance.hpp"
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


    cout << "Lecture du fichier : " << args->filename << endl;
    Instance inst =  Instance(args->filename);//
    cout << "main Instance lue" << endl;
    cout << "geg" << endl;
    cout << inst << endl;
    //cout << inst.get_grille(Coordinate(1,1)) << endl;
    //cout << inst << endl;






    // exit(0);

    delete args;
    return 0;
}
//./
