//Word.h
#ifndef _WORD_H
#define _WORD_H
#include<string>
using namespace std;

class Word {
public:
	Word();// 디폴트 생성자
	~Word();//소멸자
	Word(string spelling, string wordClass, string meaning, string example);//디폴트 매개변수 생성자
	Word(const Word& source);//복사생성자
	bool IsEqual(const Word& other);
	bool IsNotEqual(const Word& other);
	bool operator==(const Word& other);
	bool operator!=(const Word& other);
	Word& operator=(const Word& source);//치환연산자
	string& GetSpelling()const;
	string& GetWordClass()const;
	string& GetMeaning()const;
	string& GetExample()const;

private:
	string spelling;
	string wordClass;
	string meaning;
	string example;
};
inline string& Word::GetSpelling()const {
	return const_cast<string&>(this->spelling);
}
inline string& Word::GetWordClass()const {
	return const_cast<string&>(this->wordClass);
}
inline string& Word::GetMeaning()const {
	return const_cast<string&>(this->meaning);
}
inline string& Word::GetExample()const {
	return const_cast<string&>(this->example);
}

#endif//_WORD_H


