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
\*-------------------------------------------------------------------------------*/
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
\*-----------------------------------------------------------------------------*/
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
\*-------------------------------------------------------------------------*/
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
\*--------------------------------------------------------------------------*/
void insert(FILE *fp, Student *ary, int *cnt)	
{
	char studentInfo[MAX];	// Character array which is stored new data

	printf("\nInput New Infor : ");
	// If new data that inputed is perfect, it stored in the .txt file which is opened and count of stored data is counted up.  
	if (scanf("%s %s %s %s", ary[*cnt].stuNum, ary[*cnt].name, ary[*cnt].year, ary[*cnt].grade) == 4 && getchar() == '\n') {
		fprintf(fp, "%s %s %s %s\n", ary[*cnt].stuNum, ary[*cnt].name, ary[*cnt].year, ary[*cnt].grade);
		(*cnt)++;
	}
	// If new data isn't perfect, the program print error and erase buffer of input.
	else {
		printf("\nWrong Input !\n");
		myflush();
	}
	
	return;
}

/*--------------------------<<<search �Լ�>>>--------------------------*\
|	���� ���� : �����Ͱ� ����Ǿ� �ִ� ����ü �迭, ����� ������ ����
|	��� :	 1.ã���� �ϴ� �й�, �̸�, �г�, ���� �� �� 1 �Է� �ޱ�
|			 2.�ش��ϴ� �й�, �̸�, �г�, ������ ���� ������ ���
\*---------------------------------------------------------------------------*/
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
\*----------------------------------------------------------------------------*/
void update(FILE* fp,Student *ary, int *cnt)
{
	char updateNum[10];		// �����ϰ��� �ϴ� �й� �Է¹��� ���ڿ�
	char change[20];			// �����Ϸ��� ���� �Է¹��� ���ڿ�
	int choice;						// �޴� ���� ���� ����
	int index;						// ����ü �迭 ������ �����ϰ��� �ϴ� �����Ͱ� �����ϴ� ��ġ�� �ε����� ������ ����
	int i;	

	printf("\nInput the Student Number to Update : ");
	scanf("%s", updateNum);
	if ((index = checkLoca(ary, cnt, updateNum)) == -1) {		// checkLoca �Լ��� ��ȯ���� -1�� ��� �Է¹��� �й��� ���� ������ ������ ���
		printf("\n<No Information about the SN>\n");
		for (i = 0; i < *cnt; i++)		// fopen�� �ɼ��� w+�� �����Ͽ��⶧����, ������ ������ ���� ��� ������ �������Ƿ�, ����ü �迭�� ����� �����͸� ���Ͽ� ���ۼ�
			fprintf(fp, "%s %s %s %s\n", ary[i].stuNum, ary[i].name, ary[i].year, ary[i].grade);
		return;
	}

	printf("\n1.NO\n2.Name\n3.Year\n4.Grade\n");
	
	printf("SELECT : ");
	while (!scanf("%d",&choice))	// ���� �Է½� �ݺ���
	{
		printf("Wrong Selection !\n");
		myflush();
		printf("SELECT : ");
	}

	switch (choice)	// �й�, �̸�, �г�, ���� �� �ش��ϴ� �κ��� ����
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
	
	for (i = 0; i < *cnt; i++)		// ���Ͽ� �����
		fprintf(fp, "%s %s %s %s\n", ary[i].stuNum, ary[i].name, ary[i].year, ary[i].grade);

	return;
}

/*----------------------------<<<del �Լ�>>>----------------------------*\
|	���� ���� : �����͸� ������ ���ϸ�, ����ü �迭, ����� ������ ����
|	��� :	 1.�����ϰ��� �ϴ� �й� �Է� �ޱ�
|			 2.�Է� ���� �й��� ���� ������ ����
\*---------------------------------------------------------------------------*/
void del(FILE *fp, Student *ary, int *cnt)
{
	char del[10];	// �����ϰ��� �ϴ� �й� �Է� ���� ���ڿ�
	int index;		// ����ü ������ �����ϰ��� �ϴ� �����Ͱ� ��ġ�ϴ� �ε��� ���� ������ ����
	int i;

	printf("\nInput the Student Number to Delete : ");
	scanf("%s", del);
	if ((index = checkLoca(ary, cnt, del)) == -1) {			// checkLoca �Լ��� ��ȯ���� -1�� ��� �Է¹��� �й��� ���� ������ ������ ���
		printf("\n<No Information of the SN>\n");
		for (i = 0; i < *cnt; i++)		// fopen�� �ɼ��� w+�� �����Ͽ��⶧����, ������ ������ ���� ��� ������ �������Ƿ�, ����ü �迭�� ����� �����͸� ���Ͽ� ���ۼ�
			fprintf(fp, "%s %s %s %s\n", ary[i].stuNum, ary[i].name, ary[i].year, ary[i].grade);
		return;
	}

	for (i = index; i < *cnt; i++)
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
\*-------------------------------------------------------------------------------*/
int checkLoca(Student *ary,int *cnt,char *str)
{
	int i;
	for (i = 0; i < *cnt; i++)		// ����ü �迭�� ���� ã���� �ϴ� �����Ͱ� �����ϴ� ��ġ Ž��
		if (!strcmp(str, ary[i].stuNum))
			return i;

	return -1;	// ã���� �ϴ� �����Ͱ� �������� ������ -1 ��ȯ
}

/*--------------------------<<<print �Լ�>>>--------------------------*\
|	���� ���� : �����Ͱ� ����Ǿ� �ִ� ����ü �迭, ����� ������ ����
|	��� :	 1.�ؽ�Ʈ ������ �����͵��� ����Ǿ� �ִ� ����ü �迭�� ����
|				�ؽ�Ʈ ������ ������ ���
\*-------------------------------------------------------------------------*/
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
\*-----------------------------------------------------------------------------*/
void myflush()
{
	while (getchar() != '\n') { ; }
}