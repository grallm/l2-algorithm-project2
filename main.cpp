// Projet 2 - Algorithmique et structure de données - Agorithme de tri par fusion multiple de monotonies
// Malo GRALL et Paul TOMEI - 12/2019

#include <iostream>
#include <stdio.h>
#include "outilsmesure.hpp"
#include <stdlib.h> // Random
#include <time.h> // Random

using namespace std;


// ==== Partie A ====

// On utilise des ints pour tester
typedef int DATATYPE;

typedef struct _datum{
    DATATYPE valeur;
    _datum * suiv;
} data;
typedef _datum * p_data; // Changed to _datum to have auto-completion (because data is ambigous)


// Affiche sur la même ligne toutes les valeurs de la chaîne
void aff(p_data chain){
    // Variables

    // Début
    if(chain != nullptr){
        cout << chain->valeur << "; ";
        aff(chain->suiv);
    }else{
        cout << endl;
    }
    // Fin
}

// Ajouter une maille en tête et retourne la nouvelle chaîne (nouveau pointeur vers la tête)
p_data ajoutDevant(DATATYPE uneval, p_data chain){
    // Variables
    p_data maille;
    
    // Début
    maille = new _datum; // Changed to _datum to no have error (because data is ambigous)
    
    maille->valeur = uneval;
    maille->suiv = chain;
    return maille;
    // Fin
}

// Créer une chaîne, ajouter des mailles avec les valeurs jusqu'a ce que cette valeur soit sentinelle
// Ajout en tête
p_data saisieBorne(DATATYPE sentinelle){
    // Variables
    p_data head;
    DATATYPE val;

    // Début
    cout << "Saisissez une valeur pour ajouter une maille ( " << sentinelle << " pour arrêter):" << endl;
    head = nullptr;
    
    cin >> val;
    if(val != sentinelle){
        head = ajoutDevant(val, saisieBorne(sentinelle));
    }else{
        head = ajoutDevant(sentinelle, head);
    }
    
    return head;
    // Fin
}

// Créer un chaînage de nb mailles récursivement
p_data saisieNombre(int nb){
    // Variables
    p_data head;
    DATATYPE val;

    // Start
    head = nullptr;

    if(nb != 0){
      if(nb == 1){
        cout << nb << " valeur restantes: ";
        cin >> val;
        return ajoutDevant(val, nullptr);

      }else{
        cout << nb << " valeur restantes: ";
        cin >> val;

        return ajoutDevant(val, saisieNombre(--nb));
      }
    }

    return head;
    // Fin
}

// Fusionner 2 chaînes avec des éléments triés par ordre croissant, retourner la chaîne triée résultante
// Les 2 chaînes initiales seront désordonnées, l'une des 2 têtes sera au milieu de la chaîne
// Version itérative
/** Préconditions
 * Les 2 chaînes doivent être triées en ordre croissant
*/
/** Postconditions
 * Le pointeur passé en 2e argument de la fonction renvoie vers une maille de la chaîne résultante, il n'est donc plus utilisable comme tête de chaîne
*/
p_data fusion(p_data head, p_data middle){
  // Variables
  p_data left, right, actual; // Elément où l'on est rendu sur la chaîne

  // Début
  // Vérifier que les chaînes ne soient pas vides
  if(head != nullptr && middle != nullptr){
    left = head;
    right = middle;
    
    // Placer la tête de la chaîne fusionné sur la plus petite valeur des 2 têtes
    if(head->valeur > middle->valeur){
      head = middle;
      right = right->suiv;
    }else{
      left = left->suiv;
    }
    actual = head;

    // Tant qu'il reste des éléments dans les 2 chaînes on ajoute la plus petite valeur et on décale
    while (left != nullptr && right != nullptr)
    {
      if(left->valeur < right->valeur){
        actual->suiv = left;
        left = left->suiv;
      }else{
        actual->suiv = right;
        right = right->suiv;
      }
      actual = actual->suiv;
    }

    // On ajoute tous les éléments de la chaîne dans laquelle il en reste
    while (left != nullptr)
    {
      actual->suiv = left;
      left = left->suiv;
      actual = actual->suiv;
    }
    while (right != nullptr)
    {
      actual->suiv = right;
      right = right->suiv;
      actual = actual->suiv;
    }

    return head;
  }else{
    // Retourner celle non nulle ou retourner une chaîne vide
    return (head == nullptr) ? middle : head;
  }
  // Fin
}

