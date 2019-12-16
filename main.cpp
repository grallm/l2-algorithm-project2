// Projet 2 - Algorithmique et structure de données - Agorithme de tri par fusion multiple de monotonies
// Malo GRALL et Paul TOMEI - 12/2019

#include <iostream>
#include <stdio.h>

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

// Ajouter une maille en tête
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
 * Le pointeur passées en 2e argument de la fonction renvoie vers une maille de la chaîne résultante, il n'est donc plus utilisable comme tête de chaîne
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
    if((*head).valeur > (*middle).valeur){
      head = middle;
      right = (*right).suiv;
    }else{
      left = (*left).suiv;
    }
    actual = head;

    // Tant qu'il reste des éléments dans les 2 chaînes on ajoute la plus petite valeur et on décale
    while (left != nullptr && right != nullptr)
    {
      if((*left).valeur < (*right).valeur){
        (*actual).suiv = left;
        left = (*left).suiv;
      }else{
        (*actual).suiv = right;
        right = (*right).suiv;
      }
      actual = (*actual).suiv;
    }

    // On ajoute tous les éléments de la chaîne dans laquelle il en reste
    while (left != nullptr)
    {
      (*actual).suiv = left;
      left = (*left).suiv;
      actual = (*actual).suiv;
    }
    while (right != nullptr)
    {
      (*actual).suiv = right;
      right = (*right).suiv;
      actual = (*actual).suiv;
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
      return ajoutDevant((*middle).valeur, fusionR(head, (*middle).suiv));
    }else if(middle == nullptr){
      return ajoutDevant((*head).valeur, fusionR(middle, (*head).suiv));
    }else if((*head).valeur < (*middle).valeur){
      return ajoutDevant((*head).valeur, fusionR((*head).suiv, middle));
    }else{
      return ajoutDevant((*middle).valeur, fusionR((*middle).suiv, head));
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
    // Si on commence une nouvelle monotonie, si c'est le cas on ajoute 1 au compteur
    if(!inMonot){
      inMonot = true;
      nb++;
    }
    // Vérifier si l'on n'a pas plus de mailles
    if(chain != nullptr){
      // Si la valeur suivante est inférieure strictement on change de monotonie
      if((*chain).suiv != nullptr && (*chain).valeur > (*(*chain).suiv).valeur){
        inMonot = false;
      }
      // Maille suivante
      chain = (*chain).suiv;
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
      while ((*chain).suiv != nullptr && (*chain).valeur <= (*(*chain).suiv).valeur)
      {
        chain = (*chain).suiv;
      }
      tailRemChain = chain; // Récupérer la dernière maille
      chain = (*chain).suiv; // Le retirer de la chaîne chain
      (*tailRemChain).suiv = mono; // Ajouter le chaînon à mono
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
    // cout << i << endl;
    // aff(mono.monotonies[i-1]);
    // aff(mono.monotonies[i]);
    // cout << endl<< endl;
    headSuiv = mono.monotonies[i-1]; // Sauvegarder la tête de la monotonies suivante
    // cout << "i" << endl;
    tailChain = suppressionFin(mono); // Supprimer la dernière monotonie et récupérer la précédente
    // cout << "i" << endl;
    // aff(tailChain);
    if(tailChain != nullptr){
      // On cherche la queue de la chaîne;
      while (tailChain != nullptr && (*tailChain).suiv != nullptr)
      {
        // cout << "-i" << endl;
        tailChain = (*tailChain).suiv;
      }
      // cout << "i" << endl;
      (*tailChain).suiv = headSuiv; // La queue de la chaîne pointe vers la tête de la chaîne suivante du tableau
    }
  }
  // Retourner 
  return headSuiv;
  // Fin
}

/* TODO
 */
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
  // cout << nbCroissances(saisieBorne(0)) << endl;
  // cout << nbCroissances(nullptr) << endl;


  // Tests extraireCroissance()
  /* p_data chain1 = saisieNombre(2), chain2 = saisieNombre(2);
  extraireCroissance(chain1, chain2);
  aff(chain1);
  aff(chain2); */
  // ========

  // ==== Partie B ====
  // init() et ajouterFin()
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
  datalistes monos = initT(5);
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
  affT(monos); // Devrait afficher 1;
  aff(suppressionTotale(monos)); // Devrait afficher 1;
  affT(monos); // Devrait rien afficher
}