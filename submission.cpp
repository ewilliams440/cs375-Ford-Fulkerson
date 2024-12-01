#include<iostream>
#include<fstream>
#include<string>
#include<vector>
using namespace std;

vector<int> dfs(vector<vector<int>> flow, int source, int sink){
    vector<int> path = {source};
    vector<int> visited(flow.size());
    visited[source] = true;
    bool success = false;
    int i = 0;
    int u = source;
    while(u!=sink){
        //if node i is unavailable, continue
        if(visited[i]) i+=1;

        //if found an available forward edge, add it to the path
        else if(flow[u][i]>0){
            path.push_back(i);
            visited[i] = true;
            u = i;
            i = 0;
            if(u==sink) return path;
        }

        //if there are no available forward edges, search for reversed edge
        else if(i>=flow.size()){
            int j = 0;
            while(j<flow.size()){
                //if node j is unavailable, continue
                if(visited[j]) j+=1;
                
                //if found a reversed edge, add it to the path
                else if(flow[j][u]>0){
                    path.push_back(j);
                    visited[j] = true;
                    u = j;
                    i = 0;
                    if(u==sink) return path;
                    break;
                }
                else j+=1;
            }
            //if there are no available forward or reversed edges, try a different path
            if(j>=flow.size()){
                path.pop_back();
                if(path.empty()) return path;
                u = path[path.size()];
            }
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
        if(capacity.size()==0) source = node1-1;
        int resize = node1;
        if(node1<node2) resize = node2;
        if(resize>capacity.size()){
            capacity.resize(resize);
            flow.resize(resize);
            visited.resize(resize);
            for(int i=0; i<resize; ++i){
                capacity[i].resize(resize);
                flow[i].resize(resize);
            }
        }
        capacity[node1-1][node2-1] = w;
        flow[node1-1][node2-1] = w;
    }
    infile.close();
    sink = node2-1;

    //actual algorithm here
    int maxflow, pathflow, temp = 0;
    vector<int> path = dfs(flow,source,sink);
    while(path.size()>1){
        pathflow = flow[path[0]][path[1]];
        for(int i=2; i<path.size(); i++){
            temp = flow[path[i-1]][path[i]];
            if(temp<pathflow) pathflow = temp;
        }
        for(int i=1; i<path.size(); i++){
            flow[path[i-1]][path[i]] -= pathflow;
        }
        maxflow+=pathflow;
        path = dfs(flow,source,sink);
    }

    //output
    ofstream outfile(output);
    outfile << "Maximum flow: " << maxflow << endl;
    outfile.close();
}