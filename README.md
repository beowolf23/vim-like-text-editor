========TEMA1========

In acest Readme sunt explicate functionalitatile si implementarea programului
aferent temei 1 - editorul de text.

Structurile folosite:

    - Node - Structura folosita pentru a caracteriza nodul
              unei liste dublu inlantuite
           - Intr-un nod pot retine maxim 1 caracter de pe o linie a textului
           
    - DList - Structura folosita pentru a caracteriza o lista dublu inlantuita
            - Se retine lungimea fiecarei linii
            - Am acces la urmatoarele linii din text
            - Ma pot misca de pe o linie pe alta (next, prev)
            
    - listOfLists - Structura folosta pentru a caracteriza o lista de liste
                  - In aceasta retin fiecare linie din text
                  - Liniile retinute sunt de forma listelor dublu inlantuite
                  pe care le introduc in aceasta lista mare de liste
                  - Se retine numarul liniilor din text
    - stackNode - Structura folosita pentru a caracteriza nodul din stiva
                - Retin atat comanda introdusa cat si textul pe care il memorez
    
    - Stack - Structura folosita pentru a caracteriza o stiva
    
Cursorul:
    
    - cursorul este unul de tip <Node, DList>, declarat global
    
Functii:
    
    - initDList - functia primeste o lista dublu inlantuita careia 
                  ii initializeaza head-ul si tail-ul cu NULL si
                  ii aloca memorie
                  
    - initListOfLists - functia primeste o lista de liste dublu inlantuite
                        careia ii aloca memorie si ii initializeaza head-ul
                        si tail-ul cu NULL; de asemenea, lungimea listei va fi 
                        initializata cu 0
                        
    - freeDList - functia primeste o lista dublu inlantuita careia ii dezaloca
                  memoria nod cu nod, apoi listei in sine
    
    - freeListOfLists - functia primeste o lista de liste dublu inlantuite si
                        ii dezaloca memoria linie cu linie folosind functia 
                        freeDList, apoi listei de liste in sine
                        
    - freeNode - functia primeste un nod caruia ii elibereaza memoria
                        
    - getNode - functia primeste un caracter si creeaza un nod cu caracteristica
                data pe care il returneaza
    
    - getList - functia creeaza o noua lista dublu inlantuita careia
                ii aloca memorie si o initializeaza cu NULL
    
    - addNodeInDList - functia primeste o lista dublu inlantuita si un nod
                       pe care il introduce in aceasta lista
                     - este verificat cazul de baza (lista este goala) si apoi
                       se introduce nodul la inceputul listei sau la finalul
                       acesteia (in cazul in care nu este goala)
                       
    - addLineInLoL - functia primeste o lista de liste si o lista dublu
                     inlantuita pe care o introduce in lista de liste
                   - este verificat cazul de baza in care lista de liste
                     este goala, caz in care se introduce prima linie a listei
                   - daca lista de liste nu este goala se va introduce la final
                     lista dublu inlantuita

    - printDList - functia primeste o lista dublu inlantuita si un pointer la 
                   un fisier si printeaza lista in acest fisier nod cu nod
                   cat timp nu este goala
     
    - printLoL - functia primeste o lista de liste si un pointer la un fisier
                 si printeaza lista de liste in acest fisier linie cu linie
                 folosind functia printDList, cat timp lista de liste nu este
                 goala
                 
    - removeNode - functia primeste o lista dublu inlantuita si se foloseste
                   de cursor pentru a sterge nodul curent
                 - se verifica daca lista este goala, caz in care se iese fortat
                   din functie
                 - daca lista nu este goala, sunt verificate cazurile in care 
                   nodul curent este head-ul sau tail-ul caz in care este sters
                 - daca nodul curent este unul arbitrar, se va sterge din lista
                 - se dezaloca memoria nodului
                 
    - removeDListFromLoL - functia primeste o lista de liste si o lista dublu
                           inlantuita pe care o sterge din lista de liste
                         - ca la removeNode, sunt verificate cazurile de baza
                           in care linia pe care o stergem e head-ul sau tail-ul
                           listei de liste
                         - daca linia are o pozitie arbitrara se va sterge
                           din lista
                         - se dezaloca memoria liniei sterse
                         
    - DListToString - functia primeste o lista dublu inlantuita si un string
                      si transforma lista in sir de caractere pe care il
                      returneaza
    
    - stringToDList - functia primeste un string pe care il transforma intr-o
                      lista dublu inlantuita folosind functiile de adaugare
                      si functia getList
                    - se returneaza noua lista de liste

    - initStack - functia primeste o stiva pe care o initializeaza cu NULL
    
    - freeStack - functia primeste o stiva careia ii elibereaza memoria,
                  nod cu nod, apoi stivei in sine
    
    - getStackNode - functia primeste un sir de caractere si creeaza un nod
                     de stiva cu caracteristica val (char *) pe carre il
                     returneaza
    
    - isEmptyStack - functia primeste o stiva si verifica daca aceasta este
                     goala; se verifica daca varful stivei este NULL
                 
    - addStack - functia primeste o stiva si un string pe care il transforma
                 in nod de stiva si il adauga pe top-ul acesteia
             
    - popStack - functia primeste o stiva si da delete la top-ul acesteia,
                 daca exista; se foloseste isEmptyStack pentru a determina
                 existenta top-ului
                 
    - gl - functia primeste o lista de liste si pozitia liniei pe care dorim
           sa mutam cursorul
         - sunt verificate cazurile de baza, atunci cand vrem sa mergem pe prima
           respectiv ultima linie
         - daca linia dorita este una arbitrara, se va muta cursorul in urma
           operatiilor de parcurgere a listei de liste; cursorul de tip Node
           va pointa acum spre head-ul cursorului de tip DList
           
    - gc - functia primeste o lista de liste si pozitia caracterului pe care 
           vrem sa mutam cursorul
         - sunt verificate cazurile de baza, atunci cand vrem sa ajungem
           pe head-ul/tail-ul listei
         - in cazul in care caracterul dorit este unul arbitrat, se va muta
           cursorul in urma parcurgerii listei
    
    - dl - functia primeste o lista de liste si sterge linia curenta
    
    - dlPos - functia primeste o lista de liste si pozitia liniei pe care dorim
              sa o stergem
            - se foloseste functia gl pentru a deplasa cursorul la pozitia
              dorita si apoi se sterge linia 
              
    - d - functia primeste o lista dublu inlantuita si numarul de caractere 
          pe care vrem sa il stergem de dupa cursor
        - se deplaseaza cursorul nr pozitii inainte si apoi se foloseste
          functia removeNode pentru a sterge nodurile
    
    - dw - functia primeste string-ul pe care vrem sa il stergem
         - variabilele firstHead, firstList retin linia anterioara
           pe care e posibil sa fi avut cuvantul pe care il cautam
         - se cauta lungimea cuvantului
         - shiftez prin liniile de text pana la finalul lor si verific
           daca nodul pe care ma aflu contine caracterul j din cuvant
         - in cazul in care se gasesc caractere din cuvant in text
           se incrementeaza j-ul (caut urmatorul caracter din cuvant)
         - daca urmatorul nod nu contine un caracter din cuvant, atribui
           lui j valoarea 0 (resetez cautarea)
         - functia ruleaza pana nu mai sunt linii in care sa caute sau pana
           gaseste cuvantul
         - daca se gaseste un cuvant in lista, parcurg linia pana la pozitia
           de final a cuvantului cautat si sterg, folosint removeNode un numar
           de strlen(cuvant) caractere
    - re - functia este identica cu cea de delete word, numai ca, la final
           nu sterge caracterele, ci le inlocuieste cu caracterele din string-ul
           replaced
         - functia primeste doua stringuri: word si replaced si inlocuieste
           cuvantul word cu replaced

