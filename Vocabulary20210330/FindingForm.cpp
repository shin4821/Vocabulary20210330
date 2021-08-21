//FindingForm.cpp
#include"FindingForm.h"
#include"VocabularyForm.h"
#include"Vocabulary.h"
#include<afxcmn.h>//CListCtrl

BEGIN_MESSAGE_MAP(FindingForm,CDialog)
	ON_BN_CLICKED(IDC_BUTTON_SPELLING, OnSpellingButtonClicked)
	ON_BN_CLICKED(IDC_BUTTON_MEANING, OnMeaningButtonClicked)
	ON_BN_CLICKED(IDC_BUTTON_FIND, OnFindButtonClicked)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_WORDS, OnListViewItemDoubleClicked)
	ON_WM_CLOSE()
END_MESSAGE_MAP()

FindingForm::FindingForm(CWnd* parent)
	:CDialog(FindingForm::IDD, parent) {
	this->indexes = NULL;
	this->count = 0;
}

BOOL FindingForm::OnInitDialog() {
	//1. 찾기 윈도우 생성될 때
	  
	  // 리스트뷰컨트롤 스타일 설정한다.
	  ((CListCtrl*)GetDlgItem(IDC_LIST_WORDS))->SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	  //1.1. 리스트뷰컨트롤의 헤더를 만든다.
	  ((CListCtrl*)GetDlgItem(IDC_LIST_WORDS))->InsertColumn(0, "번호", LVCFMT_CENTER, 50);
	  ((CListCtrl*)GetDlgItem(IDC_LIST_WORDS))->InsertColumn(1, "철자", LVCFMT_CENTER, 100);
	  ((CListCtrl*)GetDlgItem(IDC_LIST_WORDS))->InsertColumn(2, "품사", LVCFMT_CENTER, 50);
	  ((CListCtrl*)GetDlgItem(IDC_LIST_WORDS))->InsertColumn(3, "의미", LVCFMT_CENTER, 150);
	  ((CListCtrl*)GetDlgItem(IDC_LIST_WORDS))->InsertColumn(4, "예시", LVCFMT_CENTER, 200);

	  //1.2. 철자 버튼을 체크해놓는다.
	  ((CButton*)GetDlgItem(IDC_BUTTON_SPELLING))->SetCheck(BST_CHECKED);

	  
	  return FALSE;
}

void FindingForm::OnSpellingButtonClicked() {
	//2. 철자 버튼 클릭했을 때
	  //2.1. 의미를 지운다.
	  GetDlgItem(IDC_EDIT_MEANING)->SetWindowText(""); 
}

void FindingForm::OnMeaningButtonClicked() {
	//3. 의미 버튼을 클릭했을 때
	  //3.1. 철자를 지운다.
	GetDlgItem(IDC_EDIT_SPELLING)->SetWindowText("");
}

