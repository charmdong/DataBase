#include <stdio.h>
#include <Windows.h>
#include <sql.h>
#include <sqlext.h>
#include <string.h>
#include <stdlib.h>
#define BUFF_SIZE 30
#define MAX_QUERY 500
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
void insertScenario();
void updateScenario();
void deleteScenario();
int qOrs();
void getQuery(SQLCHAR *);

SQLHENV hEnv = NULL;
SQLHDBC hDbc = NULL;

int main(int argc, char *agrv[])
{
	if (DBConnect() == true) {
		printf("+---------------------------------------+\n|\tDatabase is connected !\t\t|\n+---------------------------------------+\n");
		menu();
		DBDisconnect();
	}

	return 0;
}

//---------------------------------------------------------DB 연결 관련 함수-----------------------------------------------------------//

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

//---------------------기본 메뉴 함수---------------------//

void menu()
{
	int menuNum;
	SQLCHAR query[MAX_QUERY];
	while (1) {
		printf("\t<MENU>\n1.Show Schema\n2.SELECT\n3.INSERT\n4.UPDATE\n5.DELETE\n6.EXIT\n");
		printf("Select >> ");
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
				insertScenario();
			break;
		case 4:
			if (qOrs() == 1) {
				printf("\n1.CONSUMER (ID,DELIVERNO,SELLERNO,ADDR)\n2.MANAGER (MNGNO,NICK,WORKTIME)\n3.DELIVERER (DELNO,NAME,COMPANY,FARMNO)\n");
				printf("4.PRODUCT (TYPENO,FANO,PRICE$)\n5.FARM (FARMNO,NAME)\n6.INFO (MNO,FANO,ADDR,PDTTYPE)\n\nInput the SQL >> ");
				getchar();
				getQuery(query);
				queryOutput(query);
			}
			else 
				updateScenario();
			break;
		case 5:
			if (qOrs() == 1) {
				printf("\n1.CONSUMER (ID,DELIVERNO,SELLERNO,ADDR)\n2.MANAGER (MNGNO,NICK,WORKTIME)\n3.DELIVERER (DELNO,NAME,COMPANY,FARMNO)\n");
				printf("4.PRODUCT (TYPENO,FANO,PRICE$)\n5.FARM (FARMNO,NAME)\n6.INFO (MNO,FANO,ADDR,PDTTYPE)\n\nInput the SQL >> ");
				getchar();
				getQuery(query);
				queryOutput(query);
			}
			else
				deleteScenario();
			break;
		case 6: printf("\n<Exit the Database>\n\n");  break;
		default: printf("\n# Wrong Selection ! #\n\n");
		}
		if (menuNum == 6)
			break;
	}
}

//---------------------SQL, 시나리오 기반 선택 함수---------------------//

int qOrs()
{
	int num;
	while (1) {
		printf("+---------------------------------------+\n|\t\t1.By SQL\t\t|\n|\t\t2.By Scenario\t\t|\n+---------------------------------------+\n");
		printf("Select >> ");
		scanf("%d", &num);
		if (num == 1 || num == 2)
			break;
		else
			printf("\n# Wrong selection ! #\n");
	}
	return num;
}

//---------------------SQL 질의문 parsing 함수---------------------//

void getQuery(SQLCHAR *query) // SQL 질의문 parsing 함수
{
	int len;
	fgets((char*)query, MAX_QUERY + 1, stdin);
	len = strlen((char*)query);
	query[len - 1] = '\0';
}

//---------------------스키마 관련 함수들---------------------//

void schemaMenu() // 스키마 메뉴 함수
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

void showSchema(SQLCHAR *query) // 모든 스키마를 출력해주는 함수
{
	SQLHSTMT hStmt;
	SQLSMALLINT colCnt = 0;
	SQLINTEGER nullData[50];
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
			SQLBindCol(hStmt, i + 1, SQL_C_CHAR, data[i], BUFF_SIZE, &nullData[i]);
		printf("-----------------------------------------------------------\n");
		while (SQLFetch(hStmt) != SQL_NO_DATA) {
			for (i = 0; i < colCnt; i++) {
				if (nullData[i] == SQL_NULL_DATA)
					printf("NULL\t");
				else
					printf("%s\t", data[i]);
			}
			printf("\n");
		}
		printf("-----------------------------------------------------------\n\n");

		SQLCloseCursor(hStmt);
		SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
	}
}

//--------------------------------시나리오 기반의 select 메뉴 함수--------------------------------------//

