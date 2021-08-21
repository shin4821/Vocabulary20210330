//Word.cpp
#include"Word.h"
Word::Word()
	:spelling(""), wordClass(""), meaning(""), example("") {
}

Word::~Word() {
}

Word::Word(string spelling, string wordClass, string meaning, string example)
	:spelling(spelling), wordClass(wordClass), meaning(meaning), example(example) {
}

Word::Word(const Word& source)
	: spelling(source.spelling), wordClass(source.wordClass), meaning(source.meaning), example(source.example) {
}

bool Word::IsEqual(const Word& other) {
	bool ret = false;

	if (this->spelling.compare(other.spelling) == 0 && this->wordClass.compare(other.wordClass) == 0 &&
		this->meaning.compare(other.meaning) == 0 && this->example.compare(other.example) == 0) {
		ret = true;
	}
	return ret;
}

bool Word::IsNotEqual(const Word& other) {
	bool ret = false;

	if (this->spelling.compare(other.spelling) != 0 || this->wordClass.compare(other.wordClass) != 0 ||
		this->meaning.compare(other.meaning) != 0 || this->example.compare(other.example) != 0) {
		ret = true;
	}
	return ret;
}

bool Word::operator==(const Word& other) {
	bool ret = false;

	if (this->spelling.compare(other.spelling) == 0 && this->wordClass.compare(other.wordClass) == 0 &&
		this->meaning.compare(other.meaning) == 0 && this->example.compare(other.example) == 0) {
		ret = true;
	}
	return ret;
}

bool Word::operator!=(const Word& other) {
	bool ret = false;

	if (this->spelling.compare(other.spelling) != 0 || this->wordClass.compare(other.wordClass) != 0 ||
		this->meaning.compare(other.meaning) != 0 || this->example.compare(other.example) != 0) {
		ret = true;
	}
	return ret;
}

Word& Word::operator = (const Word & source){
	this->spelling = source.spelling;
	this->wordClass = source.wordClass;
	this->meaning = source.meaning;
	this->example = source.example;

	return *this;
}

#if 0
#include<iostream>
using namespace std;

int main(int argc, char* argv[]) {
	bool ret;

	Word word("apple", "명사", "사과", "I like apple");
	cout << word.GetSpelling() << "-" << word.GetWordClass() << "-" << word.GetMeaning() << "-" << word.GetExample() 
		<< endl;

	Word other("banana", "명사", "바나나", "Do you like banana?");
	cout << other.GetSpelling() << "-" << other.GetWordClass() << "-" << other.GetMeaning() << "-" 
		<< other.GetExample() << endl;

	Word another(word);
	cout << another.GetSpelling() << "-" << another.GetWordClass() << "-" << another.GetMeaning() << "-"
		<< another.GetExample() << endl;

	ret = word.IsEqual(another);
	if (ret == true) {
		cout << "같습니다." << endl;
	}

	


	return 0;
}
#endif





