#ifndef CUBE_H
#define CUBE_H

#include <bits/stdc++.h>

using namespace std;

vector<uint64_t> factorial();

extern vector<uint64_t> FACT;

typedef vector<vector<char>> face; 

typedef enum direction{CLOCK, COUNTER}direction;
typedef enum side{UP = 0, LEFT, FRONT, RIGHT, DOWN, BACK}side;

const char FACE_COLOR[6] = {
    'Y', // UP
    'G', // LEFT
    'R', // FRONT
    'B', // RIGHT
    'W', // DOWN
    'O'  // BACK
};

vector<face> get_cube();

void print_face(vector<face>& cube, side s);

void print_cube(vector<face> cube);

void rotate(face& f, direction d);

void U(vector<face>& cube, direction d);

void D(vector<face>& cube, direction d);

void move_column(vector<face>& cube, side from, side to, int id);

void tmp_column(vector<face>& cube, vector<char>& tmp, side to, int id);

void L(vector<face>& cube, direction d);

void R(vector<face>& cube, direction d);

vector<char> get_column(face& f, int id);

void F(vector<face>& cube, direction d);

void B(vector<face>& cube, direction d);

void move(vector<face>& cube, side s, direction d);




#endif