void selectMenu()
{
	int num;
	int tmpNum;
	char input[25];
	static SQLCHAR query[MAX_QUERY];
	
	printf("+-------------------------------------------------------+\n");
	printf("|\t\t1.Refer CONSUMER\t\t\t|\n|\t\t2.Refer MANAGER\t\t\t\t|\n|\t\t3.Refer DELIVERER\t\t\t|\n|\t\t4.Refer PRODUCT\t\t\t\t|\n|\t\t5.Refer FARM\t\t\t\t|\n|\t\t6.CONSUMER AND INFO\t\t\t|\n|\t\t7.CONSUMER, DELIVERER, AND FARM\t\t|\n");
	printf("+-------------------------------------------------------+\n");
	printf("Select >> ");
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
	default: printf("\n# Wrong Selection ! #\n\n");
	}
}

//----------------------------------SQL 기반의 select 메뉴 함수------------------------------------//

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

//----------------------------------SQL select 질의에 대한 테이블 출력해주는 함수-------------------------------//

void selectOutput(SQLCHAR* query)
{
	SQLHSTMT hStmt;
	SQLSMALLINT colCnt = 0;
	SQLRETURN retcode;
	SQLINTEGER nullData[50];
	int len;
	int i;

	if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt) == SQL_SUCCESS)
	{
		retcode = SQLExecDirect(hStmt, query, SQL_NTS);
		if (retcode == SQL_ERROR || retcode == SQL_NO_DATA_FOUND) {
			printf("\n# Query Execute Error #\n\n");
			return;
		}
		SQLNumResultCols(hStmt, &colCnt);

		SQLCHAR **data;
		data = (SQLCHAR **)malloc(sizeof(SQLCHAR *)*colCnt);
		for (i = 0; i < colCnt; i++)
			data[i] = (SQLCHAR *)malloc(sizeof(SQLCHAR) * BUFF_SIZE);

		for (i = 0; i < colCnt; i++)
			SQLBindCol(hStmt, i + 1, SQL_C_CHAR, data[i], BUFF_SIZE, &nullData[i]);

		while (SQLFetch(hStmt) != SQL_NO_DATA) {
			for (i = 0; i < colCnt; i++) {
				if (nullData[i] == SQL_NULL_DATA)
					printf("NULL\t");
				else
					printf("%s\t", data[i]);
			}
			printf("\n");
		}
		printf("\n");
		SQLCloseCursor(hStmt);
		SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
	}
}

//---------------------------------//

void queryOutput(SQLCHAR* query)	// insert, update, delete
{
	SQLHSTMT hStmt;
	SQLSMALLINT colCnt = 0;
	SQLRETURN retcode;
	int len;
	int i;

	if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt) == SQL_SUCCESS)
	{
		retcode = SQLExecDirect(hStmt, query, SQL_NTS);
		if (retcode == SQL_ERROR || retcode == SQL_NO_DATA_FOUND) {
			printf("\n# Query Execute Error #\n\n");
			return;
		}
		SQLNumResultCols(hStmt, &colCnt);

		SQLCloseCursor(hStmt);
		SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
	}
}

void insertScenario()
{
	int choice;
	char tupleStr[6][BUFF_SIZE];	// 삽입할 튜플의 값들
	int tupleInt[3];
	int i,len;
	SQLCHAR query[MAX_QUERY];

	printf("1.MANAGER\n2.DELIVERER\n3.CONSUMER(ID, ADDR)\n4.FARM\n5.INFO(MNO, FANO, ADDR) with CONSUMER and FARM\n\nSelect >> ");
	scanf("%d", &choice);
	getchar();

	switch (choice) {
	case 1:printf("Input MNGNO, NICK, and WORKTIME >> ");
		for (i = 0; i < 3; i++) {
		scanf("%s", tupleStr[i]);
		getchar();
		}
		sprintf((char*)query, "INSERT INTO MANAGER VALUES (%s, '%s', '%s');", tupleStr[0],tupleStr[1], tupleStr[2]);
		queryOutput(query);
		break;
	case 2:printf("Input DELNO, NAME, COMPANY, and FARMNO >> ");
		for (i = 0; i < 4; i++) {
		scanf("%s", tupleStr[i]);
		getchar();
		}
		sprintf((char*)query, "INSERT INTO DELIVERER VALUES (%s,'%s','%s',%s);", tupleStr[0], tupleStr[1], tupleStr[2], tupleStr[3]);
		queryOutput(query);
		break;
	case 3:printf("Input ID and ADDR >> ");
		scanf("%s", tupleStr[0]);
		getchar();
		fgets(tupleStr[1], BUFF_SIZE, stdin);
		len = strlen(tupleStr[1]);
		tupleStr[1][len - 1] = '\0';
		sprintf((char*)query, "INSERT INTO CONSUMER VALUES ('%s', NULL, NULL, '%s');",tupleStr[0],tupleStr[1]);
		queryOutput(query);
		break;
	case 4:printf("Input FARMNO and NAME >> ");
		for (i = 0; i < 2; i++) {
			scanf("%s", tupleStr[i]);
			getchar();
		}
		sprintf((char*)query, "INSERT INTO FARM VALUES(%s , '%s');",tupleStr[0], tupleStr[1]);
		queryOutput(query);
		break;
	case 5:printf("Input ID and FARMNAME >> ");
		for (i = 0; i < 2; i++) {
			scanf("%s", tupleStr[i]);
			getchar();
		}
		sprintf((char*)query, "INSERT INTO INFO (MNO, FANO, ADDR) SELECT MNGNO, FARMNO, ADDR FROM MANAGER, FARM, CONSUMER WHERE MNGNO = (SELECT SELLERNO FROM CONSUMER WHERE ID = '%s') AND FARMNO = (SELECT FARMNO FROM FARM WHERE NAME = '%s') AND ID = '%s'", tupleStr[0], tupleStr[1],tupleStr[0]);
		queryOutput(query);
		break;
	default: printf("\n# Wrong Selection ! #\n\n");
	}
	printf("\n");
}

