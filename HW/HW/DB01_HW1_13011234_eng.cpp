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
}Student;	// ����ü ����

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
	Student ary[MAX_DATA_SIZE];	// �ؽ�Ʈ ������ ������ ������ ����ü �迭
	char file[MAX];							// ����� �ؽ�Ʈ ���ϸ�
	int cnt;										// ����� �������� ����
	viewMenu(file,ary,&cnt);
	return 0;
}
/*--------------------------<<<viewMenu �Լ�>>>--------------------------*\
|	���� ���� : ������ �ϴ� ���ϸ�, ����ü �迭, ����� �������� ����
|	��� :	 1.��ü���� ���α׷��� �޴��� ���
|			 2.�޴� ��ȣ �Է� �ޱ�
|			 3.�޴� ��ȣ�� �ش��ϴ� �Լ� ����
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

/*--------------------------<<<inputFile �Լ�>>>--------------------------*\
|	���� ���� : ������ �ϴ� ���ϸ��� ������ ���ڿ�
|	��� :	 1.������ �ϴ� ���ϸ�(Ȯ���� ����) �Է� �ޱ�
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

/*--------------------------<<<load �Լ�>>>--------------------------*\
|	���� ���� :	�����͸� �޾ƿ� ���ϸ�, ������ ������ ����ü �迭, 
|					����� ������ ����
|	��� :	 1.�ؽ�Ʈ ���� ���� �����͸� ����ü �迭�� ����
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

/*--------------------------<<<insert �Լ�>>>--------------------------*\
|	���� ���� : �����͸� �߰��� ���ϸ�, ����ü �迭, ����� ������ ����
|	��� :	 1.���ο� ���� �Է� �ޱ�
|			 2.���ϰ� ����ü �迭�� ��� ����
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

/*--------------------------<<<search �Լ�>>>--------------------------*\
|	���� ���� : �����Ͱ� ����Ǿ� �ִ� ����ü �迭, ����� ������ ����
|	��� :	 1.ã���� �ϴ� �й�, �̸�, �г�, ���� �� �� 1 �Է� �ޱ�
|			 2.�ش��ϴ� �й�, �̸�, �г�, ������ ���� ������ ���
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

/*--------------------------<<<update �Լ�>>>--------------------------*\
|	���� ���� : �����͸� ������ ���ϸ�, ����ü �迭, ����� ������ ����
|	��� :	 1.�����ϰ��� �ϴ� �й� �Է� �ޱ�
|			 2.�Է� ���� �й��� �����ϰ��� �ϴ� �κп� ���� ���� �Է� �ޱ�
|			 3.�Է� ���� �޴� ��ȣ�� �ش��ϴ� �κ� ����
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

/*----------------------------<<<del �Լ�>>>----------------------------*\
|	���� ���� : �����͸� ������ ���ϸ�, ����ü �迭, ����� ������ ����
|	��� :	 1.�����ϰ��� �ϴ� �й� �Է� �ޱ�
|			 2.�Է� ���� �й��� ���� ������ ����
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

/*--------------------------<<<checkLoca �Լ�>>>--------------------------*\
|	���� ���� : �����Ͱ� ����Ǿ� �ִ� ����ü �迭, ����ü �迭 ��� Ƚ��,
|					ã���� �ϴ� ������ ���ڿ�
|	��� :	 1.ã���� �ϴ� �������� ��ġ �ľ�
|	��ȯ �� : ã���� �ϴ� �����Ͱ� �����ϴ� ����ü �迭�� �ε��� ��
\*------------------------------------------------------------------------*/
int checkLoca(Student *ary,int *cnt,char *str)
{
	int i;
	for (i = 0; i < *cnt; i++)		// Search the location of data to find in structured array 
		if (!strcmp(str, ary[i].stuNum))
			return i;

	return -1;	// If there are no data to find, it will return -1 value
}

/*--------------------------<<<print �Լ�>>>--------------------------*\
|	���� ���� : �����Ͱ� ����Ǿ� �ִ� ����ü �迭, ����� ������ ����
|	��� :	 1.�ؽ�Ʈ ������ �����͵��� ����Ǿ� �ִ� ����ü �迭�� ����
|				�ؽ�Ʈ ������ ������ ���
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

/*--------------------------<<<myflush �Լ�>>>--------------------------*\
|	���� ���� : X
|	��� :	 1.�Է� ���۸� ����ִ� �Լ�
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

	for (i = 0; i < strlen(year); i++) {	// �ҹ��ڷ� �����
		if (year[i] >= 'A' && year[i] <= 'Z')
			year[i] += 32;
	}

	if (strcmp(year, "freshman") != 0 && strcmp(year, "sophomore") != 0 && strcmp(year, "junior") != 0 && strcmp(year, "senior") != 0)
		res = 0;

	year[0] -= 32;	// ù�ڸ��� �빮�ڷ� �����

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