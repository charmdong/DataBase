#include <stdio.h>
#include <Windows.h>
#include <sql.h>
#include <sqlext.h>
#include <string.h>
#include <stdlib.h>
#define BUFF_SIZE 30
#define MAX_QUERY 300
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

//---------------------------------------------------------DB ���� ���� �Լ�-----------------------------------------------------------//

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

//---------------------�⺻ �޴� �Լ�---------------------//

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
				printf("4.PRODUCT (TYPENO,FANO,PRICE$)\n5.FARM (FARMNO,NAME)\n6.INFO (MNO,FANO,ADDR,PDTTYPE)\n\nInput the SQL >> ");
				getchar();
				getQuery(query);
				queryOutput(query);
			}
			else
				insertSenario();
			break;
		case 4:
			if (qOrs() == 1) {
				printf("\n1.CONSUMER\n2.MANAGER\n3.DELIVERER\n4.\n5.\nInput the Query >> ");
				getchar();
				getQuery(query);
				queryOutput(query);
			}
			else {
				updateSenario();
			}
			break;
		case 5:
			if (qOrs() == 1) {
				printf("\n1.test\n2.test\n3.test\nInput the Query : ");
				getchar();
				getQuery(query);
				queryOutput(query);
			}
			else {}
			break;
		case 6: printf("\n<Exit the Database>\n\n");  break;
		}
		if (menuNum == 6)
			break;
	}
}

//---------------------SQL, �ó����� ��� ���� �Լ�---------------------//

int qOrs()
{
	int num;
	while (1) {
		printf("\n1.By SQL\n2.By Senario\n\n");
		printf("Select : ");
		scanf("%d", &num);
		if (num == 1 || num == 2)
			break;
		else
			printf("\n# Wrong selection ! #\n");
	}
	return num;
}

//---------------------SQL ���ǹ� parsing �Լ�---------------------//

void getQuery(SQLCHAR *query) // SQL ���ǹ� parsing �Լ�
{
	int len;
	fgets((char*)query, MAX_QUERY + 1, stdin);
	len = strlen((char*)query);
	query[len - 1] = '\0';
}

//---------------------��Ű�� ���� �Լ���---------------------//

void schemaMenu() // ��Ű�� �޴� �Լ�
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

void showSchema(SQLCHAR *query) // ��� ��Ű���� ������ִ� �Լ�
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

//--------------------------------�ó����� ����� select �޴� �Լ�--------------------------------------//

void selectMenu()
{
	int num;
	int tmpNum;
	char input[25];
	static SQLCHAR query[MAX_QUERY];
	printf("-----------------------------------------------------------\n");
	printf("1.Refer CONSUMER\n2.Refer MANAGER\n3.Refer DELIVERER\n4.Refer PRODUCT\n5.Refer FARM\n6.CONSUMER AND INFO\n7.CONSUMER, DELIVERER, AND FARM\n");
	printf("-----------------------------------------------------------\n");
	printf("Select : ");
	scanf("%d", &num);
	getchar();

	switch (num) {
	case 1: printf("Input the Consumer ID : ");
		scanf("%s", input);
		sprintf((char*)query, "SELECT * FROM CONSUMER WHERE ID = '%s'", input);
		printf("--------------------------------------------------------\nID\t\tDELIVERNO  SELLERNO    ADDR\n");
		selectOutput(query);
		break;
	case 2: printf("Input the Manager No : ");
		scanf("%s", input);
		sprintf((char*)query, "SELECT * FROM MANAGER WHERE MNGNO = '%s'", input);
		printf("--------------------------------------------------------\nMNGNO\tNICK\t\tWORKTIME\n");
		selectOutput(query);
		break;
	case 3: printf("Input the Deliverer No : ");
		scanf("%s", input);
		sprintf((char*)query, "SELECT * FROM DELIVERER WHERE DELNO = '%s'", input);
		printf("--------------------------------------------------------\nDELNO\tNAME\t\t\tCOMPANY\t\tFARMNO\n");
		selectOutput(query);
		break;
	case 4: printf("Input the Product Type No : ");
		scanf("%s", input);
		sprintf((char*)query, "SELECT * FROM PRODUCT WHERE TYPENO = '%s'", input);
		printf("--------------------------------------------------------\nTYPENO\tFANO\tPRICE$\n");
		selectOutput(query); 
		break;
	case 5: printf("Input the Farm No : ");
		scanf("%s", input);
		sprintf((char*)query, "SELECT * FROM FARM WHERE FARMNO = '%s'", input);
		printf("--------------------------------------------------------\nFARMNO\tNAME\n");
		selectOutput(query); 
		break;
	case 6:	printf("\n1.CONSUMER ID\n2.PRODUCT NO\n\n");
		printf("Select >> ");
		scanf("%d", &tmpNum);
		getchar();
		if (tmpNum == 1) {
			printf("Input the Consumer ID : ");
			scanf("%s", input);
			sprintf((char*)query, "SELECT DISTINCT ID, PDTNO FROM CONSUMER, INFO WHERE ID = '%s' AND SELLERNO = MNO;", input);
			printf("\nID\t\t\tPDTNO\n--------------------------------------\n");
			selectOutput(query);
		}
		else	if (tmpNum == 2) {
			printf("Input the Product No : ");
			scanf("%s", input);
			sprintf((char*)query, "SELECT ID, PDTNO FROM CONSUMER, INFO WHERE PDTNO = '%s' AND SELLERNO = MNO;", input);
			printf("\nID\t\t\tPDTNO\n--------------------------------------\n");
			selectOutput(query);
		}
		else
			printf("\n# Wrong Selection ! #\n\n");
		break;
	case 7:	printf("\n1.CONSUMER's DELIVERNO\n2.FARM NAME\n\n");
		printf("Select >> ");
		scanf("%d", &tmpNum);
		getchar();
		if (tmpNum == 1) {
			printf("Input the Deliver No : ");
			scanf("%s", input);
			sprintf((char*)query, "SELECT DISTINCT C.ADDR, D.DELNO, F.NAME FROM CONSUMER C, DELIVERER D, FARM F WHERE DELIVERNO = %s AND DELIVERNO = DELNO AND D.FARMNO = F.FARMNO;",input);
			printf("\nADDR\t\t\t\tDELNO\tNAME\n----------------------------------------------\n");
			selectOutput(query);
		}
		else if (tmpNum == 2) {
			printf("Input the Farm Name : ");
			scanf("%s", input);
			sprintf((char*)query, "SELECT DISTINCT C.ADDR, D.DELNO, F.NAME FROM CONSUMER C, DELIVERER D, FARM F WHERE DELIVERNO = DELNO AND F.FARMNO = (SELECT FARMNO FROM FARM WHERE NAME = '%s') AND D.FARMNO = F.FARMNO;", input);
			printf("\nADDR\t\t\t\tDELNO\tNAME\n----------------------------------------------\n");
			selectOutput(query);
		}
		break;
	}
}

