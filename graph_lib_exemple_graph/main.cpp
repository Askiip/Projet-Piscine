///Sources:
///Openclassroom
///Cours de l'ECE
///Travail en collaboration avec le groupe 9 d'informatique du TD04 (Salom� MASBERNAT et Julia ROBILLARD)

#include "grman/grman.h"
#include <iostream>

#include "graph.h"

using namespace std;
int main()
{
    bool actu = false;
    do{
        bool fin = false;
        bool bip = false;
    /// A appeler en 1er avant d'instancier des objets graphiques etc...
        grman::init();

    /// Le nom du r�pertoire o� se trouvent les images � charger
        grman::set_pictures_path("pics");

        /// Un exemple de graphe
        Graph g;

        ///Affichage du menu
        g.menu();
        g.calcul_value();
    while(fin == false)
    {

            fin = g.outils();

        /// Vous gardez la main sur la "boucle de jeu"
        /// ( contrairement � des frameworks plus avanc�s )
            if(key[KEY_ESC] )
            {
                /// Il faut appeler les m�thodes d'update des objets qui comportent des widgets

                fin = true;
                actu = true;                /// Mise � jour g�n�rale (clavier/souris/buffer etc...)

            }
            g.update();
            grman::mettre_a_jour();
        }
        grman::fermer_allegro();

    }while(!actu);

    return 0;

}
END_OF_MAIN();

