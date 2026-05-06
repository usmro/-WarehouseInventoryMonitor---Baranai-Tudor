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
| + operator+=   |
| + operator-=   |
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

Depozit 1 ---- * Produs
Depozit 1 ---- * Furnizor
Furnizor 1 ---- * Produs
Tranzactie<TP> foloseste Produs prin produsId
```
