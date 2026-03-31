#include <bits/stdc++.h>
#include <boost/multi_index/hashed_index.hpp>
#include "pdb_api.h"
#include "cube.h"
#include "corner.h"

using namespace std;

size_t fhash(vector<face>& cube){
    size_t seed = 0;
    for(int i = 0; i < cube.size(); i++){
        for(int j = 0; j < cube[i].size(); j++){
            for(int k = 0; k < cube[i][j].size(); k++)
                boost::hash_combine(seed, cube[i][j][k]);
        }
    }
    return seed;
}

vector<vector<face>> get_adj(vector<face>& cube){
    vector<vector<face>> adj(12, cube);
    U(adj[0], CLOCK);
    D(adj[1], CLOCK);
    L(adj[2], CLOCK);
    R(adj[3], CLOCK);
    F(adj[4], CLOCK);
    B(adj[5], CLOCK);
    U(adj[6], COUNTER);
    D(adj[7], COUNTER);
    L(adj[8], COUNTER);
    R(adj[9], COUNTER);
    F(adj[10], COUNTER);
    B(adj[11], COUNTER);
    return adj;
}

bool face_solved(face& f){
    char a = f[0][0];
    for(vector<char> xs: f)
        for(char x: xs)
            if(x != a)
                return false;
    return true;
}

bool solved(vector<face>& cube) {
    for(face f: cube)
        if(!face_solved(f))
            return false;
    return true;
}

void astar(vector<face>& cube, NibblePDB& cornerPDB){
    cout << "astar started\n";
    map<size_t, vector<face>> open, closed;
    priority_queue<pair<size_t,vector<face>>> pq;
    open[fhash(cube)] = cube;
    pq.push({-cornerHeuristic(cube, cornerPDB), cube});
    while(!pq.empty()){
        vector<face> v = pq.top().second;
        pq.pop();
        open.erase(fhash(v));
        closed[fhash(v)] = v;
        vector<vector<face>> adj = get_adj(v);
        for(auto a: adj){
            if(solved(a)){
                print_cube(a);
                return;
            }
            if(!open.count(fhash(a)) && !closed.count(fhash(a))){
                open[fhash(a)] = a;
                pq.push({-cornerHeuristic(a, cornerPDB), a});
            }
        }
    }    
}

void sample(NibblePDB& cornerPDB){
    vector<face> cube = get_cube();
    print_cube(cube);
    F(cube, CLOCK);
    L(cube, CLOCK);
    B(cube, COUNTER);
    R(cube, COUNTER);
    U(cube, CLOCK);
    F(cube, COUNTER);
    U(cube, CLOCK);
    L(cube, COUNTER);
    L(cube, COUNTER);
    F(cube, CLOCK);

    print_cube(cube);
    cout << cornerHeuristic(cube, cornerPDB) << "\n";
}

int main(){
    NibblePDB cornerPDB("pdbs/corner.pdb", NUM_CORNER_STATES, 1);
    vector<face> cube = read_cube();
    astar(cube, cornerPDB);
    //sample(cornerPDB);
    return 0;
}