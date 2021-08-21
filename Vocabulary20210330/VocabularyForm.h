//VocabularyForm.h
#ifndef _VOCABULARYFORM_H
#define _VOCABULARYFORM_H
#include<afxwin.h>
#include"resource.h"
typedef signed long int Long;

class Vocabulary;
class VocabularyForm :public CDialog {
public:
	enum{IDD = IDD_VOCABULARYFORM};

public:
	VocabularyForm(CWnd* parent = NULL);
	virtual BOOL OnInitDialog();
	
public:
	Vocabulary* vocabulary;

public:
	void Load();
	void Save();
	void Insert(Long index);
	void Modify(Long index);
	void Delete(Long index);
	CString GetCode();

protected:

	afx_msg void OnRecordButtonClicked();
	afx_msg void OnFindButtonClicked();
	afx_msg void OnCorrectButtonClicked();
	afx_msg void OnEraseButtonClicked();
	afx_msg void OnArrangeButtonClicked();
	afx_msg void OnListViewItemDoubleClicked(NMHDR* pNotifyStruct, LRESULT* result);
	afx_msg void OnClose();

	DECLARE_MESSAGE_MAP()
};

#endif//_VOCABULARYFORM_H