void deleteScenario()
{
	int choice;
	char tupleStr[6][BUFF_SIZE];	// 삽입할 튜플의 값들
	int tupleInt[3];
	int i;
	SQLCHAR query[MAX_QUERY];

	printf("1.FARM\n2.DELIVERER\n3.CONSUMER and INFO\n4.MANAGER and INFO\n5.PRODUCT and FARM\nSelect >> ");
	scanf("%d", &choice);
	getchar();

	switch (choice) {
	case 1:printf("Input FARMNO >> ");
		scanf("%s", tupleStr[0]);
		getchar();
		sprintf((char*)query, "DELETE FROM FARM WHERE FARMNO = %s ", tupleStr[0]);
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
	case 5:printf("Input NAME and TYPENO >> ");
		for (i = 0; i < 2; i++) {
			scanf("%s", tupleStr[i]);
			getchar();
		}
		sprintf((char*)query, "DELETE FROM PRODUCT WHERE FANO = (SELECT FARMNO FROM FARM WHERE NAME = '%s') AND TYPENO = %s", tupleStr[0], tupleStr[1]);
		queryOutput(query);
		break;
	default: printf("\n# Wrong Selection ! #\n\n");
	}
	printf("\n");
}

void updateScenario()
{
	int choice;
	char tupleStr[6][BUFF_SIZE];	// 삽입할 튜플의 값들
	int tupleInt[3];
	int i,len;
	SQLCHAR query[MAX_QUERY];
	
	printf("1.DELIVERER\n2.INFO\n3.MANAGER\n4.PRODUCT\n5.CONSUMER and INFO\n6.SELLERNO and DELIVERNO\nSelect >> ");
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
	case 2: printf("Input ID and TYPENO >> ");
		for (i = 0; i < 2; i++) {
			scanf("%s", tupleStr[i]);
			getchar();
		}
		sprintf((char*)query, "UPDATE INFO SET PDTNO = %s WHERE MNO = (SELECT SELLERNO FROM CONSUMER WHERE ID = '%s') AND ADDR = (SELECT ADDR FROM CONSUMER WHERE ID = '%s')", tupleStr[1], tupleStr[0]);
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
	case 4:printf("Input TYPENO, FARMNAME, and PRICE >> ");
		for (i = 0; i < 3; i++) {
			scanf("%s", tupleStr[i]);
			getchar();
		}
		sprintf((char*)query, "UPDATE PRODUCT SET PRICE = %s WHERE TYPENO = %s AND FANO = (SELECT FARMNO FROM FARM WHERE NAME = '%s') ", tupleStr[2], tupleStr[0], tupleStr[1]);
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
	case 6:printf("Input ID, SELLERNO, and DELIVERNO >> ");
		for (i = 0; i < 3; i++) {
			scanf("%s", tupleStr[i]);
			getchar();
		}
		sprintf((char*)query, "UPDATE CONSUMER SET SELLERNO = %s, DELIVERNO = %s WHERE ID = '%s'", tupleStr[1], tupleStr[2], tupleStr[0]);
		queryOutput(query);
		break;
	default: printf("\n# Wrong Selection ! #\n\n");
	}
	printf("\n");
}