// Fusion de 2 chaînes triées en ordre croissant
// Retourne la tête de la chaîne résultante triée
// Algorithme récursif
p_data fusionR(p_data head, p_data middle){
  // Variables

  // Début
  if(head == nullptr && middle == nullptr){
    return nullptr;

  }else{
    if(head == nullptr){
      return ajoutDevant(middle->valeur, fusionR(head, middle->suiv));
    }else if(middle == nullptr){
      return ajoutDevant(head->valeur, fusionR(middle, head->suiv));
    }else if(head->valeur < middle->valeur){
      return ajoutDevant(head->valeur, fusionR(head->suiv, middle));
    }else{
      return ajoutDevant(middle->valeur, fusionR(middle->suiv, head));
    }
  }
  // Fin
}

// Retourne le nombre de monotonies d'une chaîne
int nbCroissances(p_data chain){
  // Variables
  int nb;
  bool inMonot;

  // Début
  nb = 0; // Compteur de monotonies
  inMonot = false; // Vérifier si on est sur une croissance

  // Tester toutes les mailles
  while (chain != nullptr)
  {
    // Vérifier si l'on n'a pas plus de mailles
    if(chain != nullptr){
      // Si on commence une nouvelle monotonie, si c'est le cas on ajoute 1 au compteur
      if(!inMonot){
        inMonot = true;
        nb++;
      }
      // Si la valeur suivante est inférieure strictement on change de monotonie
      if(chain->suiv != nullptr && chain->valeur >= chain->suiv->valeur){
        inMonot = false;
      }
      // Maille suivante
      chain = chain->suiv;
    }
  }
  
  return nb;
  // Fin
}

// Place dans le chaînage mono la première monotonie croissante de chain et l’en retire
void extraireCroissance(p_data & chain, p_data & mono){
    // Variables
    p_data headRemChain; // Tête du chaînon qui va être retiré
    p_data tailRemChain; // Queue du chaîne qui va être retiré

    // Début
    headRemChain = chain;
    // Si la chaîne est vide on ne fait rien
    if(chain != nullptr){
      // Aller jusqu'à la fin de la monotonie
      while (chain->suiv != nullptr && chain->valeur <= chain->suiv->valeur)
      {
        chain = chain->suiv;
      }
      tailRemChain = chain; // Récupérer la dernière maille
      chain = chain->suiv; // Le retirer de la chaîne chain
      tailRemChain->suiv = mono; // Ajouter le chaînon à mono
      mono = headRemChain; // Retourner le nouveau mono
    }
    // Fin
}
// ==========

// ==== Partie B ====
typedef struct _datallst{
  int capa;
  int nbmono;
  p_data * monotonies;
} datalistes ;

// Crée et renvoie une structure datalistes initialisée avec un tableau de nb chaînages
datalistes initT(int nb){
  // Variables
  datalistes newStruct;

  // Début
  newStruct = *((datalistes *) malloc(sizeof(datalistes))); // Récupérer la valeur du pointeur datalistes créé

  newStruct.capa = nb;
  newStruct.nbmono = 0;
  newStruct.monotonies = new p_data[nb];

  return newStruct;
  // Fin
}

// 
/** Préconditions
 * mono.nbmono < capa
 */
void ajouterFin(p_data chain, datalistes & mono){
  // Variables
  
  // Début
  // On ajoute la chaîne si le tableau n'est pas plein
  if(mono.nbmono < mono.capa){
    mono.monotonies[mono.nbmono] = chain;
    mono.nbmono++;
  }
  // Fin
}

// Affiche sur chaque ligne les valeurs d'une case
/** Préconditions
 * mono doit être initialisé
 */
void affT(datalistes mono){
  // Variables
  
  // Début
  for (int i = 0; i < mono.nbmono; i++)
  {
    aff(mono.monotonies[i]);
  }
  cout << endl;
  // Fin
}

// Supprime
/** Préconditions
 * mono.nbmono > 0
 */
/** Postconditions
 * Retourne nullptr si mono.nbmono <= 1
 */
