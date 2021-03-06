#include "graph.h"
#include <fstream>

using namespace std;

/***************************************************
                    VERTEX
****************************************************/

/// Le constructeur met en place les �l�ments de l'interface
VertexInterface::VertexInterface(int idx, int x, int y, std::string pic_name, int pic_idx)
{
    // La boite englobante
    m_top_box.set_pos(x, y);
    m_top_box.set_dim(130, 100);
    m_top_box.set_moveable();

    // Le slider de r�glage de valeur
    m_top_box.add_child( m_slider_value );
    m_slider_value.set_range(0.0 , 100.0); // Valeurs arbitraires, � adapter...
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
        m_img.get_pic_name(pic_name);
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


/// Gestion du Vertex avant l'appel � l'interface
void Vertex::pre_update()
{
    if (!m_interface)
        return;

    /// Copier la valeur locale de la donn�e m_value vers le slider associ�
    m_interface->m_slider_value.set_value(m_value);

    /// Copier la valeur locale de la donn�e m_value vers le label sous le slider
    m_interface->m_label_value.set_message( std::to_string( (int)m_value) );
}


/// Gestion du Vertex apr�s l'appel � l'interface
void Vertex::post_update()
{
    if (!m_interface)
        return;

    /// Reprendre la valeur du slider dans la donn�e m_value locale
    m_value = m_interface->m_slider_value.get_value();
}



/***************************************************
                    EDGE
****************************************************/

/// Le constructeur met en place les �l�ments de l'interface
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

    // Une boite pour englober les widgets de r�glage associ�s
    m_top_edge.add_child(m_box_edge);
    m_box_edge.set_dim(24,60);
    m_box_edge.set_bg_color(BLANCBLEU);

    // Le slider de r�glage de valeur
    m_box_edge.add_child( m_slider_weight );
    m_slider_weight.set_range(0.0 , 100.0); // Valeurs arbitraires, � adapter...
    m_slider_weight.set_dim(16,40);
    m_slider_weight.set_gravity_y(grman::GravityY::Up);

    // Label de visualisation de valeur
    m_box_edge.add_child( m_label_weight );
    m_label_weight.set_gravity_y(grman::GravityY::Down);

}


/// Gestion du Edge avant l'appel � l'interface
void Edge::pre_update()
{
    if (!m_interface)
        return;

    /// Copier la valeur locale de la donn�e m_weight vers le slider associ�
    m_interface->m_slider_weight.set_value(m_weight);

    /// Copier la valeur locale de la donn�e m_weight vers le label sous le slider
    m_interface->m_label_weight.set_message( std::to_string( (int)m_weight ) );
}

/// Gestion du Edge apr�s l'appel � l'interface
void Edge::post_update()
{
    if (!m_interface)
        return;

    /// Reprendre la valeur du slider dans la donn�e m_weight locale
    m_weight = m_interface->m_slider_weight.get_value();
}



/***************************************************
                    GRAPH
****************************************************/

