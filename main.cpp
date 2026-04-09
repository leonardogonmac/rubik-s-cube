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

// void astar(vector<face>& cube, NibblePDB& cornerPDB){
//     cout << "astar started\n";
//     map<size_t, vector<face>> open, closed;
//     priority_queue<pair<size_t,vector<face>>> pq;
//     open[fhash(cube)] = cube;
//     pq.push({-cornerHeuristic(cube, cornerPDB), cube});
//     while(!pq.empty()){
//         auto [w, v] = pq.top();
//         pq.pop();
//         w = -w; w -= cornerHeuristic(v, cornerPDB);
//         open.erase(fhash(v));
//         closed[fhash(v)] = v;
//         vector<vector<face>> adj = get_adj(v);
//         for(auto a: adj){
//             if(solved(a)){
//                 print_cube(a);
//                 cout << w + 1 << '\n';
//                 return;
//             }
//             if(!open.count(fhash(a)) && !closed.count(fhash(a))){
//                 open[fhash(a)] = a;
//                 pq.push({-(cornerHeuristic(a, cornerPDB) + w + 1), a});
//             }
//         }
//     }    
// }

vector<map<size_t, vector<face>>> open, closed;
vector<priority_queue<pair<size_t,vector<face>>>> pq;
vector<queue<pair<size_t,vector<face>>>> qs;
vector<mutex> mtx;
size_t num = 0;
bool solution_found = false;

void enqueue(size_t target, vector<face>& a, size_t w){
    mtx[target].lock();
    qs[target].push({(w + 1), a});
    mtx[target].unlock();
}

void dequeue(size_t id, NibblePDB& cornerPDB){
    queue<pair<size_t,vector<face>>> q;
    mtx[id].lock();
    while(!qs[id].empty()){
        q.push(qs[id].front());
        qs[id].pop();
    }
    mtx[id].unlock();
    while(!q.empty()){
        auto [w, v] = q.front();
        q.pop();
        if( !open[id].count(fhash(v)) && !closed[id].count(fhash(v)) ){
            open[id][fhash(v)] = v;
            pq[id].push({-(cornerHeuristic(v, cornerPDB) + w), v});
        }
    }
}

void thread_astar(vector<face> cube, size_t id){
    cout << "thread " << id << " started\n";
    NibblePDB cornerPDB("./pdbs/corner" + to_string(id) + ".pdb", NUM_CORNER_STATES, 1);
    open[id][fhash(cube)] = cube;
    pq[id].push({-cornerHeuristic(cube, cornerPDB), cube});
    while(!pq[id].empty()){
        if(solution_found) 
            return;
        auto [w, v] = pq[id].top();
        pq[id].pop();
        w = -w; w -= cornerHeuristic(v, cornerPDB);
        open[id].erase(fhash(v));
        closed[id][fhash(v)] = v;
        vector<vector<face>> adj = get_adj(v);
        for(auto a: adj){
            size_t target = fhash(a) % num;
            if(solved(a)){
                solution_found = true;
                print_cube(a);
                cout << w + 1 << '\n';
                return;
            }
            if(target == id && !open[id].count(fhash(a)) && !closed[id].count(fhash(a))){
                open[id][fhash(a)] = a;
                pq[id].push({-(cornerHeuristic(a, cornerPDB) + w + 1), a});
            }
            else
                enqueue(target, a, w);
        }
        dequeue(id, cornerPDB);
    }    
}

void sample(){
    vector<face> cube = get_cube();
    //print_cube(cube);
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
    U(cube, CLOCK);
    D(cube, CLOCK);
    //D(cube, CLOCK);
    //L(cube, COUNTER);
    //R(cube, CLOCK);
    //F(cube, CLOCK);
    //F(cube, CLOCK);
    //B(cube, COUNTER);
    print_sample(cube);
}

int main(int argc, char* argv[]){
    if(argc == 1){
        sample();
        return 0;
    }

    num = atoi(argv[1]);
    open = vector<map<size_t, vector<face>>>(num);
    closed = vector<map<size_t, vector<face>>>(num);
    pq = vector<priority_queue<pair<size_t,vector<face>>>>(num);
    qs = vector<queue<pair<size_t,vector<face>>>>(num);
    mtx = vector<mutex>(num);
    
    vector<face> cube = read_cube();
    
    vector<thread> ts;
    for(size_t i = 0; i < num; i++)
        ts.emplace_back(thread_astar, cube, i);

    for(size_t i = 0; i < num; i++)
        ts[i].join();

    return 0;
}