void FindingForm::OnFindButtonClicked() {
	VocabularyForm* vocabularyForm;
	int spellingButtonState;
	int meaningButtonState;
	CString spelling;
	CString wordClass;
	CString meaning;
	CString example;
	CString number;
	Long i = 0;
	Word word;

	//4. 찾기 버튼 클릭했을 때
	  //4.1. 철자버튼 읽는다.
	  spellingButtonState = ((CButton*)GetDlgItem(IDC_BUTTON_SPELLING))->GetCheck();

	  //4.2. 철자를 읽는다.
	  GetDlgItem(IDC_EDIT_SPELLING)->GetWindowText(spelling);

	  //4.3. 의미버튼 읽는다.
	  meaningButtonState = ((CButton*)GetDlgItem(IDC_BUTTON_MEANING))->GetCheck();

	  //4.4. 의미를 읽는다.
	  GetDlgItem(IDC_EDIT_MEANING)->GetWindowText(meaning);

	  //4.5. 단어장 윈도우를 찾는다.
	  vocabularyForm = (VocabularyForm*)CWnd::FindWindow("#32770", "단어장");

	  //4.6. 단어장 윈도우의 단어장에서 조건에 따라 찾는다.
	  if (this->indexes != NULL) {
		  delete[] indexes;
	  }
	  if (spellingButtonState == BST_CHECKED) {
		  vocabularyForm->vocabulary->FindBySpelling((LPCTSTR)spelling, &this->indexes, &this->count);
	  }
	  else if (meaningButtonState == BST_CHECKED) {
		  vocabularyForm->vocabulary->FindByMeaning((LPCTSTR)meaning, &this->indexes, &this->count);
	  }

	  //4.7. 리스트뷰컨트롤의 모든 항목을 지운다.
	  ((CListCtrl*)GetDlgItem(IDC_LIST_WORDS))->DeleteAllItems();

	  //4.8. 단어장의 count만큼 리스트뷰컨트롤에 항목을 추가한다.
	  while (i < this->count) {
		  number.Format("%d", i + 1);

		  ((CListCtrl*)GetDlgItem(IDC_LIST_WORDS))->InsertItem(i, number);
		  word = vocabularyForm->vocabulary->GetAt(this->indexes[i]);

		  spelling = CString(word.GetSpelling().c_str());
		  ((CListCtrl*)GetDlgItem(IDC_LIST_WORDS))->SetItemText(i, 1, spelling);

		  wordClass = CString(word.GetWordClass().c_str());
		  ((CListCtrl*)GetDlgItem(IDC_LIST_WORDS))->SetItemText(i, 2, wordClass);

		  meaning = CString(word.GetMeaning().c_str());
		  ((CListCtrl*)GetDlgItem(IDC_LIST_WORDS))->SetItemText(i, 3, meaning);

		  example = CString(word.GetExample().c_str());
		  ((CListCtrl*)GetDlgItem(IDC_LIST_WORDS))->SetItemText(i, 4, example);

		  i++;
	  }
}

void FindingForm::OnListViewItemDoubleClicked(NMHDR* pNotifyStruct, LRESULT* result) {
	VocabularyForm* vocabularyForm;
	CString spelling;
	CString wordClass;
	CString meaning;
	CString example;
	Long index;

	//5. 리스트뷰아이템 더블클릭 했을 때
	  //5.1. 리스트뷰 컨트롤 번호를 읽는다.
	  index = ((CListCtrl*)GetDlgItem(IDC_LIST_WORDS))->GetSelectionMark();

	  //5.2. 철자, 품사, 의미, 예시를 읽는다.
	  spelling = ((CListCtrl*)GetDlgItem(IDC_LIST_WORDS))->GetItemText(index, 1);
	  wordClass = ((CListCtrl*)GetDlgItem(IDC_LIST_WORDS))->GetItemText(index, 2);
	  meaning = ((CListCtrl*)GetDlgItem(IDC_LIST_WORDS))->GetItemText(index, 3);
	  example = ((CListCtrl*)GetDlgItem(IDC_LIST_WORDS))->GetItemText(index, 4);

	  //5.3. 단어장 윈도우 찾는다.
	  vocabularyForm = (VocabularyForm*)CWnd::FindWindow("#32770", "단어장");

	  //5.4. 단어장윈도우의 단어에 철자, 품사, 의미, 예시를 적는다.
	  vocabularyForm->GetDlgItem(IDC_EDIT_SPELLING)->SetWindowText(spelling);
	  vocabularyForm->GetDlgItem(IDC_EDIT_WORDCLASS)->SetWindowText(wordClass);
	  vocabularyForm->GetDlgItem(IDC_EDIT_MEANING)->SetWindowText(meaning);
	  vocabularyForm->GetDlgItem(IDC_EDIT_EXAMPLE)->SetWindowText(example);

	  //5.5. 단어장윈도우의 리스트뷰컨트롤에서 해당 항목 선택한다.
	  ((CListCtrl*)vocabularyForm->GetDlgItem(IDC_LIST_WORDS))->SetSelectionMark(this->indexes[index]);

	  if (this->indexes != NULL) {
		  delete[] this->indexes;
		  this->indexes = NULL;
	  }

	  //5.6. 찾기 윈도우를 종료한다.
	  EndDialog(0);
}

void FindingForm::OnClose() {
	//6. 닫기 버튼 클릭했을 때
	  //6.1.  찾기 윈도우 종료한다.
	if (this->indexes != NULL) {
		delete[] this->indexes;
		this->indexes = NULL;
	}
	EndDialog(0);
}
















