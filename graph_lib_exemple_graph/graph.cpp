#include "graph.h"

/***************************************************
                    VERTEX
****************************************************/


/// Le constructeur met en place les éléments de l'interface
VertexInterface::VertexInterface(int idx, int x, int y, std::string pic_name, int pic_idx)
{
    // La boite englobante
    m_top_box.set_pos(x, y);
    m_top_box.set_dim(130, 100);
    m_top_box.set_moveable();

    // Le slider de réglage de valeur
    m_top_box.add_child( m_slider_value );
    m_slider_value.set_range(0.0, 100.0);  // Valeurs arbitraires, à adapter...
    m_slider_value.set_dim(20,80);
    m_slider_value.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Up);

    // Label de visualisation de valeur
    m_top_box.add_child( m_label_value );
    m_label_value.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Down);

    // Une illustration...
    if (pic_name!="")
    {
        m_top_box.add_child( m_img );
        m_img.set_pic_name(pic_name);
        m_img.set_pic_idx(pic_idx);
        m_img.set_gravity_x(grman::GravityX::Right);
    }

    // Label de visualisation d'index du sommet dans une boite
    m_top_box.add_child( m_box_label_idx );
    m_box_label_idx.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Down);
    m_box_label_idx.set_dim(20,12);
    m_box_label_idx.set_bg_color(BLANC);

    m_box_label_idx.add_child( m_label_idx );
    m_label_idx.set_message( std::to_string(idx) );
}


/// Gestion du Vertex avant l'appel à l'interface
void Vertex::pre_update()
{
    if (!m_interface)
        return;

    /// Copier la valeur locale de la donnée m_value vers le slider associé
    m_interface->m_slider_value.set_value(m_value);

    /// Copier la valeur locale de la donnée m_value vers le label sous le slider
    m_interface->m_label_value.set_message( std::to_string( (int)m_value) );
}


/// Gestion du Vertex après l'appel à l'interface
void Vertex::post_update()
{
    if (!m_interface)
        return;

    /// Reprendre la valeur du slider dans la donnée m_value locale
    m_value = m_interface->m_slider_value.get_value();
}
/***************************************************
                    OUTILS
****************************************************/

OutilsInterface::OutilsInterface(int idx, int x, int y, std::string pic_name, int pic_idx)
{
    // La boite englobante
    m_top_box.set_pos(x, y);
    m_top_box.set_dim(86,50);
    m_top_box.set_border(0);

    // Une illustration...
    if (pic_name!="")
    {
        m_top_box.add_child( m_bouton);
        m_bouton.add_child(m_img);
        m_img.set_pic_name(pic_name);
        m_bouton.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Up);
        m_bouton.set_dim(80,50);
        m_img.set_gravity_xy(grman::GravityX::Center, grman::GravityY::Center);

    }
}


void Outils::update1(Graph *g)
{
    if (m_interface->m_bouton.clicked())
    {
        if ( m_interface->m_img.get_pic_name() == "sommet.bmp")
        {
            std::string pic_name;
            std::cout<<"Saisissez le nom de l'image : " ;
            std::cin>> pic_name ;
            g->add_interfaced_vertex(g->m_vertices.size(), 0.0, 100, 100, pic_name);
        }
        if (m_interface->m_img.get_pic_name()=="arc.bmp")
        {
            int id_vert1,id_vert2;
            std::cout<<"Saisir le sommet 1 :";
            std::cin>>id_vert1;
            std::cout<<"Saisir le sommet 2 :";
            std::cin>>id_vert2;
            g->add_interfaced_edge(g->m_edges.size(),id_vert1,id_vert2,50);
        }
        if ( m_interface->m_img.get_pic_name() == "supsommet.bmp")
        {
            int idx;
            std::cout<<"saisir l'indice du sommet :";
            std::cin>>idx;
            g->delete_interfaced_vertex(idx);
            std::cout<<"sommet numero "<<idx<<" supprime"<<std::endl;
        }
        if(m_interface->m_img.get_pic_name()=="suparc.bmp")
        {
            int eidx;
            std::cout<<"saisir l'indice de l'arrete :";
            std::cin>>eidx;
            g->test_remove_edge(eidx);
            std::cout<<"arrete numero "<<eidx<<" supprime"<<std::endl;
        }

        if ( m_interface->m_img.get_pic_name() == "chargement.bmp")
        {
            std::cout << "Charger!" << std::endl;
            if(g->m_vertices.empty()||g->m_edges.empty())///Si le graphe n'est pas instancié
            {
                g->chargement();///On l'instancie
            }
            else
            {
                g->RAZ();
                g->chargement();
            }
        }
        if ( m_interface->m_img.get_pic_name() == "sauvegarder.bmp")
        {
            std::cout << "Sauvegarder!" << std::endl;
            g->sauvegarde(g->m_vertices,g->m_edges);
        }
    }
}