/// Ici le constructeur se contente de pr�parer un cadre d'accueil des
/// �l�ments qui seront ensuite ajout�s lors de la mise ne place du Graphe
GraphInterface::GraphInterface(int x, int y, int w, int h)
{
    m_top_box.set_dim(1000,740);
    m_top_box.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Up);

    m_top_box.add_child(m_tool_box);
    m_tool_box.set_dim(80,720);
    m_tool_box.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Up);
    m_tool_box.set_bg_color(BLANCBLEU);

    m_top_box.add_child(m_main_box);
    m_main_box.set_dim(908,720);
    m_main_box.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Up);
    m_main_box.set_bg_color(BLANCJAUNE);

    ///Bouton ajouter sommet
    m_top_box.add_child(boutonAjouterSommet);
    boutonAjouterSommet.set_frame(0,0,100,100);
    boutonAjouterSommet.set_bg_color(NOIR);

    m_top_box.add_child(TboutonAjouterSommet);
    TboutonAjouterSommet.set_frame_pos(0,10);
    TboutonAjouterSommet.set_message("AJOUT SOMMET");
    TboutonAjouterSommet.set_bg_color(BLANC);

    ///Bouton ajouter arete
    m_top_box.add_child(boutonAjouterArete);
    boutonAjouterArete.set_frame(0,100,100,100);
    boutonAjouterArete.set_bg_color(VIOLET);

    m_top_box.add_child(TboutonAjouterArete);
    TboutonAjouterArete.set_frame_pos(0,110);
    TboutonAjouterArete.set_message("AJOUT ARETE ");
    TboutonAjouterArete.set_bg_color(BLANC);

    ///Bouton supprimer sommet
    m_top_box.add_child(boutonSupprimerSommet);
    boutonSupprimerSommet.set_frame(0,200,100,100);
    boutonSupprimerSommet.set_bg_color(NOIR);

    m_top_box.add_child(TboutonSupprimerSommet);
    TboutonSupprimerSommet.set_frame_pos(2,210);
    TboutonSupprimerSommet.set_message(" SUP SOMMET ");
    TboutonSupprimerSommet.set_bg_color(BLANC);

    ///Bouton supprimer arete
    m_top_box.add_child(boutonSupprimerArete);
    boutonSupprimerArete.set_frame(0,300,100,100);
    boutonSupprimerArete.set_bg_color(VIOLET);

    m_top_box.add_child(TboutonSupprimerArete);
    TboutonSupprimerArete.set_frame_pos(7,310);
    TboutonSupprimerArete.set_message(" SUP ARETE ");
    TboutonSupprimerArete.set_bg_color(BLANC);

    ///Bouton sauvegarde
    m_top_box.add_child(boutonSauvegarde);
    boutonSauvegarde.set_frame(0,400,100,100);
    boutonSauvegarde.set_bg_color(NOIR);

    m_top_box.add_child(TboutonSauvegarde);
    TboutonSauvegarde.set_frame_pos(2,410);
    TboutonSauvegarde.set_message(" SAUVEGARDE ");
    TboutonSauvegarde.set_bg_color(BLANC);

    ///Bouton connexite
    m_top_box.add_child(boutonConnexe);
    boutonConnexe.set_frame(0,500,100,100);
    boutonConnexe.set_bg_color(VIOLET);

    m_top_box.add_child(TboutonConnexe);
    TboutonConnexe.set_frame_pos(0,510);
    TboutonConnexe.set_message(" CONNEXITE  ");
    TboutonConnexe.set_bg_color(BLANC);

    ///Bouton retour menu
    m_top_box.add_child(boutonRetourMenu);
    boutonRetourMenu.set_frame(0,600,100,100);
    boutonRetourMenu.set_bg_color(NOIR);

    m_top_box.add_child(TboutonRetourMenu);
    TboutonRetourMenu.set_frame_pos(0,610);
    TboutonRetourMenu.set_message("RETOUR MENU ");
    TboutonRetourMenu.set_bg_color(BLANC);


}


/// M�thode sp�ciale qui construit un graphe arbitraire (d�mo)
/// Cette m�thode est � enlever et remplacer par un syst�me
/// de chargement de fichiers par exemple.
/// Bien s�r on ne veut pas que vos graphes soient construits
/// "� la main" dans le code comme �a.
/*void Graph::make_example()
{
    m_interface = std::make_shared<GraphInterface>(50, 0, 750, 600);
    // La ligne pr�c�dente est en gros �quivalente � :
    // m_interface = new GraphInterface(50, 0, 750, 600);

    /// Les sommets doivent �tre d�finis avant les arcs
    // Ajouter le sommet d'indice 0 de valeur 30 en x=200 et y=100 avec l'image clown1.jpg etc...
    add_interfaced_vertex(0, 30.0, 200, 100, "clown1.jpg");

    /// Les arcs doivent �tre d�finis entre des sommets qui existent !
    // AJouter l'arc d'indice 0, allant du sommet 1 au sommet 2 de poids 50 etc...
    add_interfaced_edge(0, 1, 2, 50.0);

}*/
void Graph::lirefichier(std::string nom_fichier)
{
    m_interface = std::make_shared<GraphInterface>(50, 0, 750, 600);
    ///Ouverture du fichier
    std::ifstream fichier(nom_fichier,std::ios::in);
    if(fichier)
    {
        ///Declaration des variables temporaires pour la lecture du fichier
        int nb_sommet, nb_arete;
        int indice;
        double value;
        int x;
        int y;
        std::string nom="";

        int aindice, s1, s2;
        double poids;
        float coeff;

        //infos en tete de fichier qui donne l'ordre
        fichier>>nb_sommet;
        fichier>>nb_arete;

        for (int i=0;i<nb_sommet;i++)
        {
            //recuperation des donn�es du fichier
            fichier>>indice;
            fichier>>value;
            fichier>>x;
            fichier>>y;
            fichier>>nom;

            add_interfaced_vertex(indice, value, x, y, nom);
        }

        calcul_coeff();
        for (int i=0;i<nb_arete;i++)
        {
            //recuperation des donn�es du fichier
            fichier>>aindice;
            fichier>>s1;
            fichier>>s2;
            fichier>>poids;
            fichier>>coeff;

            add_interfaced_edge(aindice, s1, s2, poids, coeff);

        }
    }
    else std::cerr<<"Probleme fichier"<<std::endl;

    ///Fermeture du fichier
    fichier.close();
}

