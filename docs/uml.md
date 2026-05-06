# UML simplificat

```text
+----------------+
| Produs         |
+----------------+
| - id           |
| - nume         |
| - cantitate    |
| - pret         |
| - pragAlerta   |
+----------------+
| + esteSubPrag  |
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