//----------------------------------SQL ����� select �޴� �Լ�------------------------------------//

void selectMenuSql() 
{
	int len;
	static SQLCHAR query[MAX_QUERY];
	printf("-----------------------------------------------------------\n");
	printf("1.CONSUMER\n2.MANAGER\n3.DELIVERER\n4.PRODUCT\n5.FARM\n6.CONSUMER AND INFO\n7.CONSUMER, DELIVERER, AND FARM\n");
	printf("-----------------------------------------------------------\n");
	getchar();

	printf("\nInput the SQL query >> ");
	getQuery(query);
	selectOutput(query);
}

//----------------------------------SQL select ���ǿ� ���� ���̺� ������ִ� �Լ�-------------------------------//

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

//---------------------------------//

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

void insertSenario()
{
	int choice;
	char tupleStr[6][BUFF_SIZE];	// ������ Ʃ���� ����
	int tupleInt[3];
	int i;
	SQLCHAR query[MAX_QUERY];

	printf("1.MANAGER\n2.DELIVERER\n3.CONSUMER\n4.FARM and PRODUCT\n5.\n\nSelect >> ");
	scanf("%d", &choice);
	getchar();

	switch (choice) {
	case 1:printf("Input MNGNO, NICK, and WORKTIME >> ");
		for (i = 0; i < 3; i++) {
		scanf("%s", tupleStr[i]);
		getchar();
		}
		sprintf((char*)query, "INSERT INTO MANAGER VALUES (%s, '%s', %s);", tupleStr[0],tupleStr[1], tupleStr[2]);
		queryOutput(query);
		break;
	case 2:printf("Input DELNO, NAME, COMPANY, and FARMNO >> ");
		for (i = 0; i < 4; i++) {
		scanf("%s", tupleStr[i]);
		getchar();
		}
		sprintf((char*)query, "INSERT INTO CONSUMER VALUES (%s,'%s','%s',%s);", tupleStr[0], tupleStr[1], tupleStr[2], tupleStr[3]);
		queryOutput(query);
		break;
	case 3:printf("Input ID and ADDR >> ");;
		for (i = 0; i < 2; i++) {
		scanf("%s", tupleStr[i]);
		getchar();
		}
		sprintf((char*)query, "INSERT INTO CONSUMER VALUES ('%s', NULL, NULL, '%s');",tupleStr[0],tupleStr[1]);
		queryOutput(query);
		break;
	case 4:printf("Input FARMNO, NAME, TYPENO, and PRICE >> ");
		for (i = 0; i < 4; i++) {
			scanf("%s", tupleStr[i]);
			getchar();
		}
		sprintf((char*)query, "INSERT INTO FARM VALUES(%s, '%s'); INSERT INTO PRODUCT VALUES (%s, %s, %s);", tupleStr[0], tupleStr[1], tupleStr[2], tupleStr[0], tupleStr[3]);
		queryOutput(query);
		break;
	case 5:break;
	}
}

