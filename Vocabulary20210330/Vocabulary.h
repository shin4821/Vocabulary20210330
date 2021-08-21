//Vocabulary.h
#ifndef _VOCABULARY_H
#define _VOCABULARY_H
typedef signed long int Long;
#include"Array.h"
#include"Word.h"
#include<string>
using namespace std;

class Vocabulary {
public:
	Vocabulary(Long capacity = 256);
	~Vocabulary();
	Vocabulary(const Vocabulary& source);
	Long Record(string spelling, string wordClass, string meaning, string example);
	Word& GetAt(Long index);
	void FindBySpelling(string spelling, Long*(*indexes), Long*count);
	void FindByMeaning(string meaning, Long* (*indexes), Long* count);
	Long Correct(Long index, string wordClass, string meaning, string example);
	Long Erase(Long index);
	void Arrange();
	Vocabulary& operator=(const Vocabulary& source);
	Word& operator[](Long index);
	Word* operator+(Long index);
	Long GetCapacity()const;
	Long GetLength()const;

private:
	Array<Word> words;
	Long capacity;
	Long length;
};
int CompareSpellings(void* one, void* other);
int CompareMeanings(void* one, void* other);
int CompareWords(void* one, void* other);

inline Long Vocabulary::GetCapacity()const {
	return this->capacity;
}
inline Long Vocabulary::GetLength()const {
	return this->length;
}



#endif//_VOCABULARY_H

















