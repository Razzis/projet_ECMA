#include "common.hpp"
#include "instance.hpp"
#include "options.hpp"
#include "solver.hpp"

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
    cout << inst << endl;


    if(args->solver == 2){
    	PLNE pl_solver(inst);

    	cout << "pl_solver initialized " << endl;
    	pl_solver.solve();

    	cout << "Solution : " << endl;
    	cout << pl_solver.get_solution() << endl;




    }







    // exit(0);

    delete args;
    return 0;
}
//./
