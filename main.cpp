#include <bits/stdc++.h>
#include "pdb_api.h"
#include "cube.h"
#include "corner.h"

using namespace std;

int main(){
    vector<face> cube = get_cube();
    char s, d;
    direction dd;
    side ss;
    NibblePDB cornerPDB("pdbs/corner.pdb", NUM_CORNER_STATES, 1);

    while(true){
        print_cube(cube);
        auto cs = all_corners(cube);
        for(auto c: cs){
            string ss;
            for(auto cc: c.colors)
                ss.push_back(cc.second);
            sort(ss.begin(), ss.end());
            cout << ss << '\n';
        }
        int heur = cornerHeuristic(cube, cornerPDB);
        cout << "HEUR = " << heur << '\n';
        cin >> s >> d;
        switch(s){
            case 'U':
                ss = UP;
                break;
            case 'D':
                ss = DOWN;
                break;
            case 'L':
                ss = LEFT;
                break;
            case 'R':
                ss = RIGHT;
                break;
            case 'F':
                ss = FRONT;
                break;
            case 'B':
                ss = BACK;
                break;
        }
        switch(d){
            case 'H':
                dd = CLOCK;
                break;
            case 'A':
                dd = COUNTER;
                break;
        }
        move(cube, ss, dd);

    }

    return 0;
}