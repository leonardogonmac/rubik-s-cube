#include "corner.h"

using namespace std;

corner get_corner(vector<face>& cube, set<side> sides){
    map<side, pair<int,int>> fcoord;
    if(sides.count(UP) && sides.count(LEFT) && sides.count(FRONT)){
        fcoord[UP] = {2,0};
        fcoord[LEFT] = {0,2};
        fcoord[FRONT] = {0,0};
    }
    else if(sides.count(UP) && sides.count(LEFT) && sides.count(BACK)){
        fcoord[UP] = {0,0};
        fcoord[LEFT] = {0,0};
        fcoord[BACK] = {0,2};
    }
    else if(sides.count(UP) && sides.count(RIGHT) && sides.count(FRONT)){
        fcoord[UP] = {2,2};
        fcoord[RIGHT] = {0,0};
        fcoord[FRONT] = {0,2};
    }
    else if(sides.count(UP) && sides.count(RIGHT) && sides.count(BACK)){
        fcoord[UP] = {0,2};
        fcoord[RIGHT] = {0,2};
        fcoord[BACK] = {0,0};
    }
    else if(sides.count(DOWN) && sides.count(LEFT) && sides.count(FRONT)){
        fcoord[DOWN] = {0,0};
        fcoord[LEFT] = {2,2};
        fcoord[FRONT] = {2,0};
    }
    else if(sides.count(DOWN) && sides.count(LEFT) && sides.count(BACK)){
        fcoord[DOWN] = {2,0};
        fcoord[LEFT] = {2,0};
        fcoord[BACK] = {2,2};
    }
    else if(sides.count(DOWN) && sides.count(RIGHT) && sides.count(FRONT)){
        fcoord[DOWN] = {0,2};
        fcoord[RIGHT] = {2,0};
        fcoord[FRONT] = {2,2};
    }
    else if(sides.count(DOWN) && sides.count(RIGHT) && sides.count(BACK)){
        fcoord[DOWN] = {2,2};
        fcoord[RIGHT] = {2,2};
        fcoord[BACK] = {2,0};
    }
    map<side, char> colors;
    for(auto s: fcoord)
        colors[s.first] = cube[s.first][s.second.first][s.second.second];
    
    return corner(sides, fcoord, colors);

}

vector<corner> all_corners(vector<face>& cube) {
    vector<corner> v;
    v.push_back(get_corner(cube, {UP, RIGHT, FRONT}));  // URF
    v.push_back(get_corner(cube, {UP, FRONT, LEFT}));   // UFL
    v.push_back(get_corner(cube, {UP, LEFT, BACK}));    // ULB
    v.push_back(get_corner(cube, {UP, BACK, RIGHT}));   // UBR
    v.push_back(get_corner(cube, {DOWN, FRONT, RIGHT}));// DFR
    v.push_back(get_corner(cube, {DOWN, LEFT, FRONT})); // DLF
    v.push_back(get_corner(cube, {DOWN, BACK, LEFT}));  // DBL
    v.push_back(get_corner(cube, {DOWN, RIGHT, BACK})); // DRB
    return v;
}

int cornerId(const corner& c) {
     // 1) observed colors at this position
    string obs;
    int k = 0;
    for (auto &kv : c.colors) {   // c.colors: side -> color
        obs.push_back(kv.second);
    }
    sort(obs.begin(), obs.end());

    // 2) check against each cubie spec
    for (int id = 0; id < 8; ++id) {
        string spec;
        for (int j = 0; j < 3; ++j) {
            side f = CORNER_SPECS[id].faces[j];
            spec.push_back(FACE_COLOR[f]);  // solved color on that face
        }
        sort(spec.begin(), spec.end());
        cout << obs <<  "||" << spec << '\n';

        if (spec == obs) {
            return id; // CornerId id
        }
    }

    throw std::runtime_error("cornerCubieIdFromColors: unknown color triple");
}

int cornerUDColor(int id) {
    switch (id) {
        case URF: case UFL: case ULB: case UBR:
            return FACE_COLOR[UP];
        case DFR: case DLF: case DBL: case DRB:
            return FACE_COLOR[DOWN];
    }
    return 0;
}

int cornerOrientation(const corner& c, int id) {
    char udColor = cornerUDColor(id);

    side udSide = UP;
    bool found = false;
    for (auto &kv : c.colors) {
        side s = kv.first;
        char col = kv.second;
        if (col == udColor) {
            udSide = s;
            found = true;
            break;
        }
    }

    if (udSide == UP || udSide == DOWN) return 0;
    if (udSide == RIGHT || udSide == LEFT) return 1;
    return 2;
}

pair<int*,int*> extractCornerCubies(vector<face>& cube){
    auto cs = all_corners(cube);
    int *perm = new int[8], 
        *ori = new int[8];

    for (int pos = 0; pos < 8; ++pos) {
        const corner& c = cs[pos];
        int id = cornerId(c);
        perm[pos] = id;
        ori[pos]  = cornerOrientation(c, id);
    }
    return {perm, ori};
}


uint64_t cornerPermutationIndex(int perm[]) {
    uint64_t rank = 0;

    for (int i = 0; i < 8; ++i) {
        int c = 0;
        for (int j = i + 1; j < 8; ++j) {
            if (perm[j] < perm[i]) ++c;
        }
        rank += c * FACT[7 - i];
    }

    return rank;
}

uint64_t cornerOrientationIndex(int ori[]) {
    uint64_t idx = 0;
    for (int i = 0; i < 7; ++i) {
        uint64_t o = (uint64_t) ori[i];
        idx = idx * 3 + o;
    }
    return idx;
}


uint64_t cornerIndex(int perm[], int ori[]) {
    uint64_t p = cornerPermutationIndex(perm);
    uint64_t o = cornerOrientationIndex(ori);
    return p * 2187ull + o;
}

int cornerHeuristic(vector<face>& cube, const NibblePDB& cornerPDB) {
    auto [perm, ori] = extractCornerCubies(cube);
    cout << "PERM: ";
    for(int i = 0; i < 8; i++)
        cout << perm[i] << ' ';
    cout << '\n';
    cout << "ORI: ";
    for(int i = 0; i < 8; i++)
        cout << ori[i] << ' ';
    cout << '\n';
    uint64_t idx = cornerIndex(perm, ori);
    cout << "IDX = " << idx << '\n';
    uint8_t raw  = cornerPDB.getDistance(idx);

    if (raw == 0xF) return 0;
    return ((int) raw) - 1;
}