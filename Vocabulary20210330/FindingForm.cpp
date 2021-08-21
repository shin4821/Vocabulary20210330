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
	//1. ã�� ������ ������ ��
	  
	  // ����Ʈ����Ʈ�� ��Ÿ�� �����Ѵ�.
	  ((CListCtrl*)GetDlgItem(IDC_LIST_WORDS))->SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	  //1.1. ����Ʈ����Ʈ���� ����� �����.
	  ((CListCtrl*)GetDlgItem(IDC_LIST_WORDS))->InsertColumn(0, "��ȣ", LVCFMT_CENTER, 50);
	  ((CListCtrl*)GetDlgItem(IDC_LIST_WORDS))->InsertColumn(1, "ö��", LVCFMT_CENTER, 100);
	  ((CListCtrl*)GetDlgItem(IDC_LIST_WORDS))->InsertColumn(2, "ǰ��", LVCFMT_CENTER, 50);
	  ((CListCtrl*)GetDlgItem(IDC_LIST_WORDS))->InsertColumn(3, "�ǹ�", LVCFMT_CENTER, 150);
	  ((CListCtrl*)GetDlgItem(IDC_LIST_WORDS))->InsertColumn(4, "����", LVCFMT_CENTER, 200);

	  //1.2. ö�� ��ư�� üũ�س��´�.
	  ((CButton*)GetDlgItem(IDC_BUTTON_SPELLING))->SetCheck(BST_CHECKED);

	  
	  return FALSE;
}

void FindingForm::OnSpellingButtonClicked() {
	//2. ö�� ��ư Ŭ������ ��
	  //2.1. �ǹ̸� �����.
	  GetDlgItem(IDC_EDIT_MEANING)->SetWindowText(""); 
}

void FindingForm::OnMeaningButtonClicked() {
	//3. �ǹ� ��ư�� Ŭ������ ��
	  //3.1. ö�ڸ� �����.
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

	//4. ã�� ��ư Ŭ������ ��
	  //4.1. ö�ڹ�ư �д´�.
	  spellingButtonState = ((CButton*)GetDlgItem(IDC_BUTTON_SPELLING))->GetCheck();

	  //4.2. ö�ڸ� �д´�.
	  GetDlgItem(IDC_EDIT_SPELLING)->GetWindowText(spelling);

	  //4.3. �ǹ̹�ư �д´�.
	  meaningButtonState = ((CButton*)GetDlgItem(IDC_BUTTON_MEANING))->GetCheck();

	  //4.4. �ǹ̸� �д´�.
	  GetDlgItem(IDC_EDIT_MEANING)->GetWindowText(meaning);

	  //4.5. �ܾ��� �����츦 ã�´�.
	  vocabularyForm = (VocabularyForm*)CWnd::FindWindow("#32770", "�ܾ���");

	  //4.6. �ܾ��� �������� �ܾ��忡�� ���ǿ� ���� ã�´�.
	  if (this->indexes != NULL) {
		  delete[] indexes;
	  }
	  if (spellingButtonState == BST_CHECKED) {
		  vocabularyForm->vocabulary->FindBySpelling((LPCTSTR)spelling, &this->indexes, &this->count);
	  }
	  else if (meaningButtonState == BST_CHECKED) {
		  vocabularyForm->vocabulary->FindByMeaning((LPCTSTR)meaning, &this->indexes, &this->count);
	  }

	  //4.7. ����Ʈ����Ʈ���� ��� �׸��� �����.
	  ((CListCtrl*)GetDlgItem(IDC_LIST_WORDS))->DeleteAllItems();

	  //4.8. �ܾ����� count��ŭ ����Ʈ����Ʈ�ѿ� �׸��� �߰��Ѵ�.
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

	//5. ����Ʈ������� ����Ŭ�� ���� ��
	  //5.1. ����Ʈ�� ��Ʈ�� ��ȣ�� �д´�.
	  index = ((CListCtrl*)GetDlgItem(IDC_LIST_WORDS))->GetSelectionMark();

	  //5.2. ö��, ǰ��, �ǹ�, ���ø� �д´�.
	  spelling = ((CListCtrl*)GetDlgItem(IDC_LIST_WORDS))->GetItemText(index, 1);
	  wordClass = ((CListCtrl*)GetDlgItem(IDC_LIST_WORDS))->GetItemText(index, 2);
	  meaning = ((CListCtrl*)GetDlgItem(IDC_LIST_WORDS))->GetItemText(index, 3);
	  example = ((CListCtrl*)GetDlgItem(IDC_LIST_WORDS))->GetItemText(index, 4);

	  //5.3. �ܾ��� ������ ã�´�.
	  vocabularyForm = (VocabularyForm*)CWnd::FindWindow("#32770", "�ܾ���");

	  //5.4. �ܾ����������� �ܾ ö��, ǰ��, �ǹ�, ���ø� ���´�.
	  vocabularyForm->GetDlgItem(IDC_EDIT_SPELLING)->SetWindowText(spelling);
	  vocabularyForm->GetDlgItem(IDC_EDIT_WORDCLASS)->SetWindowText(wordClass);
	  vocabularyForm->GetDlgItem(IDC_EDIT_MEANING)->SetWindowText(meaning);
	  vocabularyForm->GetDlgItem(IDC_EDIT_EXAMPLE)->SetWindowText(example);

	  //5.5. �ܾ����������� ����Ʈ����Ʈ�ѿ��� �ش� �׸� �����Ѵ�.
	  ((CListCtrl*)vocabularyForm->GetDlgItem(IDC_LIST_WORDS))->SetSelectionMark(this->indexes[index]);

	  if (this->indexes != NULL) {
		  delete[] this->indexes;
		  this->indexes = NULL;
	  }

	  //5.6. ã�� �����츦 �����Ѵ�.
	  EndDialog(0);
}

void FindingForm::OnClose() {
	//6. �ݱ� ��ư Ŭ������ ��
	  //6.1.  ã�� ������ �����Ѵ�.
	if (this->indexes != NULL) {
		delete[] this->indexes;
		this->indexes = NULL;
	}
	EndDialog(0);
}
















