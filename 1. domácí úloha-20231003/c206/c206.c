/* ******************************* c206.c *********************************** */
/*  Předmět: Algoritmy (IAL) - FIT VUT v Brně                                 */
/*  Úkol: c206 - Dvousměrně vázaný lineární seznam                            */
/*  Návrh a referenční implementace: Bohuslav Křena, říjen 2001               */
/*  Vytvořil: Martin Tuček, říjen 2004                                        */
/*  Upravil: Kamil Jeřábek, září 2020                                         */
/*           Daniel Dolejška, září 2021                                       */
/*           Daniel Dolejška, září 2022                                       */
/* ************************************************************************** */
/*
** Implementujte abstraktní datový typ dvousměrně vázaný lineární seznam.
** Užitečným obsahem prvku seznamu je hodnota typu int. Seznam bude jako datová
** abstrakce reprezentován proměnnou typu DLList (DL znamená Doubly-Linked
** a slouží pro odlišení jmen konstant, typů a funkcí od jmen u jednosměrně
** vázaného lineárního seznamu). Definici konstant a typů naleznete
** v hlavičkovém souboru c206.h.
**
** Vaším úkolem je implementovat následující operace, které spolu s výše
** uvedenou datovou částí abstrakce tvoří abstraktní datový typ obousměrně
** vázaný lineární seznam:
**
**      DLL_Init ........... inicializace seznamu před prvním použitím,
**      DLL_Dispose ........ zrušení všech prvků seznamu,
**      DLL_InsertFirst .... vložení prvku na začátek seznamu,
**      DLL_InsertLast ..... vložení prvku na konec seznamu,
**      DLL_First .......... nastavení aktivity na první prvek,
**      DLL_Last ........... nastavení aktivity na poslední prvek,
**      DLL_GetFirst ....... vrací hodnotu prvního prvku,
**      DLL_GetLast ........ vrací hodnotu posledního prvku,
**      DLL_DeleteFirst .... zruší první prvek seznamu,
**      DLL_DeleteLast ..... zruší poslední prvek seznamu,
**      DLL_DeleteAfter .... ruší prvek za aktivním prvkem,
**      DLL_DeleteBefore ... ruší prvek před aktivním prvkem,
**      DLL_InsertAfter .... vloží nový prvek za aktivní prvek seznamu,
**      DLL_InsertBefore ... vloží nový prvek před aktivní prvek seznamu,
**      DLL_GetValue ....... vrací hodnotu aktivního prvku,
**      DLL_SetValue ....... přepíše obsah aktivního prvku novou hodnotou,
**      DLL_Previous ....... posune aktivitu na předchozí prvek seznamu,
**      DLL_Next ........... posune aktivitu na další prvek seznamu,
**      DLL_IsActive ....... zjišťuje aktivitu seznamu.
**
** Při implementaci jednotlivých funkcí nevolejte žádnou z funkcí
** implementovaných v rámci tohoto příkladu, není-li u funkce explicitně
 * uvedeno něco jiného.
**
** Nemusíte ošetřovat situaci, kdy místo legálního ukazatele na seznam
** předá někdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodně komentujte!
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako procedury
** (v jazyce C procedurám odpovídají funkce vracející typ void).
**
**/

#include "c206.h"

bool error_flag;
bool solved;

/**
 * Vytiskne upozornění na to, že došlo k chybě.
 * Tato funkce bude volána z některých dále implementovaných operací.
 */
void DLL_Error(void) {
	printf("*ERROR* The program has performed an illegal operation.\n");
	error_flag = true;
}

/**
 * Provede inicializaci seznamu list před jeho prvním použitím (tzn. žádná
 * z následujících funkcí nebude volána nad neinicializovaným seznamem).
 * Tato inicializace se nikdy nebude provádět nad již inicializovaným seznamem,
 * a proto tuto možnost neošetřujte.
 * Vždy předpokládejte, že neinicializované proměnné mají nedefinovanou hodnotu.
 *
 * @param list Ukazatel na strukturu dvousměrně vázaného seznamu
 */
void DLL_Init( DLList *list ) {
	list->firstElement = NULL;
	list->lastElement = NULL;
	list->activeElement = NULL;
}

