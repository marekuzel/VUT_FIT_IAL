/* ******************************* c204.c *********************************** */
/*  Předmět: Algoritmy (IAL) - FIT VUT v Brně                                 */
/*  Úkol: c204 - Převod infixového výrazu na postfixový (s využitím c202)     */
/*  Referenční implementace: Petr Přikryl, listopad 1994                      */
/*  Přepis do jazyka C: Lukáš Maršík, prosinec 2012                           */
/*  Upravil: Kamil Jeřábek, září 2019                                         */
/*           Daniel Dolejška, září 2021                                       */
/* ************************************************************************** */
/*
** Implementujte proceduru pro převod infixového zápisu matematického výrazu
** do postfixového tvaru. Pro převod využijte zásobník (Stack), který byl
** implementován v rámci příkladu c202. Bez správného vyřešení příkladu c202
** se o řešení tohoto příkladu nepokoušejte.
**
** Implementujte následující funkci:
**
**    infix2postfix ... konverzní funkce pro převod infixového výrazu
**                      na postfixový
**
** Pro lepší přehlednost kódu implementujte následující pomocné funkce:
**    
**    untilLeftPar ... vyprázdnění zásobníku až po levou závorku
**    doOperation .... zpracování operátoru konvertovaného výrazu
**
** Své řešení účelně komentujte.
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako procedury
** (v jazyce C procedurám odpovídají funkce vracející typ void).
**
**/

#include "c204.h"

bool solved;

/**
 * Pomocná funkce untilLeftPar.
 * Slouží k vyprázdnění zásobníku až po levou závorku, přičemž levá závorka bude
 * také odstraněna.
 * Pokud je zásobník prázdný, provádění funkce se ukončí.
 *
 * Operátory odstraňované ze zásobníku postupně vkládejte do výstupního pole
 * znaků postfixExpression.
 * Délka převedeného výrazu a též ukazatel na první volné místo, na které se má
 * zapisovat, představuje parametr postfixExpressionLength.
 *
 * Aby se minimalizoval počet přístupů ke struktuře zásobníku, můžete zde
 * nadeklarovat a používat pomocnou proměnnou typu char.
 *
 * @param stack Ukazatel na inicializovanou strukturu zásobníku
 * @param postfixExpression Znakový řetězec obsahující výsledný postfixový výraz
 * @param postfixExpressionLength Ukazatel na aktuální délku výsledného postfixového výrazu
 */
void untilLeftPar( Stack *stack, char *postfixExpression, unsigned *postfixExpressionLength ) {
	char currentOut;
	while (!Stack_IsEmpty(stack)){
		Stack_Top(stack, &currentOut);
		if (currentOut == '('){
			Stack_Pop(stack);
			break;
		}
		postfixExpression[(*postfixExpressionLength)++] = currentOut;
		Stack_Pop(stack);
	}
}

/**
 * Pomocná funkce doOperation.
 * Zpracuje operátor, který je předán parametrem c po načtení znaku ze
 * vstupního pole znaků.
 *
 * Dle priority předaného operátoru a případně priority operátoru na vrcholu
 * zásobníku rozhodneme o dalším postupu.
 * Délka převedeného výrazu a taktéž ukazatel na první volné místo, do kterého
 * se má zapisovat, představuje parametr postfixExpressionLength, výstupním
 * polem znaků je opět postfixExpression.
 *
 * @param stack Ukazatel na inicializovanou strukturu zásobníku
 * @param c Znak operátoru ve výrazu
 * @param postfixExpression Znakový řetězec obsahující výsledný postfixový výraz
 * @param postfixExpressionLength Ukazatel na aktuální délku výsledného postfixového výrazu
 */
void doOperation( Stack *stack, char c, char *postfixExpression, unsigned *postfixExpressionLength ) {
	if(Stack_IsEmpty(stack)){
		Stack_Push(stack, c);
		return;
	}
	char temp;
	Stack_Top(stack, &temp);
	if (c == '+' || c=='-'){
		while (temp == '*' || temp == '/' || temp == '+' || temp =='-'){
			postfixExpression[(*postfixExpressionLength)++] = temp;
			Stack_Pop(stack);
			Stack_Top(stack, &temp);
		}
		Stack_Push(stack, c);
	}
	else if (c == '*' || c == '/'){
		while (temp == '*' || temp == '/'){
			postfixExpression[(*postfixExpressionLength)++] = temp;
			Stack_Pop(stack);
			Stack_Top(stack, &temp);
		}
		Stack_Push(stack, c);
	}
}