void Graph::sauvegarde(std::string nom_fichier)
{

    std::ofstream fichier(nom_fichier,std::ios::out|std::ios::trunc);
    if(fichier)
    {
        fichier<<m_vertices.size()<<"\n";
        fichier<<m_edges.size()<<"\n";
        for (const auto& it : m_vertices)
        {
           // fichier<<it.second.m_interface->m_label_idx.get_message();
            fichier<<it.first<<" ";
            fichier<< it.second.m_interface->m_label_value.get_message()<<" ";
            //fichier<<it.second.m_pop<<" ";
            fichier<< it.second.m_interface->m_top_box.get_posx()<<" ";
            fichier<< it.second.m_interface->m_top_box.get_posy()<<" ";
            fichier<< it.second.m_interface->m_img.get_pic_name("");
            fichier<<"\n";
        }

        for (const auto& it : m_edges)
        {
            fichier<<it.first<<" ";
            fichier<< it.second.m_from<<" ";
            fichier<< it.second.m_to<<" ";
            fichier<< it.second.m_weight <<" ";
            fichier<<coeff[it.first];
            fichier<<"\n";
        }
    }
    else std::cerr<<"Probleme fichier"<<std::endl;

    ///Fermeture du fichier
    fichier.close();
}
void Graph::calcul_coeff()
{
    float pop_bis;

    for(const auto& ite : m_vertices)
    {
        pop_bis = ((float)ite.second.m_value);
        coeff.push_back((pop_bis/100));
       // std::cout<<coeff[ite.first]<<std::endl;
    }

}

void Graph::calcul_value()
{
    float k_value;
    int id=0;
    int value_bis;

    for(map<int, Vertex>::iterator it=m_vertices.begin(); it!=m_vertices.end(); it++)
    {
        for(map<int, Edge>::iterator ita=m_edges.begin(); ita!=m_edges.end(); ita++)
        {
            if(it->first == ita->second.m_to)
            {
                id = ita->second.m_from;
                for(map<int, Vertex>::iterator itb=m_vertices.begin(); itb!=m_vertices.end(); itb++)
                {
                    if(itb->first == id)
                    {
                        value_bis = itb->second.m_value;
                    }
                }
                k_value = k_value + (ita->second.m_weight * value_bis);
            }
        }

        double temp = (1 - (it->second.m_value/k_value));
        it->second.m_value = it->second.m_value + (it->second.m_r * it->second.m_value * temp);
       // std::cout<<it->second.m_value<<std::endl;
    }
}

bool Graph::outils()
{
 bool fini=false;
            ///Ajouter sommet
        if(m_interface->boutonAjouterSommet.clicked())
        {
            ajouterSommet();
        }

        ///Ajouter Arete
        if(m_interface->boutonAjouterArete.clicked())
        {
            ajouterArete();
        }

        ///Supprimer sommet
        if(m_interface->boutonSupprimerSommet.clicked())
        {
            supprimerSommet();
        }

        ///Supprimer arete
        if(m_interface->boutonSupprimerArete.clicked())
        {
            supprimerArete();
        }

         ///Sauvegarde
        if(m_interface->boutonSauvegarde.clicked())
        {
            std::string nomfichier;
            cout << "Entrer le nom du fichier a sauvegarder" << endl;
            cin >> nomfichier;
            sauvegarde(nomfichier);
        }

        ///Connexite
        if(m_interface->boutonConnexe.clicked())
        {
            set_adj();
        }

        ///Retour au menu
        if(m_interface->boutonRetourMenu.clicked())
        {
            fini=true;
        }
        return fini;
}
/// La m�thode update � appeler dans la boucle de jeu pour les graphes avec interface
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


}


