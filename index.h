/* inverted index data structure
Kamran Mahmoudi [kmahmoudi@edu.ikiu.ac.ir], 2017
IR course project under supervision of dr. mohamadi zanjireh
*/

#include <string>
#include <vector>
#include <iostream>
#include <cstring>
#include <atlstr.h>
#include "database.h"

struct term
{
	string keyword;
	vector<int> posting;
	int freq;

	term()
	{
		keyword = "";
		freq = 0;
	}

	void addPosting(int docID)
	{
		bool exists = false;
		vector<int>::iterator postings;
		for (postings = posting.begin(); postings != posting.end(); ++postings)
		{
			if (*postings == docID)
				exists = true;
		}
		if(!exists)
			posting.push_back(docID);
	}
	void setKeyWord(string w)
	{
		keyword = w;
	}
};

class InvertedIndex
{
public:
	void tokenize(string line,int docID)
	{
		Tokenizer str(line);
		string token;
		
		while ((token = str.next()) != "")
		{
			token = normalize(token);
			CStringW Term= CStringW(token.c_str());
			db Database;
			Database.insert(Term, docID);
		}
	}

	void constructIndex(cran* data)
	{
		cout << "Indexing document : \n    ";
		int size = data->Documents.size();
		for (list<document>::iterator it = data->Documents.begin(); it != data->Documents.end(); ++it)
		{
			int docID = it->getID();
			cout << "\b\b\b\b\b\b\b\b\b\b\b\b\b\b";
			cout << docID << " of " << size +1 << " ;";
			tokenize(it->getA(),docID);
			tokenize(it->getB(), docID);
			tokenize(it->getT(), docID);
			tokenize(it->getW(), docID);
		}
	}
};