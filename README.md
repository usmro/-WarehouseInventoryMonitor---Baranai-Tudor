# WarehouseInventoryMonitor - Baranai Tudor

C++ console application for warehouse stock monitoring. The project manages products, categories, quantities, prices and low-stock alerts using Object-Oriented Programming concepts.

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
- suggest products that should be reordered.

## OOP Concepts Used

- classes and objects: `Produs`, `Depozit`, `Furnizor`, `Tranzactie<TP>`;
- encapsulation through private fields and public methods;
- STL containers: `std::unordered_map` and `std::vector`;
- file persistence using CSV storage;
- catalog operations: search, filter and sorting;
- operator overloading: `Produs::operator+=` and `Produs::operator-=`;
- exception handling for invalid IDs, duplicate products and invalid stock operations;
- templates through `Tranzactie<Intrare>` and `Tranzactie<Iesire>`.

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

```text
1. Adauga produs
2. Elimina produs
3. Restock produs
4. Vanzare produs
5. Afiseaza toate produsele
6. Raport produse sub prag
7. Sugestii reaprovizionare
8. Cauta produs dupa nume
9. Filtreaza dupa categorie
10. Sorteaza dupa pret crescator
11. Sorteaza dupa pret descrescator
12. Salveaza catalog
13. Incarca catalog
0. Iesire
```

## Data Storage

The product catalog is stored in:

```text
data/produse.csv
```

The file uses this format:

```text
id;nume;categorie;cantitate;pret;pragAlerta
```

The application automatically tries to load the catalog when it starts. Product changes are saved back to the CSV file after add, delete, restock and sale operations.

## Catalog Display

Products are shown in a catalog format:

```text
+----------------------------------------------------------------------+
| #101  Laptop Lenovo                                                  |
+----------------------------------------------------------------------+
| Categorie    : Electronice                                           |
| Pret         :    2499.99 lei                                        |
| Stoc         :         12 bucati                                     |
| Prag alerta  :          3                                            |
| Status       : In stoc                                               |
+----------------------------------------------------------------------+
```

Possible stock statuses:

- `In stoc`
- `Stoc redus`
- `Indisponibil`