/// eidx index of edge to remove
void Graph::test_remove_edge(int eidx)
{
    /// r�f�rence vers le Edge � enlever
    Edge &remed=m_edges.at(eidx);
    std::cout << "Removing edge " << eidx << " " << remed.m_from << "->" << remed.m_to << " " << remed.m_weight << std::endl;
    /// Tester la coh�rence : nombre d'arc entrants et sortants des sommets 1 et 2
    std::cout << m_vertices[remed.m_from].m_in.size() << " " << m_vertices[remed.m_from].m_out.size() << std::endl;
    std::cout << m_vertices[remed.m_to].m_in.size() << " " << m_vertices[remed.m_to].m_out.size() << std::endl;
    std::cout << m_edges.size() << std::endl;

    /// test : on a bien des �l�ments interfac�s
    if (m_interface && remed.m_interface)
    {
        /// Ne pas oublier qu'on a fait �a � l'ajout de l'arc :
        /* EdgeInterface *ei = new EdgeInterface(m_vertices[id_vert1], m_vertices[id_vert2]); */
        /* m_interface->m_main_box.add_child(ei->m_top_edge); */
        /* m_edges[idx] = Edge(weight, ei); */
        /// Le new EdgeInterface ne n�cessite pas de delete car on a un shared_ptr
        /// Le Edge ne n�cessite pas non plus de delete car on n'a pas fait de new (s�mantique par valeur)
        /// mais il faut bien enlever le conteneur d'interface m_top_edge de l'arc de la main_box du graphe
        m_interface->m_main_box.remove_child( remed.m_interface->m_top_edge );
    }

    /// Il reste encore � virer l'arc supprim� de la liste des entrants et sortants des 2 sommets to et from !
    /// References sur les listes de edges des sommets from et to
    std::vector<int> &vefrom = m_vertices[remed.m_from].m_out;
    std::vector<int> &veto = m_vertices[remed.m_to].m_in;
    vefrom.erase( std::remove( vefrom.begin(), vefrom.end(), eidx ), vefrom.end() );
    veto.erase( std::remove( veto.begin(), veto.end(), eidx ), veto.end() );
    /// Le Edge ne n�cessite pas non plus de delete car on n'a pas fait de new (s�mantique par valeur)
    /// Il suffit donc de supprimer l'entr�e de la map pour supprimer � la fois l'Edge et le EdgeInterface
    /// mais malheureusement ceci n'enlevait pas automatiquement l'interface top_edge en tant que child de main_box !
    m_edges.erase( eidx );
    /// Tester la coh�rence : nombre d'arc entrants et sortants des sommets 1 et 2
    std::cout << m_vertices[remed.m_from].m_in.size() << " " << m_vertices[remed.m_from].m_out.size() << std::endl;
    std::cout << m_vertices[remed.m_to].m_in.size() << " " << m_vertices[remed.m_to].m_out.size() << std::endl;
    std::cout << m_edges.size() << std::endl;
}

/// Aide � l'ajout de sommets interfac�s
void Graph::add_interfaced_vertex(int idx, double value, int x, int y, std::string pic_name, int pic_idx )
{
    if ( m_vertices.find(idx)!=m_vertices.end() )
    {
        std::cerr << "Error adding vertex at idx=" << idx << " already used..." << std::endl;
        throw "Error adding vertex";
    }
    // Cr�ation d'une interface de sommet
    VertexInterface *vi = new VertexInterface(idx, x, y, pic_name, pic_idx);
    // Ajout de la top box de l'interface de sommet
    m_interface->m_main_box.add_child(vi->m_top_box);
    // On peut ajouter directement des vertices dans la map avec la notation crochet :
    m_vertices[idx] = Vertex(value, vi);
}

/// Aide � l'ajout d'arcs interfac�s
void Graph::add_interfaced_edge(int idx, int id_vert1, int id_vert2, double weight, int coeff)
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
    m_edges[idx].m_from = id_vert1;
    m_edges[idx].m_to = id_vert2;
    m_vertices[id_vert1].m_out.push_back(idx);
    m_vertices[id_vert2].m_in.push_back(idx);
}

void Graph::ajouterSommet()
{
    ///Declaration des variables
    std::string nomImage;
    double valeur;

    ///Entree des infos du sommet � ajouter
    std::cout << "Entrer la valeur du sommet" << std::endl;
    std::cin >> valeur;
    std::cout << "Entrer le nom de l'image" << std::endl;
    std::cin >> nomImage;

    ///Ajout du sommet
    add_interfaced_vertex(m_vertices.size(), valeur, 120, 10, nomImage);
}

