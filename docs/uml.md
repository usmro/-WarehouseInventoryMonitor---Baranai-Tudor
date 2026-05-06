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

+-------------------------+
| Depozit                 |
+-------------------------+
| - produse               |
| - furnizori             |
+-------------------------+
| + adaugaProdus          |
| + eliminaProdus         |
| + restockProdus         |
| + vindeProdus           |
| + cautaProduseDupaNume  |
| + filtreazaDupaCategorie|
| + sorteazaDupaPret      |
| + raportProduseSubPrag  |
| + incarcaProduseDinFisier |
| + salveazaProduseInFisier |
| + sugereazaReaprovizionare |
+-------------------------+

+----------------+
| Furnizor       |
+----------------+
| - id           |
| - nume         |
| - contact      |
| - produse      |
+----------------+
| + adaugaProdus |
+----------------+

+-------------------+
| Tranzactie<TP>    |
+-------------------+
| - produsId        |
| - cantitate       |
| - observatii      |
+-------------------+
| + getTip          |
+-------------------+

ProdusElectronic mosteneste Produs
ProdusMobilier mosteneste Produs
Depozit 1 ---- * Produs prin shared_ptr<Produs>
Depozit 1 ---- * Furnizor
Furnizor 1 ---- * Produs
Tranzactie<TP> foloseste Produs prin produsId
```
