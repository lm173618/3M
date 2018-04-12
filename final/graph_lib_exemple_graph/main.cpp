#include "grman/grman.h"
#include <iostream>
#include <cstdio>
#include <ctime>

#include "graph.h"

int main()
{
    std::clock_t start = std::clock();
    int timer;
    int cycle = 1;

    BITMAP *Presentation;
    /// A appeler en 1er avant d'instancier des objets graphiques etc...
    grman::init();

    /// Le nom du répertoire où se trouvent les images à charger
    grman::set_pictures_path("pics");

    /// Un exemple de graphe
    Graph g;
    g.make_example();

    Presentation = load_bitmap("presentation.bmp", NULL);

    ///Affichage du titre du jeu
    while(!key[KEY_ENTER])
    {
        draw_sprite(screen, Presentation, 0, 0);
    }
    clear_bitmap(screen);


    /// Vous gardez la main sur la "boucle de jeu"
    /// ( contrairement à des frameworks plus avancés )
    while ( !key[KEY_ESC] )
    {
        timer = ( std::clock() - start );

        /// Il faut appeler les méthodes d'update des objets qui comportent des widgets
        g.update();

        /// Mise à jour générale (clavier/souris/buffer etc...)
        grman::mettre_a_jour();

        if ((timer % 50) == 0)
            if(g.get_active() == true)
            {
                std::cout << "cycle " << cycle << ":" << std::endl;
                g.dynPop();
                cycle++;
            }
        if (timer > 100000)
            timer = timer % 100000;
    }

    grman::fermer_allegro();

    return 0;
}
END_OF_MAIN();


