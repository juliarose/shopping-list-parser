# shopping-list-parser

This program calculates the total price of my shopping list.

## Requirements

- C++17

## Compilation

### Setup

Create a `bin` directory for the compiled binary.
```bash
mkdir -p bin
```

### Compliation with g++

```bash
g++ -std=c++17 -O2 -g ./src/main.cpp ./src/unit.cpp ./src/utils.cpp ./src/shopping_list.cpp ./src/display.cpp -o bin/main
```

## Usage

```bash
./bin/main ./shopping-list.txt
```

## Example Output

The shopping-list.txt file with the following contents:

```txt
2 lb. Chicken Breasts, $4.99/lb.
10 Sweet Corn, 5/$2.00
Corn Chex, $2.79
```

Produces the following output:

```txt
Chicken Breasts     2 lb.     $9.98     @ $4.99 / lb.           
Sweet Corn          10        $4.00     @ 5 / $2.00             
Corn Chex           1         $2.79     @ $2.79 / ea.           

Total: $16.77
```

## License

[MIT](https://github.com/juliarose/shopping-list-parser/tree/main/LICENSE)
