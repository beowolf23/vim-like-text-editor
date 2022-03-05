#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// nod de lista dublu inlantuita
typedef struct Node
{
	char character;
	struct Node * next;
	struct Node * prev;
}
Node;

// structura lista dublu inlantuita
typedef struct DList
{
	Node * head;
	Node * tail;
	struct DList * next;
	struct DList * prev;
	int len;
}
DList;

// nod de stiva
typedef struct stackNode
{
	char command[20];
	char text[30];
	struct stackNode * next;
}
stackNode;

// structura stiva
typedef struct Stack
{
	stackNode * top;
	int len;
}
Stack;

// structura lista de liste dublu inlantuite
typedef struct listOfLists
{
	int linesCount;
	DList * head;
	DList * tail;
}
listOfLists;

// metode liste dublu inlantuite si lista de liste
void initDList(DList **list);
void initListOfLists(listOfLists **list);
void freeListOfLists(listOfLists **list);
void freeDList(DList **ptr);
void freeNode(Node *ptr);
Node* getNode(char data);
DList* getList();
void addNodeInDList(DList *line, Node *ptr);
void addLineInLoL(listOfLists *list, DList *line);
void printDList(DList *list, FILE *f);
void printLoL(listOfLists *list, FILE *f);
void removeNode(DList *list);
void removeDListFromLoL(listOfLists *list, DList *line);
void DListToString(DList *list, char *string);
DList* stringToDList(char *string);

// metode stive
void initStack(Stack **s);
void freeStack(Stack **s);
int addStack(Stack *s, char *val);
int popStack(Stack *s);
int isEmptyStack(Stack *s);
stackNode* getStackNode(char *val);

// functionalitate editor text
void b(DList *line, Node *ptr);
void gcLine(listOfLists *list, int charPos, int linePos);
void gc(listOfLists *list, int charPos);
void gl(listOfLists *list, int linePos);
void dl(listOfLists *list);
void dlPos(listOfLists *list, int linePos);
void d(DList *list, int nr);
int dw(char word[]);
int re(char word[], char replaced[]);

// cursorul de tip - cursor caracter - cursor linie -
Node *currentNode = (Node*) sizeof(Node);
DList *currentLine = (DList*) sizeof(DList);