/***************************************************
                    EDGE
****************************************************/

/// Le constructeur met en place les éléments de l'interface
EdgeInterface::EdgeInterface(Vertex& from, Vertex& to)
{
    // Le WidgetEdge de l'interface de l'arc
    if ( !(from.m_interface && to.m_interface) )
    {
        std::cerr << "Error creating EdgeInterface between vertices having no interface" << std::endl;
        throw "Bad EdgeInterface instanciation";
    }
    m_top_edge.attach_from(from.m_interface->m_top_box);
    m_top_edge.attach_to(to.m_interface->m_top_box);
    m_top_edge.reset_arrow_with_bullet();

    // Une boite pour englober les widgets de réglage associés
    m_top_edge.add_child(m_box_edge);
    m_box_edge.set_dim(50,70);
    m_box_edge.set_bg_color(BLANCBLEU);

    // Le slider de réglage de valeur
    m_box_edge.add_child( m_slider_weight );
    m_slider_weight.set_range(0.0, 100.0);  // Valeurs arbitraires, à adapter...
    m_slider_weight.set_dim(16,50);
    m_slider_weight.set_gravity_y(grman::GravityY::Up);

    // Label de visualisation de valeur
    m_box_edge.add_child( m_label_weight );
    m_label_weight.set_gravity_xy(grman::GravityX::Left,grman::GravityY::Down);

    //Label de visualisation de l'indice de l'arete
    m_box_edge.add_child(m_label_idx);
    m_label_idx.set_gravity_xy(grman::GravityX::Right,grman::GravityY::Down);
}


/// Gestion du Edge avant l'appel à l'interface
void Edge::pre_update()
{
    if (!m_interface)
        return;

    /// Copier la valeur locale de la donnée m_weight vers le slider associé
    m_interface->m_slider_weight.set_value(m_weight);
    /// Copier la valeur locale de la donnée m_weight vers le label sous le slider
    m_interface->m_label_weight.set_message( std::to_string( (int)m_weight ) );
    m_interface->m_label_idx.set_message( std::to_string( (int)m_idx ) );
}

/// Gestion du Edge après l'appel à l'interface
void Edge::post_update()
{
    if (!m_interface)
        return;

    /// Reprendre la valeur du slider dans la donnée m_weight locale
    m_weight = m_interface->m_slider_weight.get_value();
}



/***************************************************
                    GRAPH
****************************************************/

/// Ici le constructeur se contente de préparer un cadre d'accueil des
/// éléments qui seront ensuite ajoutés lors de la mise ne place du Graphe
GraphInterface::GraphInterface(int x, int y, int w, int h)
{
    m_top_box.set_dim(790,590);
    m_top_box.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Up);


    m_top_box.add_child(m_tool_box);
    m_tool_box.set_dim(785,50);
    m_tool_box.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Up);
    m_tool_box.set_bg_color(BLANCBLEU);

    m_top_box.add_child(m_main_box);
    m_main_box.set_dim(785,535);
    m_main_box.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Down);
    m_main_box.set_bg_color(BLANCJAUNE);
}


