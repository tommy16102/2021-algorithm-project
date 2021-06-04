// BWT 알고리즘을 이용한 Reference Algorithm

#include<iostream>
#include<string>
#include<fstream>
#include<vector>
#include<ctime>
#include<random>
#include<algorithm>

#define N 1000
#define M 500
#define L 5

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

// Short read를 만드는 함수
vector<string> Make_shortread(vector<BWT_Mapping>& BWT)
{
	mt19937_64 random((unsigned int)time(NULL));
	uniform_int_distribution<int> dist(0, N - L);

	vector<string> short_read;

	vector<int> S;

	unsigned int random_pointer = 0;
	cout << "Short read를 만듭니다." << endl;

	for (int i = 0; i < M; i++)
	{
		// reference.txt를 열어온다.
		ifstream openFile("reference.txt");
		char reads[L + 1]; //Short_read를 저장할 문자열

	back:
		random_pointer = dist(random);

		vector<int>::iterator itr;

		if (i == 0)
			random_pointer = 0;
		if (i == M - 1)
			random_pointer = N - L;

		itr = find(S.begin(), S.end(), random_pointer);
		if (itr != S.end())
			goto back;

		S.push_back(random_pointer);

		//random_pointer부터 L개의 정보를 읽는다
		openFile.seekg(random_pointer, ios::beg);
		openFile.getline(reads, L + 1);

		short_read.push_back(reads);

		openFile.close();
	}
	cout << "Short reads를 만들었습니다." << endl << endl << endl;


	// shortread.txt를 만든다.
	cout << "shortread.txt를 생성합니다." << endl;
	ofstream writeFile("shortread.txt");

	//short_read에 저장된 내용을 순서대로 shortread.txt에 기록
	for (unsigned int i = 0; i < short_read.size(); i++)
	{
		BWT_Mapping Short_Read;
		BWT.push_back(Short_Read);

		if (short_read[i].empty() == false)
		{
			writeFile << short_read[i] << endl;
			BWT_mapping(short_read[i], BWT[i].BWT, BWT[i].rank, BWT[i].counter);
		}
		else
			cout << "failed" << endl;
	}

	writeFile.close();

	cout << "shortread.txt를 완성했습니다." << endl << endl << endl;


	return short_read;
}

int main()
{
	cout << "N :" << N << endl<<endl;

	vector<string> short_read;
	Make_Reference(); // Reference.txt를 생성한다.
	vector<BWT_Mapping> BWT;
	short_read = Make_shortread(BWT); // reference.txt로부터 short_reads를 만든다.
}