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
- suggest products that should be reordered.

The catalog structure is inspired by product listing systems used in online stores: products have categories, prices, stock status, filtering, sorting and type-specific details.

## OOP Concepts Used

| Concept | Where it is used |
|---------|------------------|
| Classes and objects | `Produs`, `ProdusElectronic`, `ProdusMobilier`, `Depozit`, `Furnizor`, `Tranzactie<TP>` |
| Encapsulation | private fields with controlled public methods |
| Inheritance | `ProdusElectronic` and `ProdusMobilier` extend `Produs` |
| Polymorphism | virtual methods `getTip()`, `getDetaliiSpecifice()`, `clone()` |
| STL | `std::unordered_map`, `std::vector`, `std::shared_ptr` |
| Operator overloading | `Produs::operator+=` and `Produs::operator-=` |
| Templates | `Tranzactie<Intrare>` and `Tranzactie<Iesire>` |
| Exceptions | duplicate IDs, missing products, invalid quantities, invalid CSV data |
| File persistence | product catalog stored in `data/produse.csv` |

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
tip;id;nume;categorie;cantitate;pret;pragAlerta;detaliuExtra
```

The application automatically tries to load the catalog when it starts. Product changes are saved back to the CSV file after add, delete, restock and sale operations.

Example:

```text
Electronic;101;Laptop Lenovo IdeaPad;Laptopuri;12;2499.99;3;24
Mobilier;201;Dulap alb cu doua usi;Dulapuri;4;699.50;2;PAL melaminat
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
