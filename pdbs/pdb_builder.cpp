#include <bits/stdc++.h>
#include "../corner.h"

using namespace std;

vector<uint8_t> pdb(NUM_CORNER_STATES, 0xFF); // 0xFF = "unvisited/uninitialized"

typedef struct CornerState{
    int perm[8];
    int ori[8];
}CornerState;

// Corner permutation for each face move
// (cp'[pos] = cp[CP[m][pos]])
static const int CP[6][8] = {
    // U
    { UBR, URF, UFL, ULB, DFR, DLF, DBL, DRB },
    // R
    { DFR, UFL, ULB, URF, DRB, DLF, DBL, UBR },
    // F
    { UFL, DLF, ULB, UBR, URF, DFR, DBL, DRB },
    // D
    { URF, UFL, ULB, UBR, DLF, DBL, DRB, DFR },
    // L
    { URF, ULB, DBL, UBR, DFR, UFL, DLF, DRB },
    // B
    { URF, UFL, UBR, DRB, DFR, DLF, ULB, DBL }
};

// Corner orientation deltas for each face move
// (co'[pos] = (co[CP[m][pos]] + CO[m][pos]) % 3)
static const int CO[6][8] = {
    // U
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    // R
    { 2, 0, 0, 1, 1, 0, 0, 2 },
    // F
    { 1, 2, 0, 0, 2, 1, 0, 0 },
    // D
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    // L
    { 0, 1, 2, 0, 0, 2, 1, 0 },
    // B
    { 0, 0, 1, 2, 0, 0, 2, 1 }
};

inline CornerState applyMove(const CornerState &s, int m) {
    CornerState r;
    for (int pos = 0; pos < 8; ++pos) {
        int from = CP[m][pos];
        r.perm[pos] = s.perm[from];
        int ori   = s.ori[from] + CO[m][pos];
        r.ori[pos] = ori % 3;
    }
    return r;
}
CornerState applyMovePrime(const CornerState &s, int m) {
    // three quarter-turns = inverse
    return applyMove(applyMove(applyMove(s, m), m), m);
}

CornerState applyMove2(const CornerState &s, int m) {
    // two quarter-turns
    return applyMove(applyMove(s, m), m);
}

void buildCornerPDB() {
    queue<CornerState> q;

    CornerState solved;
    for (int i = 0; i < 8; ++i) {
        solved.perm[i] = i;
        solved.ori[i] = 0;
    }

    uint64_t idx0 = cornerIndex(solved.perm, solved.ori);
    pdb[idx0] = 0;        // distance 0
    q.push(solved);


    while (!q.empty()) {
        CornerState s = q.front(); q.pop();
        uint8_t d = pdb[cornerIndex(s.perm, s.ori)];

        // Apply all moves you want the PDB to respect (e.g., 6 faces × 3 turns)
        CornerState neighbors[18];
        int nbCount = 0;

        for(int i = 0; i < 6; i++){
            neighbors[nbCount++] = applyMove(s, i);
            neighbors[nbCount++] = applyMove2(s, i);
            neighbors[nbCount++] = applyMovePrime(s, i);
        }

        // ... similarly for D, L, R, F, B ...

        for (int i = 0; i < nbCount; ++i) {
            uint64_t idx = cornerIndex(neighbors[i].perm, neighbors[i].ori);
            if (pdb[idx] == 0xFF) {
                pdb[idx] = d + 1;  // store d (or d+1, up to you)
                q.push(neighbors[i]);
            }
        }
    }

    // Optionally sanity-check that every state got visited:
    // count how many entries != 0xFF, it should be NUM_STATES.
}

void writeNibblePDB(const std::string& filename,bool storeAsDPlus1 = true){
    std::ofstream out(filename, std::ios::binary);
    if (!out) throw std::runtime_error("Failed to open output file");

    // Optional 1-byte header; can store something like version or pattern type.
    uint8_t header = 0;
    out.write(reinterpret_cast<char*>(&header), 1);

    uint8_t byte = 0;
    bool high = false;

    for (uint64_t i = 0; i < pdb.size(); ++i) {
        uint8_t d = pdb[i];

        if (d == 0xFF) {
            // Unreachable (shouldn't happen if BFS finished); pick a sentinel like 0xF
            d = 0xF;
        } else if (storeAsDPlus1) {
            d = std::min<uint8_t>(d + 1, 0xE); // keep <= 14 if you reserve 0xF
        }

        if (!high) {
            byte = d & 0x0F;
            high = true;
        } else {
            byte |= (d & 0x0F) << 4;
            out.put(static_cast<char>(byte));
            high = false;
            byte = 0;
        }
    }

    // If odd number of entries, flush last half-filled byte
    if (high) {
        out.put(static_cast<char>(byte));
    }
}


int main(){
    buildCornerPDB();
    writeNibblePDB("corner.pdb");
    return 0;
}