void Graph::ajouterArete()
{
    ///Declaration des variables
    int indiceS1, indiceS2;
    double poids;

    ///Entree des infos sur l'arete a ajouter
    cout << "Entrer l'indice du sommet de depart" << endl;
    cin >> indiceS1;
    cout << "Entrer l'indice du sommet d'arrivee" << endl;
    cin >> indiceS2;
    cout << "Entrer le poids de l'arete" << endl;
    cin >> poids;

    ///Ajout de l'arete
    add_interfaced_edge(m_edges.size(), indiceS1, indiceS2, poids);
}

void Graph::supprimerArete()
{
    ///Declaration des variables
    int indiceS1, indiceS2, indiceArete;

    ///Entree des infos sur l'arete a supprimer
    cout << "Entrer l'indice du sommet de depart" << endl;
    cin >> indiceS1;
    cout << "Entrer l'indice du sommet d'arrivee" << endl;
    cin >> indiceS2;

    if(m_vertices.count(indiceS1) + m_vertices.count(indiceS2) == 2)
    {
        for(auto elem: m_edges)
        {
            if(elem.second.m_from==indiceS1 && elem.second.m_to==indiceS2)
            {
                indiceArete=elem.first;
            }
        }
        ///Suppression de l'arete
        test_remove_edge(indiceArete);
    }
}

void Graph::supprimerSommet()
{
    ///Declaration des variables
    int indiceSommet;

    ///Entree des infos sur le sommet a supprimer
    cout <<"Entrer l'indice du sommet" << endl;
    cin >> indiceSommet;

    if(m_vertices.count(indiceSommet) == 1)
    {
        for(int i=0; i<m_vertices[indiceSommet].m_in.size(); i++)
        {
            this->test_remove_edge(m_vertices[indiceSommet].m_in[i]);
        }

        for(int i=0; i<m_vertices[indiceSommet].m_out.size(); i++)
        {
            this->test_remove_edge(m_vertices[indiceSommet].m_out[i]);
        }

        m_interface->m_main_box.remove_child( m_vertices[indiceSommet].m_interface->m_top_box );
        m_vertices.erase(indiceSommet);
    }
}

