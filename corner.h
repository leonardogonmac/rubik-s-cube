#ifndef CORNER_H
#define CORNER_H

#include <bits/stdc++.h>
#include "cube.h"
#include "pdb_api.h"

using namespace std;

constexpr uint64_t NUM_CORNER_STATES = 40320ull * 2187ull;

struct CornerSpec {
    side faces[3];  // which faces this cubie touches in the solved state
};

const CornerSpec CORNER_SPECS[8] = {
    { {UP, RIGHT, FRONT} }, // URF
    { {UP, FRONT, LEFT}  }, // UFL
    { {UP, LEFT, BACK}   }, // ULB
    { {UP, BACK, RIGHT}  }, // UBR
    { {DOWN, FRONT, RIGHT} }, // DFR
    { {DOWN, LEFT, FRONT}  }, // DLF
    { {DOWN, BACK, LEFT}   }, // DBL
    { {DOWN, RIGHT, BACK}  }  // DRB
};

typedef struct corner{
    set<side> sides;
    map<side, pair<int,int>> fcoord;
    map<side,char> colors;
    corner(set<side> s, map<side,pair<int,int>> f, map<side, char> c){
        sides = s;
        fcoord = f;
        colors = c;
    }
}corner;

enum CornerId {
    URF = 0,
    UFL = 1,
    ULB = 2,
    UBR = 3,
    DFR = 4,
    DLF = 5,
    DBL = 6,
    DRB = 7
};

int cornerUDColor(CornerId id);

int cornerOrientation(const corner& c, int id);

int cornerId(const corner& c);

corner get_corner(vector<face>& cube, set<side> sides);

vector<corner> all_corners(vector<face>& cube);

pair<int*,int*> extractCornerCubies(vector<face>& cube);


uint64_t cornerPermutationIndex(int perm[]);

uint64_t cornerOrientationIndex(int ori[]);


uint64_t cornerIndex(int perm[], int ori[]);

int cornerHeuristic(vector<face>& cube, const NibblePDB& cornerPDB);

#endif