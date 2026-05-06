# Documentatie proiect

## Titlu

WarehouseInventoryMonitor - Sistem de Monitorizare a Stocurilor unui Depozit

## Obiectiv

Aplicatia are rolul de a gestiona produsele dintr-un depozit. Utilizatorul poate adauga produse, elimina produse, actualiza cantitati, afisa lista produselor si genera rapoarte pentru produsele care au stocul sub pragul de alerta.

Interfata de consola este organizata ca un catalog de produse, inspirat de magazine precum IKEA sau Altex: fiecare produs are categorie, pret, cantitate disponibila si status de stoc.

Datele produselor sunt pastrate in fisierul `data/produse.csv`, care functioneaza ca o baza de date simpla pentru proiect. La pornire, aplicatia incearca sa incarce produsele din acest fisier. Dupa operatii importante precum adaugare, eliminare, restock sau vanzare, catalogul este salvat automat.

## Clase principale

### Produs

Clasa `Produs` reprezinta un produs din depozit.

Atribute:

- `id`
- `nume`
- `categorie`
- `cantitate`
- `pret`
- `pragAlerta`

Metode importante:

- `esteSubPrag()` verifica daca produsul trebuie reaprovizionat;
- `getStatusStoc()` returneaza `In stoc`, `Stoc redus` sau `Indisponibil`;
- `operator+=` adauga cantitate in stoc;
- `operator-=` scade cantitate din stoc.

### Depozit

Clasa `Depozit` gestioneaza colectia de produse folosind `std::unordered_map<int, Produs>`, unde cheia este ID-ul produsului.

Metode importante:

- `adaugaProdus()`
- `eliminaProdus()`
- `restockProdus()`
- `vindeProdus()`
- `cautaProduseDupaNume()`
- `filtreazaDupaCategorie()`
- `sorteazaDupaPret()`
- `raportProduseSubPrag()`
- `sugereazaReaprovizionare()`
- `incarcaProduseDinFisier()`
- `salveazaProduseInFisier()`

### Furnizor

Clasa `Furnizor` reprezinta un furnizor asociat cu unul sau mai multe produse.

Atribute:

- `id`
- `nume`
- `contact`
- `produseFurnizate`

### Tranzactie<TP>

Clasa template `Tranzactie<TP>` modeleaza o tranzactie de stoc. Tipul poate fi `Intrare` pentru restock sau `Iesire` pentru vanzare.

## Concepte POO folosite

- incapsulare: atributele claselor sunt private;
- clase si obiecte: fiecare entitate importanta este reprezentata printr-o clasa;
- supraincarcare operatori: `+=` si `-=` pentru actualizarea cantitatii;
- exceptii: sunt aruncate exceptii pentru ID duplicat, produs lipsa, cantitati invalide si stoc insuficient;
- STL: se folosesc `std::unordered_map` si `std::vector`;
- persistenta datelor: catalogul este citit si salvat in format CSV;
- cautare, filtrare si sortare pentru comportament de catalog;
- template: `Tranzactie<TP>` permite reutilizarea logicii pentru intrari si iesiri.

## Testare

Testele sunt implementate in `tests/test_inventory.cpp` folosind `assert`. Ele verifica adaugarea produselor, ID-uri duplicate, operatorii de cantitate, statusul de stoc, cautarea, filtrarea, sortarea dupa pret, exceptiile, raportul de produse sub prag, sortarea sugestiilor de reaprovizionare, clasa template pentru tranzactii si persistenta produselor in fisier CSV.

## Posibile imbunatatiri

- istoric complet al tranzactiilor intr-un fisier separat;
- cautare dupa nume;
- interfata grafica;
- rapoarte exportate in CSV;