Main: 
    
    - initializez lista de liste, stivele si prima linie din text
    - initializez flagCommand cu 0 (mod inserare text) 
    - deschid fisierul f
    - citirea din fisier se face in felul urmator:
        - citesc linie cu linie pana la finalul fisierului
        - daca flagCommand = 0 (sunt in modul inserare text) - verific daca
            am dat de ::i; daca da, intru in modul pentru comenzi, flagCommand = 1
            iar daca nu sunt in modul pentru comenzi:
              - parcurg linia, creez noduri pentru fiecare caracter din linie,
                adaug nodurile in linie (newLine) si updatez cursorul, pana cand
                dau de finalul liniei ('\n')
              - daca am dat de finalul liniei, adaug linia in lista de liste, 
                aloc memorie pentru o linie noua si updatez cursorul
        - daca intru in modul de comenzi (flagCommand = 1):
              - verific daca dau de "::i" si updatez flag-ul daca este necesar
              - daca sunt inca in modul pentru comenzi:
                    - trunchiez fiecare comanda (din '\n' fac '\0')
                    - identific fiecare comanda si fac operatiile necesare
                      pentru fiecare
                      
Comenzi:

    - La inceputul fiecarei comenzi, se adauga in stiva de undo fiecare comanda
    - s - deschid fisierul editor.out pe modul write
        - printez lista de liste in fisier
        - inchid fisierul
    - q - dau free la toate structurile de date folosite
        - inchid fisierul din care citesc
        - ies din program
    - u - verific daca ultima comanda data este chiar un insert de text;
          daca da, sterg textul adaugat, adaug textul in stiva de redo
          si eliberez memoria pentru text
    - r - verific daca ultima comanda data este chiar un insert de text;
          daca da, adaug textul sters din lista inapoi in stiva de undo
          si in lista de liste
    - gl - parsez pozitia pe care vreau sa ajung din comanda
         - folosind functia gl, mut cursorul la pozitia dorita
    - dl - parsez linia careia vreau sa ii dau delete din comanda
         - folosing functia dl, sterg linia dorita
    - gc - parsez pozitiile liniei si caracterului pe care vreau sa ajung
         - daca se specifica si linia, folosind gl si gc, mut cursorul unde se
           specifica
         - daca nu se specifica linia, folosind gc, mut cursorul pe caracterul
           dorit de pe linia curenta
    - b - folosind functia removeNode, sterg caracterul de dinaintea cursorului
    - dw - parsez cuvantul pe care vreau sa il sterg din comanda, ii adaug
           terminatorul de sir
         - folosind functia dw sterg prima ocurenta a cuvantului din lista
    - re - parsez sirurile word si replaced din comanda, le adaug terminatoare
           de sir si folosind functia re, inlocuiesc prima ocurenta a word cu 
           replaced
    - d - parsez din comanda numarul de caractere pe care vreau sa le sterg
        - folosind functia d, sterg nr de caractere de pe linia curenta sau 
          1 caracter daca nu se specifica nr
