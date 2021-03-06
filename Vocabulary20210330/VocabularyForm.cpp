//VocabularyForm.cpp
#include"VocabularyForm.h"
#include"Vocabulary.h"
#include"FindingForm.h"
#include<afxcmn.h>//CListCtrl
#include<afxdb.h>//Dababase
#include<string.h>

BEGIN_MESSAGE_MAP(VocabularyForm, CDialog)

	ON_BN_CLICKED(IDC_BUTTON_RECORD, OnRecordButtonClicked)
	ON_BN_CLICKED(IDC_BUTTON_FIND, OnFindButtonClicked)
	ON_BN_CLICKED(IDC_BUTTON_CORRECT,OnCorrectButtonClicked)
	ON_BN_CLICKED(IDC_BUTTON_ERASE, OnEraseButtonClicked)
	ON_BN_CLICKED(IDC_BUTTON_ARRANGE, OnArrangeButtonClicked)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_WORDS, OnListViewItemDoubleClicked)
	ON_WM_CLOSE()
END_MESSAGE_MAP()

VocabularyForm::VocabularyForm(CWnd* parent)
	:CDialog(VocabularyForm::IDD, parent) {
	this->vocabulary = NULL;
}

void VocabularyForm::Load() {
	CDatabase db; 
	CRecordset rs(&db);
	CString sql = "SELECT Word.spelling, Word.wordClass, Word.meaning, Word.example FROM Word;";

	db.OpenEx("DSN=Vocabulary;UID=Sydney;PWD=4821");
	rs.Open(AFX_DB_USE_DEFAULT_TYPE, sql);
	CString spelling;
	CString wordClass;
	CString meaning;
	CString example;

	while (!rs.IsEOF()) {
		rs.GetFieldValue((short)0, spelling);
		rs.GetFieldValue((short)1, wordClass);
		rs.GetFieldValue((short)2, meaning);
		rs.GetFieldValue((short)3, example);

		this->vocabulary->Record((LPCTSTR)spelling, (LPCTSTR)wordClass, (LPCTSTR)meaning, (LPCTSTR)example);

		rs.MoveNext();
	}
	rs.Close();
	db.Close();
}

void VocabularyForm::Insert(Long index) {
	CDatabase db;
	Word word = this->vocabulary->GetAt(index);
	CString sql;
	CString code;
	CString example;

	code = GetCode();

	example = CString(word.GetExample().c_str());
	example.Replace("'", "\\'");

	sql.Format("INSERT INTO Word(code, spelling, wordClass, meaning, example) VALUES('%s','%s','%s','%s','%s');",
		(LPCTSTR)code, word.GetSpelling().c_str(), word.GetWordClass().c_str(), word.GetMeaning().c_str(),
		(LPCTSTR)example);

	db.OpenEx("DSN=Vocabulary;UID=Sydney;PWD=4821");
	db.ExecuteSQL(sql);
	db.Close();
}

void VocabularyForm::Modify(Long index) {
	CDatabase db;
	CRecordset rs(&db);
	CString sql = "SELECT Word.code FROM Word;";
	CString code;
	CString example;
	Long i = 0;
	Word word;

	//1. index?????? ?????? ??????.
	db.OpenEx("DSN=Vocabulary;UID=Sydney;PWD=4821");
	rs.Open(AFX_DB_USE_DEFAULT_TYPE, sql);

	while (i <= index) {
		rs.GetFieldValue((short)0, code);
		rs.MoveNext();
		i++;
	}
	//2. ???? ?????? Update????.
	word = this->vocabulary->GetAt(index);

	example = CString(word.GetExample().c_str());
	example.Replace("'", "\\'");


	sql.Format("UPDATE Word SET wordClass='%s', meaning='%s', example='%s' WHERE code='%s';",
		word.GetWordClass().c_str(), word.GetMeaning().c_str(), (LPCTSTR)example, (LPCTSTR)code);

	db.ExecuteSQL(sql);
	rs.Close();
	db.Close();
}