/// Méthode spéciale qui construit un graphe arbitraire (démo)
/// Cette méthode est à enlever et remplacer par un système
/// de chargement de fichiers par exemple.
/// Bien sûr on ne veut pas que vos graphes soient construits
/// "à la main" dans le code comme ça.
void Graph::make_example()
{
    m_interface = std::make_shared<GraphInterface>(50, 0, 750, 600);
    // La ligne précédente est en gros équivalente à :
    // m_interface = new GraphInterface(50, 0, 750, 600);

    /// Les sommets doivent être définis avant les arcs
    // Ajouter le sommet d'indice 0 de valeur 30 en x=200 et y=100 avec l'image clown1.jpg etc...


    //add_interfaced_vertex(0, 30.0, 200, 100, "clown1.jpg");
    // chargement();
    /*add_interfaced_vertex(1, 60.0, 400, 100, "clown2.jpg");
    add_interfaced_vertex(2,  50.0, 200, 300, "clown3.jpg");
    add_interfaced_vertex(3,  0.0, 400, 300, "clown4.jpg");
    add_interfaced_vertex(4,  100.0, 600, 300,"clown5.jpg");
    add_interfaced_vertex(5,  0.0, 100, 500, "bad_clowns_xx3xx.jpg", 0);
    add_interfaced_vertex(6,  0.0, 300, 500, "bad_clowns_xx3xx.jpg", 1);
    add_interfaced_vertex(7,  0.0, 500, 500, "bad_clowns_xx3xx.jpg", 2);



    /// Les arcs doivent être définis entre des sommets qui existent !
    // AJouter l'arc d'indice 0, allant du sommet 1 au sommet 2 de poids 50 etc...
    add_interfaced_edge(0, 1, 2, 50.0);
    add_interfaced_edge(1, 0, 1, 50.0);
    add_interfaced_edge(2, 1, 3, 75.0);
    add_interfaced_edge(3, 4, 1, 25.0);
    add_interfaced_edge(4, 6, 3, 25.0);
    add_interfaced_edge(5, 7, 3, 25.0);
    add_interfaced_edge(6, 3, 4, 0.0);
    add_interfaced_edge(7, 2, 0, 100.0);
    add_interfaced_edge(8, 5, 2, 20.0);
    add_interfaced_edge(9, 3, 7, 80.0);
    */

    ///image barre d'outils
    add_interfaced_outil(100, 30, 0, 0, "sommet.bmp");
    add_interfaced_outil(101, 30.0, 200, 0,"supsommet.bmp");
    add_interfaced_outil(102, 30, 100, 0, "arc.bmp");
    add_interfaced_outil(103, 30, 300, 0, "suparc.bmp");
    add_interfaced_outil(14,30,400,0,"sauvegarder.bmp");
    add_interfaced_outil(104,30,500,0,"chargement.bmp");

}

/// La méthode update à appeler dans la boucle de jeu pour les graphes avec interface
void Graph::update()
{
    if (!m_interface)
        return;

    for (auto &elt : m_vertices)
        elt.second.pre_update();

    for (auto &elt : m_edges)
        elt.second.pre_update();

    m_interface->m_top_box.update();

    for (auto &elt : m_vertices)
        elt.second.post_update();

    for (auto &elt : m_edges)
        elt.second.post_update();

    for (auto &elt : m_outils)
        elt.second.update1(this);
}
void Graph::delete_interfaced_vertex(int idx)
{
    Vertex remed=m_vertices.at(idx);
    if (m_interface && remed.m_interface)
    {
        /// Ne pas oublier qu'on a fait ça à l'ajout de l'arc :
        /* EdgeInterface *ei = new EdgeInterface(m_vertices[id_vert1], m_vertices[id_vert2]); */
        /* m_interface->m_main_box.add_child(ei->m_top_edge);  */
        /* m_edges[idx] = Edge(weight, ei); */
        /// Le new EdgeInterface ne nécessite pas de delete car on a un shared_ptr
        /// Le Edge ne nécessite pas non plus de delete car on n'a pas fait de new (sémantique par valeur)
        /// mais il faut bien enlever le conteneur d'interface m_top_edge de l'arc de la main_box du graphe
        m_interface->m_main_box.remove_child( remed.m_interface->m_top_box );
    }
    m_vertices.erase(idx);
}
/// Aide à l'ajout de sommets interfacés
void Graph::add_interfaced_vertex(int idx, double value, int x, int y, std::string pic_name, int pic_idx )
{
    if ( m_vertices.find(idx)!=m_vertices.end() )
    {
        std::cerr << "Error adding vertex at idx=" << idx << " already used..." << std::endl;
        throw "Error adding vertex";
    }
    // Création d'une interface de sommet
    VertexInterface *vi = new VertexInterface(idx, x, y, pic_name, pic_idx);
    // Ajout de la top box de l'interface de sommet
    m_interface->m_main_box.add_child(vi->m_top_box);
    // On peut ajouter directement des vertices dans la map avec la notation crochet :
    m_vertices[idx] = Vertex(value, vi);
}

