#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <ctype.h>
#define BUFSIZE 50
#define STRSIZE 20

typedef struct record {
	char category[BUFSIZE];
	char city[BUFSIZE];
	char address[2*BUFSIZE];
	int area;
	int price;
	char info[4*BUFSIZE];
	struct record* next;
}REALITY;


/* funkcia freeList pracuje s dvomi ukazovatelmi na strukturu.
posuvam sa v zozname pomocou premennej actual, free_node je 
vzdy o strukturu dozadu, ktoru v kazdom cykli uvolnujem. nakoniec 
nastavim hlavicku na NULL. */
void freeList(REALITY** head) {
	REALITY* actual, * previous;
	if (*head != NULL) {
		actual = *head;
		previous = actual;
		while (actual != NULL) {
			actual = actual->next;
			free(previous);
			previous = actual;
		}
		*head = NULL;
	}
}

/*kazdy zaznam o nehnutelnosti zacina jednym ampersandom
a koncom riadka, vdaka tomuto viem, ze ked spocitam ampersandy
v subore tak dostanem pocet nehnutelnosti, ktory nasledne vo funkcii
loadList_file vyuzijem. */
void totalReality(FILE *stream, int *numEntries) {
	char character;
	while ((character = fgetc(stream)) != EOF) {
		if (character == '&')
			(*numEntries)++;
	}rewind(stream);

}

//vytvori novy uzol: input -> file
REALITY* new_node(FILE** stream) {

	char ampersand[5];
	REALITY* temp = (REALITY*)malloc(sizeof(REALITY));
	fgets(ampersand, 5, *stream);
	fgets(temp->category, BUFSIZE, *stream);
	fgets(temp->city, BUFSIZE, *stream);
	fgets(temp->address, 2*BUFSIZE, *stream);
	fscanf(*stream, "%d\n", &temp->area);
	fscanf(*stream, "%d\n", &temp->price);
	fgets(temp->info, 4*BUFSIZE, *stream);
	temp->next = NULL;
	return temp;
}

//vytvori novy uzol: input -> standard input
REALITY* new_node_input() {

	REALITY* temp = (REALITY*)malloc(sizeof(REALITY));
	fgets(temp->category, BUFSIZE, stdin);
	fgets(temp->city, BUFSIZE, stdin);
	fgets(temp->address, 2*BUFSIZE, stdin);
	scanf("%d\n", &temp->area);
	scanf("%d\n", &temp->price);
	fgets(temp->info, 4*BUFSIZE, stdin);
	temp->next = NULL;
	return temp;
}

// funkcia, ktora mi automaticky prevedie string na uppercase 
void UpperStr(char* temp) {

	
	int index=0;
	while (temp[index]) {							//kedze funkcia toupper prevedie iba jeden char na uppercase, tak som
		temp[index] = toupper(temp[index]);			//musel napisat funkciu, ktora prevedie cely string. Postupne prechadzam 
		if (temp[index] == '\n') {					//cele slovo a menim pismeno po pismene + ked cyklus narazi na znak konca riadka
			temp[index] = '\0';						// tak ho vymaze
			break;							
		}											// DOVOD: UpperStr pouzivam v spolupraci s funckiou strstr(), ktory by inak hladal
		index++;									// mnou zadany substring spolu AJ S koncom riadku (nacitavanie cez fgets)
	}
}