int main(int argc, char *argv[])
{
	listOfLists * list;
	Stack *undo, *redo;
	initListOfLists(&list);
	initStack(&undo);
	initStack(&redo);
	char character;
	char linie[128], string[128];
	int flagCommand = 0;
	FILE * f;
	f = fopen("editor.in", "r");
	DList *newLine = getList();
	if (f == NULL)
	{
		perror("Unable to open file!");
		exit(1);
	}
	// setez cursorul pe prima linie
	currentLine = newLine;
	// citesc linie cu linie
	while (fgets(linie, 128, f) != NULL)
	{
		// verific daca sunt in modul inserare text
		if (flagCommand == 0)
		{
			// verific daca am dat de ::i si updatez flag-ul
			if (strcmp(linie, "::i\n") == 0 || strcmp(linie, "::i ") == 0)
			{
				flagCommand = 1;
				continue;
			}
			// mod inserare text - inserez caracter cu caracter in lista
			else
			{
				int i;
				for (i = 0; i < strlen(linie); i++)
				{
				 		// inserez caracter cu caracter
					if (linie[i] != '\n')
					{
						Node *ptr = getNode(linie[i]);
						addNodeInDList(newLine, ptr);
						currentNode = ptr;
						continue;
					}
					// daca am dat de '\n' adaug lista(linia) in lista de liste
					else
					{
						addLineInLoL(list, newLine);
						addStack(undo, "insertLine");
						newLine = getList();
						// updatez cursorul
						currentLine = newLine;
						currentNode = newLine->head;
						continue;
					}
				}
			}
		}
		// daca sunt in modul inserare comenzi
		if (flagCommand == 1)
		{
			// verific daca am dat de ::i si updatez flag-ul
			if (strcmp(linie, "::i\n") == 0 || strcmp(linie, "::i \n") == 0)
			{
				flagCommand = 0;
				continue;
			}
			// verific comenzile primite si le identific
			else
			{
				linie[strcspn(linie, "\n")] = 0;
				// comanda undo - adaug in stiva de redo si updatez textul
				if (strncmp(linie, "u", 1) == 0)
				{
					if (strcmp(undo->top->command, "insertLine") == 0)
					{
						gl(list, list->linesCount);
						addStack(redo, "insertLine");
						// string = malloc(sizeof(currentLine->len) + 1);
						DListToString(currentLine, string);
						strcpy(redo->top->text, string);
						popStack(undo);
						dl(list);
						freeDList(&currentLine);
						// free(string);
					}
				}
				// comanda de redo - adaug in stiva de undo si updatez textul
				else if (linie[0] == 'r' && linie[1] == '\0')
				{
					if (strcmp(redo->top->command, "insertLine") == 0)
					{
						DList *newDList = stringToDList(redo->top->text);
						addLineInLoL(list, newDList);
						addStack(undo, redo->top->command);
						strcpy(undo->top->text, redo->top->text);
						popStack(redo);
					}
				}
				// comanda go to line - mut cursorul la linia specificata
				else if (strncmp(linie, "gl", 2) == 0)
				{
					addStack(undo, linie);
					// parsez numarul liniei din comanda
					int pos = atoi(&linie[3]);
					gl(list, pos);
				}
				// comanda delete line - sterg linia specificata
				else if (strncmp(linie, "dl", 2) == 0)
				{
					addStack(undo, linie);
					// parsez numarul liniei daca exista
					if (linie[3])
					{
						int pos = atoi(&linie[3]);
						dlPos(list, pos);
					}
					else
					{
						dl(list);
					}
				}
				// comanda go to char - mut cursorul pe o anumita pozitie
				else if (strncmp(linie, "gc", 2) == 0)
				{
					addStack(undo, linie);
					// daca se specifica si numarul liniei parsez informatiile
					if (linie[5])
					{
						int pos1 = atoi(&linie[3]);
						int pos2 = atoi(&linie[5]);
						gl(list, pos2);
						gc(list, pos1);
					}
					// mut pe caracterul de pe linia curenta
					else
					{
						int pos1 = atoi(&linie[3]);
						gc(list, pos1);
					}
				}
				// comanda backspace - sterg inapoia cursorului un caracter
				else if (strncmp(linie, "b", 1) == 0)
				{
					addStack(undo, linie);
					removeNode(currentLine);
				}
				// comanda dw - sterge un cuvant specificat de dupa cursor
				else if (strncmp(linie, "dw", 2) == 0)
				{
					addStack(undo, linie);
					int i, j = 0;
					char word[20];
					for (i = 3; i <= strlen(linie); i++)
					{
						word[j] = linie[i];
						j++;
					}
					word[strlen(word)] = '\0';
					j = dw(word);
				}
				// comanda re - dau replace la un string cu altul
				else if (strncmp(linie, "re", 2) == 0)
				{
					addStack(undo, linie);
					int i, j = 0, k = 0;
					char word[20] = { 0 };
					char replaced[20] = { 0 };
					for (i = 3; linie[i] != ' '; i++)
					{
						word[j] = linie[i];
						j++;
					}
					word[strlen(word)] = '\0';
					j = 0;
					for (i = strlen(word) + 4; i < strlen(linie); i++)
					{
						replaced[j] = linie[i];
						j++;
					}
					replaced[strlen(replaced)] = '\0';
					j = re(word, replaced);
				}
				// comanda delete - sterg inaintea cursorului un nr de caractere
				else if (strncmp(linie, "d", 1) == 0)
				{
					addStack(undo, linie);
					// parsez numarul de caractere care trebuie sterse
					if (linie[2])
					{
						int nr = atoi(&linie[2]);
						d(currentLine, nr);
					}
					// sterg doar un caracter daca nu se specifica numarul
					else
						d(currentLine, 1);
				}
				// comanda save - salvez in fisier
				else if (strncmp(linie, "s", 1) == 0)
				{
					FILE * g;
					g = fopen("editor.out", "w");
					addStack(undo, linie);
					printLoL(list, g);
					fclose(g);
				}
				// comanda quit - ies din editor si dau free la structuri
				else if (strncmp(linie, "q", 1) == 0)
				{
					addStack(undo, linie);
					freeDList(&newLine);
					freeListOfLists(&list);
					freeStack(&undo);
					freeStack(&redo);
					fclose(f);
					exit(0);
				}
			}
		}
	}
}

