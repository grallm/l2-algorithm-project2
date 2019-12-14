#include <iostream>
#include <stdio.h>

using namespace std;


// On utilise des ints pour tester
typedef int DATATYPE;

typedef struct _datum{
    DATATYPE valeur;
    _datum * suiv;
} data;
typedef _datum * p_data;


/** TODO
 * Garder l'ordre de saisie dans saisieNombre et saisieBorne
*/

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
    maille = new data;
    
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
*/
// Postconditions
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
    }else if((*head).valeur >= (*middle).valeur){
      return ajoutDevant((*middle).valeur, fusionR((*middle).suiv, head));
    }else{
      aff(head);
      aff(middle);
      cout << "err" << endl;
      return head;
    }
  }
  // Fin
}


/** TODO
int nbCroissances(p_data chain){}

void extraireCroissance(p_data & chain, p_data & mono){}
*/


int main(){
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
    
    // Tests fusionR()
    // aff(fusionR(saisieNombre(1), saisieNombre(1))); // 1 seul élement par chaîne Ok
    // aff(fusionR(saisieNombre(2), saisieNombre(2))); // Plusieurs élements par chaîne Ok
    // aff(fusionR(nullptr, saisieNombre(3))); // 1 chaîne avec des éléments, l'autre pas Ok
    aff(fusionR(saisieNombre(1), saisieNombre(3))); // 1 chaîne avec 1 élément, l'autre plusieurs Ok
}