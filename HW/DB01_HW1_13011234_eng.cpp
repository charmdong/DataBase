#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 100
#define MAX_DATA_SIZE 1000

typedef struct {
	char stuNum[10];
	char name[20];
	char year[10];
	char grade[3];
}Student;	// 구조체 정의

void viewMenu(char *,Student *,int *);
void inputFile(char *);
void insert(FILE *, Student *, int *);
void search(Student *, int *);
void update(FILE *, Student *, int *);
void del(FILE *, Student *, int *);
int checkLoca(Student *, int *, char *);
void load(FILE *, Student *, int *);
void print(Student *,int *);
void myflush();
int checkID(Student *,int *, char *);
int checkName(char *);
int checkYear(char *);
int checkGrade(char *);
void sortAry(Student *, int *);

int main()
{
	Student ary[MAX_DATA_SIZE];	// 텍스트 파일의 내용을 저장할 구조체 배열
	char file[MAX];							// 사용할 텍스트 파일명
	int cnt;										// 저장된 데이터의 갯수
	viewMenu(file,ary,&cnt);
	return 0;
}
/*--------------------------<<<viewMenu 함수>>>--------------------------*\
|	전달 인자 : 열고자 하는 파일명, 구조체 배열, 저장된 데이터의 갯수
|	기능 :	 1.전체적인 프로그램의 메뉴를 출력
|			 2.메뉴 번호 입력 받기
|			 3.메뉴 번호에 해당하는 함수 실행
\*-----------------------------------------------------------------------*/
void viewMenu(char *file, Student *ary,int *cnt)
{
	FILE *fp;	
	int choice;		// To select menu
	while (1)
	{
		printf("------------------MENU------------------\n");
		printf("1.LOAD\n");
		printf("2.INSERT\n");
		printf("3.SEARCH\n");
		printf("4.UPDATE\n");
		printf("5.DELETE\n");
		printf("6.EXIT\n");
		printf("SELECT : ");
		
		if (!scanf("%d", &choice))	// when user inputs character
		{
			printf("Input only Num!\n\n");
			myflush();
		}
		else	// when user inputs integer
		{
			switch (choice)
			{
			case 1:	inputFile(file);
						fp = fopen(file, "rt");
						if (fp == NULL)
							printf("\nFile open Error!\n\n");
						else {
							load(fp, ary, cnt);
							print(ary, cnt);
							//sortAry(ary, cnt);
							//print(ary, cnt);
							fclose(fp);
						}
						break;
			case 2:	fp = fopen(file, "a+");
						if (fp == NULL)
							printf("\nFile open Error!\n\n");
						else {
							insert(fp, ary, cnt);
							print(ary, cnt);
							fclose(fp);
						}
						break;
			case 3:	search(ary,cnt);
						break;
			case 4:	fp = fopen(file, "w+");
						if (fp == NULL)
							printf("\nFile open Error!\n\n");
						else {
							update(fp, ary, cnt);
							print(ary,cnt);
							fclose(fp);
						}
						break;
			case 5:	fp = fopen(file, "w+");
						if (fp == NULL)
							printf("\nFile open Error!\n\n");
						else {
							del(fp, ary, cnt);
							print(ary, cnt);
							fclose(fp);
						}
						break;
			case 6:	printf("\nExit the Program\n\n");
						break;
			default:	printf("\nWrong Selection\n\n");
						break;
			}
		}
		if (choice == 6)
			break;
	}
	return;
}

/*--------------------------<<<inputFile 함수>>>--------------------------*\
|	전달 인자 : 열고자 하는 파일명을 저장할 문자열
|	기능 :	 1.열고자 하는 파일명(확장자 포함) 입력 받기
\*------------------------------------------------------------------------*/
void inputFile(char* fname)	
{
	printf("\nSelcet File\n");
	printf("  >>  ");
	
	while (1) {		// Loop during user input correct file name
		scanf("%s", fname);
		if (strcmp(fname, "Student.txt") == 0)
			break;
		else
			printf("  >>  ");
	}
	return;
}

