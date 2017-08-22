///////////////////////////////////////////////////////////////////////////////
// Tokenizer.h
// ===========
// General purpose string tokenizer (C++ string version)
//
// The default delimiters are space(" "), tab(\t, \v), newline(\n),
// carriage return(\r), and form feed(\f).
// If you want to use different delimiters, then use setDelimiter() to override
// the delimiters. Note that the delimiter string can hold multiple characters.
//
//  AUTHOR: Song Ho Ahn (song.ahn@gmail.com)
// CREATED: 2005-05-25
// UPDATED: 2011-03-08
///////////////////////////////////////////////////////////////////////////////

#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <string>
#include <vector>
#include <algorithm>

// default delimiter string (space, tab, newline, carriage return, form feed)
const std::string DEFAULT_DELIMITER = " \t\v\n\r\f";

class Tokenizer
{
public:
    // ctor/dtor
    Tokenizer();
    Tokenizer(const std::string& str, const std::string& delimiter=DEFAULT_DELIMITER);
    ~Tokenizer();

    // set string and delimiter
    void set(const std::string& str, const std::string& delimiter=DEFAULT_DELIMITER);
    void setString(const std::string& str);             // set source string only
    void setDelimiter(const std::string& delimiter);    // set delimiter string only

    std::string next();                                 // return the next token, return "" if it ends

    std::vector<std::string> split();                   // return array of tokens from current cursor

protected:


private:
    void skipDelimiter();                               // ignore leading delimiters
    bool isDelimiter(char c);                           // check if the current char is delimiter

    std::string buffer;                                 // input string
    std::string token;                                  // output string
    std::string delimiter;                              // delimiter string
    std::string::const_iterator currPos;                // string iterator pointing the current position

};

#endif // TOKENIZER_H


Tokenizer::Tokenizer() : buffer(""), token(""), delimiter(DEFAULT_DELIMITER)
{
	currPos = buffer.begin();
}

Tokenizer::Tokenizer(const std::string& str, const std::string& delimiter) : buffer(str), token(""), delimiter(delimiter)
{
	currPos = buffer.begin();
}

Tokenizer::~Tokenizer()
{
}

void Tokenizer::set(const std::string& str, const std::string& delimiter)
{
	this->buffer = str;
	this->delimiter = delimiter;
	this->currPos = buffer.begin();
}

void Tokenizer::setString(const std::string& str)
{
	this->buffer = str;
	this->currPos = buffer.begin();
}

void Tokenizer::setDelimiter(const std::string& delimiter)
{
	this->delimiter = delimiter;
	this->currPos = buffer.begin();
}

std::string Tokenizer::next()
{
	if (buffer.size() <= 0) return "";           // skip if buffer is empty

	token.clear();                              // reset token string

	this->skipDelimiter();                      // skip leading delimiters

												// append each char to token string until it meets delimiter
	while (currPos != buffer.end() && !isDelimiter(*currPos))
	{
		token += *currPos;
		++currPos;
	}
	return token;
}

void Tokenizer::skipDelimiter()
{
	while (currPos != buffer.end() && isDelimiter(*currPos))
		++currPos;
}

bool Tokenizer::isDelimiter(char c)
{
	return (delimiter.find(c) != std::string::npos);
}

std::vector<std::string> Tokenizer::split()
{
	std::vector<std::string> tokens;
	std::string token;
	while ((token = this->next()) != "")
	{
		tokens.push_back(token);
	}

	return tokens;
}

//------------------------------------------- end of tokenizer.h -----------------------------------------------



char easytolower(char in) {
	if (in <= 'Z' && in >= 'A')
		return in - ('Z' - 'z');
	return in;
}

string normalize(string token)
{
	std::transform(token.begin(), token.end(), token.begin(), easytolower);
	token.erase(remove_if(token.begin(), token.end(), [](int c) { return !isalnum(c); }), token.end());	
	return token;
}