void deleteSenario()
{
	int choice;
	char tupleStr[6][BUFF_SIZE];	// ������ Ʃ���� ����
	int tupleInt[3];
	int i;
	SQLCHAR query[MAX_QUERY];

	printf("1.PRODUCT\n2.DELIVERER\n3.CONSUMER and INFO\n4.INFO and MANAGER\n5.INFO with FARM and PRODUCT\nSelect >> ");
	scanf("%d", &choice);
	getchar();

	switch (choice) {
	case 1:printf("Input TYPENO and FANO >> ");
		for (i = 0; i < 2; i++) {
			scanf("%s", tupleStr[i]);
			getchar();
		}
		sprintf((char*)query, "DELETE FROM PRODUCT WHERE TYPENO = %s AND FANO = %s", tupleStr[0], tupleStr[1]);
		queryOutput(query);
		break;
	case 2:printf("Input DELNO >> ");
		scanf("%s", tupleStr[0]);
		getchar();
		sprintf((char*)query, "DELETE FROM DELIVERER WHERE DELNO = %s", tupleStr[0]);
		queryOutput(query);
		break;
	case 3:printf("Input ID >> ");
		scanf("%s", tupleStr[0]);
		getchar();
		sprintf((char*)query, "DELETE FROM INFO WHERE MNO = (SELECT SELLERNO FROM CONSUMER WHERE ID = '%s') AND ADDR = (SELECT ADDR FROM CONSUMER WHERE ID = '%s') DELETE FROM CONSUMER WHERE ID = '%s'",tupleStr[0], tupleStr[0],tupleStr[0]);
		queryOutput(query);
		break;
	case 4:printf("Input MNGNO >> ");
		scanf("%s", tupleStr[0]);
		getchar();
		sprintf((char*)query, "DELETE FROM INFO WHERE MNO = %s DELETE FROM MANAGER WHERE MNGNO = %s", tupleStr[0], tupleStr[0]);
		queryOutput(query);
		break;
	case 5:break;
	}
}

void updateSenario()
{
	int choice;
	char tupleStr[6][BUFF_SIZE];	// ������ Ʃ���� ����
	int tupleInt[3];
	int i,len;
	SQLCHAR query[MAX_QUERY];
	
	printf("1.DELIVERER\n2.FARM\n3.MANAGER\n4.PRODUCT\n5.CONSUMER and INFO\nSelect >> ");
	scanf("%d", &choice);
	getchar();

	switch (choice) {
	case 1:printf("Input DELNO and COMPANY >> ");
		for (i = 0; i < 2; i++) {
			scanf("%s", tupleStr[i]);
			getchar();
		}
		sprintf((char*)query, "UPDATE DELIVERER SET COMPANY = '%s' WHERE DELNO = %s", tupleStr[1], tupleStr[0]);
		queryOutput(query);
		break;
	case 2: printf("Input FARMNO and NAME >> ");
		for (i = 0; i < 2; i++) {
			scanf("%s", tupleStr[i]);
			getchar();
		}
		sprintf((char*)query, "UPDATE FARM SET NAME = '%s' WHERE FARMNO = %s", tupleStr[1], tupleStr[0]);
		queryOutput(query);
		break;
	case 3:printf("Input MNGNO and WORKTIME >> ");
		for (i = 0; i < 2; i++) {
			scanf("%s", tupleStr[i]);
			getchar();
		}
		sprintf((char*)query, "UPDATE MANAGER SET WORKTIME = '%s' WHERE MNGNO = %s", tupleStr[1], tupleStr[0]);
		queryOutput(query);
		break;
	case 4:printf("Input TYPENO, FANO, and PRICE >> ");
		for (i = 0; i < 3; i++) {
			scanf("%s", tupleStr[i]);
			getchar();
		}
		sprintf((char*)query, "UPDATE PRODUCT SET PRICE = %s WHERE TYPENO = %s AND FANO = %s", tupleStr[2], tupleStr[0], tupleStr[1]);
		queryOutput(query);
		break;
	case 5: printf("Input ID and ADDR >> ");
		scanf("%s", tupleStr[0]);
		getchar();
		fgets(tupleStr[1], BUFF_SIZE, stdin);
		len = strlen(tupleStr[1]);
		tupleStr[1][len - 1] = '\0';
		sprintf((char*)query, "UPDATE INFO SET ADDR = '%s' WHERE MNO = (SELECT SELLERNO FROM CONSUMER WHERE ID = '%s') AND ADDR = (SELECT ADDR FROM CONSUMER WHERE ID = '%s') UPDATE CONSUMER SET ADDR = '%s' WHERE ID = '%s'", tupleStr[1], tupleStr[0], tupleStr[0], tupleStr[1], tupleStr[0]);
		queryOutput(query);
		break;
	}
}

