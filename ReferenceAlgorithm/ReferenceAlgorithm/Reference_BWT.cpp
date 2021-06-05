// BWT �˰����� �̿��� Reference Algorithm

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
	// A,T,G,C�� ��
	int counter[4] = { 0 };

	BWT_Mapping()
	{}
};
class Node
{
public:

	// Node�� ���ڿ�
	string genome = "";
	// Node�� index
	int j = 0;
	// overlap ����
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

// Reference.txt �����ϴ� �Լ�
void Make_Reference()
{
	cout << "Original.txt�� Reference.txt�� �о�ɴϴ�." << endl;

	// Original.txt�� reference.txt�� �о���� 
	ifstream O("Original.txt");
	ofstream R("Reference.txt");
	R << O.rdbuf();
	O.close();
	R.close();


	cout << "Reference.txt�� �����Ͽ� �����մϴ�." << endl;
	ifstream ChangeR("Reference.txt");
	char referenceArray[N + 1];

	ChangeR.getline(referenceArray, N + 1);

	// N * ���ڸ� �����ϸ� �� %��ŭ ����
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

	cout << "Reference.txt�� ������ �Ϸ��Ͽ����ϴ�." << endl;
}

// Short read�� ����� �Լ�
vector<string> Make_shortread(vector<BWT_Mapping>& BWT)
{
	mt19937_64 random((unsigned int)time(NULL));
	uniform_int_distribution<int> dist(0, N - L);

	vector<string> short_read;

	vector<int> S;

	unsigned int random_pointer = 0;
	cout << "Short read�� ����ϴ�." << endl;

	for (int i = 0; i < M; i++)
	{
		// reference.txt�� ����´�.
		ifstream openFile("reference.txt"); 
		//Short_read�� ������ ���ڿ�
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

		//random_pointer���� L���� ������ �д´�
		openFile.seekg(random_pointer, ios::beg);
		openFile.getline(reads, L + 1);

		short_read.push_back(reads);

		openFile.close();
	}
	cout << "Short reads�� ��������ϴ�." << endl;


	// shortread.txt�� �����.
	cout << "shortread.txt�� �����մϴ�." << endl;
	ofstream writeFile("shortread.txt");

	//short_read�� ����� ������ ������� shortread.txt�� ���
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

	cout << "shortread.txt�� �ϼ��߽��ϴ�." << endl;


	return short_read;
}

int BWT_Match(string str, vector<vector<char>> matrix, int rank[], int counter[])
{
	// back tracking �� ���� ����
	int back = -1;
	// rank�� check �ϱ� ���� ����
	int rcheck = 0;
	// count ����
	int count = 0;
	// left�� rank
	int lrank = 0;
	// right�� rank
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

	// rank �迭 �����
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

// Original Genome�� ���ϴ� �Լ�
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

	cout << "Original�� Reference�� Mismatch : "
		<< ((double)ORMismatch / N) * 100 << "%" << endl;

	cout << "Original�� Output�� Mismatch : "
		<< ((double)OOMismatch / N) * 100 << "%" << endl;
}

int main()
{
	cout << "N : " << N << endl;
	cout << "M : " << M << endl;
	cout << "L : " << L << endl << endl;

	vector<string> short_read;
	Make_Reference(); // Reference.txt�� �����Ѵ�.
	vector<BWT_Mapping> BWT;
	short_read = Make_shortread(BWT); // reference.txt�κ��� short_reads�� �����.

	new_genome += short_read[0];

	// ���α׷� �ð� ����
	clock_t end;
	clock_t start = clock();

	// Overlap �׷��� ����
	while (new_genome.size() < N)
	{
		int check = 0;
		//Graph
		vector<Node> graph;

		// ���ؾ��ϴ� ���ڿ��� �����Ѵ�.
		string compare = new_genome.substr(new_genome.size() - L, L);

		// Overlap �׷��� ����
		for (int j = 1; j < short_read.size(); j++)
		{
			// ��� short_read�� overlap�� ã�´�.
			int compare_overlap = 0;

			// compare�� short_read ������ overlap�� string�� ���� ã�´�.
			compare_overlap = BWT_Match(compare, BWT[j].BWT, BWT[j].rank, BWT[j].counter);

			if (compare_overlap != -1)
			{
				// ��带 ����� �׷����� �����Ѵ�.
				graph.push_back(Node(short_read[j], j, compare_overlap));
				short_read.erase(short_read.begin() + j);
				BWT.erase(BWT.begin() + j);
				j--;
				check++;
			}
		}

		sort(graph.begin(), graph.end());

		// ������� �ʴ� ��� ����
		remove_nodes(graph, short_read, BWT);
	}
	end = clock();


	cout << "BWT�� �̿��Ͽ� �ɸ��� �ð��� " << (float)(end - start) / CLOCKS_PER_SEC << " �� �Դϴ�." << endl << endl;

	ofstream output_file;
	output_file.open("Output.txt");

	// Output.txt ������ �����.
	for (int j = 0; j < new_genome.size(); j++)
		output_file << new_genome[j];
	output_file.close();

	// �̸� Compare�Ѵ�.
	Compare();

	return 0;
}