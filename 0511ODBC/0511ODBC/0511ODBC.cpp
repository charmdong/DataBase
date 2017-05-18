#include <stdio.h>
#include <Windows.h>
#include <sql.h>
#include <sqlext.h>
#include <string.h>
#include <stdlib.h>
#define BUFF_SIZE 30
#define MAX_QUERY 200
#pragma warning(disable : 4996)

bool DBConnect();
void DBDisconnect();
void menu();
void selectMenu();
void selectMenuSql();
void selectOutput(SQLCHAR *);
void queryOutput(SQLCHAR *);
void schemaMenu();
void showSchema(SQLCHAR *);
void insertSenario();
void updateSenario();
void deleteSenario();
int qOrs();
void getQuery(SQLCHAR *);

SQLHENV hEnv = NULL;
SQLHDBC hDbc = NULL;

int main(int argc, char *agrv[])
{
	if (DBConnect() == true) {
		printf("\n\tDatabase is connected !\n\n");
		menu();
		DBDisconnect();
	}

	return 0;
}

bool DBConnect()
{
	SQLRETURN Ret;

	if (SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv) != SQL_SUCCESS)
		return false;

	if (SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, SQL_IS_INTEGER) != SQL_SUCCESS)
		return false;

	if (SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDbc) != SQL_SUCCESS)
		return false;

	Ret = SQLConnect(hDbc, (SQLCHAR *)"DB01_Server", SQL_NTS, (SQLCHAR *)"chungdk", SQL_NTS, (SQLCHAR *)"dusalwjd2@", SQL_NTS);
	if ((Ret != SQL_SUCCESS) && (Ret != SQL_SUCCESS_WITH_INFO))
		return false;

	return true;
}

void DBDisconnect()
{
	SQLDisconnect(hDbc);// Disconnect from the SQL Server
	SQLFreeHandle(SQL_HANDLE_DBC, hDbc);// Free the Connection Handle
	SQLFreeHandle(SQL_HANDLE_ENV, hEnv);// Free the Environment Handle
}

void menu()
{
	int menuNum;
	SQLCHAR query[MAX_QUERY];
	while (1) {
		printf("\t<MENU>\n1.Show Schema\n2.SELECT\n3.INSERT\n4.UPDATE\n5.DELETE\n6.EXIT\n");
		printf("Select : ");
		scanf("%d", &menuNum);

		switch (menuNum) {
		case 1: schemaMenu(); break;
		case 2: 
			if (qOrs() == 1)
				selectMenuSql();
			else
				selectMenu(); break;
		case 3: 
			if (qOrs() == 1) {
				printf("\n1.CONSUMER (ID,DELIVERNO,SELLERNO,ADDR)\n2.MANAGER (MNGNO,NICK,WORKTIME)\n3.DELIVERER (DELNO,NAME,COMPANY,FARMNO)\n");
				printf("4.PRODUCT (TYPENO,FANO,PRICE$)\n5.FARM (FARMNO,NAME)\n6.INFO (MNO,FANO,ADDR,PDTTYPE)\nInput the SQL : ");
				getchar();
				getQuery(query);
				queryOutput(query);
			}
			else{}
			break;
		case 4: 
			if (qOrs() == 1) {
				printf("\n1.test\n2.test\n3.test\nInput the Query : ");
				getchar();
				getQuery(query);
				queryOutput(query);
			}
			else{}
			break;
		case 5: 
			if (qOrs() == 1) {
				printf("\n1.test\n2.test\n3.test\nInput the Query : ");
				getchar();
				getQuery(query);
				queryOutput(query);
			}
			else{}
			break;
		case 6: printf("\n<Exit the Database>\n\n");  break;
		}
		if (menuNum == 6)
			break;
	}
}

int qOrs()	// SQL , 시나리오 기반 선택
{
	int num;
	while (1) {
		printf("\n1.By SQL\n2.By Senario\n\n");
		printf("Select : ");
		scanf("%d", &num);
		if (num == 1 || num == 2)
			break;
		else
			printf("Wrong selection !\n");
	}
	return num;
}

void selectMenu()	// 시나리오 기반의 select
{
	int num;
	char input[25];
	static SQLCHAR query[MAX_QUERY];
	printf("-----------------------------------------------------------\n");
	printf("1.CONSUMER\n2.MANAGER\n3.DELIVERER\n4.CONSUMER AND INFO\n5.CONSUMER, DELIVERER, AND FARM\n");
	printf("-----------------------------------------------------------\n");
	printf("Select : ");
	scanf("%d", &num);
	getchar();

	switch (num) {
	case 1: printf("Input the Consumer ID : ");
		scanf("%s",input); 
		sprintf((char*)query,"SELECT * FROM CONSUMER WHERE ID = '%s'",input); 
		printf("--------------------------------------------------------\nID\t\tDELIVERNO  SELLERNO    ADDR\n");
		selectOutput(query);
			break;
	case 2: printf("Input the Manager No : ");
		scanf("%s", input);
		sprintf((char*)query, "SELECT * FROM MANAGER WHERE MNGNO = '%s'", input);
		printf("--------------------------------------------------------\nMNGNO\tNICK\t\tWORKTIME\n");
		selectOutput(query);
			break;
	case 3: break;
	case 4: break;
	case 5: break;
	case 6: break;
	case 7: break;
	}
}

void selectMenuSql()
{
	int len;
	static SQLCHAR query[MAX_QUERY];
	printf("-----------------------------------------------------------\n");
	printf("1.CONSUMER\n2.MANAGER\n3.DELIVERER\n4.CONSUMER AND INFO\n5.CONSUMER, DELIVERER, AND FARM\n");
	printf("-----------------------------------------------------------\n");
	getchar();

	printf("Input the SQL query : ");
	getQuery(query);
	selectOutput(query);
}

