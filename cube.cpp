#include "cube.h"

vector<uint64_t> factorial(){
    vector<uint64_t> f(9);
    f[0] = 1;
    for (int i = 1; i <= 8; ++i) f[i] = f[i-1] * i;
    return f;
}

vector<uint64_t> FACT = factorial();

vector<face> read_cube(){
    vector<face> cube(6, face(3, vector<char>(3, 0)));
        for(int i = 0; i < 6; i++)
            for(int j = 0; j < 3; j++)
                for(int k = 0; k < 3; k++)
                    cin >> cube[i][j][k];
    return cube;
}

vector<face> get_cube(){
    vector<face> cube(6, face(3, vector<char>(3, 0)));
    for(int i = 0; i < 6; i++)
        for(int j = 0; j < 3; j++)
            for(int k = 0; k < 3; k++){
                switch (i){
                case 0:
                    cube[i][j][k] = 'Y';
                    break;
                case 1:
                    cube[i][j][k] = 'G';
                    break;
                case 2:
                    cube[i][j][k] = 'R';
                    break;
                case 3:
                    cube[i][j][k] = 'B';
                    break;
                case 4:
                    cube[i][j][k] = 'W';
                    break;
                case 5:
                    cube[i][j][k] = 'O';
                    break;
                }
            }
    return cube;
}

void print_face(vector<face>& cube, side s){
    cout << "=======================\n";
    for(auto r: cube[s]){
        for(auto x: r)
            cout << x << ' ';
        cout << '\n';
    }
}

void print_sample(vector<face>& cube){
    for(int i = 0; i < 6; i++){
        for(int j = 0; j < 3; j++){
            for(int k = 0; k < 3; k++)
                cout << cube[i][j][k] << " ";
            cout << "\n";
        }
        cout << '\n';
    }
}