/// Aide à l'ajout d'arcs interfacés
void Graph::add_interfaced_edge(int idx, int id_vert1, int id_vert2, double weight)
{
    if ( m_edges.find(idx)!=m_edges.end() )
    {
        std::cerr << "Error adding edge at idx=" << idx << " already used..." << std::endl;
        throw "Error adding edge";
    }

    if ( m_vertices.find(id_vert1)==m_vertices.end() || m_vertices.find(id_vert2)==m_vertices.end() )
    {
        std::cerr << "Error adding edge idx=" << idx << " between vertices " << id_vert1 << " and " << id_vert2 << " not in m_vertices" << std::endl;
        throw "Error adding edge";
    }

    EdgeInterface *ei = new EdgeInterface(m_vertices[id_vert1], m_vertices[id_vert2]);
    m_interface->m_main_box.add_child(ei->m_top_edge);
    m_edges[idx] = Edge(weight, ei);
    /// OOOPS ! Prendre en compte l'arc ajouté dans la topologie du graphe !
    m_edges[idx].m_from = id_vert1;
    m_edges[idx].m_to = id_vert2;
    m_edges[idx].m_idx = idx;

    m_vertices[id_vert1].m_out.push_back(id_vert2);
    m_vertices[id_vert2].m_in.push_back(id_vert1);


}

void Graph::add_interfaced_outil(int idx, double value, int x, int y, std::string pic_name, int pic_idx )
{
    if ( m_vertices.find(idx)!=m_vertices.end() )
    {
        std::cerr << "Error adding vertex at idx=" << idx << " already used..." << std::endl;
        throw "Error adding vertex";
    }

    // Création d'une interface de sommet
    OutilsInterface *vi = new OutilsInterface(idx, x, y, pic_name, pic_idx);
    // Ajout de la top box de l'interface de sommet
    m_interface->m_tool_box.add_child(vi->m_top_box);
    // On peut ajouter directement des vertices dans la map avec la notation crochet :
    m_outils[idx] = Outils(value, vi);
}
void Graph::test_remove_edge(int eidx)
{
    /// référence vers le Edge à enlever
    Edge remed=m_edges.at(eidx);

    std::cout << "Removing edge " << eidx << " " << remed.m_from << "->" << remed.m_to << " " << remed.m_weight << std::endl;

    /// Tester la cohérence : nombre d'arc entrants et sortants des sommets 1 et 2
    std::cout << m_vertices[remed.m_from].m_in.size() << " " << m_vertices[remed.m_from].m_out.size() << std::endl;
    std::cout << m_vertices[remed.m_to].m_in.size() << " " << m_vertices[remed.m_to].m_out.size() << std::endl;
    std::cout << m_edges.size() << std::endl;

    /// test : on a bien des éléments interfacés
    if (m_interface && remed.m_interface)
    {
        /// Ne pas oublier qu'on a fait ça à l'ajout de l'arc :
        /* EdgeInterface *ei = new EdgeInterface(m_vertices[id_vert1], m_vertices[id_vert2]); */
        /* m_interface->m_main_box.add_child(ei->m_top_edge);  */
        /* m_edges[idx] = Edge(weight, ei); */
        /// Le new EdgeInterface ne nécessite pas de delete car on a un shared_ptr
        /// Le Edge ne nécessite pas non plus de delete car on n'a pas fait de new (sémantique par valeur)
        /// mais il faut bien enlever le conteneur d'interface m_top_edge de l'arc de la main_box du graphe
        m_interface->m_main_box.remove_child( remed.m_interface->m_top_edge );
    }

    /// Il reste encore à virer l'arc supprimé de la liste des entrants et sortants des 2 sommets to et from !
    /// References sur les listes de edges des sommets from et to
    std::vector<int> &vefrom = m_vertices[remed.m_from].m_out;
    std::vector<int> &veto = m_vertices[remed.m_to].m_in;
    vefrom.erase( std::remove( vefrom.begin(), vefrom.end(), eidx ), vefrom.end() );
    veto.erase( std::remove( veto.begin(), veto.end(), eidx ), veto.end() );

    /// Le Edge ne nécessite pas non plus de delete car on n'a pas fait de new (sémantique par valeur)
    /// Il suffit donc de supprimer l'entrée de la map pour supprimer à la fois l'Edge et le EdgeInterface
    /// mais malheureusement ceci n'enlevait pas automatiquement l'interface top_edge en tant que child de main_box !
    m_edges.erase( eidx );


    /// Tester la cohérence : nombre d'arc entrants et sortants des sommets 1 et 2
    std::cout << m_vertices[remed.m_from].m_in.size() << " " << m_vertices[remed.m_from].m_out.size() << std::endl;
    std::cout << m_vertices[remed.m_to].m_in.size() << " " << m_vertices[remed.m_to].m_out.size() << std::endl;
    std::cout << m_edges.size() << std::endl;

}