void selectOutput(SQLCHAR* query)
{
		SQLHSTMT hStmt;
		SQLSMALLINT colCnt = 0;
		int len;
		int i;

		if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt) == SQL_SUCCESS)
		{
			SQLExecDirect(hStmt, query, SQL_NTS);
			SQLNumResultCols(hStmt, &colCnt);

			SQLCHAR **data;
			data = (SQLCHAR **)malloc(sizeof(SQLCHAR *)*colCnt);
			for (i = 0; i < colCnt; i++)
				data[i] = (SQLCHAR *)malloc(sizeof(SQLCHAR) * BUFF_SIZE);

			for (i = 0; i < colCnt; i++)
				SQLBindCol(hStmt, i + 1, SQL_C_CHAR, data[i], BUFF_SIZE, NULL);

			while (SQLFetch(hStmt) != SQL_NO_DATA) {
				for (i = 0; i < colCnt; i++)
					printf("%s\t", data[i]);
				printf("\n");
			}
			printf("\n");
			//printf("--------------------------------------------------------\n");
			SQLCloseCursor(hStmt);
			SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
		}
}

void queryOutput(SQLCHAR* query)	// insert, update, delete
{
		SQLHSTMT hStmt;
		SQLSMALLINT colCnt = 0;
		int len;
		int i;

		if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt) == SQL_SUCCESS)
		{
			SQLExecDirect(hStmt, query, SQL_NTS);
			SQLNumResultCols(hStmt, &colCnt);

			SQLCloseCursor(hStmt);
			SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
		}
}

void schemaMenu()
{
	SQLCHAR query[MAX_QUERY];
	char value[6][20] = { "CONSUMER","MANAGER","DELIVERER","PRODUCT","FARM","INFO" };
	char attr[6][50] = { "\t\t<CONSUMER TABLE>\nID\t\tDELIVERNO\tSELLERNO\tADDR\n","\t\t<FROM MANAGER>\nMNGNO\tNICK\t\tWORKTIME\n","\t\t<DELIVERER TABLE>\nDELNO\tNAME\t\t\tCOMPANY\t\tFARMNO\n",
		"\t\t<PRODUCT TABLE>\nTYPENO\tFANO\tPRICE$\n","\t\t<FARM TABLE>\nFARMNO\tNAME\n","\t\t<INFO TABLE>\nMNO\tFANO\tADDR\t\t\t\tPDTNO\n" };
	int i;

	printf("-----------------------------------------------------------\n");
	printf("\n\t\t\t<Show all Schema>\n\n");
	printf("-----------------------------------------------------------\n");

	for (i = 0; i < 6; i++) {
		sprintf((char*)query, "SELECT * FROM %s;", value[i]);
		printf("%s", attr[i]);
		showSchema(query);
	}
}

void showSchema(SQLCHAR *query)
{
		SQLHSTMT hStmt;
		SQLSMALLINT colCnt = 0;
		int len;
		int i;

		if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt) == SQL_SUCCESS)
		{
			SQLExecDirect(hStmt, query, SQL_NTS);
			SQLNumResultCols(hStmt, &colCnt);

			SQLCHAR **data;
			data = (SQLCHAR **)malloc(sizeof(SQLCHAR *)*colCnt);
			for (i = 0; i < colCnt; i++)
				data[i] = (SQLCHAR *)malloc(sizeof(SQLCHAR) * BUFF_SIZE);

			for (i = 0; i < colCnt; i++)
				SQLBindCol(hStmt, i + 1, SQL_C_CHAR, data[i], BUFF_SIZE, NULL);
			printf("-----------------------------------------------------------\n");
			while (SQLFetch(hStmt) != SQL_NO_DATA) {
				for (i = 0; i < colCnt; i++)
					printf("%s\t", data[i]);
				printf("\n");
			}
			printf("-----------------------------------------------------------\n\n");

			SQLCloseCursor(hStmt);
			SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
		}
}

void insertSenario()
{
	int choice;
	char tupleStr[6][BUFF_SIZE];	// 삽입할 튜플의 값들
	int tupleInt[3];
	SQLCHAR query[MAX_QUERY];

	printf("1.a\n2.b\n3.c\n4.d\n5.e\nSelect : ");
	scanf("%d", &choice);
	getchar();

	switch (choice) {
	case 1:
		scanf("%s", tupleStr[0]);
		sprintf((char*)query, "INSERT INTO %s VALUES ();", tupleStr[0]);
		queryOutput(query);
		break;
	case 2:break;
	case 3:break;
	case 4:break;
	case 5:break;
	}
}

void deleteSenario()
{
	int choice;
	printf("1.a\n2.b\n3.c\n4.d\n5.e\nSelect : ");
	scanf("%d", &choice);
	getchar();

	switch (choice) {
	case 1:break;
	case 2:break;
	case 3:break;
	case 4:break;
	case 5:break;
	}
}

void updateSenario()
{
	int choice;
	printf("1.a\n2.b\n3.c\n4.d\n5.e\nSelect : ");
	scanf("%d", &choice);

	switch (choice) {
	case 1:break;
	case 2: break;
	case 3:break;
	case 4:break;
	case 5: break;
	}
}

void getQuery(SQLCHAR *query)
{
	int len;
	fgets((char*)query, MAX_QUERY + 1, stdin);
	len = strlen((char*)query);
	query[len - 1] = '\0';
}