/**
 * Zruší všechny prvky seznamu list a uvede seznam do stavu, v jakém se nacházel
 * po inicializaci.
 * Rušené prvky seznamu budou korektně uvolněny voláním operace free.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Dispose( DLList *list ) {
	//we create temporary variable tempElem to momentarly store address of the last element, we then go shift the 
	while (list->firstElement != NULL){
		DLLElementPtr tempElem = list->firstElement;
		if (list->firstElement == list->lastElement){
			list->firstElement = NULL;
			list->lastElement = NULL;
		}
		else{
			list->firstElement = tempElem->nextElement;
			list->firstElement->previousElement = NULL;
		}
		free(tempElem);
	}
	list->activeElement = NULL;
}

/**
 * Vloží nový prvek na začátek seznamu list.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení na začátek seznamu
 */
void DLL_InsertFirst( DLList *list, int data ) {
	DLLElementPtr tempElem = malloc(sizeof(struct DLLElement));
	if (tempElem == NULL){
		DLL_Error();
		return;
	}
	tempElem->data = data;
	if (list->firstElement == NULL && list->lastElement == NULL){
		list->firstElement = tempElem;
		list->lastElement = tempElem;
		return;
	}
	tempElem->nextElement = list->firstElement;
	list->firstElement->previousElement = tempElem;
	list->firstElement = tempElem;
	
}

/**
 * Vloží nový prvek na konec seznamu list (symetrická operace k DLL_InsertFirst).
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení na konec seznamu
 */
void DLL_InsertLast( DLList *list, int data ) {
	DLLElementPtr tempElem = malloc(sizeof(struct DLLElement));
	if (tempElem == NULL){
		DLL_Error();
		return;
	}
	tempElem->data = data;
	if (list->firstElement == NULL){
		list->firstElement = tempElem;
		list->lastElement = tempElem;
		return;
	}
	tempElem->previousElement = list->lastElement;
	list->lastElement->nextElement = tempElem;
	list->lastElement = tempElem;
	tempElem->nextElement = NULL;
}

/**
 * Nastaví první prvek seznamu list jako aktivní.
 * Funkci implementujte jako jediný příkaz, aniž byste testovali,
 * zda je seznam list prázdný.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_First( DLList *list ) {
	list->activeElement = list->firstElement;
}

/**
 * Nastaví poslední prvek seznamu list jako aktivní.
 * Funkci implementujte jako jediný příkaz, aniž byste testovali,
 * zda je seznam list prázdný.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Last( DLList *list ) {
	list->activeElement = list->lastElement;
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu prvního prvku seznamu list.
 * Pokud je seznam list prázdný, volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetFirst( DLList *list, int *dataPtr ) {
	if (list->firstElement == NULL){
		DLL_Error();
		return;
	}
	*dataPtr = list->firstElement->data;
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu posledního prvku seznamu list.
 * Pokud je seznam list prázdný, volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetLast( DLList *list, int *dataPtr ) {
	*dataPtr = list->lastElement->data;
	if (list->firstElement == NULL){
		DLL_Error();
		return;
	}
}

/**
 * Zruší první prvek seznamu list.
 * Pokud byl první prvek aktivní, aktivita se ztrácí.
 * Pokud byl seznam list prázdný, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteFirst( DLList *list ) {
	if (list->firstElement == NULL)return;
	DLLElementPtr tempElem = list->firstElement;
	if (list->firstElement == list->activeElement){
		list->activeElement = NULL;
	}
	else if (list->firstElement == list->lastElement){
		list->firstElement = NULL;
		list->lastElement = NULL;
	}
	else{
		list->firstElement = tempElem->nextElement;
		list->firstElement->previousElement = NULL;
	}
	free(tempElem);
}

/**
 * Zruší poslední prvek seznamu list.
 * Pokud byl poslední prvek aktivní, aktivita seznamu se ztrácí.
 * Pokud byl seznam list prázdný, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteLast( DLList *list ) {	
	if (list->firstElement == NULL)return;
	DLLElementPtr tempElem = list->lastElement;
	if (list->lastElement == list->activeElement){
		list->activeElement = NULL;
	}
	else if (list->firstElement == list->lastElement){
		list->firstElement = NULL;
		list->lastElement = NULL;
	}
	else{
		list->lastElement = tempElem->previousElement;
		list->lastElement->nextElement = NULL;
	}
	free(tempElem);
}

/**
 * Zruší prvek seznamu list za aktivním prvkem.
 * Pokud je seznam list neaktivní nebo pokud je aktivní prvek
 * posledním prvkem seznamu, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteAfter( DLList *list ) { //TODO: In some cases last element needs to be updated
	if (list->activeElement==NULL)return;
	if (list->activeElement->nextElement != NULL){
		DLLElementPtr tempElem = list->activeElement->nextElement;
		list->activeElement->nextElement = tempElem->nextElement;
		if (tempElem == list->lastElement) list->lastElement = list->activeElement;
		else tempElem->nextElement->previousElement = list->activeElement;
	free(tempElem);
	}
}

/**
 * Zruší prvek před aktivním prvkem seznamu list .
 * Pokud je seznam list neaktivní nebo pokud je aktivní prvek
 * prvním prvkem seznamu, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteBefore( DLList *list ) { //TODO: In some cases first element needs to be updated
	if (list->activeElement==NULL)return;
	if (list->activeElement->previousElement != NULL){
		DLLElementPtr tempElem = list->activeElement->previousElement;
		list->activeElement->previousElement = tempElem->previousElement;
		if (tempElem == list->firstElement) list->firstElement = list->activeElement;
		else tempElem->previousElement->nextElement = list->activeElement;
	free(tempElem);
	}
}

/**
 * Vloží prvek za aktivní prvek seznamu list.
 * Pokud nebyl seznam list aktivní, nic se neděje.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení do seznamu za právě aktivní prvek
 */
