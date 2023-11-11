/*
 * Binární vyhledávací strom — rekurzivní varianta
 *
 * S využitím datových typů ze souboru btree.h a připravených koster funkcí
 * implementujte binární vyhledávací strom pomocí rekurze.
 */

#include "../btree.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Inicializace stromu.
 *
 * Uživatel musí zajistit, že inicializace se nebude opakovaně volat nad
 * inicializovaným stromem. V opačném případě může dojít k úniku paměti (memory
 * leak). Protože neinicializovaný ukazatel má nedefinovanou hodnotu, není
 * možné toto detekovat ve funkci. 
 */

void bst_init(bst_node_t **tree){
  //initialized tree by pointing root to NULL
  *tree = NULL;
}

/*
 * Vyhledání uzlu v stromu.
 *
 * V případě úspěchu vrátí funkce hodnotu true a do proměnné value zapíše
 * hodnotu daného uzlu. V opačném případě funkce vrátí hodnotu false a proměnná
 * value zůstává nezměněná.
 * 
 * Funkci implementujte rekurzivně bez použité vlastních pomocných funkcí.
 */
bool bst_search(bst_node_t *tree, char key, int *value) {
  if (tree == NULL) return false; //value was not found
  else {
    if (tree->key == key){
      //the value was found in root
      //value is assigned and returns true
      *value = tree->value;
      return true;
    }
    //if not found and key is smaller than root, recursively call function on left subtree
    else if (key < tree->key)return (bst_search(tree->left, key, value));
    //otherwise ke is larger, thus trying to find match in right subtree
    else return (bst_search(tree->right, key, value));
    return false;
  }
}
/*
 * Vložení uzlu do stromu.
 *
 * Pokud uzel se zadaným klíče už ve stromu existuje, nahraďte jeho hodnotu.
 * Jinak vložte nový listový uzel.
 *
 * Výsledný strom musí splňovat podmínku vyhledávacího stromu — levý podstrom
 * uzlu obsahuje jenom menší klíče, pravý větší. 
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_insert(bst_node_t **tree, char key, int value) {
  if ((*tree)== NULL){ //if there are no other nodes in the tree (or subtree), we simply initiate one node at the root
    bst_node_t *newNode = (bst_node_t *)malloc(sizeof(bst_node_t));
    if (newNode == NULL) return;
    (*tree) = newNode;
    (*tree)->key = key;
    (*tree)->value = value;
    (*tree)->right = NULL;
    (*tree)->left = NULL;
  }
  else{
    if ((*tree)->key > key){ //if key of existing node is higher than the one we are inserting, we recursively call insert on right subtree
      bst_insert(&((*tree)->left), key, value);
    }
    else if ((*tree)->key < key){
      bst_insert(&((*tree)->right), key, value); //if key is lower, we do the same on left side
    }
    else{
      (*tree)->value = value; //we already have this key in our tree, so we rewrite the node value
    }
  }
}

/*
 * Pomocná funkce která nahradí uzel nejpravějším potomkem.
 * 
 * Klíč a hodnota uzlu target budou nahrazeny klíčem a hodnotou nejpravějšího
 * uzlu podstromu tree. Nejpravější potomek bude odstraněný. Funkce korektně
 * uvolní všechny alokované zdroje odstraněného uzlu.
 *
 * Funkce předpokládá, že hodnota tree není NULL.
 * 
 * Tato pomocná funkce bude využitá při implementaci funkce bst_delete.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {
  if ((*tree)->right == NULL){ //we found the rightmost node
    target->key = (*tree)->key;
    target->value = (*tree)->value; 
    bst_delete(tree, target->key);
  }
  else{ //we shift by recursivelly calling this function into next right node
    bst_replace_by_rightmost(target,&((*tree)->right)); 
  }

}

/*
 * Odstranění uzlu ze stromu.
 *
 * Pokud uzel se zadaným klíčem neexistuje, funkce nic nedělá.
 * Pokud má odstraněný uzel jeden podstrom, zdědí ho rodič odstraněného uzlu.
 * Pokud má odstraněný uzel oba podstromy, je nahrazený nejpravějším uzlem
 * levého podstromu. Nejpravější uzel nemusí být listem.
 * 
 * Funkce korektně uvolní všechny alokované zdroje odstraněného uzlu.
 * 
 * Funkci implementujte rekurzivně pomocí bst_replace_by_rightmost a bez
 * použití vlastních pomocných funkcí.
 */
void bst_delete(bst_node_t **tree, char key) {
  if (tree == NULL || (*tree) == NULL)return; //if the tree is empty
  else{
    if (key < (*tree)->key){ //if the key we are trying to delete is in left subtree
      bst_delete(&((*tree)->left), key); 
    }
    else if (key > (*tree)->key){ //or in right subtree
      bst_delete(&((*tree)->right), key);
    } 
    else if (key == (*tree)->key){ //we found desired node
      if ((*tree)->left == NULL && (*tree)->right == NULL){ //if it has neither left nor right subtree, we can simply delete it
        free(*tree);
        *tree = NULL;
      }
      else if ((*tree)->left != NULL && (*tree)->right != NULL){//we use helper function to replace the node with rightmost node of left subtree
        bst_replace_by_rightmost(*tree, &((*tree)->left));
      }
      else if ((*tree)->left == NULL){ //if it has only right subtree, we replace it with it
          bst_node_t *tmp = *tree;
          (*tree) = (*tree)->right;
          free(tmp);
      }
      else{ //if it has only left subtree, we replace it with it
          bst_node_t *tmp = *tree;
          (*tree) = (*tree)->left;
          free(tmp);
      }
    }
  }
}

/*
 * Zrušení celého stromu.
 * 
 * Po zrušení se celý strom bude nacházet ve stejném stavu jako po 
 * inicializaci. Funkce korektně uvolní všechny alokované zdroje rušených 
 * uzlů.
 * 
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_dispose(bst_node_t **tree) {
  //we use postorder algorithm and recursivelly call dispose first on subtrees and then on a node itself
  if ((*tree) != NULL){
      bst_dispose(&((*tree)->left));
      bst_dispose(&((*tree)->right));
      bst_delete(tree, (*tree)->key);
  }
  *tree = NULL;
}

/*
 * Preorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_preorder(bst_node_t *tree, bst_items_t *items) {
  if (tree != NULL){
    //we add the node to items, and afterwards call preorder on left and right subtree
    bst_add_node_to_items(tree, items);
    bst_preorder(tree->left, items);
    bst_preorder(tree->right, items);
  }
}

/*
 * Inorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_inorder(bst_node_t *tree, bst_items_t *items) {
  if (tree != NULL){
    //similarly to other recusive traversal functions, we call inorder on left subtree,then add the node to items and then call inorder on right subtree
    bst_inorder(tree->left, items);
    bst_add_node_to_items(tree, items);
    bst_inorder(tree->right, items);
  }
}

/*
 * Postorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_postorder(bst_node_t *tree, bst_items_t *items) {
    if (tree != NULL){
      //we call postorder on left and right subtree and then add the node to items
      bst_postorder(tree->left, items);
      bst_postorder(tree->right, items);
      bst_add_node_to_items(tree, items);
  }
}
