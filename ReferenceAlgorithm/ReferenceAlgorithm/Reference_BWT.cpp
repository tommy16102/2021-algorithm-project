// BWT 알고리즘을 이용한 Reference Algorithm

#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<ctime>
#include<random>
#include<algorithm>

#define N 1000
#define M 500
#define L 50
#define OL 5

using namespace std;

string new_genome;

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

	// Node의 문자열
	string genome = "";
	// Node의 index
	int j = 0;
	// overlap 변수
	int overlap = 0;

	Node *next = nullptr;

	Node(string genome, int j, int overlap)
		:genome(genome), j(j), overlap(overlap)
	{}

	void set_next(Node * next)
	{
		this->next = next;
	}
	bool operator < (const Node& node1)
	{
		return this->overlap < node1.overlap;
	}
};

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

	cout << "Reference.txt의 수정을 완료하였습니다." << endl;
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
		//Short_read를 저장할 문자열
		char reads[L + 1];

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
	cout << "Short reads를 만들었습니다." << endl;


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

	cout << "shortread.txt를 완성했습니다." << endl;


	return short_read;
}

int BWT_Match(string str, vector<vector<char>> matrix, int rank[], int counter[])
{
	// back tracking 을 위한 변수
	int back = -1;
	// rank를 check 하기 위한 변수
	int rcheck = 0;
	// count 변수
	int count = 0;
	// left의 rank
	int lrank = 0;
	// right의 rank
	int rrank = 0;
	int overlap = 0;

	while (1) {
		int num = L - 1;
		int temp = -1;
		for (int i = back + 1; i < L + 1; i++)
		{
			if (matrix[i][1] == str.at(num))
			{
				back = i;
				temp = i;
				rcheck = rank[i];
				break;
			}
		}
		if (temp == -1)
			break;
		temp = -1;

		while (num >= 0)
		{
			if (str.at(num) == 'A')
			{
				if (matrix[1 + rcheck][1] == '$')
				{
					if (overlap < L - num)
						overlap = L - num;
					break;
				}
				else if (matrix[1 + rcheck][1] == str.at(num - 1))
				{
					rcheck = rank[1 + rcheck];
					num--;
				}
				else
				{
					break;
				}
			}
			else if (str.at(num) == 'C')
			{
				if (matrix[counter[0] + rcheck + 1][1] == '$')
				{
					if (overlap < L - num)
						overlap = L - num;
					break;
				}
				else if (matrix[counter[0] + rcheck + 1][1] == str.at(num - 1))
				{
					rcheck = rank[counter[0] + 1 + rcheck];
					num--;
				}
				else
				{
					break;
				}
			}
			else if (str.at(num) == 'G')
			{
				if (matrix[counter[0] + counter[3] + rcheck + 1][1] == '$')
				{
					if (overlap < L - num)
						overlap = L - num;
					break;
				}
				else if (matrix[counter[0] + counter[3] + rcheck + 1][1] == str.at(num - 1))
				{
					rcheck = rank[counter[0] + counter[3] + rcheck + 1];
					num--;
				}
				else
				{
					break;
				}
			}
			else if (str.at(num) == 'T')
			{
				if (matrix[counter[0] + counter[2] + counter[3] + rcheck + 1][1] == '$')
				{
					if (overlap < L - num)
						overlap = L - num;
					break;
				}
				else if (matrix[counter[0] + counter[2] + counter[3] + rcheck + 1][1] == str.at(num - 1))
				{
					rcheck = rank[counter[0] + counter[2] + counter[3] + rcheck + 1];
					num--;
				}

				else
				{
					break;
				}
			}

		}
	}

	if (overlap > OL)
		return overlap;
	else
		return -1;
}

void BWT_mapping(string str, vector<vector<char>>& BWT, int rank[], int counter[])
{
	string str2 = '$' + str;
	vector<string> matrix;


	int num = L;

	matrix.push_back(str2);

	for (int i = 0; i < L; i++)
	{
		string temp = matrix[i];
		char temp2 = matrix[i][L];

		temp.pop_back();
		temp = temp2 + temp;

		matrix.push_back(temp);
	}

	sort(matrix.begin(), matrix.end());


	BWT.reserve(L + 1);
	BWT.resize(L + 1);

	for (int i = 0; i < L + 1; i++)
	{
		BWT[i].push_back(matrix[i].at(0));
		BWT[i].push_back(matrix[i].at(L));
	}

	// rank 배열 만들기
	for (int i = 0; i < L + 1; i++)
	{
		if (BWT[i][1] == 'A')
		{
			rank[i] = counter[0];
			counter[0]++;
		}
		else if (BWT[i][1] == 'T')
		{
			rank[i] = counter[1];
			counter[1]++;
		}
		else if (BWT[i][1] == 'G')
		{
			rank[i] = counter[2];
			counter[2]++;
		}
		else if (BWT[i][1] == 'C')
		{
			rank[i] = counter[3];
			counter[3]++;
		}
		else
		{
			rank[i] = -1;
		}
	}

	return;
}

