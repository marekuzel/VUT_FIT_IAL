/*
 * Použití binárních vyhledávacích stromů.
 *
 * S využitím Vámi implementovaného binárního vyhledávacího stromu (soubory ../iter/btree.c a ../rec/btree.c)
 * implementujte funkci letter_count. Výstupní strom může být značně degradovaný (až na úroveň lineárního seznamu) 
 * a tedy implementujte i druhou funkci (bst_balance), která strom, na požadavek uživatele, vybalancuje.
 * Funkce jsou na sobě nezávislé a tedy automaticky NEVOLEJTE bst_balance v letter_count.
 * 
 */

#include "../btree.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * Vypočítání frekvence výskytů znaků ve vstupním řetězci.
 * 
 * Funkce inicilializuje strom a následně zjistí počet výskytů znaků a-z (case insensitive), znaku 
 * mezery ' ', a ostatních znaků (ve stromu reprezentováno znakem podtržítka '_'). Výstup je v 
 * uložen ve stromu.
 * 
 * Například pro vstupní řetězec: "abBccc_ 123 *" bude strom po běhu funkce obsahovat:
 * 
 * key | value
 * 'a'     1
 * 'b'     2
 * 'c'     3
 * ' '     2
 * '_'     5
 * 
 * Pro implementaci si můžete v tomto souboru nadefinovat vlastní pomocné funkce.
*/
char upperToLower(char c){ //simle function to convert upper case to lower case to make program case insensitive
    int diff = ('a'-'A');
    return c + diff;
}

void letter_count(bst_node_t **tree, char *input) {
    bst_init(tree); //initialize tree structure

    for (int i = 0;(char)input[i] != '\0'; i++){
        char c = input[i];
        if (c <= 'Z' && c >= 'A'){
            c = upperToLower(c); //convert to lower case
        }
        else if ((c >= 'a' && c <= 'z') || c == ' '){
            c = c; //do nothing
        }
        else{
            c = '_';//convert to underscore
        }

        int count;
        if (bst_search(*tree, c, &count)){ //if key is already in tree
            bst_insert(tree, c, ++count); //increase value
        }
        else bst_insert(tree, c, 1); //insert with value 1
    }
}





/**
 * Vyvážení stromu.
 * 
 * Vyvážený binární vyhledávací strom je takový binární strom, kde hloubka podstromů libovolného uzlu se od sebe liší maximálně o jedna.
 * 
 * Předpokládejte, že strom je alespoň inicializován. K získání uzlů stromu využijte vhodnou verzi vámi naimplmentovaného průchodu stromem.
 * Následně můžete například vytvořit nový strom, kde pořadím vkládaných prvků zajistíte vyváženost.
 *  
 * Pro implementaci si můžete v tomto souboru nadefinovat vlastní pomocné funkce. Není nutné, aby funkce fungovala *in situ* (in-place).
*/

bst_node_t* constrBrnaches(bst_items_t *items, int start, int end){
    if (start > end) return NULL;
    int middle = (start + end) / 2;//get middle node
    bst_node_t *root = items->nodes[middle];
    root->left = constrBrnaches(items, start, middle-1); //construct left branch from nodes smaller than middle
    root->right = constrBrnaches(items, middle+1, end);//construct right branch from nodes bigger than middle
    return root; //return root of new tree
}

void bst_balance(bst_node_t **tree) {
    //ill use recursive algorithm of adding nodes to tree to balance it
    bst_items_t *items = malloc(sizeof(bst_items_t));
    items->nodes = malloc(sizeof(bst_items_t) * 20);
    items->capacity = 20;
    items->size = 0;
    //fill items with nodes in inorder
    bst_inorder(*tree, items);

    
    bst_node_t *newTree = constrBrnaches(items, 0, items->size-1); //call recursive function to construct branches
    *tree = newTree;
    free(items->nodes);
    free(items);
}