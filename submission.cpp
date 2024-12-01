#include<iostream>
#include<fstream>
#include<string>
#include<vector>
using namespace std;

vector<int> dfs(vector<vector<int>> flow, vector<vector<int>> capacity, int source, int sink){
    vector<int> path = {source};
    vector<bool> visited(flow.size());
    visited[source] = true;
    int i = 0;
    int u = source;
    while(u!=sink){
        //if there were no available forward or reversed edges, try a different path
        if(i>=flow.size()){
            path.pop_back();
            if(path.empty()) return path;
            u = path[path.size()-1];
            i = 0;
        }
        
        int f = flow[u][i]; //available flow
        int rf = capacity[i][u] - flow[i][u]; //total capacity - available flow = reverse flow

        //if node i is unavailable, continue
        if(visited[i] || (f==0 && rf==0)) i+=1;

        //if found an available forward edge, add it to the path
        else if(f>0){
            path.push_back(i);
            visited[i] = true;
            u = i;
            i = 0;
            if(u==sink) return path;
        }

        //if found an available reverse edge, add it to the path
        else if(rf>0){
            path.push_back(i);
            visited[i] = true;
            u = i;
            i = 0;
            if(u==sink) return path;
        }
    }
    path.clear();
    return path; //return an empty path when no path from source to sink is found
}

int main(int argc, char *argv[]) {
    string input = "input.txt";
    string output = "output.txt";
    if(argc!=3){
        cout << "Takes 2 args: inputfile, outputfile" << endl;
    }
    else{
        input = argv[1];
        output = argv[2];
    }
    ifstream infile(input);
    vector<vector<int>> capacity;
    vector<vector<int>> flow;
    vector<int> q;
    vector<bool> visited;
    int node1, node2, w = 0;
    int source, sink = 0;

    //input (note: assumed that first read node is the source and last read node is the sink)
    while(infile >> std::hex >> node1 >> node2 >> w){
        if(capacity.size()==0) source = node1;
        int resize = node1+1;
        if(node1<node2) resize = node2+1;
        if(resize>capacity.size()){
            capacity.resize(resize);
            flow.resize(resize);
            visited.resize(resize);
            for(int i=0; i<resize; ++i){
                capacity[i].resize(resize);
                flow[i].resize(resize);
            }
        }
        capacity[node1][node2] = w;
        flow[node1][node2] = w;
    }
    infile.close();
    sink = node2;

    //actual algorithm here
    int maxflow, pathflow, temp = 0;
    maxflow = 0;

    //call to dfs to find the first augmented path
    vector<int> path = dfs(flow,capacity,source,sink);

    //while(can find an augmented path)
    while(path.size()>1){
        pathflow = flow[path[0]][path[1]];
        if(pathflow==0) pathflow = capacity[path[1]][path[0]] - flow[path[1]][path[0]]; //detects reversed edge

        for(int i=2; i<path.size(); i++){
            temp = flow[path[i-1]][path[i]];
            if(temp==0) temp = capacity[path[i]][path[i-1]] - flow[path[i]][path[i-1]]; //detects reversed edge
            if(temp<pathflow) pathflow = temp;
        }
        //^^finding pathflow, the max flow across that path

        for(int i=1; i<path.size(); i++){
            if(flow[path[i-1]][path[i]] > 0) flow[path[i-1]][path[i]] -= pathflow;
            else flow[path[i]][path[i-1]] += pathflow; //detects reversed edge
        }
        //^^updates flow[][] accordingly

        maxflow+=pathflow;
        path = dfs(flow,capacity,source,sink);
    }

    //output
    ofstream outfile(output);
    outfile << "Maximum flow: " << maxflow << endl;
    outfile.close();
}