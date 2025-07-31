#include <bits/stdc++.h>
using namespace std;


typedef vector<vector<int>> face; 

typedef enum direction{UP, DOWN, LEFT, RIGHT, CLOCK, COUNTER}direction;

vector<face> get_cube(){
    vector<face> cube(6, face(3, vector<int>(3, 0)));
    for(int i = 0; i < 6; i++)
        for(int j = 0; j < 3; j++)
            for(int k = 0; k < 3; k++){
                if(((j == 0 || j == 2) && (k == 0 || k == 2)) || j == 1 && k == 1)
                    cube[i][j][k] = i;
                else if(j == 0 && k == 1)
                    cube[i][j][k] = 6;
                else
                    cube[i][j][k] = 7;
            }
    return cube;
}

void print_cube(vector<face> cube){
    cout << "===================================================\n\n";
    for(int i = 0; i < 6; i++){
        if(i == 0 || i > 3){
            for(int j = 0; j < 3; j++){
                cout << "        ";
                for(int k = 0; k < 3; k++)
                        cout << cube[i][j][k] << ' ';
                cout << "\n";
            }
        }
        else{
            for(int j = 0; j < 3; j++){
                for(int k = 1; k <= 3; k++){
                    for(int l = 0; l < 3; l++)
                        cout << cube[k][j][l] << ' ';
                    cout << "  ";
                }
                cout << "\n";
            }
            i = 3;
        }
        cout << "\n\n";
    }
}

void rotate(vector<face>& cube, int f, direction d){
    int c20 = cube[f][2][0],
        c10 = cube[f][1][0],
        c00 = cube[f][0][0],
        c01 = cube[f][0][1],
        c02 = cube[f][0][2],
        c12 = cube[f][1][2],
        c22 = cube[f][2][2],
        c21 = cube[f][2][1];
    if(d == CLOCK){
        cube[f][0][0] = c20;
        cube[f][0][1] = c10;
        cube[f][0][2] = c00;
        cube[f][1][2] = c01;
        cube[f][2][2] = c02;
        cube[f][2][1] = c12;
        cube[f][2][0] = c22;
        cube[f][1][0] = c21;
    }
    else{
        cube[f][0][0] = c02;
        cube[f][0][1] = c12;
        cube[f][0][2] = c22;
        cube[f][1][2] = c21;
        cube[f][2][2] = c20;
        cube[f][2][1] = c10;
        cube[f][2][0] = c00;
        cube[f][1][0] = c01;
    }
}

void vertical(vector<face>& cube, int column, direction d){
    vector<int> tmp;
    for(int i = 0; i < 3; i++)
        tmp.emplace_back(cube[0][i][column]);
    if(d == UP){
        for(int i = 0; i < 3; i++){
            cube[0][i][column] = cube[2][i][column];
            cube[2][i][column] = cube[4][i][column];
            cube[4][i][column] = cube[5][i][column];
            cube[5][i][column] = tmp[i];
        }
    }
    else{
        for(int i = 0; i < 3; i++){
            cube[0][i][column] = cube[5][i][column];
            cube[5][i][column] = cube[4][i][column];
            cube[4][i][column] = cube[2][i][column];
            cube[2][i][column] = tmp[i];
        }
    }
    if(column == 0){
        if(d == UP)
            rotate(cube, 1, COUNTER);
        else
            rotate(cube, 1, CLOCK);
    }
    else if(column == 2){
        if(d == UP)
            rotate(cube, 3, CLOCK);
        else
            rotate(cube, 3, COUNTER);
    }
}

int main(){
    vector<face> cube = get_cube();
    while(true){
        print_cube(cube);
        int move, index, dir;
        cin >> move >> index >> dir;
        if(dir == 1)
            vertical(cube, index, UP);
        else
            vertical(cube, index, DOWN);
    }

    return 0;
}