/*--------------------------<<<load 함수>>>--------------------------*\
|	전달 인자 :	데이터를 받아올 파일명, 정보를 저장할 구조체 배열, 
|					저장된 데이터 갯수
|	기능 :	 1.텍스트 파일 내의 데이터를 구조체 배열에 저장
\*-------------------------------------------------------------------*/
void load(FILE *fp, Student *ary, int *cnt)
{
	*cnt = 0;	// reset the count of stored data to Zero
	while (!feof(fp))	// loop until enf of file
	{
		// when the data isn't perfect, the program prints error and break the loop
		if (fscanf(fp, "%s %s %s %s ", ary[*cnt].stuNum, ary[*cnt].name, ary[*cnt].year, ary[*cnt].grade) != 4){
			printf("\nData Error!\n");
			break;
		}
		(*cnt)++;
	}

	return;
}

/*--------------------------<<<insert 함수>>>--------------------------*\
|	전달 인자 : 데이터를 추가할 파일명, 구조체 배열, 저장된 데이터 갯수
|	기능 :	 1.새로운 정보 입력 받기
|			 2.파일과 구조체 배열에 모두 저장
\*---------------------------------------------------------------------*/
void insert(FILE *fp, Student *ary, int *cnt)	
{
	char studentInfo[MAX];	// Character array which is stored new data
	int checkId;

	printf("Input Student ID : ");
	scanf("%s", ary[*cnt].stuNum);
	checkId = checkID(ary, cnt, ary[*cnt].stuNum);
	
	if (checkId ==0) {
		printf("\nWrong Input !\n");
		myflush();
		return;
	}
	else if (checkId == -1) {
		printf("\nIt already exist !\n");
		myflush();
		return;
	}

	printf("Input Student Name : ");
	scanf("%s", ary[*cnt].name);
	if (!checkName(ary[*cnt].name)) {
		printf("\nWrong Input !\n");
		myflush();
		return;
	}

	printf("Input Student Year : ");
	scanf("%s", ary[*cnt].year);
	if (!checkYear(ary[*cnt].year)) {
		printf("\nWrong Input !\n");
		myflush();
		return;
	}

	printf("Input Student Grade : ");
	scanf("%s", ary[*cnt].grade);
	if (!checkGrade(ary[*cnt].grade)) {
		printf("\nWrong Input !\n");
		myflush();
		return;
	}

	fprintf(fp, "%s %s %s %s\n", ary[*cnt].stuNum, ary[*cnt].name, ary[*cnt].year, ary[*cnt].grade);
	(*cnt)++;
	return;
}

/*--------------------------<<<search 함수>>>--------------------------*\
|	전달 인자 : 데이터가 저장되어 있는 구조체 배열, 저장된 데이터 갯수
|	기능 :	 1.찾고자 하는 학번, 이름, 학년, 학점 중 택 1 입력 받기
|			 2.해당하는 학번, 이름, 학년, 학점에 대한 데이터 출력
\*---------------------------------------------------------------------*/
void search(Student *ary, int *cnt)	
{
	int i;
	int checkCnt = 1;	// Variable to check the existence of data that user search for
	int choice;			// Variable to select menu
	char search[20];	// Variable of character array that stored NO or name or year or grade

	printf("\n1.No\n2.Name\n3.Year\n4.Grade\nSELECT >> ");
	while (1) {
		if (scanf("%d", &choice) && choice > 0 && choice < 5)	// If user inputs one of 1~4, break the loop
			break;
		printf("  >>  ");
	}

	printf("Input to Search : ");
	scanf("%s", search);
	printf("\n");
	
	switch (choice) {
	case 1: for (i = 0; i < *cnt; i++) {
					if (strcmp(search, ary[i].stuNum) == 0) {	// If there are same data that user searching for, the program prints information of that data.
						printf("%s %s %s %s\n", ary[i].stuNum, ary[i].name, ary[i].year, ary[i].grade);
						checkCnt = 0;	// Also, checkCnt is changed to 0.
					}
				}
				if (checkCnt)
					printf("<No Information>\n");
				break;
	case 2: for (i = 0; i < *cnt; i++) {
					if (strcmp(search, ary[i].name) == 0) {		// If there are same data that user searching for, the program prints information of that data.
						printf("%s %s %s %s\n", ary[i].stuNum, ary[i].name, ary[i].year, ary[i].grade);
						checkCnt = 0;
					}
				}
				if (checkCnt)
					printf("<No Information>\n");
				break;
	case 3: for (i = 0; i < *cnt; i++) {
					if (strcmp(search, ary[i].year) == 0) {			// If there are same data that user searching for, the program prints information of that data.
						printf("%s %s %s %s\n", ary[i].stuNum, ary[i].name, ary[i].year, ary[i].grade);
						checkCnt = 0;
					}
				}
				if (checkCnt)
					printf("<No Information>\n");
				break;
	case 4: for (i = 0; i < *cnt; i++) {
					if (strcmp(search, ary[i].grade) == 0) {		// If there are same data that user searching for, the program prints information of that data.
						printf("%s %s %s %s\n", ary[i].stuNum, ary[i].name, ary[i].year, ary[i].grade);
						checkCnt = 0;
					}
				}
				if (checkCnt)
					printf("<No Information>\n");
				break;
	}
	
	printf("\n");
	return;
}

