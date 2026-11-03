# WarehouseInventoryMonitor - Baranai Tudor

[![CI](https://github.com/usmro/-WarehouseInventoryMonitor---Baranai-Tudor/actions/workflows/ci.yml/badge.svg)](https://github.com/usmro/-WarehouseInventoryMonitor---Baranai-Tudor/actions/workflows/ci.yml)

**Autor:** Baranai Tudor
**Grupa:** 3122B
**Disciplina:** Programare Orientata pe Obiecte in C++
**Tema:** Sistem de Monitorizare a Stocurilor unui Depozit

A C++17 console application for running a small warehouse. It tracks products, stock levels, suppliers and every stock movement, warns about low stock, and keeps everything in CSV files between runs. The menus and messages are in Romanian.

## What it does

- **Products:** add, remove, restock and sell products. Each product has a category, price, quantity and alert threshold, and shows a status: `In stoc`, `Stoc redus` or `Indisponibil`.
- **Catalog view:** search by name, filter by category, and sort by price, shown as product cards (see below).
- **Low stock:** a report of products under their alert threshold, plus reorder suggestions sorted by how little is left.
- **Suppliers:** link products to suppliers, then get the reorder list grouped per supplier, so one order can go to each supplier.
- **Transaction history:** every restock and sale is saved with a timestamp. You can view all of it, filter by product, or show the last N movements.
- **Persistence:** products, suppliers and history are loaded from `data/` at startup and saved again after every change.

Products are shown as catalog cards:

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

## Build and run

You need a C++17 compiler (`g++`, `clang++` or MSVC) and either `make` or `cmake`. On Windows, MSYS2/MinGW or Visual Studio works.

```bash
git clone https://github.com/usmro/-WarehouseInventoryMonitor---Baranai-Tudor.git
cd -WarehouseInventoryMonitor---Baranai-Tudor
```

With `make`:

```bash
make          # build
make run      # start the app
make test     # run the unit tests
make clean    # remove build files
```

With CMake:

```bash
cmake -S . -B build
cmake --build build
./build/warehouse_inventory
ctest --test-dir build
```

In CLion, open the folder and it picks up `CMakeLists.txt`. In VS Code, install the C/C++ extension (and optionally CMake Tools) and build from the terminal.

GitHub Actions builds the project and runs the 20 unit tests with both `make` and CMake on every push to `main` and `develop`.

## Main menu

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

## OOP concepts used

| Concept | Where it is used |
|---------|------------------|
| Classes and objects | `Produs`, `ProdusElectronic`, `ProdusMobilier`, `Depozit`, `Furnizor`, `Tranzactie<TP>`, `MiscareStoc`, `IstoricTranzactii` |
| Encapsulation | private fields with controlled public methods |
| Inheritance | `ProdusElectronic` and `ProdusMobilier` extend `Produs`; `MiscareIntrare` and `MiscareIesire` extend `MiscareStoc` |
| Polymorphism | virtual methods `getTip()`, `getDetaliiSpecifice()`, `clone()`, `getDeltaStoc()` |
| Abstract class | `MiscareStoc` with pure virtual methods, stored through `std::shared_ptr<MiscareStoc>` |
| STL | `std::unordered_map`, `std::vector`, `std::shared_ptr`, `std::chrono` |
| Operator overloading | `Produs::operator+=` and `Produs::operator-=` |
| Templates | `Tranzactie<Intrare>` and `Tranzactie<Iesire>` |
| Exceptions | duplicate IDs, missing products, invalid quantities, invalid CSV data |
| File persistence | products, suppliers and history in three separate CSV files |

### Product types

- `Produs`: a standard warehouse product.
- `ProdusElectronic`: adds a warranty in months.
- `ProdusMobilier`: adds the material.

The warehouse stores products as `std::shared_ptr<Produs>`, so the catalog can show type-specific details without knowing the exact class.

### Stock movements

```text
MiscareStoc (abstract)
+-- MiscareIntrare    getDeltaStoc() -> +cantitate
+-- MiscareIesire     getDeltaStoc() -> -cantitate
```

Movements are stored as `std::shared_ptr<MiscareStoc>` in `IstoricTranzactii`. When a product is removed, it is also removed from every supplier that listed it.

## Data files

All three files use `;` as the separator. Lines starting with `#` are headers.

**`data/produse.csv`:** the product catalog.

```text
tip;id;nume;categorie;cantitate;pret;pragAlerta;detaliuExtra
Electronic;101;Laptop Lenovo IdeaPad;Laptopuri;12;2499.99;3;24
Mobilier;201;Dulap alb cu doua usi;Dulapuri;4;699.50;2;PAL melaminat
```

**`data/furnizori.csv`:** suppliers and the products they provide.

```text
id;nume;contact;produseIds (separated by commas)
10;ElectroDist SRL;electrodist@example.com;101,102,103,104
```

**`data/istoric.csv`:** the transaction history.

```text
tip;timestamp;produsId;cantitate;observatii
Intrare;2026-05-04 09:15:22;101;10;comanda lunara furnizor
Iesire;2026-05-05 11:40:08;101;3;vanzare online
```

The sample data is made up, and the supplier emails use the reserved `example.com` domain.

## Project structure

```text
src/        C++ source files
tests/      unit tests
docs/       project documentation (Markdown, PDF, DOCX) and UML notes
data/       CSV files used as storage
Makefile    build and test commands
```

## Known limitations

- **One user at a time.** The CSV files are rewritten after every change, with no locking, so two copies of the app running at once would overwrite each other.
- **Names can't contain `;`,** and supplier names can't contain `,` either, because those are the CSV separators. The app rejects them.
- **Clearing the history is permanent.** There is no undo.
- **Romanian interface only,** and prices are always in lei.

## Rights

© 2026 Baranai Tudor. All rights reserved. This code is public so it can be read and graded, not reused.
