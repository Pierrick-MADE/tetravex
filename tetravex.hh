#pragma once

#include <unistd.h>
#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <string>
#include <set>

#define MAX_SIZE 6

struct TILE {
    unsigned char n; // North
    unsigned char w; // West
    unsigned char e; // East
    unsigned char s; // South
    bool fixed = false;
};

class Tetravex {
    private:
        TILE board[MAX_SIZE * MAX_SIZE];
        int movements[MAX_SIZE * MAX_SIZE];
        int size = MAX_SIZE;

        void set_tile(int i, int j, TILE tile);
        void set_tile(int pos, TILE tile);
        void copy_movements_to(int *out_arr);
        void load_movements_from(int *in_arr);

        float find_initial_t();

    public:
        // get / set
        TILE get_tile(int i, int j);
        TILE get_tile(int pos);

        // moves
        bool swap_tiles(int pos1, int pos2);
        void randomize_board(int nb_random_swaps=0);

        // utils
        void draw_simple_board();
        void draw_board();
        bool load_file(const char* file_path);

        // rules
        bool check_board();

        // solver
        int get_score();
        void solve();
};