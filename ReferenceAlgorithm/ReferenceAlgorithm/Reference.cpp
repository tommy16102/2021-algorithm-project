// BWT 알고리즘을 이용한 Reference Algorithm

#include<iostream>
#include<string>
#include<fstream>
#include<vector>
#include<ctime>
#include<random>
#include<algorithm>

#define N 1000
#define M 700
#define L 70

using namespace std;

void BWT_mapping(string str, vector<vector<char>>& BWT, int rank[], int counter[]);

class BWT_Mapping
{
public:
	// BWT matrix
	vector<vector<char>> BWT;
	// BWT rank
	int rank[L + 1] = { 0 };
	// A,T,G,C의 수
	int counter[4] = { 0 };

	BWT_Mapping()
	{}
};
class Node
{
public:

	//Node의 문자열
	string genome = "";
	//Node의 index
	int j = 0;
	//Node's overlap
	int overlap = 0;

	Node *next = nullptr;

	Node(string genome, int j, int overlap)
		:genome(genome), j(j), overlap(overlap)
	{}

	void set_nest(Node * next)
	{
		this->next = next;
	}

	bool operator < (const Node& node1)
	{
		return this->overlap < node1.overlap;
	}
};

string new_genome;

// Reference.txt 생성하는 함수
void Make_Reference()
{
	cout << "Original.txt를 Reference.txt로 읽어옵니다." << endl;

	// Original.txt를 reference.txt로 읽어오기 
	ifstream O("Original.txt");
	ofstream R("Reference.txt");
	R << O.rdbuf();
	O.close();
	R.close();


	cout << "Reference.txt를 수정하여 생성합니다." << endl;
	ifstream ChangeR("Reference.txt");
	char referenceArray[N + 1];

	ChangeR.getline(referenceArray, N + 1);

	// N * 숫자를 변경하면 그 %만큼 변경
	for (int i = 0; i < N * 0.02; i++)
	{
		int pointer = (((long)rand() << 15 | rand())) % N;
		if (referenceArray[pointer] == 'A')
			referenceArray[pointer] = 'T';
		else if (referenceArray[pointer] == 'T')
			referenceArray[pointer] = 'A';
		else if (referenceArray[pointer] == 'C')
			referenceArray[pointer] = 'G';
		else if (referenceArray[pointer] == 'G')
			referenceArray[pointer] = 'A';
	}

	ChangeR.close();

	ofstream ChangeRefer("Reference.txt");

	for (int i = 0; i < N; i++)
		ChangeRefer << referenceArray[i];

	ChangeRefer.close();

	cout << "Reference.txt의 수정을 완료하였습니다." << endl << endl << endl;
}

int main()
{
	cout << "N :" << N << endl<<endl;
	Make_Reference(); // Reference.txt 생성
}