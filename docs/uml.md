# UML simplificat

```text
+----------------+
| Produs         |
+----------------+
| - id           |
| - nume         |
| - categorie    |
| - cantitate    |
| - pret         |
| - pragAlerta   |
+----------------+
| + esteSubPrag  |
| + getStatusStoc|
| + getTip       |
| + getDetaliiSpecifice |
| + clone        |
| + operator+=   |
| + operator-=   |
+----------------+

+--------------------+
| ProdusElectronic   |
+--------------------+
| - garantieLuni     |
+--------------------+
| + getTip           |
| + getDetaliiSpecifice |
+--------------------+

+----------------+
| ProdusMobilier |
+----------------+
| - material     |
+----------------+
| + getTip       |
| + getDetaliiSpecifice |
+----------------+

+-------------------------------+
| Depozit                       |
+-------------------------------+
| - produse                     |
| - furnizori                   |
| - istoric                     |
+-------------------------------+
| + adaugaProdus                |
| + eliminaProdus               |
| + restockProdus               |
| + vindeProdus                 |
| + cautaProduseDupaNume        |
| + filtreazaDupaCategorie      |
| + sorteazaDupaPret            |
| + raportProduseSubPrag        |
| + sugereazaReaprovizionare    |
| + incarcaProduseDinFisier     |
| + salveazaProduseInFisier     |
| + adaugaFurnizor              |
| + asociazaProdusCuFurnizor    |
| + sugestiiReaprovizionarePeFurnizor |
| + getIstoric                  |
+-------------------------------+

+----------------------+
| Furnizor             |
+----------------------+
| - id                 |
| - nume               |
| - contact            |
| - produseFurnizate   |
+----------------------+
| + adaugaProdusFurnizat   |
| + eliminaProdusFurnizat  |
| + furnizeazaProdusul     |
+----------------------+

+-------------------+
| Tranzactie<TP>    |
+-------------------+
| - produsId        |
| - cantitate       |
| - observatii      |
+-------------------+
| + getTip          |
+-------------------+

+----------------------------+
| MiscareStoc (abstracta)    |
+----------------------------+
| # produsId                 |
| # cantitate                |
| # timestamp                |
| # observatii               |
+----------------------------+
| + getTip = 0               |
| + getDeltaStoc = 0         |
| + clone = 0                |
+----------------------------+

+--------------------+      +--------------------+
| MiscareIntrare     |      | MiscareIesire      |
+--------------------+      +--------------------+
| + getDeltaStoc     |      | + getDeltaStoc     |
|   -> +cantitate    |      |   -> -cantitate    |
+--------------------+      +--------------------+

+--------------------------------+
| IstoricTranzactii              |
+--------------------------------+
| - miscari: vector<shared_ptr<MiscareStoc>> |
+--------------------------------+
| + inregistreazaIntrare         |
| + inregistreazaIesire          |
| + miscariPentruProdus          |
| + ultimele                     |
| + incarcaDinFisier             |
| + salveazaInFisier             |
+--------------------------------+

ProdusElectronic mosteneste Produs
ProdusMobilier mosteneste Produs
MiscareIntrare mosteneste MiscareStoc
MiscareIesire mosteneste MiscareStoc
Depozit 1 ---- * Produs prin shared_ptr<Produs>
Depozit 1 ---- * Furnizor
Depozit 1 ---- 1 IstoricTranzactii
IstoricTranzactii 1 ---- * MiscareStoc prin shared_ptr<MiscareStoc>
Furnizor 1 ---- * Produs (prin ID-uri)
Tranzactie<TP> foloseste Produs prin produsId
```
