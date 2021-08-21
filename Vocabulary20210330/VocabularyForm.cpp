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

	//1. index번째의 코드를 구한다.
	db.OpenEx("DSN=Vocabulary;UID=Sydney;PWD=4821");
	rs.Open(AFX_DB_USE_DEFAULT_TYPE, sql);

	while (i <= index) {
		rs.GetFieldValue((short)0, code);
		rs.MoveNext();
		i++;
	}
	//2. 해당 코드를 Update한다.
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

	//1. index번째 코드를 찾는다.
	db.OpenEx("DSN=Vocabulary;UID=Sydney;PWD=4821");
	rs.Open(AFX_DB_USE_DEFAULT_TYPE, sql);
	
	while (i <= index) {
		rs.GetFieldValue((short)0, code);
		rs.MoveNext();
		i++;
	}

	//2. 해당 코드를 삭제한다.
	sql.Format("DELETE FROM Word WHERE code='%s';", (LPCTSTR)code);
	db.ExecuteSQL(sql);

	//3. db, rs를 닫는다.
	rs.Close();
	db.Close();
}

CString VocabularyForm::GetCode() {
	CDatabase db;
	CRecordset rs(&db);
	CString sql = "SELECT Word.code FROM Word ORDER BY code DESC;";
	CString code;
	int number;

	//1. 마지막 코드를 찾는다.
	db.OpenEx("DSN=Vocabulary;UID=Sydney;PWD=4821");
	rs.Open(AFX_DB_USE_DEFAULT_TYPE, sql);

	if (!rs.IsEOF()) {//코드가 있으면, 맨 마지막 코드를 찾는다.
		rs.GetFieldValue((short)0, code);
	}
	else {//코드가 없으면, 초기값을 설정해준다.
		code = "P0000";
	}

	//2. 코드의 숫자부분을 분리한다.
	code=code.Right(4);

	//3. 분리한 숫자부분을 정수로 변환한다.
	number = _ttoi(code);

	//4. 숫자를 1 더해준다.
	number++;

	//5. 숫자를 P0000형식 문자열로 변환한다.
	code.Format("P%04d", number);

	//6. 코드를 출력한다.
	rs.Close();
	db.Close();

	return code;
}

void VocabularyForm::Save() {
	CDatabase db;
	CRecordset rs(&db);
	CString code;
	CString sql = "SELECT Word.code FROM Word;"; //코드만 따로 빼준다.
	CString sql_;
	CString example;
	Long i = 0;
	Word word;

	//1. db의 모든 데이터를 삭제한다.
	db.OpenEx("DSN=Vocabulary;UID=Sydney;PWD=4821");
	rs.Open(AFX_DB_USE_DEFAULT_TYPE, sql);

	db.ExecuteSQL("DELETE FROM Word");

	//2. Vocabulary의 데이터를 가져와 Insert한다.
	
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
	//3. db, rs 닫는다.
	rs.Close();
	db.Close();
}


BOOL VocabularyForm::OnInitDialog() {
	CDialog::OnInitDialog();
	CString wordClasses[] = { "동사","명사","부사","전치사","접속사","형용사","감탄사","대명사" };
	Word word;
	CString spelling;
	CString wordClass;
	CString meaning;
	CString example;
	CString number;
	Long i;

	//1. 윈도우가 생성될 때
	  //1.1. 단어장을 만든다.
	  this->vocabulary = new Vocabulary;

	  //(4/1 추가) db에서 Load한다.
	  Load();

	  //리스트뷰 컨트롤 스타일 설정한다.
	  ((CListCtrl*)GetDlgItem(IDC_LIST_WORDS))->SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	  //1.2. 리스트뷰컨트롤의 헤더를 만든다.
	  ((CListCtrl*)GetDlgItem(IDC_LIST_WORDS))->InsertColumn(0, "번호", LVCFMT_LEFT, 50);
	  ((CListCtrl*)GetDlgItem(IDC_LIST_WORDS))->InsertColumn(1, "철자", LVCFMT_LEFT, 100);
	  ((CListCtrl*)GetDlgItem(IDC_LIST_WORDS))->InsertColumn(2, "품사", LVCFMT_CENTER, 50);
	  ((CListCtrl*)GetDlgItem(IDC_LIST_WORDS))->InsertColumn(3, "의미", LVCFMT_LEFT, 150);
	  ((CListCtrl*)GetDlgItem(IDC_LIST_WORDS))->InsertColumn(4, "예시", LVCFMT_LEFT, 200);


	  //1.3. 콤보박스 목록에 품사항목을 추가한다.
	  for (i = 0; i < 8; i++) {
		  ((CComboBox*)GetDlgItem(IDC_EDIT_WORDCLASS))->AddString((LPCTSTR)wordClasses[i]);
	  }

	  //1.4.(4/1 추가) length만큼 리스트뷰컨트롤에 추가한다.
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

	//2. 기재하기 버튼을 클릭했을 때
	  //2.1. 철자, 품사, 의미, 예시를 읽는다.
	  GetDlgItem(IDC_EDIT_SPELLING)->GetWindowText(spelling);
	  GetDlgItem(IDC_EDIT_WORDCLASS)->GetWindowText(wordClass);
	  GetDlgItem(IDC_EDIT_MEANING)->GetWindowText(meaning);
	  GetDlgItem(IDC_EDIT_EXAMPLE)->GetWindowText(example);

	  //2.2. 단어장에 기재한다.
	  index = this->vocabulary->Record((LPCTSTR)spelling, (LPCTSTR)wordClass, (LPCTSTR)meaning, 
		  (LPCTSTR)example);

	  //(4/1 추가) Insert한다.
	  Insert(index);

	  //2.3. 리스트뷰 컨트롤에 해당 항목을 추가한다.
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
	//3. 찾기 버튼을 클릭했을 때
	  //3.1. 찾기 윈도우를 출력한다.
	  FindingForm findingForm;
	  findingForm.DoModal();
}