//funkcia na nacitanie zaznamov o realitach zo 
//suboru reality.txt do spájaného zoznamu struktur.
void loadList_file(REALITY** head, int* numEntries) {

	FILE* stream;
	REALITY* actual;
	REALITY* node = (REALITY*)malloc(sizeof(REALITY));
	*numEntries = 0;
	
	int cycle = 0;
	

	freeList(&(*head));

	if ((stream=fopen("reality.txt","r")) == NULL) {
		printf("Zaznamy neboli nacitane. \n");
		free(node);
		return;
	}

	totalReality(&(*stream),&(*numEntries));

	for (; cycle < *numEntries; cycle++) {
		node = new_node(&stream);
		if (*head == NULL)							//najskor potrebujem nasmerovat hlavicku na zaciatok zoznamu
			*head = node;							//a az potom pridavat postupne struktury na koniec, to som zabezpecil
		else {										//podmienkou, ktora prebehne v cykle iba raz, potom sa bude spustat iba
			actual= *head;							//else zlozka == prejde na koniec zoznamu a posledny_node->next nebude null
			while (actual->next != NULL) {			//ale novy node nacitany zo suboru
				actual = actual->next;
			} actual->next = node;
		}
	}

	if (*head != NULL)
		printf("Nacitalo sa %d zaznamov\n", *numEntries);

	if (fclose(stream) == EOF)
		printf("Subor sa nepodarilo zatvorit\n");
}


//funkcia na pridanie zaznamu o realitnej 
//ponuke do dynamického zoznamu na nami zadanu poziciu.
//ak taka pozicia nie je v zozname alebo zoznam nie je nacitany, tak 
//pridame node na koniec.
// + funkcia vlozi strukturu na danu poziciu a struktura, ktora bola na tej pozicii
// sa posunie a miesto dopredu
void addNode_position(REALITY** head, int* numEntries) {

	int StructPosition, CountPosition;
	REALITY* insert = (REALITY*)malloc(sizeof(REALITY));
	REALITY* ptr = *head;
	REALITY* previous = *head;

	scanf("%d \n", &StructPosition);
	insert = new_node_input();

	if (StructPosition == 1) {
		if (*head == NULL)			//ak pridavame na prve miesto, a hlavicka je prazdna (zoznam nie je nacitany)
			*head = insert;			//tak hlavicka bude novo vytvorena struktura
		else {
			*head = insert;
			insert->next = ptr;
		}
		(*numEntries)++;
		return;
	}

	if (StructPosition <= *numEntries) {

		CountPosition = 1;
		ptr = (*head)->next;

		while (CountPosition < StructPosition - 1 && ptr != NULL) {
			ptr = ptr->next;					//ak dana pozicia sa nachadza v zozname tak pracujem s dvomi smernikami
			previous = previous->next;			//ptr (aktualny) a previous (predchadzajuci), najdem poziciu a vlozim 
			CountPosition++;					//tam mnou vygenerovanu strukturu
		} 
		
		previous->next = insert;
		insert->next = ptr;
		(*numEntries)++;						// zoznam je o jednu strukturu vacsi, tak musim aj pocet struktur
		return;									// inkrementovat o jedno
	}

	if (StructPosition > * numEntries) {

		if (*head == NULL)
			*head = insert;
		else {
			ptr = *head;
			while (ptr->next != NULL) {
				ptr = ptr->next;
			} 
			ptr->next = insert;
		}
		(*numEntries)++;
		return;
	}



}

//funkcia na zmazanie zaznamov o realitach podla miesta ponuky
void deleteNode(REALITY** head) {

	if (*head == NULL)
		return;

	REALITY* ptr = *head;
	REALITY* prev;
	char needle[STRSIZE], haystack[STRSIZE];
	int index = 0, numDeleted = 0;

	getchar();
	fgets(needle, STRSIZE, stdin);
	UpperStr(needle);

	while (ptr != NULL) {
		strcpy(haystack, ptr->city);
		UpperStr(haystack);

		if (ptr == *head) {								//ak je node, ktory chcem mazat na prvom mieste, tak musim
			if (strstr(haystack, needle) != NULL) {		//premiestnit hlavicku a tu nasledne uvolnit a nasmerovat na 
				*head = ptr->next;						//dalsi prvok v zozname
				free(ptr);
				ptr = *head;
				numDeleted++;
				continue;
			}
		}
		prev = *head;
		if (strstr(haystack, needle) != NULL) {
			while (prev->next != NULL && prev->next != ptr && prev->next->next != NULL)
				prev = prev->next;						//ak sa node, ktory chcem vymazat nachadza na inom mieste ako 
			prev->next = prev->next->next;				//na prvom, tak musim pracovat s dvoma pointermi, jeden co ukazuje
			numDeleted++;								//na aktualny a druhy, ktory ukazuje na predosli, ak je aktualny ten
		}												//ktory potrebujem vymazat, tak predosli nasmerujem na ->next->next
		ptr = ptr->next;
	}
	printf("Vymazalo sa %d zaznamov\n", numDeleted);
}

