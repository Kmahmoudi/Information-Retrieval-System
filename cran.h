/* Cran class representing a class for accessing cran dataset for information retreaval system
Kamran Mahmoudi [kmahmoudi@edu.ikiu.ac.ir], 2017
IR course project under supervision of dr. mohamadi zanjireh
*/

#include <iostream>
#include <string>
#include <fstream>
#include <windows.h>
#include <list>

using namespace std;

void gotoxy(int x, int y)
{
	COORD c = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

class document
{
	int id;
	string T;
	string A;
	string B;
	string W;

public:

	document* next;

	document()
	{
		id = 0;
		T = "";
		A = "";
		B = "";
		W = "";
		next = NULL;
	}

	void setID(int i) {
		id = i;
	}

	void setT(string title) {
		T = title;
	}

	void setA(string author) {
		A = author;
	}

	void setB(string b) {
		B = b;
	}

	void setW(string text) {
		W = text;
	}

	int getID() {
		return id;
	}
	string getT() {
		return T;
	}

	string getA() {
		return A;
	}

	string getB() {
		return B;
	}

	string getW() {
		return W;
	}
};

class cran {
	string filepath;
public:
	list<document> Documents;

	cran(string path = "cran.all.1400")
	{
		filepath = path;
	}

	void load()
	{
		fstream dataset;
		string line;
		int docCount = 0;
		cout << "Loading dataset : "<<filepath<<endl;
		dataset.open(filepath);
		if (dataset.is_open())
		{
			while (getline(dataset, line))
			{
			//cout << line << endl;
			newdoc:
				document* current = new document;
				if (line.substr(0,3)==".I ")
				{	
					docCount++;
					gotoxy(50, 0);
					cout << "Document Count:" << docCount;
					//cout << "id: " << line.substr(1, line.length() - 1) << endl;
					current->setID(docCount);
					getline(dataset, line);
					if (line == ".T")
					{
						while (getline(dataset, line))
						{
							//cout << line << endl;
							if (line != ".A")
							{
								current->setT(current->getT() + " " + line);
							}
							else
							{
								while (getline(dataset, line))
								{
									//cout << line << endl;
									if (line != ".B")
									{
										current->setA(current->getA() + " " + line);
									}
									else
									{
										while (getline(dataset, line))
										{
											//cout << line << endl;
											if (line != ".W")
											{
												current->setB(current->getB() + " " + line);
											}
											else
											{
												while (getline(dataset, line))
												{
													//cout << line << endl;
													if (line.substr(0,3) != ".I ")
													{
														current->setW(current->getW() + " " + line);
													}
													else
													{
														Documents.push_back(*current);
														goto newdoc;
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}

		}else
		{
			cout << "cannot open dataset file";
		}
		dataset.close();
		cout << endl;
	}

	void listTitles()
	{
		for (list<document>::iterator it = Documents.begin(); it != Documents.end(); ++it)
		{
			cout << endl << it->getID() << " | " << it->getT() << endl;
			getchar();
		}
	}
	void show(int id) {
		
		for (list<document>::iterator it = Documents.begin(); it != Documents.end(); ++it)
		{
			if (it->getID() == id)
			{
				cout << it->getT() << endl << it->getA() << endl << it->getB() << endl << it->getW()<< endl;
			}
		}
	}
};