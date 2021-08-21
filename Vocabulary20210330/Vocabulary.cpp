//Vocabulary.cpp
#include"Vocabulary.h"

Vocabulary::Vocabulary(Long capacity)
	:words(capacity) {
	this->capacity = capacity;
	this->length = 0;
}

Vocabulary::~Vocabulary() {
}

Vocabulary::Vocabulary(const Vocabulary& source)
	:words(source.words) {

	this->capacity = source.capacity;
	this->length = source.length;
}

Long Vocabulary::Record(string spelling, string wordClass, string meaning, string example) {
	Long index;
	Word word(spelling, wordClass, meaning, example);

	if (this->length < this->capacity) {
		index = this->words.Store(this->length, word);
	}
	else if (this->length >= this->capacity) {
		index = this->words.AppendForRear(word);
		this->capacity++;
	}
	this->length++;

	return index;
}

void Vocabulary::FindBySpelling(string spelling, Long* (*indexes), Long* count) {
	this->words.LinearSearchDuplicate(&spelling, indexes, count, CompareSpellings);
}

void Vocabulary::FindByMeaning(string meaning, Long* (*indexes), Long* count) {
	this->words.LinearSearchDuplicate(&meaning, indexes, count, CompareMeanings);
}

Long Vocabulary::Correct(Long index, string wordClass, string meaning, string example) {
	Word word;

	word = this->words.GetAt(index);
	index = this->words.Modify(index, Word(word.GetSpelling(), wordClass, meaning, example));

	return index;
}

Long Vocabulary::Erase(Long index) {
	index = this->words.Delete(index);
	this->capacity--;
	this->length--;

	return index;
}

void Vocabulary::Arrange() {
	this->words.SelectionSort(CompareWords);
}

Word& Vocabulary::GetAt(Long index) {
	return this->words.GetAt(index);
}

Vocabulary& Vocabulary::operator=(const Vocabulary& source) {
	this->words = source.words;
	this->capacity = source.capacity;
	this->length = source.length;

	return *this;
}

Word& Vocabulary::operator[](Long index) {
	return this->words[index];
}

Word* Vocabulary::operator+(Long index) {
	return this->words + index;
}



int CompareSpellings(void* one, void* other) {
	Word* one_ = (Word*)one;
	string* other_ = (string*)other;
	int ret;

	ret = one_->GetSpelling().compare(*other_);
	return ret;
}

int CompareMeanings(void* one, void* other) {
	Word* one_ = (Word*)one;
	string* other_ = (string*)other;
	int ret;

	ret = one_->GetMeaning().compare(*other_);
	return ret;
}


int CompareWords(void* one, void* other) {
	Word* one_ = (Word*)one;
	Word* other_ = (Word*)other;
	int ret;

	ret = one_->GetSpelling().compare(other_->GetSpelling());
	return ret;
}

#if 0

#include<iostream>
using namespace std;
int main(int argc, char* argv[]) {
	Vocabulary vocabulary;
	Word word;
	Long index;
	Long(*indexes);
	Long count;
	Long i = 0;

	index = vocabulary.Record("apple", "명사", "사과", "This apple is good.");
	word = vocabulary.GetAt(index);
	cout << word.GetSpelling() << "-" << word.GetWordClass() << "-" << word.GetMeaning() << "-" << word.GetExample() 
		<< endl;

	index = vocabulary.Record("on", "전치사", "~위에", "A picture on a wall.");
	word = vocabulary.GetAt(index);
	cout << word.GetSpelling() << "-" << word.GetWordClass() << "-" << word.GetMeaning() << "-" << word.GetExample()
		<< endl;

	index = vocabulary.Record("sunny", "형용사", "맑은", "What a sunny day!");
	word = vocabulary.GetAt(index);
	cout << word.GetSpelling() << "-" << word.GetWordClass() << "-" << word.GetMeaning() << "-" << word.GetExample()
		<< endl;

	index = vocabulary.Record("on", "부사", "계속하여", "He worked on without a break.");
	word = vocabulary.GetAt(index);
	cout << word.GetSpelling() << "-" << word.GetWordClass() << "-" << word.GetMeaning() << "-" << word.GetExample()
		<< endl;

	vocabulary.FindBySpelling("on", &indexes, &count);
	i = 0;
	while (i < count) {
		word = vocabulary.GetAt(indexes[i]);
		cout << word.GetSpelling() << "-" << word.GetWordClass() << "-" << word.GetMeaning() << "-" << word.GetExample()
			<< endl;
		i++;
	}

	if (indexes != 0) {
		delete[] indexes;
		indexes = 0;
	}

	vocabulary.FindByMeaning("맑은", &indexes, &count);
	i = 0;
	while (i < count) {
		word = vocabulary.GetAt(indexes[i]);
		cout << word.GetSpelling() << "-" << word.GetWordClass() << "-" << word.GetMeaning() << "-" << word.GetExample()
			<< endl;
		i++;
	}

	if (indexes != 0) {
		delete[] indexes;
		indexes = 0;
	}

	index = vocabulary.Correct(2, "형용사", "명량한", "She's so sunny.");
	word = vocabulary.GetAt(index);
	cout << word.GetSpelling() << "-" << word.GetWordClass() << "-" << word.GetMeaning() << "-" << word.GetExample()
		<< endl;

	index = vocabulary.Erase(1);
	if (index == -1) {
		cout << "지워졌습니다." << endl;
	}

	cout << "정렬하기" << endl;
	vocabulary.Arrange();
	i = 0;
	while (i < vocabulary.GetLength()) {
		word = vocabulary.GetAt(i);
		cout << word.GetSpelling() << "-" << word.GetWordClass() << "-" << word.GetMeaning() << "-" << word.GetExample()
			<< endl;
		i++;
	}


	return 0;
}

#endif