void Graph::chargement()
{
    std::string choice;
    std::string save;
    std::string picname,ligne,ligne2;
    int idx,value,x,y;
    int idx2, id_vert1, id_vert2;
    double weight;
    std::cout<<" Quel graphe voulez-vous charger ? ";
    std::cin>> choice ;
    std::ifstream fichier(choice+".txt", std::ios::in);
    if (fichier)
    {
        while (std::getline(fichier,ligne))
        {
            if (ligne=="a")
            {
                fichier>>idx;
                fichier>>value;
                fichier>>x;
                fichier>>y;
                fichier.ignore();
                std::getline(fichier, picname);
                add_interfaced_vertex(idx,  value, x, y, picname);
                std::cout<<idx<<std::endl;
                std::cout<<value<<std::endl;
                std::cout<<x<<std::endl;
                std::cout<<y<<std::endl;
                std::cout<<picname<<std::endl;
            }
            if (ligne=="b")
            {

                fichier>>idx2;
                fichier>>id_vert1;
                fichier>> id_vert2;
                fichier>>weight;
                add_interfaced_edge(idx2,id_vert1,id_vert2,weight);

                std::cout<<"idx2:"<<idx2<<std::endl;
                std::cout<<"idvert1:"<<id_vert1<<std::endl;
                std::cout<<"idvert2:"<<id_vert2<<std::endl;
                std::cout<<"weight:"<<weight<<std::endl;
            }



        }

        fichier.close();

    }
    else
        std::cerr<<"impossible d'ouvrir le fichier"<<std::endl;
}

void Graph::sauvegarde( std::map<int, Vertex> m_vertices,std::map<int, Edge> m_edges)
{
    std::string chaine;
    std::cout<<"saisir le nom d'enregistrement du fichier pour les sommets"<<std::endl;
    std::cin>>chaine;
    std::ofstream fichier(chaine +".txt", std::ios::out);

    if (fichier)
    {
        for (unsigned int x=0; x<m_vertices.size(); x++)
        {
            fichier<<"a";
            fichier<<m_vertices[x].m_interface->m_box_label_idx.get_message()<<std::endl;// Pb à la sauvegarde
            fichier<<m_vertices[x].m_value<<std::endl;
            fichier<<m_vertices[x].m_interface->m_top_box.get_posx()<<std::endl;
            fichier<<m_vertices[x].m_interface->m_top_box.get_posy()<<std::endl;
            fichier<<m_vertices[x].m_interface->m_img.get_pic_name()<<std::endl;
        }
        for (unsigned int x=0; x<m_edges.size(); x++)
        {
            fichier<<"b"<<std::endl;
            fichier<<x<<std::endl;
            fichier<<m_edges[x].m_from<<std::endl;
            fichier<<m_edges[x].m_to<<std::endl;
            fichier<<m_edges[x].m_weight<<std::endl;
        }

    }

}
void Graph::RAZ()
{
    for(unsigned int i=0 ; i<m_edges.size()+1; i++)
    {
        test_remove_edge(i);
    }
    for(unsigned int j=0 ; j<m_vertices.size()+2; j++)
    {
        delete_interfaced_vertex(j);
    }
}