/**
 * Konverzní funkce infix2postfix.
 * Čte infixový výraz ze vstupního řetězce infixExpression a generuje
 * odpovídající postfixový výraz do výstupního řetězce (postup převodu hledejte
 * v přednáškách nebo ve studijní opoře).
 * Paměť pro výstupní řetězec (o velikosti MAX_LEN) je třeba alokovat. Volající
 * funkce, tedy příjemce konvertovaného řetězce, zajistí korektní uvolnění zde
 * alokované paměti.
 *
 * Tvar výrazu:
 * 1. Výraz obsahuje operátory + - * / ve významu sčítání, odčítání,
 *    násobení a dělení. Sčítání má stejnou prioritu jako odčítání,
 *    násobení má stejnou prioritu jako dělení. Priorita násobení je
 *    větší než priorita sčítání. Všechny operátory jsou binární
 *    (neuvažujte unární mínus).
 *
 * 2. Hodnoty ve výrazu jsou reprezentovány jednoznakovými identifikátory
 *    a číslicemi - 0..9, a..z, A..Z (velikost písmen se rozlišuje).
 *
 * 3. Ve výrazu může být použit předem neurčený počet dvojic kulatých
 *    závorek. Uvažujte, že vstupní výraz je zapsán správně (neošetřujte
 *    chybné zadání výrazu).
 *
 * 4. Každý korektně zapsaný výraz (infixový i postfixový) musí být uzavřen
 *    ukončovacím znakem '='.
 *
 * 5. Při stejné prioritě operátorů se výraz vyhodnocuje zleva doprava.
 *
 * Poznámky k implementaci
 * -----------------------
 * Jako zásobník použijte zásobník znaků Stack implementovaný v příkladu c202.
 * Pro práci se zásobníkem pak používejte výhradně operace z jeho rozhraní.
 *
 * Při implementaci využijte pomocné funkce untilLeftPar a doOperation.
 *
 * Řetězcem (infixového a postfixového výrazu) je zde myšleno pole znaků typu
 * char, jenž je korektně ukončeno nulovým znakem dle zvyklostí jazyka C.
 *
 * Na vstupu očekávejte pouze korektně zapsané a ukončené výrazy. Jejich délka
 * nepřesáhne MAX_LEN-1 (MAX_LEN i s nulovým znakem) a tedy i výsledný výraz
 * by se měl vejít do alokovaného pole. Po alokaci dynamické paměti si vždycky
 * ověřte, že se alokace skutečně zdrařila. V případě chyby alokace vraťte namísto
 * řetězce konstantu NULL.
 *
 * @param infixExpression vstupní znakový řetězec obsahující infixový výraz k převedení
 *
 * @returns znakový řetězec obsahující výsledný postfixový výraz
 */
char *infix2postfix( const char *infixExpression ) {
	char * postFix = (char*) malloc (MAX_LEN);
	if (postFix == NULL) return NULL;
	unsigned postfixExpressionLength = 0;
	Stack *stack = malloc(sizeof(Stack));
	if (stack == NULL) return NULL;
	Stack_Init(stack);
	char c = infixExpression[0];
	int i = 0;
	while (c != '\0'){
		if ((c >= '0' && c <= '9')||(c >= 'a' && c <= 'z')||(c>='A' && c<='Z')){
			postFix[postfixExpressionLength++] = c;
		}
		else if((c == '*') || (c == '+') || (c == '-') || (c == '/')){
			doOperation(stack, c, postFix, &postfixExpressionLength);
		}
		else if (c == '('){
			Stack_Push(stack, c);
		}
		else if (c == ')'){
			untilLeftPar(stack, postFix, &postfixExpressionLength);
		}
		else if (c == '='){
			while (!(Stack_IsEmpty(stack))){
				Stack_Top(stack, &postFix[postfixExpressionLength]);
				postfixExpressionLength += 1;
				Stack_Pop(stack);
			}
			postFix[postfixExpressionLength] = '=';
			postfixExpressionLength++;
			
		}
		c = infixExpression[++i];
	}
	free(stack);
	postFix[postfixExpressionLength] = '\0';
	return postFix;
}


/**
 * Pomocná metoda pro vložení celočíselné hodnoty na zásobník.
 *
 * Použitá implementace zásobníku aktuálně umožňuje vkládání pouze
 * hodnot o velikosti jednoho byte (char). Využijte této metody
 * k rozdělení a postupné vložení celočíselné (čtyřbytové) hodnoty
 * na vrchol poskytnutého zásobníku.
 *
 * @param stack ukazatel na inicializovanou strukturu zásobníku
 * @param value hodnota k vložení na zásobník
 */
