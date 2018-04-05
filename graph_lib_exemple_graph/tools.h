#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED

#include <vector>
#include <map>
#include <string>
#include <memory>

#include "grman/grman.h"


/***************************************************
                    TOOLS
****************************************************/

class ToolsInterface
{
    friend class Tools;
    friend class GraphInterface;

    private :

    public :

        // Le constructeur met en place les éléments de l'interface
        // voir l'implémentation dans le .cpp
        ToolsInterface(int x, int y, int w, int h, grman::WidgetBox m_top_box);
};


class Tools
{
    private :

        /// La "liste" des arêtes
        std::map<int, Edge> m_edges;

        /// La liste des sommets
        std::map<int, Vertex> m_vertices;

        /// le POINTEUR sur l'interface associée, nullptr -> pas d'interface
        std::shared_ptr<GraphInterface> m_interface = nullptr;


    public:

        /// Les constructeurs sont à compléter selon vos besoin...
        /// Ici on ne donne qu'un seul constructeur qui peut utiliser une interface
        Tools (GraphInterface *interface=nullptr) :
            m_interface(interface)  {  }

        void add_interfaced_tool(int idx, double value, int x, int y, std::string pic_name="", int pic_idx=0 );
        void add_interfaced_edge(int idx, int vert1, int vert2, double weight=0);

        /// Méthode spéciale qui construit un graphe arbitraire (démo)
        /// Voir implémentation dans le .cpp
        /// Cette méthode est à enlever et remplacer par un système
        /// de chargement de fichiers par exemple.
        void make_example();


        /// La méthode update à appeler dans la boucle de jeu pour les graphes avec interface
        void update();
};


#endif // GRAPH_H_INCLUDED
