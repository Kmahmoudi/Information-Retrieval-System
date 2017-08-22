// IR.cpp : Defines the entry point for the console application.

#include "stdafx.h"
#include "cran.h"
#include "Tokenize.h"
#include "index.h"
#include <set>
#include <algorithm>
#include <iterator>

using namespace std;
using std::set;
using std::ostream;
using std::inserter;
using std::set_intersection;

vector<SQLINTEGER> instersection(vector<SQLINTEGER> &v1, vector<SQLINTEGER> &v2)
{

	vector<SQLINTEGER> v3;

	sort(v1.begin(), v1.end());
	sort(v2.begin(), v2.end());

	set_intersection(v1.begin(), v1.end(), v2.begin(), v2.end(), back_inserter(v3));

	return v3;
}

vector<SQLINTEGER> _union(vector<SQLINTEGER> &v1, vector<SQLINTEGER> &v2)
{

	vector<SQLINTEGER> v3;

	sort(v1.begin(), v1.end());
	sort(v2.begin(), v2.end());

	set_union(v1.begin(), v1.end(), v2.begin(), v2.end(), back_inserter(v3));

	return v3;
}

vector <SQLINTEGER> cquery(string line)
{
	Tokenizer str(line);
	string token;
	vector <SQLINTEGER> results;;
	while ((token = str.next()) != "")
	{
		token = normalize(token);
		CStringW Term = CStringW(token.c_str());
		db Database;
		vector <SQLINTEGER> TermDocs;
		Database.getDocID(Term, &TermDocs);
		if (results.empty() == false)
			results = instersection(results, TermDocs);
		else
			results = TermDocs;
	}
	return results;
}

vector <SQLINTEGER> dquery(string line)
{
	Tokenizer str(line);
	string token;
	vector <SQLINTEGER> results;;
	while ((token = str.next()) != "")
	{
		token = normalize(token);
		CStringW Term = CStringW(token.c_str());
		db Database;
		vector <SQLINTEGER> TermDocs;
		Database.getDocID(Term, &TermDocs);
		results = _union(results, TermDocs);
	}
	return results;
}

int main()
{
	cran data;
	data.load();
	while (true)
	{
		string command;
		cout << "cmd?> ";
		cin >> command;
		if (command == "index")
		{
			system("cls");
			InvertedIndex index;
			index.constructIndex(&data);
		}

		if (command == "help")
		{
			system("cls");
			cout << "IR Sys commands :" << endl << "cls\tclear screen\nexit\tterminate the program\nindex\tstarts the index construction progress and reconstruction";
			cout << endl << "help\tshows current help menu\nquery\tstart asking questions and find related documents";
		}

		if (command == "cls")
			system("cls");
		
		if (command == "exit")
			break;

		if (command == "cquery")
		{
			string q;
			vector <SQLINTEGER> conj_results, disj_results;
			std::getline(std::cin, q);
			conj_results = cquery(q);
			vector<SQLINTEGER>::iterator postings;
			list<document>::iterator doc;

			cout << "Found " << conj_results.size() << " results using  conjunctive query\n\n\n";

			int i = 1;
			for (postings = conj_results.begin(); postings != conj_results.end(); ++postings)
			{
				for (doc = data.Documents.begin(); doc != data.Documents.end(); ++doc)
				{
					if (doc->getID() == *postings && i <= 10) {
						cout << i << "- " << doc->getT() << endl << endl;
						i++;
					}
				}
			}
		}
		
		if (command == "query")
		{
				string q;
				vector <SQLINTEGER> conj_results,disj_results;
				std::getline(std::cin, q);
				conj_results =cquery(q);
				vector<SQLINTEGER>::iterator postings;
				list<document>::iterator doc;

				
				
				if (conj_results.size() < 10)
				{
					disj_results = dquery(q);
					vector <SQLINTEGER> combinedResults;
					combinedResults = _union(conj_results, disj_results);
					int i = 1;

					cout << "Found " << combinedResults.size() << " results using both conjunctive and disjunctive queries\n\n\n";
					if (combinedResults.size() > 10) cout << "top 10 results : \n\n";

					for (postings = conj_results.begin(); postings != conj_results.end(); ++postings)
					{
						for (doc = data.Documents.begin(); doc != data.Documents.end(); ++doc)
						{
							
								if (doc->getID() == *postings && i<=10) {
									cout << i << "- doc[" << doc->getID() << "]: " << doc->getT() << endl << endl;
									i++;
								}
						}
					}
					vector <SQLINTEGER> diff;
					set_difference(disj_results.begin(), disj_results.end(), conj_results.begin(), conj_results.end(), back_inserter(diff));

					for (postings = diff.begin(); postings != diff.end(); ++postings)
					{
						for (doc = data.Documents.begin(); doc != data.Documents.end(); ++doc)
						{

							if (doc->getID() == *postings && i <= 10) {
								cout << i << "- doc[" << doc->getID() << "]: " << doc->getT() << endl << endl;
								i++;
							}
						}
					}
				}
				else
				{
					cout << "Found " << conj_results.size() << " results using  conjunctive query\n\n\n";
					if (conj_results.size() > 10) cout << "top 10 results : \n\n";

					int i = 1;
					for (postings = conj_results.begin(); postings != conj_results.end(); ++postings)
					{
						for (doc = data.Documents.begin(); doc != data.Documents.end(); ++doc)
						{
							if (doc->getID() == *postings && i <= 10){
								cout << i << "- " << doc->getT() << endl << endl;
							i++;
							}
						}
					}
				}
				
		}
	}
	cout << "\nEnter a key to exit ... ";
	getchar();
    return 0;
}