// functie pentru initializarea listei de liste
void initListOfLists(listOfLists **list)
{
	(*list) = (listOfLists*) malloc(sizeof(listOfLists));
	(*list)->head = NULL;
	(*list)->tail = NULL;
	(*list)->linesCount = 0;
}

// functie pentru initializarea unei linii din lista de liste
void initDList(DList **line)
{
	(*line) = malloc(sizeof(DList));
	(*line)->head = NULL;
	(*line)->tail = NULL;
	(*line)->next = NULL;
	(*line)->prev = NULL;
	(*line)->len = 0;
}

// functie pentru crearea unui nod cu caracteristica 'data'
Node* getNode(char data)
{
	Node *newNode = (Node*) malloc(sizeof(Node));
	newNode->character = data;
	newNode->next = NULL;
	newNode->prev = NULL;
	return newNode;
}

// functie pentru crearea unei linii din lista de liste
DList* getList()
{
	DList *newList = (DList*) malloc(sizeof(DList));
	newList->head = NULL;
	newList->tail = NULL;
	newList->len = 0;
	return newList;
}

// functie pentru adaugarea unui nod in linie (lista dublu inlantuita)
void addNodeInDList(DList *line, Node *ptr)
{
	Node *current = line->head;
	if (line->head == NULL)
	{
		ptr->prev = NULL;
		line->head = ptr;
		line->tail = ptr;
		line->len += 1;
		return;
	}
	while (current->next != NULL)
	{
		current = current->next;
	}
	current->next = ptr;
	ptr->prev = current;
	line->tail = ptr;
	line->len += 1;
	return;
}

// functie pentru adaugarea unei linii in lista de liste
void addLineInLoL(listOfLists *list, DList *line)
{
	DList * tmp;
	listOfLists * lst;
	if (list != NULL)
	{
		if (list->head == NULL)
		{
			list->head = line;
			list->head->next = NULL;
			list->linesCount += 1;
			return;
		}
		else
		{
			tmp = list->head;
			lst = list;
			while (tmp->next != NULL)
			{
				tmp = tmp->next;
			}
			tmp->next = line;
			line->prev = tmp;
			list->tail = line;
			list->tail->next = NULL;
			list->linesCount += 1;
			return;
		}
	}
}

// functie pentru printarea in fisier a unei linii
void printDList(DList *list, FILE *f)
{
	int i = 0;
	Node *ptr = list->head;
	while (i < list->len)
	{
		if (ptr != NULL)
		{
			fprintf(f, "%c", ptr->character);
			ptr = ptr->next;
			i++;
		}
		else
			break;
	}
	fprintf(f, "\n");
}

// functie pentru printarea in fisier a unei liste de liste
void printLoL(listOfLists *list, FILE *f)
{
	int i = 0;
	DList *line = list->head;
	while (i < list->linesCount)
	{
		if (line != NULL)
		{
			printDList(line, f);
			line = line->next;
			i++;
		}
		else
			break;
	}
}

// functie pentru stergerea unui nod dintr-o linie
void removeNode(DList *list)
{
	Node *ptr = currentNode;
	if (list->head == NULL || currentNode == NULL)
	{
		return;
	}
	if (currentNode == list->head)
	{
		list->head = currentNode->next;
		list->head->prev = NULL;
		currentNode = currentNode->next;
	}
	else if (currentNode == list->tail)
	{
		list->tail = currentNode->prev;
		list->tail->next = NULL;
		currentNode = currentNode->prev;
	}
	else
	{
		currentNode->next->prev = currentNode->prev;
		currentNode->prev->next = currentNode->next;
		currentNode = currentNode->prev;
	}
	freeNode(ptr);
}

