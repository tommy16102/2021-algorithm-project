#include <iostream>
#include <vector>
#define NUM 100000
using namespace std;

class Debruijn{
public:
    class Node;
    vector<int> graph[NUM];
    vector<Node> nodes;
    int start;

    class Node{
    public:
        static int iter;
        int id;
        string data;
        int incoming;
        int outgoing;
        bool marked;

        Node(){
            id = iter++;
            incoming = 0;
            outgoing = 0;
            marked = false;
        }
        void set_data(string st){
            data = st;
        }
    };

    Debruijn(){
        start = 0;
    }

    void dfs(){
        FILE *f = fopen("result.txt", "w");
        dfs(f, start);
    }

    void dfs(FILE *f,int x){
        
    }

    void new_node(string st){
        if(find_idx(st) != -1){
            // cout<<st<<" is in graph\n";
            return;
        }
        Node *new_node = new Node();
        new_node->set_data(st);
        nodes.push_back(*new_node);
    }

    int find_idx(string st){
        for(int i=0;i<nodes.size();i++){
            if(nodes[i].data == st) return i;
        }
        return -1;
    }

    Node get_node(int it){
        for(int i=0;i<nodes.size();i++){
            if(nodes[i].id == it) return nodes[i];
        }
    }

    void update_graph(string st, int l){
        string sub1 = st.substr(0,l-1);
        string sub2 = st.substr(1,l-1);
        new_node(sub1);
        new_node(sub2);
        nodes[find_idx(sub1)].outgoing++;
        nodes[find_idx(sub2)].incoming++;

        graph[find_idx(sub1)].push_back(find_idx(sub2));
    }

    void update_start(){
        int tmp = 0;
        int st = 0;
        for(int i=0;i<nodes.size();i++){
            if(nodes[i].incoming - nodes[i].outgoing < tmp){
                tmp = nodes[i].incoming - nodes[i].outgoing;
                st = i;
            }
        }
        start = st;
    }
};

int Debruijn::Node::iter = 0;

int main(){
    Debruijn d;

    int l = 30;
    
    FILE *fp = fopen("ShortRead.txt","r");
    char line[NUM];
    int u,i = 0;

    while(fgets(line, sizeof(line), fp) != NULL){
        line[l] = '\0';
        string tmp = line;
        d.update_graph(tmp, l);
    }

    // for(int i=0;i<d.nodes.size();i++){
    //     cout<<d.nodes[i].data<<" , "<<i<<", in : "<<d.nodes[i].incoming<<", out : "<<d.nodes[i].outgoing<<"\n";
    // }
    d.update_start();
    cout<<"start Node data : "<<d.nodes[d.start].data;
}