p_data suppressionFin(datalistes & mono){
  // Variables
  
  // Début
  if(mono.nbmono > 0){
    mono.nbmono--; // On "supprime" le dernier élément en abaissant le nombre de cases remplis, on consière la case cuivante inexistante
    if(mono.nbmono > 0){
      return mono.monotonies[mono.nbmono-1];
    }
  }
  return nullptr;
  // Fin
}

// Supprime toutes les chaînes de mono et les met bout à bout
/** Préconditions
 * mono doit être initialisé
 * mono.nbmono > 0
 */
p_data suppressionTotale(datalistes & mono){
  // Variables
  p_data tailChain; // Permet d'allaer chercher la queue de la chaîne actuelle pour ajouter la tête de la suivante
  p_data headSuiv; // Permet de sauvegarder la tête de la monotonies suivante
  
  // Début
  // S'il n'y aucune monotonie
  if(mono.nbmono == 0){
    return nullptr;
  }

  // Supprimer toutes les monotonies en partant de la fin
  for (int i = mono.nbmono; i > 0; i--)
  {
    headSuiv = mono.monotonies[i-1]; // Sauvegarder la tête de la monotonies suivante
    tailChain = suppressionFin(mono); // Supprimer la dernière monotonie et récupérer la précédente
    if(tailChain != nullptr){
      // On cherche la queue de la chaîne;
      while (tailChain != nullptr && tailChain->suiv != nullptr)
      {
        tailChain = tailChain->suiv;
      }
      tailChain->suiv = headSuiv; // La queue de la chaîne pointe vers la tête de la chaîne suivante du tableau
    }
  }
  // Retourner la première monotonie avec toutes les autres en queue
  return headSuiv;
  // Fin
}
// ==========

// ==== Partie D ====
// Supprime les monotonies de chain pour les mettre dans une nouvelle datalistes
datalistes separation(p_data & chain){
  // Variables
  datalistes mono; // Accueille toutes les monotonies
  p_data tempChain;

  // Début
  mono = initT(nbCroissances(chain)); // Affecter nouveau datalistes et récupérer la mémoire de la structure

  for (int i = 0; i < mono.capa; i++)
  {
    tempChain = nullptr;
    extraireCroissance(chain, tempChain);
    ajouterFin(tempChain, mono);
  }
  
  return mono;
  // Fin
}

// Fusionne toutes les monotonies du tableau de tabmono
void trier(datalistes & tabmono){
  // Variables
  
  // Début
  // S'il n'y aucune monotonie
  if(tabmono.nbmono > 0){
    // Supprimer toutes les monotonies en partant de la fin
    for (int i = tabmono.nbmono-1; i > 0; i--)
    {
      // Fusionner les 2 chaînes dans la première
      tabmono.monotonies[i-1] = fusion(tabmono.monotonies[i-1], tabmono.monotonies[i]); // fusion() pour la fusion non récurrente
    }
    tabmono.nbmono = 1;
  }
  // Fin
}

// Effectue un tri de chain en utilisant une fusion multiple
void trier(p_data & chain){
  // Variables
  p_data tmpChain; // Chaîne temporaire extraite
  p_data sortedChain; // Chaîne finale
  int nbCroiss; // Fin de la boucle for
  
  // Début
  sortedChain = nullptr;
  nbCroiss = nbCroissances(chain);
  // while(chain != nullptr){ // Moins coûteux mais plus compliqué de calculer le coût
  for(int i=0; i < nbCroiss; i++){ // Plus coûteux mais plus facile à calculer
    tmpChain = nullptr;
    extraireCroissance(chain, tmpChain);
    sortedChain = fusion(sortedChain, tmpChain); // fusion() pour la fusion non récurrente
  }
  chain = sortedChain;
  // Fin
}
// ==========

// ==== Partie D ====  
// Permet de générer une chaîne d'entiers contenant len entiers de valeur variant entre min et max (inclus)
p_data generateChain(int len, int min, int max){
  // Variables
  p_data chain;

  // Début
  chain = nullptr;
  for (int i = 0; i < len; i++)
  {
    chain = ajoutDevant(rand() % (max-min + 1) + min, chain);
  }
  
  return chain;
  // Fin
}
// ==========