void VocabularyForm::Delete(Long index) {
	CDatabase db;
	CRecordset rs(&db);
	CString sql = "SELECT Word.code FROM Word;";
	Long i = 0;
	CString code;

	//1. index???? ?????? ??????.
	db.OpenEx("DSN=Vocabulary;UID=Sydney;PWD=4821");
	rs.Open(AFX_DB_USE_DEFAULT_TYPE, sql);
	
	while (i <= index) {
		rs.GetFieldValue((short)0, code);
		rs.MoveNext();
		i++;
	}

	//2. ???? ?????? ????????.
	sql.Format("DELETE FROM Word WHERE code='%s';", (LPCTSTR)code);
	db.ExecuteSQL(sql);

	//3. db, rs?? ??????.
	rs.Close();
	db.Close();
}

CString VocabularyForm::GetCode() {
	CDatabase db;
	CRecordset rs(&db);
	CString sql = "SELECT Word.code FROM Word ORDER BY code DESC;";
	CString code;
	int number;

	//1. ?????? ?????? ??????.
	db.OpenEx("DSN=Vocabulary;UID=Sydney;PWD=4821");
	rs.Open(AFX_DB_USE_DEFAULT_TYPE, sql);

	if (!rs.IsEOF()) {//?????? ??????, ?? ?????? ?????? ??????.
		rs.GetFieldValue((short)0, code);
	}
	else {//?????? ??????, ???????? ??????????.
		code = "P0000";
	}

	//2. ?????? ?????????? ????????.
	code=code.Right(4);

	//3. ?????? ?????????? ?????? ????????.
	number = _ttoi(code);

	//4. ?????? 1 ????????.
	number++;

	//5. ?????? P0000???? ???????? ????????.
	code.Format("P%04d", number);

	//6. ?????? ????????.
	rs.Close();
	db.Close();

	return code;
}

void VocabularyForm::Save() {
	CDatabase db;
	CRecordset rs(&db);
	CString code;
	CString sql = "SELECT Word.code FROM Word;"; //?????? ???? ??????.
	CString sql_;
	CString example;
	Long i = 0;
	Word word;

	//1. db?? ???? ???????? ????????.
	db.OpenEx("DSN=Vocabulary;UID=Sydney;PWD=4821");
	rs.Open(AFX_DB_USE_DEFAULT_TYPE, sql);

	db.ExecuteSQL("DELETE FROM Word");

	//2. Vocabulary?? ???????? ?????? Insert????.
	
	while (i < this->vocabulary->GetLength()) {
		rs.GetFieldValue((short)0, code);
		word = this->vocabulary->GetAt(i);

		example = CString(word.GetExample().c_str());
		example.Replace("'", "\\'");

		sql_.Format("INSERT INTO Word(code,spelling,wordClass,meaning,example) VALUES('%s','%s','%s','%s','%s');",
			(LPCTSTR)code, word.GetSpelling().c_str(), word.GetWordClass().c_str(), word.GetMeaning().c_str(),
			(LPCTSTR)example);

		db.ExecuteSQL(sql_);
		rs.MoveNext();
		i++;
	}
	//3. db, rs ??????.
	rs.Close();
	db.Close();
}


