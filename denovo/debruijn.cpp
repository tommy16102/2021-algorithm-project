#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <random>
#define NUM 10000
using namespace std;

char atgc[4] = { 'A','T','G','C' };
int num_of_item = 0;
random_device rd;
mt19937 randmt(rd());
uniform_int_distribution<int> ref_dis(0, 3);
uniform_int_distribution<int> seq_dis(0, NUM - 50);

void init_dna() {
    FILE* f = fopen("DNAsequence.txt", "w");
    for (int j = 0; j < NUM; j++) {
        fprintf(f, "%c", atgc[ref_dis(randmt)]);
    }
    fclose(f);
}

void init_read(int l, int n) {   //길이 l, 개수 n
    FILE* f_dna = fopen("DNAsequence.txt", "r");
    char* reference = new char[NUM];

    for (int i = 0; i < NUM; i++) {                 //레퍼런스 파일을 읽어오기
        char tmp = fgetc(f_dna);
        reference[i] = tmp;
    }
    fclose(f_dna);

    FILE* f_short = fopen("ShortRead.txt", "w");
    int dir = 0;
    char* short_read = new char[l];         //n개의 shortRead 생성
    for (int i = 0; i < n; i++) {
        dir = seq_dis(randmt);
        for (int j = 0; j < l; j++) {
            short_read[j] = reference[dir + j];
            fprintf(f_short, "%c", short_read[j]);
        }fprintf(f_short, "\n");
    }
    fclose(f_short);
}

class Debruijn {
public:
    class Node;
    vector<int> graph[NUM];
    vector<Node> nodes;
    map<string, int> node_map;
    int start;

    class Node {
    public:
        int id;
        string data;
        int incoming;
        int outgoing;
        bool marked;

        Node() {
            id = num_of_item++;
            incoming = 0;
            outgoing = 0;
            marked = false;
        }
        void set_data(string st) {
            data = st;
        }
    };

    Debruijn() {
        start = 0;
    }

    void dfs() {
        FILE* f = fopen("result.txt", "w");
        node_map.clear();
        dfs(f, start);
    }

    void dfs(FILE* f, int x) {
        vector<int> stack;
        stack.push_back(x);
        nodes[x].marked = true;
        while (!stack.empty()) {
            int k = stack.back();
            stack.pop_back();
            size_t size = graph[k].size();
            for (int i = 0; i < size; i++) {
                int p = graph[k].back();
                if (nodes[p].marked == false) {
                    nodes[p].marked = true;
                    stack.push_back(p);
                }
            }
            fprintf(f, "%s\n", nodes[k].data.c_str());
        }
    }


    int new_node(string st) {
        // int idx = find_idx(st);
        int idx;
        auto iter = node_map.find(st);
        if (iter != node_map.end()) {
            idx = iter->second;
        }
        else {
            idx = -1;
        }

        if (idx != -1) {
            // cout<<st<<" is in graph\n";
            return idx;
        }
        else {
            Node* new_node = new Node();
            new_node->set_data(st);
            nodes.push_back(*new_node);
            node_map.insert({ st,num_of_item - 1 });
            return num_of_item - 1;
        }
    }

    int find_idx(string st) {
        for (int i = 0; i < nodes.size(); i++) {
            if (nodes[i].data == st) return i;
        }
        return -1;
    }

    void update_graph(string st, int l) {
        string sub1 = st.substr(0, l - 1);
        string sub2 = st.substr(1, l - 1);
        int idx1 = new_node(sub1);
        int idx2 = new_node(sub2);
        nodes[idx1].outgoing++;
        nodes[idx2].incoming++;

        graph[idx1].push_back(idx2);
    }

    void update_start() {
        int tmp = 0;
        int st = 0;
        for (int i = 0; i < nodes.size(); i++) {
            if (nodes[i].incoming - nodes[i].outgoing < tmp) {
                tmp = nodes[i].incoming - nodes[i].outgoing;
                st = i;
            }
        }
        start = st;
    }
};

int main() {
    int l = 50, n = 1000000;
    init_dna();
    init_read(l, n); //l과 n의 값 변경해서 사용

    cout << "리드 생성 완료\n";

    Debruijn* d = new Debruijn;

    cout << "그래프 그리는중...";
    FILE* fp = fopen("ShortRead.txt", "r");
    char line[NUM];
    int u, i = 0;
    while (fgets(line, sizeof(line), fp) != NULL) {
        line[l] = '\0';
        string tmp = line;
        d->update_graph(tmp, l);
    }
    cout << "그래프 그리기 완료\n";
    cout << "업데이트 시작" << endl;
    // for(int i=0;i<d.nodes.size();i++){
    //     cout<<d.nodes[i].data<<" , "<<i<<", in : "<<d.nodes[i].incoming<<", out : "<<d.nodes[i].outgoing<<"\n";
    // }
    d->update_start();
    cout << "업데이트 끝" << endl;
    cout << "start Node data : " << d->nodes[d->start].data;

    d->dfs();
}