// functie pentru stergerea unei linii din lista de liste
void removeDListFromLoL(listOfLists *list, DList *line)
{
	if (line == list->head)
	{
		line->next->prev = NULL;
		list->head = list->head->next;
		return;
	}
	else if (line == list->tail)
	{
		line->prev->next = NULL;
		list->tail = list->tail->prev;
		return;
	}
	else
	{
		line->prev->next = line->next;
		line->next->prev = line->prev;
	}
	freeDList(&line);
}

// functie pentru transformarea unei linii in string
void DListToString(DList *list, char *string)
{
	Node *ptr = list->head;
	int i = 0;
	while (ptr != NULL)
	{
		string[i] = ptr->character;
		ptr = ptr->next;
		i++;
	}
	string[list->len] = '\0';
}

// functii pentru transformarea unui string intr-o linie
DList* stringToDList(char *string)
{
	DList *newDList = getList();
	int i;
	for (i = 0; i < strlen(string); i++)
	{
		Node *newNode = getNode(string[i]);
		addNodeInDList(newDList, newNode);
	}
	return newDList;
}

// functie pentru eliberarea unei liste de liste
void freeListOfLists(listOfLists **list)
{
	DList *ptr = NULL;
	ptr = (*list)->head;
	DList * next;
	while (ptr != NULL)
	{
		next = ptr->next;
		freeDList(&ptr);
		ptr = next;
	}
	free(*list);
}

// functie pentru eliberarea unei linii
void freeDList(DList **list)
{
	Node * ptr;
	ptr = (*list)->head;
	Node * next;
	while (ptr != NULL)
	{
		next = ptr->next;
		freeNode(ptr);
		ptr = next;
	}
	free(*list);
}

// functie pentru eliberarea unui nod
void freeNode(Node *ptr)
{
	free(ptr);
}

// functie pentru initializarea unei stive
void initStack(Stack **s)
{
	(*s) = malloc(sizeof(Stack));
	(*s)->top = NULL;
	(*s)->len = 0;
}

// functie pentru eliberarea unei stive
void freeStack(Stack **s)
{
	stackNode * ptr;
	stackNode *current = (*s)->top;
	while (current != NULL)
	{
		ptr = current;
		current = current->next;
		free(ptr);
	}
	free(*s);
}

// functie pentru crearea unui nod de stiva cu caracteristica 'val'
stackNode* getStackNode(char *val)
{
	stackNode *newNode = (stackNode*) malloc(sizeof(stackNode));
	newNode->next = NULL;
	strcpy(newNode->command, val);
	return newNode;
}

// functie pentru a verifica daca stiva e goala
int isEmptyStack(Stack *s)
{
	if (s->top)
		return 0;
	else
		return 1;
}

// functie pentru adaugarea in stiva
int addStack(Stack *s, char *val)
{
	stackNode *ptr = getStackNode(val);
	ptr->next = s->top;
	s->top = ptr;
	s->len += 1;
	return 1;
}

// functie pentru stergerea capului de stiva
int popStack(Stack *s)
{
	if (isEmptyStack(s) == 1)
		return 0;
	else
	{
		stackNode *ptr = s->top;
		s->top = ptr->next;
		free(ptr);
		s->len -= 1;
		return 1;
	}
}

// functia go to line
void gl(listOfLists *list, int linePos)
{
	DList *line = list->head;
	// daca linia dorita este prima din lista
	if (linePos == 1)
	{
		currentLine = list->head;
		currentNode = list->head->head;
	}
	// daca linia dorita e ultima linie din lista
	else if (linePos == list->linesCount)
	{
		currentLine = list->tail;
		currentNode = list->tail->head;
	}
	// daca linia dorita este intre prima si ultima din lista
	else
	{
		int i;
		for (i = 0; i < linePos - 1; i++)
		{
			line = line->next;
		}
		currentLine = line;
		currentNode = line->head;
	}
}