int main(){
  // ==== Partie A ====
  // Tests aff() et ajoutDevant()
  // 0-1 élément / Ok-OK
  /* p_data test = (p_data) malloc(sizeof(data));
  test->suiv = nullptr;
  test->valeur = 1;
  // Plusieurs éléments / Ok
  test = ajoutDevant(3, ajoutDevant(2, test));
  
  aff(test); */


  // Tests ajoutDerriere() / Ajoute bien dans l'ordre
  /* p_data test = (p_data) malloc(sizeof(data));
  test->suiv = nullptr;
  test->valeur = 1;
  test = ajoutDerriere(3, ajoutDerriere(2, test));
  aff(test); */
  // aff(nullptr);
  
  
  // Tests saisieBorne()
  // aff(saisieBorne(0));

  
  // Tests saisieNombre()
  // aff(saisieNombre(0));
  // aff(saisieNombre(1));
  // aff(saisieNombre(3));


  // Tests fusion()
  // aff(fusion(saisieNombre(1), saisieNombre(1))); // 1 seul élement par chaîne Ok
  // aff(fusion(saisieNombre(2), saisieNombre(2))); // Plusieurs élements par chaîne Ok
  // aff(fusion(nullptr, saisieNombre(3))); // 1 chaîne avec des éléments, l'autre pas Ok
  // aff(fusion(saisieNombre(1), saisieNombre(3))); // 1 chaîne avec 1 élément, l'autre plusieurs Ok
  // aff(fusion(nullptr, nullptr); // Ok
  
  // Tests fusionR()
  // aff(fusionR(saisieNombre(1), saisieNombre(1))); // 1 seul élement par chaîne Ok
  // aff(fusionR(saisieNombre(2), saisieNombre(2))); // Plusieurs élements par chaîne Ok
  // aff(fusionR(nullptr, saisieNombre(3))); // 1 chaîne avec des éléments, l'autre pas Ok
  // aff(fusionR(saisieNombre(1), saisieNombre(3))); // 1 chaîne avec 1 élément, l'autre plusieurs Ok
  // aff(fusionR(nullptr, nullptr); // Ok


  // Tests nbCroissances()
  // cout << nbCroissances(saisieBorne(0)) << endl; // Saisie 0; --> Retourne 1 Ok
  // cout << nbCroissances(saisieBorne(0)) << endl; // Saisie 1;0; --> Retourne 2 ok
  cout << nbCroissances(saisieBorne(0)) << endl; // Saisie 1;1;2;3;0; --> Retourne 3 Ok
  cout << nbCroissances(nullptr) << endl; // Retourne 0 Ok


  // Tests extraireCroissance()
  /* p_data chain1 = saisieNombre(2), chain2 = saisieNombre(2);
  extraireCroissance(chain1, chain2);
  aff(chain1);
  aff(chain2); */
  // ========

  // ==== Partie B ====
  // initT() et ajouterFin()
  /* datalistes monos = initT(5);
  ajouterFin(ajoutDevant(1, nullptr), monos);
  cout << monos.nbmono << " : " << monos.capa << endl; // Devrait afficher 1 : 5 Ok
  aff(monos.monotonies[0]); // Devrait afficher la chaîne chain, donc 1; Ok */

  
  // Tests affT()
  /* datalistes monos = initT(5);
  ajouterFin(ajoutDevant(1, nullptr), monos);
  affT(monos); // Devrait afficher la chaîne chain, donc 1; Ok
  ajouterFin(ajoutDevant(1, ajoutDevant(2, ajoutDevant(3, nullptr))), monos);
  affT(monos); // Devrait afficher 1; et 1;2;3; Ok */


  // Tests suppressionFin()
  /* datalistes monos = initT(5);
  ajouterFin(ajoutDevant(1, nullptr), monos);
  ajouterFin(ajoutDevant(1, ajoutDevant(2, ajoutDevant(3, nullptr))), monos);
  affT(monos); // Devrait afficher 1; et 1;2;3; Ok
  aff(suppressionFin(monos)); // Devrait afficher 1; Ok
  affT(monos); // Devrait afficher 1; Ok */


  // Tests suppressionTotale()
  /* datalistes monos = initT(5);
  // Plusieurs monotonies à supprimer
  ajouterFin(ajoutDevant(1, nullptr), monos);
  ajouterFin(ajoutDevant(1, ajoutDevant(2, nullptr)), monos);
  ajouterFin(ajoutDevant(1, ajoutDevant(2, ajoutDevant(3, nullptr))), monos);
  affT(monos); // Devrait afficher 1;, 1;2; et 1;2;3; Ok
  aff(suppressionTotale(monos)); // Devrait afficher 1;1;2;1;2;3; Ok
  affT(monos); // Devrait rien afficher Ok
  // Aucune monotonie à supprimer
  aff(suppressionTotale(monos)); // Devrait rien Ok
  // 1 monotonie à supprimer
  ajouterFin(ajoutDevant(1, nullptr), monos);
  affT(monos); // Devrait afficher 1; Ok
  aff(suppressionTotale(monos)); // Devrait afficher 1; Ok
  affT(monos); // Devrait rien afficher Ok */
  // ========

  // ==== Partie D ====    
  // Tests separation()
  // p_data chain = saisieNombre(3); // Essayer 1,2,3 --> devrait donner 1;2;3; Ok
  // p_data chain = saisieNombre(4); // Essayer 1,2,1,2 --> devrait donner 1;2; / 1;2; Ok
  // p_data chain = saisieNombre(3); // Essayer 1,1,2 --> devrait donner 1;1;2; (car croissance non stricte) Ok
  // p_data chain = saisieNombre(1); // Essayer 1 --> devrait donner 1; Ok
  // p_data chain = nullptr; // Essayer sans valeurs --> devrait donner (rien) Ok
  // affT(separation(chain));


  // Tests trier(datalistes)
  // datalistes mono = initT(2);
  // 2 chaînes
  /* p_data chain1 = saisieNombre(2), chain2 = saisieNombre(2);
  ajouterFin(chain1, mono);
  ajouterFin(chain2, mono);
  trier(mono);
  affT(mono); */
  // 0 chaîne
  /* mono = initT(2);
  trier(mono);
  affT(mono); // Affiche rien Ok */
  // 1 chaîne
  /* mono = initT(2);
  ajouterFin(ajoutDevant(1, nullptr), mono);
  trier(mono);
  affT(mono); // Affiche 1; Ok */
  // 3 chaînes (pour ne pas voir que la première fusionnée avec 2e)
  /* datalistes mono3 = initT(3);
  p_data chain1 = saisieNombre(2), chain2 = saisieNombre(2), chain3 = saisieNombre(2);
  ajouterFin(chain1, mono3);
  ajouterFin(chain2, mono3);
  ajouterFin(chain3, mono3);
  trier(mono3);
  affT(mono3); */


  // Tests trier(p_data)
  /* p_data chain = saisieBorne(0);
  aff(chain); // Chaîne non triée
  trier(chain);
  aff(chain); // Chaîne triée */
  // ========

  // ==== Partie E ====  
  // Tests generateChain()
  /* srand(time(NULL));
  p_data chain = generateChain(15, 0, 9);
  aff(chain); // Doit retourner une chaîne aléatoire
  cout << nbCroissances(chain)<< endl; */


  // Calcul pratique du coût temporel de trier(p_data) avec une génération d'entiers
  /* srand(time(NULL)); // Générer un seed pour avoir une séquence aléatoire
  cout << "Longueur" << TAB << "Nombre monotonies" << TAB << "Temps"  <<  endl ;
  int nMin = 10, nMax = 3000, pas = 10; // Intervalle de longueur de la chaîne
  int valMin = 0, valMax = 9; // Intervalle de valeurs de la chaîne
  for (int i = nMin; i <= nMax; i+=pas)
  {
    p_data chain = generateChain(i, valMin, valMax);
    
    int nbMonos = nbCroissances(chain);

    // Temps que prends trier(p_data)
    START;
    trier(chain);
    STOP;

    cout << i << TAB << nbMonos << TAB << (long) TEMPS << endl;
  } */


  // Calcul pratique du coût temporel de trier(p_data) avec une insertions de DATATYPE
  /* int nbVal = 8; // Nombre de valeurs que l'on veut insérer (depuis un fichier par exemple)

  p_data chain = saisieNombre(nbVal);
  
  cout << endl << "Longueur" << TAB << "Nombre monotonies" << TAB << "Temps"  <<  endl ;
  int nbMonos = nbCroissances(chain);

  // Temps que prends trier(p_data)
  START;
  trier(chain);
  STOP;

  cout << nbVal << TAB << nbMonos << TAB << (long) TEMPS << endl; */
}