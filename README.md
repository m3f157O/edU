Prova finale di API
Progetto di Algoritmi e Strutture Dati.

Ingegneria Informatica, Politecnico di Milano - A.A. 2019-2020

# Specifiche:
## Comandi
' (ind1,ind2)c '
      Cambia il testo presente alle righe comprese tra ind1 ed ind2 (estremi
      inclusi). Il testo che segue il comando deve essere fatto di un numero di
      righe pari a ind2-ind1+1. ind1 deve essere o un indirizzo effettivamente
      presente nel testo, oppure il primo indirizzo dopo l’ultima riga presente
      nel testo (o 1 se il testo `e ancora vuoto).
### (ind1,ind2)d
      Cancella le righe comprese tra ind1 ed ind2 (estremi inclusi), spostando
      verso l’alto le righe successive a quella di indirizzo ind2 (se ce ne sono).
      La cancellazione di una riga che non esiste nel testo non ha alcun effetto.
      
# (ind1,ind2)p
Stampa le righe comprese tra ind1 ed ind2 (o tra ind1 e la fine del testo
se ind2 non corrisponde ad alcun indirizzo nel testo). Per ogni riga non
esistente compresa nella regione tra ind1 e ind2 (inclusi) l’ editor stampa
una riga contenente il solo carattere punto.
# (numero)u
Effettua l’annullamento (undo) di un numero di comandi (c o d) pari a
quello specificato tra parentesi tonde (laddove numero `e un intero strettamente maggiore di zero). Una sequenza di comandi di undo consecutivi
annulla un numero di passi pari alla somma dei passi specificati in ognuno di essi. Se il numero di comandi di cui effettuare l’annullamento `e
superiore a quello dei comandi eseguiti, vengono annullati tutti i passi.
L’esecuzione di un comando di modifica del testo (c, d) dopo un undo
cancella gli effetti dei comandi annullati definitivamente. Si noti che nel
numero dei comandi da annullare vengono contati anche i comandi che
non hanno effetto (per esempio la cancellazione di un blocco di righe che
non esistono).
# (numero)r
Annulla l’effetto di undo per un numero di comandi pari a numero a partire
dalla versione corrente (funzione di redo). Si noti che numero deve essere
un intero strettamente maggiore di zero. Si ha quindi che una sequenza
di comandi del tipo
10u
5r
`e a tutti gli effetti equivalente al solo comando 5u. Similmente, la sequenza
12u
3r
2u
7r
equivale al comando 4u. Nel caso in cui il numero di comandi di cui effettuare il redo `e superiore a quelli annullati correntemente, viene effettuato
il numero massimo di redo possibili.
# q
Termina l’esecuzione dell’editor