// functia go to char
void gc(listOfLists *list, int charPos)
{
	Node *ptr = currentLine->head;
	// caracterul dorit este primul de pe linia curenta
	if (charPos == 1)
	{
		currentNode = ptr;
	}
	// caracterul dorit este ultimul de pe linia curenta
	else if (charPos == currentLine->len)
	{
		currentNode = currentLine->tail;
	}
	// caracterul dorit are o pozitie intermediara pe linia curenta
	else
	{
		int i;
		for (i = 0; i < charPos - 1; i++)
		{
			ptr = ptr->next;
		}
		currentNode = ptr;
	}
}

// functia delete line
void dl(listOfLists *list)
{
	removeDListFromLoL(list, currentLine);
}

// functia delete line at position
void dlPos(listOfLists *list, int linePos)
{
	// deplasez cursorul la linia dorita si dau delete
	gl(list, linePos);
	removeDListFromLoL(list, currentLine);
}

// functia delete
void d(DList *list, int nr)
{
	int i = 0;
	// mut cursorul un 'nr' de noduri inainte si sterg nodurile
	for (i = 0; i < nr; i++)
	{
		currentNode = currentNode->next;
	}
	for (i = 0; i < nr; i++)
	{
		removeNode(list);
	}
}

int dw(char word[])
{
	int i, j, length, pos = 0;
	Node *firstHead = currentLine->head;
	DList *firstList = currentLine;
	for (length = 0; word[length] != '\0'; length++)
	;
	for (i = 0, j = 0; word[j] != '\0' || j != length; i++)
	{
		if (currentNode != NULL)
		{
			if (currentNode->character == word[j])
			{
				j++;
				pos++;
				currentNode = currentNode->next;
			}
			else
			{
				j = 0;
				pos++;
				currentNode = currentNode->next;
			}
		}
		else
		{
			if (currentLine->next == NULL || j == length)
			{
				firstList = currentLine;
				firstHead = currentLine->head;
				break;
			}
			else
			{
				currentLine = currentLine->next;
				if (currentLine == NULL)
					break;
				else
				{
					currentNode = currentLine->head;
					firstList = currentLine;
					firstHead = currentNode;
					pos = 0;
				}
			}
		}
	}
	currentNode = firstHead;
	currentLine = firstList;
	if (j == length)
	{
		for (i = 0; i < pos - 1; i++)
		{
			firstHead = firstHead->next;
		}
		currentNode = firstHead;
		for (i = 0; i < strlen(word); i++)
			removeNode(currentLine);
	}
	return j;
}

int re(char word[], char replaced[])
{
	int i, j, length, pos = 0, k;
	Node *firstHead = currentLine->head;
	DList *firstList = currentLine;
	for (length = 0; word[length] != '\0'; length++)
	;
	for (i = 0, j = 0; word[j] != '\0' || j != length; i++)
	{
		if (currentNode != NULL)
		{
			if (currentNode->character == word[j])
			{
				j++;
				pos++;
				currentNode = currentNode->next;
			}
			else
			{
				j = 0;
				pos++;
				currentNode = currentNode->next;
			}
		}
		else
		{
			if (currentLine->next == NULL || j == length)
			{
				firstList = currentLine;
				firstHead = currentLine->head;
				break;
			}
			else
			{
				currentLine = currentLine->next;
				if (currentLine == NULL)
					break;
				else
				{
					currentNode = currentLine->head;
					firstList = currentLine;
					firstHead = currentNode;
					pos = 0;
				}
			}
		}
	}
	currentNode = firstHead;
	currentLine = firstList;
	if (j == length)
	{
		for (i = 0; i < pos - 1; i++)
		{
			firstHead = firstHead->next;
		}
		currentNode = firstHead;
		for (i = 0; i < length; i++)
		{
			currentNode->character = replaced[length - i - 1];
			currentNode = currentNode->prev;
		}
	}
	currentNode = currentLine->head;
	return j;
}