void remove_nodes(vector<Node> &Graph, vector<string> &short_read, vector<BWT_Mapping> &BWT)
{
	if (Graph.size() != 0)
	{
		for (int i = Graph.size() - 1; i > 0; i--)
		{
			int num = Graph[i].overlap;

			for (int j = i - 1; j >= 0; j--)
			{
				int compare = Graph[j].overlap;

				if (Graph[i].genome.substr(num, L - num) != Graph[j].genome.substr(compare, L - num))
				{
					BWT_Mapping re;
					short_read.push_back(Graph[j].genome);
					BWT_mapping(Graph[j].genome, re.BWT, re.rank, re.counter);
					BWT.push_back(re);
					Graph.erase(Graph.begin() + j);
				}
				else
					break;
			}
		}

		new_genome += Graph[0].genome.substr(Graph[0].overlap, L - Graph[0].overlap);
	}
}

// Original Genome과 비교하는 함수
void Compare()
{
	// original.txt
	fstream ofile;
	// output.txt
	fstream outputfile;
	// reference.txt
	fstream rfile;
	ofile.open("Original.txt");
	outputfile.open("Output.txt");
	rfile.open("Reference.txt");

	int ORMismatch = 0;
	int OOMismatch = 0;

	while (!ofile.eof())
	{
		char input = ofile.get();
		char output = outputfile.get();
		char reference = rfile.get();

		if (input != output)
			ORMismatch++;

		if (input != reference)
			OOMismatch++;
	}

	ofile.close();
	outputfile.close();
	rfile.close();

	cout << "Original과 Reference의 Mismatch : "
		<< ((double)ORMismatch / N) * 100 << "%" << endl;

	cout << "Original과 Output의 Mismatch : "
		<< ((double)OOMismatch / N) * 100 << "%" << endl;
}

int main()
{
	cout << "N : " << N << endl;
	cout << "M : " << M << endl;
	cout << "L : " << L << endl << endl;

	vector<string> short_read;
	Make_Reference(); // Reference.txt를 생성한다.
	vector<BWT_Mapping> BWT;
	short_read = Make_shortread(BWT); // reference.txt로부터 short_reads를 만든다.

	new_genome += short_read[0];

	// 프로그램 시간 측정
	clock_t end;
	clock_t start = clock();

	// Overlap 그래프 생성
	while (new_genome.size() < N)
	{
		int check = 0;
		//Graph
		vector<Node> graph;

		// 비교해야하는 문자열을 정의한다.
		string compare = new_genome.substr(new_genome.size() - L, L);

		// Overlap 그래프 생성
		for (int j = 1; j < short_read.size(); j++)
		{
			// 모든 short_read의 overlap을 찾는다.
			int compare_overlap = 0;

			// compare과 short_read 사이의 overlap된 string의 수를 찾는다.
			compare_overlap = BWT_Match(compare, BWT[j].BWT, BWT[j].rank, BWT[j].counter);

			if (compare_overlap != -1)
			{
				// 노드를 만들어 그래프에 삽입한다.
				graph.push_back(Node(short_read[j], j, compare_overlap));
				short_read.erase(short_read.begin() + j);
				BWT.erase(BWT.begin() + j);
				j--;
				check++;
			}
		}

		sort(graph.begin(), graph.end());

		// 사용하지 않는 노드 제거
		remove_nodes(graph, short_read, BWT);
	}
	end = clock();


	cout << "BWT를 이용하여 걸리는 시간은 " << (float)(end - start) / CLOCKS_PER_SEC << " 초 입니다." << endl << endl;

	ofstream output_file;
	output_file.open("Output.txt");

	// Output.txt 파일을 만든다.
	for (int j = 0; j < new_genome.size(); j++)
		output_file << new_genome[j];
	output_file.close();

	// 이를 Compare한다.
	Compare();

	return 0;
}