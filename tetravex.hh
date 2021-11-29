#pragma once

#include <assert.h>
#include <math.h>
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
        int size = 3;

    public:
        // get / set
        TILE get_tile(int i, int j);
        TILE get_tile(int pos);
        void set_tile(int i, int j, TILE tile);
        void set_tile(int pos, TILE tile);

        void set_movement(int i, int j, int value);
        void set_movement(int pos, int value);

        // utils
        void draw_simple_board();
        void draw_board();
        bool load_file(const char* file_path);

        // rules
        bool check_board();

        // solver
        int solve();
};