/*
 * Tabulka s rozptýlenými položkami
 *
 * S využitím datových typů ze souboru hashtable.h a připravených koster
 * funkcí implementujte tabulku s rozptýlenými položkami s explicitně
 * zretězenými synonymy.
 *
 * Při implementaci uvažujte velikost tabulky HT_SIZE.
 */

#include "hashtable.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int HT_SIZE = MAX_HT_SIZE;

/*
 * Rozptylovací funkce která přidělí zadanému klíči index z intervalu
 * <0,HT_SIZE-1>. Ideální rozptylovací funkce by měla rozprostírat klíče
 * rovnoměrně po všech indexech. Zamyslete sa nad kvalitou zvolené funkce.
 */
int get_hash(char *key) {
  int result = 1;
  int length = strlen(key);
  for (int i = 0; i < length; i++) {
    result += key[i];
  }
  return (result % HT_SIZE);
}

/*
 * Inicializace tabulky — zavolá sa před prvním použitím tabulky.
 */
void ht_init(ht_table_t *table) {
  for (int i = 0; i < HT_SIZE; i++){ //creates a table with size of HT_SIZE
    (*table)[i] = NULL;
  }
}

/*
 * Vyhledání prvku v tabulce.
 *
 * V případě úspěchu vrací ukazatel na nalezený prvek; v opačném případě vrací
 * hodnotu NULL.
 */
ht_item_t *ht_search(ht_table_t *table, char *key) {
  if ((*table) == NULL){ //table is empty
    return NULL;
  }
  //generates hash and find position with said hash
  int hash = get_hash(key); 
  ht_item_t * tElement = (*table)[hash];
  while (tElement != NULL){
    //while we dont find element thats empty we compare the key
    if (strcmp(tElement->key, key) != 0){
      tElement = tElement->next;
    }
    else{
      //when we find the one element we are looking for we return the pointer
      return tElement;
    }
    }
  //nothing was found
  return NULL;
}

/*
 * Vložení nového prvku do tabulky.
 *
 * Pokud prvek s daným klíčem už v tabulce existuje, nahraďte jeho hodnotu.
 *
 * Při implementaci využijte funkci ht_search. Pri vkládání prvku do seznamu
 * synonym zvolte nejefektivnější možnost a vložte prvek na začátek seznamu.
 */
void ht_insert(ht_table_t *table, char *key, float value) {
  //we find the position where we want to insert the element
  ht_item_t *item = ht_search(table, key);
  //if its not empty we can simply rewrite the value
  if (item != NULL){
    item->value = value;
  }
  else{ //otherwise create a new item
    ht_item_t * newItem = (ht_item_t*)malloc(sizeof(ht_item_t));
    if (newItem == NULL)return;

    newItem->key = key;
    newItem->value = value;
    newItem->next = NULL;


    int hash = get_hash(key); //we look if there are any other elements with the same hash
    if ((*table)[hash] != NULL){
      newItem->next = (*table)[get_hash(key)]; //we put these elements behind our new item
    }
    (*table)[hash] = newItem;
  }
}

/*
 * Získání hodnoty z tabulky.
 *
 * V případě úspěchu vrací funkce ukazatel na hodnotu prvku, v opačném
 * případě hodnotu NULL.
 *
 * Při implementaci využijte funkci ht_search.
 */
float *ht_get(ht_table_t *table, char *key) {
  ht_item_t *item = ht_search(table, key);
  if (item != NULL){ 
    return &(item->value);
  }
  return NULL;
}

/*
 * Smazání prvku z tabulky.
 *
 * Funkce korektně uvolní všechny alokované zdroje přiřazené k danému prvku.
 * Pokud prvek neexistuje, funkce nedělá nic.
 *
 * Při implementaci NEPOUŽÍVEJTE funkci ht_search.
 */
void ht_delete(ht_table_t *table, char *key) {
  int hash = get_hash(key);
    ht_item_t *tElement = (*table)[hash];
    ht_item_t *prevTElement = NULL;

    while (tElement != NULL) {
        if (strcmp(tElement->key, key) == 0) {
            if (prevTElement == NULL) {
                (*table)[hash] = tElement->next;
            } else {
                prevTElement->next = tElement->next;
            }
            free(tElement);
            return;
        }
        prevTElement = tElement;
        tElement = tElement->next;
    }
}

/*
 * Smazání všech prvků z tabulky.
 *
 * Funkce korektně uvolní všechny alokované zdroje a uvede tabulku do stavu po 
 * inicializaci.
 */
void ht_delete_all(ht_table_t *table) {
  for (int i = 0; i < HT_SIZE; i++){
    ht_item_t *current = (*table)[i];
    while (current != NULL){
      ht_item_t *tmp = current->next;
      free(current);
      current = tmp;
    }
    (*table)[i] = NULL;
  }
  table = NULL;
}