#include <iostream>
#include <random>
#define NUM 10000
using namespace std;


char atgc[4] = { 'A','T','G','C' };
std::random_device rd;                          
std::mt19937 randmt(rd());
std::uniform_int_distribution<int> ref_dis(0, 3);
std::uniform_int_distribution<int> seq_dis(0, NUM - 50);

void init_dna(){
    FILE* f = fopen("DNAsequence.txt", "w");    
    for (int j = 0; j < NUM; j++) {
        fprintf(f, "%c", atgc[ref_dis(randmt)]);
    }
    fclose(f);
}

void init_read(int l, int n){   //길이 l, 개수 n
    FILE *f_dna = fopen("DNAsequence.txt", "r");
    char *reference = new char[NUM];

    for(int i=0;i<NUM;i++){                 //레퍼런스 파일을 읽어오기
        char tmp = fgetc(f_dna);
        reference[i] = tmp;
    }
    fclose(f_dna);

    FILE *f_short = fopen("ShortRead.txt","w");
    int dir = 0;
    char *short_read = new char[l];         //n개의 shortRead 생성
    for(int i=0;i<n;i++){
        dir = seq_dis(randmt);
        for(int j=0;j<l;j++){
            short_read[j] = reference[dir+j];     
            fprintf(f_short,"%c",short_read[j]);
        }fprintf(f_short,"\n");
    }
    fclose(f_short);
}

int main() {
    int l = 10, n = 40;
    init_dna();
    init_read(l,n); //l과 n의 값 변경해서 사용
    FILE *fp = fopen("ShortRead.txt","r");
    char line[NUM];
    int i = 1;
    while(fgets(line, sizeof(line), fp) != NULL){
        line[l] = '\0';
        printf("%s  %d\n",line,i++);
    }
}