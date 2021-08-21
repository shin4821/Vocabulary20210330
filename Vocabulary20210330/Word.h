//Word.h
#ifndef _WORD_H
#define _WORD_H
#include<string>
using namespace std;

class Word {
public:
	Word();// ����Ʈ ������
	~Word();//�Ҹ���
	Word(string spelling, string wordClass, string meaning, string example);//����Ʈ �Ű����� ������
	Word(const Word& source);//���������
	bool IsEqual(const Word& other);
	bool IsNotEqual(const Word& other);
	bool operator==(const Word& other);
	bool operator!=(const Word& other);
	Word& operator=(const Word& source);//ġȯ������
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


