# Documentatie proiect

## Titlu

WarehouseInventoryMonitor - Sistem de Monitorizare a Stocurilor unui Depozit

## Obiectiv

Aplicatia are rolul de a gestiona produsele dintr-un depozit. Utilizatorul poate adauga produse, elimina produse, actualiza cantitati, afisa lista produselor si genera rapoarte pentru produsele care au stocul sub pragul de alerta.

Interfata de consola este organizata ca un catalog de produse, inspirat de magazine online: fiecare produs are categorie, pret, cantitate disponibila, status de stoc si detalii specifice tipului sau.

Datele produselor sunt pastrate in fisierul `data/produse.csv`, care functioneaza ca o baza de date simpla pentru proiect. La pornire, aplicatia incearca sa incarce produsele din acest fisier. Dupa operatii importante precum adaugare, eliminare, restock sau vanzare, catalogul este salvat automat.

## Clase principale

### Produs

Clasa `Produs` reprezinta clasa de baza pentru produsele din depozit.

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
- `getTip()` metoda virtuala pentru tipul produsului;
- `getDetaliiSpecifice()` metoda virtuala pentru detalii afisate polimorfic;
- `clone()` metoda virtuala folosita pentru copiere polimorfica;
- `operator+=` adauga cantitate in stoc;
- `operator-=` scade cantitate din stoc.

### ProdusElectronic

Clasa `ProdusElectronic` mosteneste `Produs` si adauga atributul `garantieLuni`.

Aceasta suprascrie metodele virtuale:

- `getTip()` returneaza `Electronic`;
- `getDetaliiSpecifice()` afiseaza garantia produsului.

### ProdusMobilier

Clasa `ProdusMobilier` mosteneste `Produs` si adauga atributul `material`.

Aceasta suprascrie metodele virtuale:

- `getTip()` returneaza `Mobilier`;
- `getDetaliiSpecifice()` afiseaza materialul produsului.

### Depozit

Clasa `Depozit` gestioneaza colectia de produse folosind `std::unordered_map<int, std::shared_ptr<Produs>>`, unde cheia este ID-ul produsului.

Folosirea pointerilor catre clasa de baza permite stocarea produselor standard, electronice si mobilier in aceeasi colectie, cu apel polimorfic al metodelor virtuale.

Pe langa produse, `Depozit` detine si colectia de furnizori
(`std::unordered_map<int, Furnizor>`) si un obiect `IstoricTranzactii` care
inregistreaza fiecare miscare de stoc.

Metode importante:

- `adaugaProdus()`, `eliminaProdus()`
- `restockProdus()`, `vindeProdus()` (actualizeaza stocul si inregistreaza miscarea in istoric)
- `cautaProduseDupaNume()`, `filtreazaDupaCategorie()`, `sorteazaDupaPret()`
- `raportProduseSubPrag()`, `sugereazaReaprovizionare()`
- `incarcaProduseDinFisier()`, `salveazaProduseInFisier()`
- `adaugaFurnizor()`, `eliminaFurnizor()`, `asociazaProdusCuFurnizor()`
- `produsePentruFurnizor()`, `sugestiiReaprovizionarePeFurnizor()`
- `incarcaFurnizoriDinFisier()`, `salveazaFurnizoriInFisier()`
- `getIstoric()`, `incarcaIstoricDinFisier()`, `salveazaIstoricInFisier()`

### Furnizor

Clasa `Furnizor` reprezinta un furnizor asociat cu unul sau mai multe produse.

Atribute:

- `id`
- `nume`
- `contact`
- `produseFurnizate` (lista de ID-uri de produse)

Metode importante:

- `adaugaProdusFurnizat()` / `eliminaProdusFurnizat()` gestioneaza asocierile;
- `furnizeazaProdusul()` verifica daca un produs este asociat furnizorului.

La eliminarea unui produs din depozit, acesta este automat dezasociat de la toti
furnizorii care il refereau.

### MiscareStoc, MiscareIntrare, MiscareIesire

`MiscareStoc` este o **clasa abstracta** care modeleaza o miscare de stoc
(produs, cantitate, timestamp, observatii). Metodele `getTip()`, `getDeltaStoc()`
si `clone()` sunt pur virtuale.

- `MiscareIntrare` reprezinta un restock: `getDeltaStoc()` intoarce `+cantitate`;
- `MiscareIesire` reprezinta o vanzare: `getDeltaStoc()` intoarce `-cantitate`.

### IstoricTranzactii

Container care pastreaza miscarile polimorfic, prin
`std::vector<std::shared_ptr<MiscareStoc>>`. Ofera inregistrarea intrarilor si
iesirilor, filtrarea pe produs, ultimele N miscari si persistenta in
`data/istoric.csv`.

### Tranzactie<TP>

Clasa template `Tranzactie<TP>` modeleaza o tranzactie de stoc. Tipul poate fi `Intrare` pentru restock sau `Iesire` pentru vanzare.

## Concepte POO folosite

- incapsulare: atributele claselor sunt private;
- clase si obiecte: fiecare entitate importanta este reprezentata printr-o clasa;
- mostenire: `ProdusElectronic` si `ProdusMobilier` extind `Produs`; `MiscareIntrare` si `MiscareIesire` extind `MiscareStoc`;
- polimorfism: `getTip()`, `getDetaliiSpecifice()`, `clone()` si `getDeltaStoc()` sunt metode virtuale suprascrise;
- clasa abstracta: `MiscareStoc` cu metode pur virtuale, folosita polimorfic prin `std::shared_ptr<MiscareStoc>`;
- supraincarcare operatori: `+=` si `-=` pentru actualizarea cantitatii;
- exceptii: sunt aruncate exceptii pentru ID duplicat, produs lipsa, cantitati invalide, stoc insuficient si date CSV invalide;
- STL: se folosesc `std::unordered_map`, `std::vector`, `std::shared_ptr` si `std::chrono`;
- persistenta datelor: produsele, furnizorii si istoricul sunt citite si salvate in trei fisiere CSV separate;
- cautare, filtrare si sortare pentru comportament de catalog;
- template: `Tranzactie<TP>` permite reutilizarea logicii pentru intrari si iesiri.

## Testare

Testele sunt implementate in `tests/test_inventory.cpp` folosind `assert`. Ele verifica adaugarea produselor, ID-uri duplicate, operatorii de cantitate, statusul de stoc, mostenirea si polimorfismul, cautarea, filtrarea, sortarea dupa pret, exceptiile, raportul de produse sub prag, sortarea sugestiilor de reaprovizionare, clasa template pentru tranzactii, persistenta produselor in fisier CSV, gestionarea furnizorilor (asociere, dezasociere automata la stergere, reaprovizionare pe furnizor, persistenta) si istoricul de tranzactii (inregistrare, filtrare pe produs, ultimele N miscari, persistenta).

## Functionalitate unica

Functionalitatea principala a proiectului este monitorizarea stocului printr-un catalog persistent:

- catalogul este incarcat automat din CSV;
- modificarile sunt salvate dupa operatii importante;
- produsele sub prag sunt raportate automat;
- sugestiile de reaprovizionare sunt sortate dupa cantitatea ramasa;
- tipurile diferite de produse afiseaza detalii diferite prin polimorfism.

## Posibile imbunatatiri

- interfata grafica (de exemplu cu o biblioteca de UI);
- export al rapoartelor (produse sub prag, valoare totala stoc) in CSV sau HTML;
- raport cu valoarea totala a stocului pe categorii;
- baza de date reala (de exemplu SQLite) in locul fisierelor CSV.
