# WarehouseInventoryMonitor - Baranai Tudor

**Autor:** Baranai Tudor
**Grupa:** 3122B
**Disciplina:** Programare Orientata pe Obiecte in C++

C++ console application for warehouse stock monitoring. The project manages products, categories, quantities, prices, low-stock alerts and persistent catalog data using Object-Oriented Programming concepts.

## Project Theme

Tema 3122B: Sistem de Monitorizare a Stocurilor unui Depozit

The application allows a warehouse operator to:

- add and remove products;
- restock products;
- register product sales;
- keep product data stored in `data/produse.csv`;
- list all products;
- display products in a catalog style, inspired by stores like IKEA or Altex;
- search products by name;
- filter products by category;
- sort products by price;
- generate a report with products below the alert threshold;
- suggest products that should be reordered;
- manage suppliers and link them to the products they provide;
- generate reorder suggestions grouped per supplier;
- keep a full, timestamped transaction history of every restock and sale.

The catalog structure is inspired by product listing systems used in online stores: products have categories, prices, stock status, filtering, sorting and type-specific details.

## OOP Concepts Used

| Concept | Where it is used |
|---------|------------------|
| Classes and objects | `Produs`, `ProdusElectronic`, `ProdusMobilier`, `Depozit`, `Furnizor`, `Tranzactie<TP>`, `MiscareStoc`, `IstoricTranzactii` |
| Encapsulation | private fields with controlled public methods |
| Inheritance | `ProdusElectronic` / `ProdusMobilier` extend `Produs`; `MiscareIntrare` / `MiscareIesire` extend `MiscareStoc` |
| Polymorphism | virtual methods `getTip()`, `getDetaliiSpecifice()`, `clone()`, `getDeltaStoc()` |
| Abstract class | `MiscareStoc` with pure virtual methods, stored polymorphically through `std::shared_ptr<MiscareStoc>` |
| STL | `std::unordered_map`, `std::vector`, `std::shared_ptr`, `std::chrono` |
| Operator overloading | `Produs::operator+=` and `Produs::operator-=` |
| Templates | `Tranzactie<Intrare>` and `Tranzactie<Iesire>` |
| Exceptions | duplicate IDs, missing products, invalid quantities, invalid CSV data |
| File persistence | products, suppliers and history stored in three separate CSV files |

## Product Types

The project uses inheritance to differentiate products:

- `Produs` - standard warehouse product;
- `ProdusElectronic` - adds warranty in months;
- `ProdusMobilier` - adds material information.

The warehouse stores products polymorphically using `std::shared_ptr<Produs>`, so the console catalog can display type-specific details without knowing the exact derived class in advance.

## Project Structure

```text
src/        C++ source files
tests/      simple unit tests
docs/       project documentation and UML notes
data/       CSV product catalog used as persistent storage
Makefile    build and test commands
README.md   project overview and instructions
```

## Build

Requirements:

- C++17 compiler, for example `g++`
- `make` or `cmake`

Build the application:

```bash
make
```

Run the application:

```bash
make run
```

Run tests:

```bash
make test
```

Clean generated files:

```bash
make clean
```

Alternative CMake build:

```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build
```

## Main Menu

The menu is grouped in four sections: products, catalog, suppliers, transaction history.

```text
[Produse]
   1. Adauga produs              5. Afiseaza toate produsele
   2. Elimina produs             6. Raport produse sub prag
   3. Restock produs             7. Sugestii reaprovizionare
   4. Vanzare produs

[Catalog]
   8. Cauta produs dupa nume    11. Sorteaza pret descrescator
   9. Filtreaza dupa categorie  12. Salveaza tot
  10. Sorteaza pret crescator   13. Reincarca tot din fisiere

[Furnizori]
  14. Adauga furnizor           17. Produse pentru un furnizor
  15. Listeaza furnizori        18. Reaprovizionare pe furnizor
  16. Asociaza produs furnizor  19. Elimina furnizor

[Istoric tranzactii]
  20. Istoric complet           22. Ultimele N miscari
  21. Istoric pentru un produs  23. Sterge istoric

   0. Iesire
```

## Data Storage

The application persists three separate CSV files under `data/`. All three are automatically loaded at startup (when they exist) and rewritten after every mutating operation.

### `data/produse.csv` - product catalog

```text
tip;id;nume;categorie;cantitate;pret;pragAlerta;detaliuExtra
```

Example:

```text
Electronic;101;Laptop Lenovo IdeaPad;Laptopuri;12;2499.99;3;24
Mobilier;201;Dulap alb cu doua usi;Dulapuri;4;699.50;2;PAL melaminat
```

### `data/furnizori.csv` - suppliers and their product associations

```text
id;nume;contact;produseIds (separated by commas)
```

Example:

```text
10;ACME SRL;acme@example.ro;101,102
20;TechDist;tech@example.ro;301
```

### `data/istoric.csv` - transaction history

Every restock and every sale is recorded with a real timestamp.

```text
tip;timestamp;produsId;cantitate;observatii
```

Example:

```text
Intrare;2026-05-14 18:45:38;101;5;comanda lunara
Iesire;2026-05-14 18:50:11;101;2;vanzare online
```

## Catalog Display

Products are shown in a catalog format:

```text
+----------------------------------------------------------------------+
| #101  Laptop Lenovo                                                  |
+----------------------------------------------------------------------+
| Tip          : Electronic                                            |
| Categorie    : Laptopuri                                             |
| Pret         :    2499.99 lei                                        |
| Stoc         :         12 bucati                                     |
| Prag alerta  :          3                                            |
| Status       : In stoc                                               |
| Detalii      : Garantie: 24 luni                                     |
+----------------------------------------------------------------------+
```

Possible stock statuses:

- `In stoc`
- `Stoc redus`
- `Indisponibil`

## Unique Feature

The main feature of this project is the stock monitoring catalog:

- products remain stored between runs through CSV persistence;
- each product has a visible stock status;
- low-stock products are reported automatically;
- reorder suggestions are sorted by remaining quantity;
- different product types display different details through polymorphism.

## Suppliers

Suppliers (`Furnizor`) are managed alongside the product catalog and persisted in `data/furnizori.csv`. From the menu the operator can:

- add and remove suppliers;
- associate a product with a supplier;
- list every supplier with their product count;
- list all products of a given supplier;
- generate a reorder report grouped per supplier - each supplier appears with the products below their alert threshold, so a single purchase order can be sent per supplier.

When a product is removed it is automatically dissociated from every supplier that referenced it.

## Transaction History

Every restock and every sale is automatically recorded in an in-memory history and persisted in `data/istoric.csv`. The history is built on a small polymorphic hierarchy:

```text
MiscareStoc (abstract)
+-- MiscareIntrare    getDeltaStoc() -> +cantitate
+-- MiscareIesire     getDeltaStoc() -> -cantitate
```

Movements are stored polymorphically through `std::shared_ptr<MiscareStoc>` inside the `IstoricTranzactii` container. The operator can:

- view the entire history in a tabular format;
- filter the history for a specific product ID;
- view only the last N movements;
- clear the history.