void DLL_InsertAfter( DLList *list, int data ) {//TODO: In some cases last element needs to be updated
	if (list->activeElement == NULL){
		return;
	}
	DLLElementPtr Elem = malloc(sizeof(struct DLLElement));
	if (Elem == NULL){
		DLL_Error();
		return;
	}
	Elem->data = data;
	Elem->previousElement = list->activeElement;
	if (list->activeElement == list->lastElement){
		list->activeElement->nextElement = Elem;
		list->lastElement = Elem;
		list->lastElement->nextElement = NULL;
	}
	else{
		Elem->nextElement = list->activeElement->nextElement;
		list->activeElement->nextElement->previousElement = Elem;
		list->activeElement->nextElement = Elem;
	}
	
}

/**
 * Vloží prvek před aktivní prvek seznamu list.
 * Pokud nebyl seznam list aktivní, nic se neděje.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení do seznamu před právě aktivní prvek
 */
void DLL_InsertBefore( DLList *list, int data ) { //TODO: In some cases first element needs to be updated
	if (list->activeElement == NULL){
		return;
	}
	DLLElementPtr Elem = malloc(sizeof(struct DLLElement));
	if (Elem == NULL){
		DLL_Error();
		return;
	}
	Elem->data = data;
	Elem->nextElement = list->activeElement;
	if (list->activeElement == list->firstElement){
		list->activeElement->previousElement = Elem;
		list->firstElement = Elem;
		list->firstElement->previousElement = NULL;
	}
	else{
		Elem->previousElement = list->activeElement->previousElement;
		list->activeElement->previousElement->nextElement = Elem;
		list->activeElement->previousElement = Elem;
	}
	
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu aktivního prvku seznamu list.
 * Pokud seznam list není aktivní, volá funkci DLL_Error ().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetValue( DLList *list, int *dataPtr ) {
	if (list->activeElement == NULL){
		DLL_Error();
		return;
	}
	*dataPtr = list->activeElement->data;
}

/**
 * Přepíše obsah aktivního prvku seznamu list.
 * Pokud seznam list není aktivní, nedělá nic.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Nová hodnota právě aktivního prvku
 */
void DLL_SetValue( DLList *list, int data ) {
	if (list->activeElement != NULL){
		list->activeElement->data = data;
	}
}

/**
 * Posune aktivitu na následující prvek seznamu list.
 * Není-li seznam aktivní, nedělá nic.
 * Všimněte si, že při aktivitě na posledním prvku se seznam stane neaktivním.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Next( DLList *list ) {
	if (list->activeElement == list->lastElement){
		list->activeElement = NULL;
		return;
	}
	else if (list->activeElement == NULL) return;
	DLLElementPtr tempElem = list->activeElement;
	list->activeElement = tempElem->nextElement;
}


/**
 * Posune aktivitu na předchozí prvek seznamu list.
 * Není-li seznam aktivní, nedělá nic.
 * Všimněte si, že při aktivitě na prvním prvku se seznam stane neaktivním.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Previous( DLList *list ) {
	if (list->activeElement == list->firstElement){
		list->activeElement = NULL;
		return;
	}
	else if (list->activeElement == NULL) return;
	DLLElementPtr tempElem = list->activeElement;
	list->activeElement = tempElem->previousElement;
}

/**
 * Je-li seznam list aktivní, vrací nenulovou hodnotu, jinak vrací 0.
 * Funkci je vhodné implementovat jedním příkazem return.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 *
 * @returns Nenulovou hodnotu v případě aktivity prvku seznamu, jinak nulu
 */
int DLL_IsActive( DLList *list ) {
	return list->activeElement != NULL;
}

/* Konec c206.c */
