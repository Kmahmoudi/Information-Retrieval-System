#include <windows.h>
#include <sqltypes.h>
#include <sql.h>
#include <sqlext.h>
#include <cstring>
#include <atlstr.h>  

using namespace std;
class db
{
public:

	void show_error(unsigned int handletype, const SQLHANDLE& handle)
	{
		SQLWCHAR sqlstate[2024];
		SQLWCHAR message[2024];
		if (SQL_SUCCESS == SQLGetDiagRec(handletype, handle, 1, sqlstate, NULL, message, 1024, NULL)) {
			//wcout << "Message: " << message << "\nSQLSTATE: " << sqlstate << endl;
		}
	}
		

	void getDocID(CStringW Token,vector<SQLINTEGER> *Result)
	{
		SQLHANDLE sqlenvhandle;
		SQLHANDLE sqlconnectionhandle = new SQLHANDLE();
		SQLHANDLE sqlstatementhandle = new SQLHANDLE();
		SQLRETURN retcode;
		SQLINTEGER DocID;
		do
		{
			if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &sqlenvhandle))
				break;

			if (SQL_SUCCESS != SQLSetEnvAttr(sqlenvhandle, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0))
				break;

			if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_DBC, sqlenvhandle, &sqlconnectionhandle))
				break;

			SQLCHAR retconstring[1024];
			switch (SQLDriverConnectA(sqlconnectionhandle, NULL,
				(SQLCHAR*)"DRIVER={SQL Server};SERVER=localhost, 1433;DATABASE=dbIR;",
				SQL_NTS, retconstring, 1024, NULL, SQL_DRIVER_NOPROMPT))
			{
			case SQL_SUCCESS_WITH_INFO:
				show_error(SQL_HANDLE_DBC, sqlconnectionhandle);
				break;
			case SQL_INVALID_HANDLE:
			case SQL_ERROR:
				show_error(SQL_HANDLE_DBC, sqlconnectionhandle);
				retcode = -1;
				break;
			default:
				break;
			}

			if (retcode == -1)
				break;

			if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, sqlconnectionhandle, &sqlstatementhandle))
				break;

			wchar_t sql[1024];
			swprintf_s(sql, 1024, L"SELECT DocID FROM TermDoc WHERE Term='%s'", Token);

			if (SQL_SUCCESS != SQLExecDirect(sqlstatementhandle, (SQLWCHAR*)sql, SQL_NTS))
			{
				show_error(SQL_HANDLE_STMT, sqlstatementhandle);
				break;
			}
			else
			{
				// something
				while (SQLFetch(sqlstatementhandle) == SQL_SUCCESS)
				{
					SQLGetData(sqlstatementhandle, 1, SQL_C_DEFAULT, &DocID, 10, NULL);
					Result->push_back(DocID);

					//cout << " DocID: " << DocID << endl;
				}
			}

		} while (false);
		SQLFreeHandle(SQL_HANDLE_STMT, sqlstatementhandle);
		SQLDisconnect(sqlconnectionhandle);
		SQLFreeHandle(SQL_HANDLE_DBC, sqlconnectionhandle);
		SQLFreeHandle(SQL_HANDLE_ENV, sqlenvhandle);
	}

	void insert(CStringW Term, int DocID)
	{
		SQLHANDLE sqlenvhandle;
		SQLHANDLE sqlconnectionhandle = new SQLHANDLE();
		SQLHANDLE sqlstatementhandle = new SQLHANDLE();
		SQLRETURN retcode;
		do
		{
			if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &sqlenvhandle))
				break;

			if (SQL_SUCCESS != SQLSetEnvAttr(sqlenvhandle, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0))
				break;

			if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_DBC, sqlenvhandle, &sqlconnectionhandle))
				break;

			SQLCHAR retconstring[1024];
			switch (SQLDriverConnectA(sqlconnectionhandle, NULL,
				(SQLCHAR*)"DRIVER={SQL Server};SERVER=localhost, 1433;DATABASE=dbIR;",
				SQL_NTS, retconstring, 1024, NULL, SQL_DRIVER_NOPROMPT))
			{
			case SQL_SUCCESS_WITH_INFO:
				show_error(SQL_HANDLE_DBC, sqlconnectionhandle);
				break;
			case SQL_INVALID_HANDLE:
			case SQL_ERROR:
				show_error(SQL_HANDLE_DBC, sqlconnectionhandle);
				retcode = -1;
				break;
			default:
				break;
			}

			if (retcode == -1)
				break;

			if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, sqlconnectionhandle, &sqlstatementhandle))
				break;

			wchar_t sql[1024];
			//swprintf_s(sql, 1024, L"INSERT INTO TermDoc(Term,DocID) VALUES ('%s','%d') WHERE NOT EXSITS (SELECT Term,DocID FROM TermDoc WHERE Term='%s' AND DocID='%d')", Term, DocID, Term, DocID);
			swprintf_s(sql, 1024, L"BEGIN IF NOT EXISTS (SELECT * FROM TermDoc WHERE Term = '%s' AND DocID = '%d') BEGIN INSERT INTO TermDoc (Term,DocID) VALUES ('%s','%d') END END", Term, DocID, Term, DocID);

			if (SQL_SUCCESS != SQLExecDirect(sqlstatementhandle, (SQLWCHAR*)sql, SQL_NTS))
			{
				show_error(SQL_HANDLE_STMT, sqlstatementhandle);
				break;
			}
			else
			{
				// something
				//cout << "Insert Done!";
			}

		} while (false);
		SQLFreeHandle(SQL_HANDLE_STMT, sqlstatementhandle);
		SQLDisconnect(sqlconnectionhandle);
		SQLFreeHandle(SQL_HANDLE_DBC, sqlconnectionhandle);
		SQLFreeHandle(SQL_HANDLE_ENV, sqlenvhandle);
	}

};