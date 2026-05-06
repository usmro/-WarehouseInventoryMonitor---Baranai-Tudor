# WarehouseInventoryMonitor - Baranai Tudor

C++ console application for warehouse stock monitoring. The project manages products, quantities, prices and low-stock alerts using Object-Oriented Programming concepts.

## Project Theme

Tema 3122B: Sistem de Monitorizare a Stocurilor unui Depozit

The application allows a warehouse operator to:

- add and remove products;
- restock products;
- register product sales;
- list all products;
- generate a report with products below the alert threshold;
- suggest products that should be reordered.

## OOP Concepts Used

- classes and objects: `Produs`, `Depozit`, `Furnizor`, `Tranzactie<TP>`;
- encapsulation through private fields and public methods;
- STL containers: `std::unordered_map` and `std::vector`;
- operator overloading: `Produs::operator+=` and `Produs::operator-=`;
- exception handling for invalid IDs, duplicate products and invalid stock operations;
- templates through `Tranzactie<Intrare>` and `Tranzactie<Iesire>`.

## Project Structure

```text
src/        C++ source files
tests/      simple unit tests
docs/       project documentation and UML notes
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
0. Iesire
```
