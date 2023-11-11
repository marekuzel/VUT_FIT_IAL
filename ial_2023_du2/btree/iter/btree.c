/*
 * Binární vyhledávací strom — iterativní varianta
 *
 * S využitím datových typů ze souboru btree.h, zásobníku ze souboru stack.h 
 * a připravených koster funkcí implementujte binární vyhledávací 
 * strom bez použití rekurze.
 */

#include "../btree.h"
#include "stack.h"
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
void bst_init(bst_node_t **tree) {
  //initialize the tree
  (*tree) = NULL;
}

/*
 * Vyhledání uzlu v stromu.
 *
 * V případě úspěchu vrátí funkce hodnotu true a do proměnné value zapíše
 * hodnotu daného uzlu. V opačném případě funkce vrátí hodnotu false a proměnná
 * value zůstává nezměněná.
 * 
 * Funkci implementujte iterativně bez použité vlastních pomocných funkcí.
 */
bool bst_search(bst_node_t *tree, char key, int *value) {
  bool search = false; //set the search to false
  while (tree != NULL){
    if (tree->key == key){ //we found the key
      *value = tree->value; //we set the value o the key and set the search to true
      search = true;
      break;
    }
    else{
      if (key < tree->key){ //if the key is smaller than the current key, we traverse into the left subtree
        tree = tree->left;
      }
      else{
        tree = tree->right; //simularly, if the key is bigger than the current key, we traverse into the right subtree
      }
    }
  }
  return search;
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
 * Funkci implementujte iterativně bez použití vlastních pomocných funkcí.
 */
void bst_insert(bst_node_t **tree, char key, int value) {
  bst_node_t *newNode = malloc(sizeof(bst_node_t)); //allocate the new node
  if (newNode == NULL) return;
  newNode->key = key;
  newNode->value = value;
  newNode->right = NULL;
  newNode->left = NULL;

  //see if the tree is empty
  if ((*tree) == NULL || tree == NULL){
    (*tree) = newNode;
    return;
  }
  bst_node_t *position; //set pointer for position 
  bst_node_t *tempTree = *tree; //set pointer for temporary tree so we dont lose the root

  while (tempTree != NULL){
    position = tempTree; //set new position

    if (tempTree->key > key){ //if the key is smaller than the current key, we traverse into the left subtree
      tempTree = tempTree->left;
    }
    else if (tempTree != NULL){//simularly, if the key is bigger than the current key, we traverse into the right subtree
    if (tempTree->key < key){
      tempTree = tempTree->right;
    }
    else if (tempTree->key == key){ //if the key value is the same as the current key, we replace the value
      position->value = value;
      free(newNode);//free the new node that we now dont need
      return;
    }
  }
  
  }
  if (position == NULL){
    position = newNode; //if the position is null we set the new node as the root
  }
  else if (position->key > key){//if the key is smaller than the current key, we insert the new node into the left subtree
    position->left = newNode;
    position->left->value = value;
    position->left->key = key;
  }
  else if (position->key < key){//otherwise we insert the new node into the right subtree
    position->right = newNode;
  }
}

/*
 * Pomocná funkce která nahradí uzel nejpravějším potomkem.
 * 
 * Klíč a hodnota uzlu target budou nahrazené klíčem a hodnotou nejpravějšího
 * uzlu podstromu tree. Nejpravější potomek bude odstraněný. Funkce korektně
 * uvolní všechny alokované zdroje odstraněného uzlu.
 *
 * Funkce předpokládá, že hodnota tree není NULL.
 * 
 * Tato pomocná funkce bude využita při implementaci funkce bst_delete.
 *
 * Funkci implementujte iterativně bez použití vlastních pomocných funkcí.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {
  bst_node_t *tempTree = *tree; //we create a temporary tree and target so we dont lose the root
  bst_node_t *tempTarget = target;

  if (tempTree == NULL)return; //if the tree is empty, we return

  while (tempTree->right != NULL){
    tempTarget = tempTree; //while the right pointer is not null, we traverse into the right subtree
    tempTree = tempTree->right;
  }
  target->value = tempTree->value; //we replace the value and key of the target with the value and key from the temporary tree
  target->key = tempTree->key;
  if(target == tempTarget){ 
      tempTarget->left = NULL;
  }
  else{
      tempTarget->right= NULL;
  }
  free(tempTree);
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
 * Funkci implementujte iterativně pomocí bst_replace_by_rightmost a bez
 * použití vlastních pomocných funkcí.
 */
void bst_delete(bst_node_t **tree, char key) {
  bst_node_t *current = *tree;
  bst_node_t *previous = NULL;
  
  while (current != NULL){ //we look for the key, when we find it, we break the loop
    if (current->key == key){ 
      break;
    }
    else{
      previous = current;
      if (current->key > key){
        current = current->left;
      }
      else{
        current = current->right;
      }
    }
  }
  if (current == NULL){
    return; //if the key is not in the tree, we exit the function
  }
  else{
    if (current->left == NULL && current->right == NULL){ //if the node is a leaf we can simply delete it
      if (previous == NULL){
        free(current);
        *tree = NULL;
      }
      else{
        //we check if the node is the left or right child of the parent
        //if it is we need to change pointer to NULL
        if (current == previous->left){ 
          previous->left = NULL;
        }
        else if (current == previous->right){
          previous->right = NULL;
        }
        free(current); 
      }
    }
    else if (current->left == NULL){ 
      //if the node has only right subtree, we replace it with it
      if (previous == NULL){ 
        *tree = current->right;
        free(current);
      }
      else{
        //we check if the node is the left or right child of the parent
        //if it is we change the pointer to the right subtree
        if (previous->left == current){
          previous->left = current->right;
        }
        else{
          previous->right = current->right;
        }
        free(current);
      }
    }
    else if (current->right == NULL){
      //in case the node has only left subtree, we replace it with it
      if (previous == NULL){
        *tree = current->left;
        free(current);
      }
      else{
        //in the same fashion, we check if the node is the left or right child of the parent
        if (previous->left == current){
          previous->left = current->left;
        }
        else{
          previous->right = current->left;
        }
        free(current);
      }
    }
    else{ //if the node has both subtrees, we use our helper function
      bst_replace_by_rightmost(current, &current->left);
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
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití 
 * vlastních pomocných funkcí.
 */
void bst_dispose(bst_node_t **tree) {
  if ((*tree) == NULL || tree == NULL)return; //if the tree is empty, we return

  stack_bst_t *stack = malloc(sizeof(stack_bst_t)); //initialize the stack
  stack_bst_init(stack);

  bst_node_t *temp = *tree; //we create a temporary tree so we dont lose the root
  while (temp != NULL || !stack_bst_empty(stack)){ //while the temporary tree is not null or the stack is not empty
    if (temp == NULL){
      if (!stack_bst_empty(stack)){//in case the stack is not empty we pop the top of the stack
        temp = stack_bst_top(stack);
        stack_bst_pop(stack);
      }
    }
    else{ //if the temp tree is not empty, we push the right subtree into the stack and free the temporary tre
      if (temp->right != NULL){
        stack_bst_push(stack, temp->right);
      }
      bst_node_t *tmpPtr = temp;
      temp = tmpPtr->left;
      free (tmpPtr);
    }
  }
  *tree = NULL; //we set the tree to null and free the stack
  free(stack);
}

/*
 * Pomocná funkce pro iterativní preorder.
 *
 * Prochází po levé větvi k nejlevějšímu uzlu podstromu.
 * Nad zpracovanými uzly zavolá bst_add_node_to_items a uloží je do zásobníku uzlů.
 *
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití 
 * vlastních pomocných funkcí.
 */
void bst_leftmost_preorder(bst_node_t *tree, stack_bst_t *to_visit, bst_items_t *items) {
  bst_node_t *temp = tree; //we create a temporary tree so we dont lose the root
  while (temp != NULL){  //while the tree isnt empty, we push the elements into the stack and add them to the items
    stack_bst_push(to_visit, temp);
    bst_add_node_to_items (temp, items);
    temp = temp->left; //we traverse into the left subtree
  }
}

/*
 * Preorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_preorder a
 * zásobníku uzlů a bez použití vlastních pomocných funkcí.
 */
void bst_preorder(bst_node_t *tree, bst_items_t *items) {
  stack_bst_t *stack = malloc(sizeof(stack_bst_t));
  if (stack == NULL)return;
  stack_bst_init(stack);
  bst_leftmost_preorder(tree, stack, items);
  while (!stack_bst_empty(stack)){
    tree = stack_bst_top(stack);
    stack_bst_pop(stack);
    bst_leftmost_preorder(tree->right, stack, items);
  }
  free(stack);
}

/*
 * Pomocná funkce pro iterativní inorder.
 * 
 * Prochází po levé větvi k nejlevějšímu uzlu podstromu a ukládá uzly do
 * zásobníku uzlů.
 *
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití 
 * vlastních pomocných funkcí.
 */
void bst_leftmost_inorder(bst_node_t *tree, stack_bst_t *to_visit) {
bst_node_t *temp = tree;
  while (temp != NULL){
    stack_bst_push(to_visit, temp);
    temp = temp->left;
  }
}

/*
 * Inorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_inorder a
 * zásobníku uzlů a bez použití vlastních pomocných funkcí.
 *
 *
 * Funkci implementujte iterativně pomocí zásobníku uzlů a bool hodnot a bez použití
 * vlastních pomocných funkcí.
 */
void bst_inorder(bst_node_t *tree, bst_items_t *items) {
  stack_bst_t *stack = malloc(sizeof(stack_bst_t));
  stack_bst_init(stack);
  bst_leftmost_inorder(tree, stack);
  while (!stack_bst_empty(stack)){
    tree = stack_bst_top(stack);
    stack_bst_pop(stack);
    bst_add_node_to_items(tree, items);
    bst_leftmost_inorder(tree->right, stack);
  }
  free(stack);
}


void bst_leftmost_postorder(bst_node_t *tree, stack_bst_t *to_visit, stack_bool_t *first_visit) {
  bst_node_t *tmp = tree;
  while(tmp != NULL){
    stack_bst_push(to_visit, tmp);
    stack_bool_push(first_visit, true);
    tmp = tmp->left;
  }
  free(tmp); 
}

/*
 * Postorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_postorder a
 * zásobníku uzlů a bool hodnot a bez použití vlastních pomocných funkcí.
 */
void bst_postorder(bst_node_t *tree, bst_items_t *items) {
  bool fromLeft;
  stack_bst_t *stack = malloc(sizeof(stack_bst_t));
  stack_bst_init(stack);
  stack_bool_t *bStack = malloc(sizeof(stack_bool_t));
  stack_bool_init(bStack);
  bst_leftmost_postorder(tree, stack, bStack);
  while (!(stack_bst_empty(stack))){
    tree = stack_bst_top(stack);
    fromLeft = stack_bool_top(bStack);
    stack_bool_pop(bStack);
    if (fromLeft){
      stack_bool_push(bStack, false);
      bst_leftmost_postorder(tree->right,stack, bStack);
    }
    else{
      stack_bst_pop(stack);
      bst_add_node_to_items(tree, items);
    }
  }
  free(stack);
  free(bStack);
}