BOOL VocabularyForm::OnInitDialog() {
	CDialog::OnInitDialog();
	CString wordClasses[] = { "????","????","????","??????","??????","??????","??????","??????" };
	Word word;
	CString spelling;
	CString wordClass;
	CString meaning;
	CString example;
	CString number;
	Long i;

	//1. ???????? ?????? ??
	  //1.1. ???????? ??????.
	  this->vocabulary = new Vocabulary;

	  //(4/1 ????) db???? Load????.
	  Load();

	  //???????? ?????? ?????? ????????.
	  ((CListCtrl*)GetDlgItem(IDC_LIST_WORDS))->SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	  //1.2. ???????????????? ?????? ??????.
	  ((CListCtrl*)GetDlgItem(IDC_LIST_WORDS))->InsertColumn(0, "????", LVCFMT_LEFT, 50);
	  ((CListCtrl*)GetDlgItem(IDC_LIST_WORDS))->InsertColumn(1, "????", LVCFMT_LEFT, 100);
	  ((CListCtrl*)GetDlgItem(IDC_LIST_WORDS))->InsertColumn(2, "????", LVCFMT_CENTER, 50);
	  ((CListCtrl*)GetDlgItem(IDC_LIST_WORDS))->InsertColumn(3, "????", LVCFMT_LEFT, 150);
	  ((CListCtrl*)GetDlgItem(IDC_LIST_WORDS))->InsertColumn(4, "????", LVCFMT_LEFT, 200);


	  //1.3. ???????? ?????? ?????????? ????????.
	  for (i = 0; i < 8; i++) {
		  ((CComboBox*)GetDlgItem(IDC_EDIT_WORDCLASS))->AddString((LPCTSTR)wordClasses[i]);
	  }

	  //1.4.(4/1 ????) length???? ???????????????? ????????.
	  i = 0;
	  while (i < this->vocabulary->GetLength()) {
		  
		  word = this->vocabulary->GetAt(i);

		  number.Format("%d", i + 1);
		  ((CListCtrl*)GetDlgItem(IDC_LIST_WORDS))->InsertItem(i, number);

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

	  return FALSE;
}

void VocabularyForm::OnRecordButtonClicked() {
	CString spelling;
	CString wordClass;
	CString meaning;
	CString example;
	CString number;
	Long index;
	Word word;

	//2. ???????? ?????? ???????? ??
	  //2.1. ????, ????, ????, ?????? ??????.
	  GetDlgItem(IDC_EDIT_SPELLING)->GetWindowText(spelling);
	  GetDlgItem(IDC_EDIT_WORDCLASS)->GetWindowText(wordClass);
	  GetDlgItem(IDC_EDIT_MEANING)->GetWindowText(meaning);
	  GetDlgItem(IDC_EDIT_EXAMPLE)->GetWindowText(example);

	  //2.2. ???????? ????????.
	  index = this->vocabulary->Record((LPCTSTR)spelling, (LPCTSTR)wordClass, (LPCTSTR)meaning, 
		  (LPCTSTR)example);

	  //(4/1 ????) Insert????.
	  Insert(index);

	  //2.3. ???????? ???????? ???? ?????? ????????.
	  word = this->vocabulary->GetAt(index);

	  number.Format("%d", index + 1);
	  ((CListCtrl*)GetDlgItem(IDC_LIST_WORDS))->InsertItem(index, number);

	  spelling = CString(word.GetSpelling().c_str());
	  ((CListCtrl*)GetDlgItem(IDC_LIST_WORDS))->SetItemText(index, 1, spelling);

	  wordClass = CString(word.GetWordClass().c_str());
	  ((CListCtrl*)GetDlgItem(IDC_LIST_WORDS))->SetItemText(index, 2, wordClass);

	  meaning = CString(word.GetMeaning().c_str());
	  ((CListCtrl*)GetDlgItem(IDC_LIST_WORDS))->SetItemText(index, 3, meaning);

	  example = CString(word.GetExample().c_str());
	  ((CListCtrl*)GetDlgItem(IDC_LIST_WORDS))->SetItemText(index, 4, example);
}

void VocabularyForm::OnFindButtonClicked() {
	//3. ???? ?????? ???????? ??
	  //3.1. ???? ???????? ????????.
	  FindingForm findingForm;
	  findingForm.DoModal();
}

void VocabularyForm::OnCorrectButtonClicked() {
	CString wordClass;
	CString meaning;
	CString example;
	Word word;
	Long index;

	//4. ?????? ???? ???????? ??
	  //4.1. ????, ????, ?????? ??????
	  GetDlgItem(IDC_EDIT_WORDCLASS)->GetWindowText(wordClass);
	  GetDlgItem(IDC_EDIT_MEANING)->GetWindowText(meaning);
	  GetDlgItem(IDC_EDIT_EXAMPLE)->GetWindowText(example);

	  //4.2. ???????????????? ?????? ??????.
	  index = ((CListCtrl*)GetDlgItem(IDC_LIST_WORDS))->GetSelectionMark();

	  //4.3. ?????????? ??????.
	  index = this->vocabulary->Correct(index, (LPCTSTR)wordClass, (LPCTSTR)meaning, (LPCTSTR)example);

	  //(4/1 ????) db???? Modify????.
	  Modify(index);


	  //4.4. ?????????????????? ???? ?????? ??????.
	  word = this->vocabulary->GetAt(index);
	  wordClass = CString(word.GetWordClass().c_str());
	  ((CListCtrl*)GetDlgItem(IDC_LIST_WORDS))->SetItemText(index, 2, wordClass);

	  meaning = CString(word.GetMeaning().c_str());
	  ((CListCtrl*)GetDlgItem(IDC_LIST_WORDS))->SetItemText(index, 3, meaning);

	  example = CString(word.GetExample().c_str());
	  ((CListCtrl*)GetDlgItem(IDC_LIST_WORDS))->SetItemText(index, 4, example);
}

void VocabularyForm::OnEraseButtonClicked() {
	Long index;
	CString number;

	//5. ?????? ???? ???????? ??
	  //5.1. ???????????????? ??????  ??????.
	  index = ((CListCtrl*)GetDlgItem(IDC_LIST_WORDS))->GetSelectionMark();

	  //5.2. ?????????? ??????.
	  this->vocabulary->Erase(index);

	  //(4/1 ????) db???? ??????.
	  Delete(index);

	  //5.3. ?????????????????? ???? ?????? ??????.
	  ((CListCtrl*)GetDlgItem(IDC_LIST_WORDS))->DeleteItem(index);

	  //5.4. ?????? ???? ?????? ???? ???????? ?????? 1?? ????.
	  while (index < this->vocabulary->GetLength()) {
		  number.Format("%d", index + 1);
		  ((CListCtrl*)GetDlgItem(IDC_LIST_WORDS))->SetItemText(index, 0, number);
		  index++;
	  }
}

void VocabularyForm::OnArrangeButtonClicked() {
	Word word;
	CString spelling;
	CString wordClass;
	CString meaning;
	CString example;
	Long i = 0;
	CString number;

	//6. ???????? ?????? ???????? ??
	  //6.1. ?????????? ????????.
      this->vocabulary->Arrange();

	  //6.2. ???????? ?????????? ???? ?????? ??????.
	  ((CListCtrl*)GetDlgItem(IDC_LIST_WORDS))->DeleteAllItems();

	  //6.3. ???????? length???? ???????????????? ???????? ????????.
	  while (i < this->vocabulary->GetLength()) {
		  word = this->vocabulary->GetAt(i);

		  number.Format("%d", i + 1);

		  ((CListCtrl*)GetDlgItem(IDC_LIST_WORDS))->InsertItem(i, number);

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

void VocabularyForm::OnListViewItemDoubleClicked(NMHDR* pNotifyStruct, LRESULT* result) {
	CString spelling;
	CString wordClass;
	CString meaning;
	CString example;
	Long index;

	//7. ?????????????? ?????? ???????? ???? ??
	  //7.1. ???????????????? ???? ?????? ??????.
	  index = ((CListCtrl*)GetDlgItem(IDC_LIST_WORDS))->GetSelectionMark();

	  //7.2. ????, ????, ????, ?????? ??????.
	  spelling = ((CListCtrl*)GetDlgItem(IDC_LIST_WORDS))->GetItemText(index, 1);
	  wordClass = ((CListCtrl*)GetDlgItem(IDC_LIST_WORDS))->GetItemText(index, 2);
	  meaning = ((CListCtrl*)GetDlgItem(IDC_LIST_WORDS))->GetItemText(index, 3);
	  example = ((CListCtrl*)GetDlgItem(IDC_LIST_WORDS))->GetItemText(index, 4);

	  //7.3. ?????? ????, ????, ????, ?????? ??????.
	  GetDlgItem(IDC_EDIT_SPELLING)->SetWindowText(spelling);
	  GetDlgItem(IDC_EDIT_WORDCLASS)->SetWindowText(wordClass);
	  GetDlgItem(IDC_EDIT_MEANING)->SetWindowText(meaning);
	  GetDlgItem(IDC_EDIT_EXAMPLE)->SetWindowTextA(example);
}


void VocabularyForm::OnClose() {
	//8. ???? ?????? ???????? ??
	  //8.1 ???????? ??????.
	if (this->vocabulary != NULL) {

		//(4/1 ????) db?? ????????.
		Save();

		delete this->vocabulary;
		this->vocabulary = NULL;
	}
	//8.2. ???????? ????????.
	EndDialog(0);
}