/*--------------------------<<<update 함수>>>--------------------------*\
|	전달 인자 : 데이터를 수정할 파일명, 구조체 배열, 저장된 데이터 갯수
|	기능 :	 1.수정하고자 하는 학번 입력 받기
|			 2.입력 받은 학번의 수정하고자 하는 부분에 대한 선택 입력 받기
|			 3.입력 받은 메뉴 번호에 해당하는 부분 수정
\*---------------------------------------------------------------------*/
void update(FILE* fp,Student *ary, int *cnt)
{
	char updateNum[10];		// Variable of character string that inputed student number by user to update
	char change[20];		// Variable of character string that inputed by user to change the data
	int choice;				// Type of integer variable to select menu
	int index;				// The location of the data in structure array that user want to change 
	int i;	

	printf("\nInput the Student Number to Update : ");
	scanf("%s", updateNum);
	if ((index = checkLoca(ary, cnt, updateNum)) == -1) {		// If function of checkLoca's return value is -1, the program will print error
		printf("\n<No Information about the SN>\n");
		for (i = 0; i < *cnt; i++)		// The function of fopen's option is 'w+'. So, the program has to backup the data in .txt file
			fprintf(fp, "%s %s %s %s\n", ary[i].stuNum, ary[i].name, ary[i].year, ary[i].grade);
		return;
	}

	printf("\n1.NO\n2.Name\n3.Year\n4.Grade\n");
	
	printf("SELECT : ");
	while (!scanf("%d",&choice))	// When user inputs character variable, then the program will require the correct number
	{
		printf("Wrong Selection !\n");
		myflush();
		printf("SELECT : ");
	}

	switch (choice)
	{
	case 1:	printf("Input New NO of the student : ");
				scanf("%s", change);
				strcpy(ary[index].stuNum, change);
				break;
	case 2:	printf("Input New Name of the student : ");
				scanf("%s", change);
				strcpy(ary[index].name, change);
				break;
	case 3:	printf("Input New Year of the student : ");
				scanf("%s", change);
				strcpy(ary[index].year, change);
				break;
	case 4:	printf("Input New Grade of the student : ");
				scanf("%s", change);
				strcpy(ary[index].grade, change);
				break;
	default: printf("\nWrong Selection !\n"); break;
	}
	
	for (i = 0; i < *cnt; i++)		// Write all data after changing
		fprintf(fp, "%s %s %s %s\n", ary[i].stuNum, ary[i].name, ary[i].year, ary[i].grade);

	return;
}

/*----------------------------<<<del 함수>>>----------------------------*\
|	전달 인자 : 데이터를 삭제할 파일명, 구조체 배열, 저장된 데이터 갯수
|	기능 :	 1.삭제하고자 하는 학번 입력 받기
|			 2.입력 받은 학번에 대한 데이터 삭제
\*----------------------------------------------------------------------*/
void del(FILE *fp, Student *ary, int *cnt)
{
	char del[10];	// Variable of character array that will store the student number to delete
	int index;		// Type of integer variable to store the location of data to delete in array of structures
	int i;

	printf("\nInput the Student Number to Delete : ");
	scanf("%s", del);
	if ((index = checkLoca(ary, cnt, del)) == -1) {			// If function of checkLoca's return value is -1, the program will print error
		printf("\n<No Information of the SN>\n");
		for (i = 0; i < *cnt; i++)		// The function of fopen's option is 'w+'. So, the program has to backup the data in .txt file
			fprintf(fp, "%s %s %s %s\n", ary[i].stuNum, ary[i].name, ary[i].year, ary[i].grade);
		return;
	}

	for (i = index; i < *cnt; i++)	// If there are the data to delete in array of structures, all data's index behind the data to delete will be changed 
		ary[i] = ary[i + 1];
	(*cnt)--;

	for (i = 0; i < *cnt; i++)
		fprintf(fp, "%s %s %s %s\n", ary[i].stuNum, ary[i].name, ary[i].year, ary[i].grade);

	return;
}