//prikaz na aktualizaciu (zmenu) zaznamu podla miesta ponuky
void updateNode(REALITY** head) {

	if (*head == NULL)
		return;

	REALITY* ptr = *head, *nextNode;
	char needle[STRSIZE], haystack[STRSIZE];
	int index = 0, numUpdated = 0;

	getchar();
	fgets(needle, STRSIZE, stdin);
	UpperStr(needle);

	while (ptr != NULL) {

		strcpy(haystack, ptr->city);
		UpperStr(haystack);

		if (strstr(haystack, needle) != NULL) {
			nextNode = ptr->next;					// funkcia new_node_input automaticky nastavavi ptr->next na NULL 
			free(ptr);								// aby som nemusil vytvarat novu funkciu, ktora toto nebude robit, tak som si dalsiu strukturu
			ptr = new_node_input();					// ulozil do pomocnej premennej, nastavil som nove hodnoty na ptr
			ptr->next = nextNode;					// a na koniec ptr->next som nasmeroval na pomocnu premennu 
			numUpdated++;
		}
		ptr = ptr->next;
	}
	printf("Aktualizovalo sa %d zaznamov\n", numUpdated);
}

//funkcia umozni pouzivatelovi vyhladat a vypisat vsetky polozky zaznamu podla ceny ponuky
void filterList_price(REALITY* head) {

	if (head == NULL)
		return;

	REALITY* temp = head;
	int count = 1, priceFilter;

	scanf("%d", &priceFilter);
	while (temp != NULL) {
		if ((temp->price) > priceFilter) {
			temp = temp->next;
			continue;
		}

		printf("%d.\n", count);
		printf("kategoria ponuky: %s", (temp->category));
		printf("miesto ponuky: %s", (temp->city));
		printf("ulica: %s", (temp->address));
		printf("rozloha v m2: %d\n", (temp->area));
		printf("cena: %d\n", (temp->price));
		printf("popis: %s", (temp->info));
		temp = temp->next;
		count++;
	}

	if (count == 1)
		printf("V ponuke su len reality s vyssou cenou\n");
}

//funkcia, ktora skonci program a uvolni pamat spajaneho zoznamu
void endProgram(int* trigger, REALITY** head) {
	*trigger = 0;
	freeList(&*head);
}




//funkcia na vypis celeho spajaneho zoznamu zaznamov
void printList(REALITY* head) {

	if (head == NULL)
		return;

	REALITY* temp = head;
	int count = 0;

	while (temp != NULL) {
		printf("%d.\n", count + 1);
		printf("kategoria ponuky: %s", (temp->category));
		printf("miesto ponuky: %s", (temp->city));
		printf("ulica: %s", (temp->address));
		printf("rozloha v m2: %d\n", (temp->area));
		printf("cena: %d\n", (temp->price));
		printf("popis: %s", (temp->info));
		temp = temp->next;
		count++;
	}
}


int main() {

	int trigger = 1, numEntries = 0;
	char function;
	REALITY* head = NULL;


	while (trigger)
	{
		scanf("%c", &function);
		switch (function) {
		case 'n':
			loadList_file(&head, &numEntries);
			break;
		case 'v':
			printList(head);
			break;
		case 'k':
			endProgram(&trigger, &head);
			break;
		case 'h':
			filterList_price(head);
			break;
		case 'p':
			addNode_position(&head, &numEntries);
			break;
		case 'z':
			deleteNode(&head);
			break;
		case 'a':
			updateNode(&head);
			break;
		}
	}
	return 0;
}

