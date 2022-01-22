
# Tetravex

## Members

- marie-charlotte.calisto
- pierrick.made

## How to use


```Bash
# Create executable

cd tetravex
g++ *.cc
```

```Bash
# Solve a board from a file
# ./a.out in.txt out.txt

./a.out boards/board_4 out.txt
```

```Bash
# Solve random boards for benchmark
# ./a.out random BOARD_SIZE NB_BOARDS

./a.out random 4 100
```