void Graph::menu()
{
    ///Declaration des variables
    BITMAP *buffer;
    BITMAP *fond;
    bool quitter = false;
    int mouse_prec;
    int mouse_nouv;

    ///Creation du buffer
    buffer=create_bitmap(800, 600);
    clear_bitmap(buffer);

    ///Chargement des images
    fond = load_bitmap("fond.bmp", NULL);
    if(!fond)
    {
        allegro_message("Image non trouvee");
        allegro_exit();
        exit(EXIT_FAILURE);
    }

    while ( quitter==false )
    {
        ///Affichage du menu
        blit(fond, buffer, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
        blit(buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);

        clear_bitmap(buffer);

        mouse_prec=mouse_nouv;
        mouse_nouv=mouse_b&1;

        ///Si on clique sur la case du graphe 1
        if( mouse_x>60 && mouse_x<346 && mouse_y>47 && mouse_y<262)
        {
            if(!mouse_prec && mouse_nouv)
            {
                /// Affichage du graphe 1
                lirefichier("sauv_foret.txt");
                quitter=true;
            }
        }

        ///Si on clique sur la case du graphe 2
        if( mouse_x>454 && mouse_x<740 && mouse_y>47 && mouse_y<262)
        {
            if(!mouse_prec && mouse_nouv)
            {
                /// Affichage du graphe 2
                lirefichier("sauv_mer.txt");
                quitter=true;
            }
        }

        ///Si on clique sur la case du graphe 3
        if( mouse_x>250 && mouse_x<536 && mouse_y>338 && mouse_y<553)
        {
            if(!mouse_prec && mouse_nouv)
            {
                /// Affichage du graphe 3
                lirefichier("sauv_desert.txt");
                quitter=true;
            }
        }

        ///Si on clique sur la case quitter
        if(!mouse_prec && mouse_nouv && mouse_x>639 && mouse_x<739 && mouse_y>478 && mouse_y<553)
        {
            ///On quitte le jeu
            quitter=true;
        }
    }
}

///Composante fortement connexe
int* Graph::comp_fort_connexe(int**adj,int ordre, int s)
{
    int *c1,*c2,*c,*marques;
    int x,y,ajoute=1;
    c1=new int [ordre];
    c2=new int [ordre];
    c=new int [ordre];
    marques= new int [ordre];
    for  (int i=0;i<ordre;i++)
    {
        c1[i]=0;c2[i]=0;c[i]=0;
        marques[i]=0;
    }
    c1[s]=1;c2[s]=1;
    while(ajoute)
    {
        ajoute=0;
        for(x=0;x<ordre;x++)
        {
            if(!marques[x]&&c1[x])
            {
                marques[x]=1;
                for(y=0;y<ordre;y++)
                {
                    if(adj[x][y]&&!marques[y])
                    {
                        c1[y]=1;ajoute=1;
                    }
                }
            }
        }
        for(x=0;x<ordre;x++)
        {
            c[x]=c1[x]&c2[x];
        }
    }
    ajoute=1;
    for (int i=0;i<ordre;i++){marques[i]=0;}
    while(ajoute)
    {
        ajoute=0;
        for(x=0;x<ordre;x++)
        {
            if(!marques[x]&&c2[x])
            {
                marques[x]=1;
                for(y=0;y<ordre;y++)
                {
                    if(adj[y][x]&&!marques[y])
                    {
                        c2[y]=1;ajoute=1;
                    }
                }
            }
        }
        for(x=0;x<ordre;x++)
        {
            c[x]=c1[x]&c2[x];
        }
    }
    return c;
    }

void Graph::set_adj()
{
    int m_nbarrete=m_edges.size();
    int** tabc;
    int** adjac;
    int m_ordre=m_vertices.size();
    adjac=new int* [m_ordre];tabc=new int* [m_ordre];
    for (int i=0;i<m_ordre;i++)
    {
        adjac[i]=new int [m_ordre];tabc[i]=new int [m_ordre];
    }
    for (int i=0;i<m_ordre;i++)
    {
        for (int j=0;j<m_ordre;j++)
        {adjac[i][j]=0;}
    }
    for (int i=0;i<m_ordre;i++)
    {
        for (int j=0;j<m_ordre;j++)
        {
            for ( std::map<int,Edge>::iterator k=m_edges.begin();k!=m_edges.end(); k++)
            {
                if (k->second.m_from==i && k->second.m_to==j)// a changer
                    {adjac[i][j]=1;}
            }
        }
    }

    for (int i=0;i<m_ordre;i++)
    {

        for (int j=0;j<m_ordre;j++)
        {

            std::cout<<adjac[i][j]<<" ";
        }
        std::cout<<std::endl;
    }

    tabc=fort_connexe(adjac,m_ordre);
    aff_connexe(tabc);
}

int** Graph::fort_connexe(int**adj,int ordre)
{

    int **tabc;int*marques;int x,y;
    tabc=new int* [ordre];
    for (int i=0;i<ordre;i++)
    {
        tabc[i]=new int [ordre];
    }

    marques=new int [ordre];
    for (int i=0;i<ordre;i++)
    {
        marques[i]=0;
    }
    for(int i=0; i<ordre; i++)
    {
        for(int j=0;j<ordre;j++)
            {
                tabc[i][j]=0;
            }
    }


    for(x=0;x<ordre;x++)
    {
        if(!marques[x])
        {
            tabc[x]=comp_fort_connexe(adj,ordre,x);
            marques[x]=1;
            for(y=0;y<ordre;y++)
            {
                if(tabc[x][y]&&!marques[y])
                    marques[y]=1;
            }
        }
    }
    return tabc;
}
void Edge::set_couleur(int col)
{
    std::cout<<"composante de couleur"<<col;
}

void Graph::aff_connexe(int**tabc)
{
    std::vector<int> vec;
    std::vector<int> veccoul;
    int b=0;int cp=0;


    for (unsigned int i=0;i<m_vertices.size();i++)
    {
        b=0;
        for (unsigned int j=0;j<m_vertices.size();j++)
        {

            if(tabc[i][j]==1)
            {
                b=1;  //vec.push_back(i);veccoul.push_back(cp);
                std::cout<<std::endl<<"couleur "<<cp<< " : ";
                std::cout<<j;
               // m_vertices[i].set_couleur(cp);
            }

        }

        cout<<std::endl;
        if(b==1 )
                cp++;
    }
}