void expr_value_push( Stack *stack, int value ) {
	unsigned char bytes[4];
	
    bytes[0] = (value >> 24) & 0xFF;
    bytes[1] = (value >> 16) & 0xFF;
    bytes[2] = (value >> 8) & 0xFF;
    bytes[3] = value & 0xFF;
	printf ("push=");
    for (int i = 0; i < 4; i++) {
		printf ("%hhx",bytes[i]);
        Stack_Push(stack, bytes[i]);
		
    }
	printf ("\n");
}

/**
 * Pomocná metoda pro extrakci celočíselné hodnoty ze zásobníku.
 *
 * Využijte této metody k opětovnému načtení a složení celočíselné
 * hodnoty z aktuálního vrcholu poskytnutého zásobníku. Implementujte
 * tedy algoritmus opačný k algoritmu použitému v metodě
 * `expr_value_push`.
 *
 * @param stack ukazatel na inicializovanou strukturu zásobníku
 * @param value ukazatel na celočíselnou proměnnou pro uložení
 *   výsledné celočíselné hodnoty z vrcholu zásobníku
 */
void expr_value_pop(Stack *stack, int *value) {
    char bytes[4] = {0};

    printf("pop=");
    for (int i = 0; i < 4; i++) {
        Stack_Top(stack, &bytes[3 - i]);  
        Stack_Pop(stack);
        printf("%hhx", bytes[i]); 
    }
    printf("\n");

    *value = (bytes[0] << 24) | (bytes[1] << 16) | (bytes[2] << 8) | bytes[3];
    printf("reconstructed %d\n", *value);
}



/// @brief finds variable in array of values that matches the needles value
/// @param needle desired variables name
/// @param haystack array of variables
/// @return value of the variable
int findValue(char needle, VariableValue haystack[]){
	int i = 0;
	while (haystack[i].name != needle){
		i++;
	}
	return haystack[i].value;
}


int expr_operation(int a, int b, char operator){
	if (operator == '+'){
		return a + b;
	}
	else if (operator == '/'){
		return b/a;
	}
	else if (operator == '*'){
		// printf ("a=%d, b=%d\n", a, b);
		// printf ("%d\n", a*b);
		return a*b;
	}
	else{
		return b-a;
	}
}

/**
 * Tato metoda provede vyhodnocení výrazu zadaném v `infixExpression`,
 * kde hodnoty proměnných použitých v daném výrazu jsou definovány
 * v poli `variableValues`.
 *
 * K vyhodnocení vstupního výrazu využijte implementaci zásobníku
 * ze cvičení c202. Dále také využijte pomocných funkcí `expr_value_push`,
 * respektive `expr_value_pop`. Při řešení si můžete definovat libovolné
 * množství vlastních pomocných funkcí.
 *
 * Předpokládejte, že hodnoty budou vždy definovány
 * pro všechy proměnné použité ve vstupním výrazu.
 *
 * @param infixExpression vstpní infixový výraz s proměnnými
 * @param variableValues hodnoty proměnných ze vstupního výrazu
 * @param variableValueCount počet hodnot (unikátních proměnných
 *   ve vstupním výrazu)
 * @param value ukazatel na celočíselnou proměnnou pro uložení
 *   výsledné hodnoty vyhodnocení vstupního výrazu
 *
 * @return výsledek vyhodnocení daného výrazu na základě poskytnutých hodnot proměnných
 */
bool eval( const char *infixExpression, VariableValue variableValues[], int variableValueCount, int *value ) {
	char * postFix = infix2postfix(infixExpression);
	int postfixIndex = 0;
	char c = postFix[postfixIndex];

	Stack *stack = malloc(sizeof(Stack));
	if (stack == NULL) return NULL;
	Stack_Init(stack);
	while (c != '\0'){	
		if ((c == '*')||(c == '/')||(c == '-')||(c == '+')){
			
			int a, b;
			expr_value_pop(stack, &a);
			expr_value_pop(stack, &b);
			expr_value_push(stack, expr_operation(a,b,c));
		}
		else if (c == '='){
			expr_value_pop(stack, value);
			return 1;
		}
		else{
			int vValue = findValue(c, variableValues);
			expr_value_push(stack, vValue);
		}
		c = postFix[++postfixIndex];
	}
	return 0;
}


/* Konec c204.c */