void print_cube(vector<face>& cube){
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

void rotate(face& f, direction d){
    if(d == CLOCK){
        char tmp = f[0][0];
        f[0][0] = f[2][0];
        f[2][0] = f[2][2];
        f[2][2] = f[0][2];
        f[0][2] = tmp;

        tmp = f[0][1];
        f[0][1] = f[1][0];
        f[1][0] = f[2][1];
        f[2][1] = f[1][2];
        f[1][2] = tmp;
        
    }
    else{
        char tmp = f[0][0];
        f[0][0] = f[0][2];
        f[0][2] = f[2][2];
        f[2][2] = f[2][0];
        f[2][0] = tmp;

        tmp = f[0][1];
        f[0][1] = f[1][2];
        f[1][2] = f[2][1];
        f[2][1] = f[1][0];
        f[1][0] = tmp;
    }
}

void U(vector<face>& cube, direction d){
    vector<char> tmp = cube[FRONT][0];
    if(d == CLOCK){
        cube[FRONT][0] = cube[RIGHT][0];
        cube[RIGHT][0] = vector<char>(cube[BACK][2].rbegin(), cube[BACK][2].rend());
        cube[BACK][2] = vector<char>(cube[LEFT][0].rbegin(), cube[LEFT][0].rend());
        cube[LEFT][0] = tmp;
        rotate(cube[UP], CLOCK);
    }
    else{
        cube[FRONT][0] = cube[LEFT][0];
        cube[LEFT][0] = vector<char>(cube[BACK][2].rbegin(), cube[BACK][2].rend());
        cube[BACK][2] = vector<char>(cube[RIGHT][0].rbegin(), cube[RIGHT][0].rend());
        cube[RIGHT][0] = tmp;
        rotate(cube[UP], COUNTER);
    }
}



void D(vector<face>& cube, direction d){
    vector<char> tmp = cube[FRONT][2];
    if(d == COUNTER){
        cube[FRONT][2] = cube[RIGHT][2];
        cube[RIGHT][2] = vector<char>(cube[BACK][0].rbegin(), cube[BACK][0].rend());
        cube[BACK][0] = vector<char>(cube[LEFT][2].rbegin(), cube[LEFT][2].rend());
        cube[LEFT][2] = tmp;
        rotate(cube[DOWN], COUNTER);
    }
    else{
        cube[FRONT][2] = cube[LEFT][2];
        cube[LEFT][2] = vector<char>(cube[BACK][0].rbegin(), cube[BACK][0].rend());
        cube[BACK][0] = vector<char>(cube[RIGHT][2].rbegin(), cube[RIGHT][2].rend());
        cube[RIGHT][2] = tmp;
        rotate(cube[DOWN], CLOCK);
    }
}

void move_column(vector<face>& cube, side from, side to, int id){
    cube[to][0][id] = cube[from][0][id];
    cube[to][1][id] = cube[from][1][id];
    cube[to][2][id] = cube[from][2][id];
}

void tmp_column(vector<face>& cube, vector<char>& tmp, side to, int id, bool rev){
    if(!rev){
        cube[to][0][id] = tmp[0];
        cube[to][1][id] = tmp[1];
        cube[to][2][id] = tmp[2];
    }
    else{
        cube[to][0][id] = tmp[2];
        cube[to][1][id] = tmp[1];
        cube[to][2][id] = tmp[0];
    }
}

void L(vector<face>& cube, direction d){
    vector<char> tmp = {cube[FRONT][0][0], cube[FRONT][1][0], cube[FRONT][2][0]};
    if(d == CLOCK){
        move_column(cube, UP, FRONT, 0);
        move_column(cube, BACK, UP, 0);
        move_column(cube, DOWN, BACK, 0);
        tmp_column(cube, tmp, DOWN, 0);
        rotate(cube[LEFT], CLOCK);
    }
    else{
        move_column(cube, DOWN, FRONT, 0);
        move_column(cube, BACK, DOWN, 0);
        move_column(cube, UP, BACK, 0);
        tmp_column(cube, tmp, UP, 0);
        rotate(cube[LEFT], COUNTER);
    }

}
void R(vector<face>& cube, direction d){
    vector<char> tmp = {cube[FRONT][0][2], cube[FRONT][1][2], cube[FRONT][2][2]};
    if(d == COUNTER){
        move_column(cube, UP, FRONT, 2);
        move_column(cube, BACK, UP, 2);
        move_column(cube, DOWN, BACK, 2);
        tmp_column(cube, tmp, DOWN, 2);
        rotate(cube[RIGHT], COUNTER);
    }
    else{
        move_column(cube, DOWN, FRONT, 2);
        move_column(cube, BACK, DOWN, 2);
        move_column(cube, UP, BACK, 2);
        tmp_column(cube, tmp, UP, 2);
        rotate(cube[RIGHT], CLOCK);
    }

}

vector<char> get_column(face& f, int id){
    vector<char> c(3);
    c[0] = f[0][id];
    c[1] = f[1][id];
    c[2] = f[2][id];
    return c;
}

void F(vector<face>& cube, direction d){
    vector<char> tmp = cube[UP][2];
    if(d == CLOCK){
        vector<char> u2 = get_column(cube[LEFT], 2);
        cube[UP][2] = vector<char>(u2.rbegin(), u2.rend());
        tmp_column(cube, cube[DOWN][0], LEFT, 2);
        vector<char> r0 = get_column(cube[RIGHT], 0);
        cube[DOWN][0] = vector<char>(r0.rbegin(), r0.rend());
        tmp_column(cube, tmp, RIGHT, 0);
        rotate(cube[FRONT], CLOCK);
    }
    else{
        cube[UP][2] = get_column(cube[RIGHT], 0);
        tmp_column(cube, cube[DOWN][0], RIGHT, 0, true);
        cube[DOWN][0] = get_column(cube[LEFT], 2);
        tmp_column(cube, tmp, LEFT, 2, true);
        rotate(cube[FRONT], COUNTER);
    }

}
void B(vector<face>& cube, direction d){
    vector<char> tmp = cube[UP][0];
    if(d == COUNTER){
        vector<char> u0 = get_column(cube[LEFT], 0);
        cube[UP][0] = vector<char>(u0.rbegin(), u0.rend());
        tmp_column(cube, cube[DOWN][2], LEFT, 0);
        vector<char> d2 = get_column(cube[RIGHT], 2);
        cube[DOWN][2] = vector<char>(d2.rbegin(), d2.rend());
        tmp_column(cube, tmp, RIGHT, 2);
        rotate(cube[BACK], COUNTER);
    }
    else{
        cube[UP][0] = get_column(cube[RIGHT], 2);
        tmp_column(cube, cube[DOWN][2], RIGHT, 2, true);
        cube[DOWN][2] = get_column(cube[LEFT], 0);
        tmp_column(cube, tmp, LEFT, 0, true);
        rotate(cube[BACK], CLOCK);
    }

}

void move(vector<face>& cube, side s, direction d){
    switch (s){
        case UP:
            U(cube, d);
            break;
        case DOWN:
            D(cube, d);
            break;
        case LEFT:
            L(cube, d);
            break;
        case RIGHT:
            R(cube, d);
            break;
        case FRONT:
            F(cube, d);
            break;
        case BACK:
            B(cube, d);
            break;
    }
}



