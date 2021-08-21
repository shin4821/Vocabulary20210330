//VocabularyApp.cpp
#include"VocabularyApp.h"
#include"VocabularyForm.h"

BOOL VocabularyApp::InitInstance() {
	VocabularyForm vocabularyForm;

	this->m_pMainWnd = &vocabularyForm;
	vocabularyForm.DoModal();

	return TRUE;
}
VocabularyApp vocabularyApp;