/*--------------------------<<<checkLoca 함수>>>--------------------------*\
|	전달 인자 : 데이터가 저장되어 있는 구조체 배열, 구조체 배열 사용 횟수,
|					찾고자 하는 데이터 문자열
|	기능 :	 1.찾고자 하는 데이터의 위치 파악
|	반환 값 : 찾고자 하는 데이터가 존재하는 구조체 배열의 인덱스 값
\*------------------------------------------------------------------------*/
int checkLoca(Student *ary,int *cnt,char *str)
{
	int i;
	for (i = 0; i < *cnt; i++)		// Search the location of data to find in structured array 
		if (!strcmp(str, ary[i].stuNum))
			return i;

	return -1;	// If there are no data to find, it will return -1 value
}

/*--------------------------<<<print 함수>>>--------------------------*\
|	전달 인자 : 데이터가 저장되어 있는 구조체 배열, 저장된 데이터 갯수
|	기능 :	 1.텍스트 파일의 데이터들이 저장되어 있는 구조체 배열을 통해
|				텍스트 파일의 데이터 출력
\*--------------------------------------------------------------------*/
void print(Student *ary, int *cnt)
{
	int i;
	printf("\n");
	if (*cnt == 0) {
		printf("<No Informaton>\n\n");
		return;
	}
	for(i=0;i<*cnt;i++)
		printf("%s %s %s %s\n", ary[i].stuNum, ary[i].name, ary[i].year, ary[i].grade);
	printf("\n");

	return;
}

/*--------------------------<<<myflush 함수>>>--------------------------*\
|	전달 인자 : X
|	기능 :	 1.입력 버퍼를 비워주는 함수
\*----------------------------------------------------------------------*/
void myflush()
{
	while (getchar() != '\n') { ; }
}

int checkID(Student *ary,int *cnt, char *str)
{
	int i;
	int len = strlen(str);
	int res=1;

	for (i = 0; i < len; i++) {
		if (str[i]<'0' || str[i]>'9') {
			res = 0;
			break;
		}
	}

	for (i = 0; i < *cnt; i++) {
		if (!strcmp(str, ary[i].stuNum)) {
			res = -1;
			break;
		}
	}

	return res;
}

int checkName(char *name)
{
	int res = 1;
	int i;

	for (i = 0; i < strlen(name); i++) {
		if (name[i] >= '0' && name[i] <= '9')
			res = 0;
	}

	return res;
}

int checkYear(char *year)
{
	int res = 1;
	int i;

	for (i = 0; i < strlen(year); i++) {	// 소문자로 만들기
		if (year[i] >= 'A' && year[i] <= 'Z')
			year[i] += 32;
	}

	if (strcmp(year, "freshman") != 0 && strcmp(year, "sophomore") != 0 && strcmp(year, "junior") != 0 && strcmp(year, "senior") != 0)
		res = 0;

	year[0] -= 32;	// 첫자리만 대문자로 만들기

	return res;
}

int checkGrade(char *grade)
{
	int res = 1;

	if (grade[0]<'A' || grade[0]>'D' && grade[0] != 'F')
		res = 0;
	if (res==1 && grade[1] != '+' && grade[1] != '-' &&grade[1] != '\0')
		res = 0;

	return res;
}

void sortAry(Student *ary, int *cnt)
{
	int i,j;
	Student tmp;

	for (i = 0; i < *cnt - 1; i++) {
		for (j = i; j < *cnt; j++) {
			if (strcmp(ary[i].stuNum, ary[j].stuNum) > 0) {
				tmp = ary[i];
				ary[i] = ary[j];
				ary[j] = tmp;
			}
		}
	}

	return;
}