void VocabularyForm::OnCorrectButtonClicked() {
	CString wordClass;
	CString meaning;
	CString example;
	Word word;
	Long index;

	//4. 고치기 버튼 클릭했을 때
	  //4.1. 품사, 의미, 예시를 읽는다
	  GetDlgItem(IDC_EDIT_WORDCLASS)->GetWindowText(wordClass);
	  GetDlgItem(IDC_EDIT_MEANING)->GetWindowText(meaning);
	  GetDlgItem(IDC_EDIT_EXAMPLE)->GetWindowText(example);

	  //4.2. 리스트뷰컨트롤의 번호를 읽는다.
	  index = ((CListCtrl*)GetDlgItem(IDC_LIST_WORDS))->GetSelectionMark();

	  //4.3. 단어장에서 고친다.
	  index = this->vocabulary->Correct(index, (LPCTSTR)wordClass, (LPCTSTR)meaning, (LPCTSTR)example);

	  //(4/1 추가) db에서 Modify한다.
	  Modify(index);


	  //4.4. 리스트뷰컨트롤에서 해당 항목을 고친다.
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

	//5. 지우기 버튼 클릭했을 때
	  //5.1. 리스트뷰컨트롤의 번호를  읽는다.
	  index = ((CListCtrl*)GetDlgItem(IDC_LIST_WORDS))->GetSelectionMark();

	  //5.2. 단어장에서 지운다.
	  this->vocabulary->Erase(index);

	  //(4/1 추가) db에서 지운다.
	  Delete(index);

	  //5.3. 리스트뷰컨트롤에서 해당 항목을 지운다.
	  ((CListCtrl*)GetDlgItem(IDC_LIST_WORDS))->DeleteItem(index);

	  //5.4. 지워진 항목 번호의 하위 번호들에 대하여 1씩 뺀다.
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

	//6. 정리하기 버튼을 클릭했을 때
	  //6.1. 단어장에서 정리한다.
      this->vocabulary->Arrange();

	  //6.2. 리스트뷰 컨트롤에서 전체 항목을 지운다.
	  ((CListCtrl*)GetDlgItem(IDC_LIST_WORDS))->DeleteAllItems();

	  //6.3. 단어장의 length만큼 리스트뷰컨트롤에 항목들을 추가한다.
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

	//7. 리스트뷰컨트롤 아이템 더블클릭 했을 때
	  //7.1. 리스트뷰컨트롤의 항목 번호를 읽는다.
	  index = ((CListCtrl*)GetDlgItem(IDC_LIST_WORDS))->GetSelectionMark();

	  //7.2. 철자, 품사, 의미, 예시를 읽는다.
	  spelling = ((CListCtrl*)GetDlgItem(IDC_LIST_WORDS))->GetItemText(index, 1);
	  wordClass = ((CListCtrl*)GetDlgItem(IDC_LIST_WORDS))->GetItemText(index, 2);
	  meaning = ((CListCtrl*)GetDlgItem(IDC_LIST_WORDS))->GetItemText(index, 3);
	  example = ((CListCtrl*)GetDlgItem(IDC_LIST_WORDS))->GetItemText(index, 4);

	  //7.3. 단어에 철자, 품사, 의미, 예시를 적는다.
	  GetDlgItem(IDC_EDIT_SPELLING)->SetWindowText(spelling);
	  GetDlgItem(IDC_EDIT_WORDCLASS)->SetWindowText(wordClass);
	  GetDlgItem(IDC_EDIT_MEANING)->SetWindowText(meaning);
	  GetDlgItem(IDC_EDIT_EXAMPLE)->SetWindowTextA(example);
}


void VocabularyForm::OnClose() {
	//8. 닫기 버튼을 클릭했을 때
	  //8.1 단어장을 없앤다.
	if (this->vocabulary != NULL) {

		//(4/1 추가) db에 저장한다.
		Save();

		delete this->vocabulary;
		this->vocabulary = NULL;
	}
	//8.2. 윈도우를 종료한다.
	EndDialog(0);
}




