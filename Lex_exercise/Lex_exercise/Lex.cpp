#include<iostream>
#include<cstdio>
#include<cstdlib>

#include<vector>
#include<fstream>
#include"tire_tree.h"
using namespace std;



string current_line = "";
TireNode *KeyRoot;


//���ʵĻ�������

const string k_lib[8] = { "do", "end","for", "if", "printf", "scanf", "then", "while" };//�ؼ��ֱ�
const string s_lib[16] = { ",", ";", "(", ")", "[", "]", "+", "-", "*", "/", "<","<=" , "=", ">", ">=", "<>" };//��ŷֽ����
vector<string> id_lib;//��ű�ʶ��
vector<string> ci_lib; // ��ų���


////����һ������
//enum Type {
//	error = 0,
//	key,
//	dividing,
//	calculatingop,
//	relationop,
//	NUM,
//	ID,
//	CHAR,
//	STRING,
//	COMMENT
//};

//������
class Lex{
public:
	Lex();
	//~Lex();
	TireNode * BuildKeyLib(const string *lib);

	void getche();
	bool isNum(const string &str);	//�Ƿ�������
	int getNumPointer = 5;
	bool isKey(const string &str);	//�Ƿ��ǹؼ���
	int getKeyPointer = 1;
	bool isIde(const string &str);	//�Ƿ��Ǳ�ʶ��
	int getIdePointer = 6;
	bool isOptr(const string &symbol);	//�Ƿ��ǲ�����
	int getOptrPointer(const string &str);
	string getOptrType(const string &str);//�жϷ��ŵĲ������ͣ��ٽ磬�������㻹�ǹ�ϵ���㣩
	bool isChar(const string &str);	//�ж��Ƿ�Ϊһ���ַ�
	


	int run(int row);
private:
	
	//vector<pair<Type, string>> optrs;//���������
	//vector<pair<Type, string>> dividings;	//�ֽ��
	//vector<pair<Type, string>> indetifiers;	//��ʶ��
	//vector<pair<Type, string>> constants;	//����
	//vector<pair<Type, string>> keywords;	//�ؼ���
	//vector<pair<Type, string>> strings;		//�ַ���

};

Lex::Lex(){
}


bool Lex::isChar(const string &str) {
	if (str.length() == 1 || (str[0] == '\\' && str.length() == 2)) {
		return true;
	}
	else return false;
}

bool Lex::isIde(const string &str) {
	if (!isalpha(str.c_str()[0]) && str.c_str()[0] != '_') {
		return false;
	}
	for (unsigned int i = 1; i < str.length(); i++) {
		if (!isalnum(str.c_str()[i]) && str.c_str()[i] != '_') {
			return false;
		}
	}
	return true;
}

bool Lex::isKey(const string &str){
	if (TreeFind(KeyRoot, str)!=-1) {
		return true;
	}
	else return false;
}

bool Lex::isNum(const string &str) {
	for (unsigned int i = 0; i < str.length(); i++) {
		if (!isdigit(str.c_str()[i])) {
			return false;
		}
	}
	return true;
}
bool Lex::isOptr(const string &str) {
	for (int i = 0; i < 16; i++) {
		if (s_lib[i] == str.c_str()) {
			return true;
		}
	}
	return false;
}
//
//int Lex::getIdePointer(const string &str) {
//
//}
//
//
//int Lex::getKeyPointer (const string &str) {
//
//}
//
//int Lex::getNumPointer (const string &str) {
//
//}
//
//int Lex::getOptrPointer (const string &str) {
//
//}

string Lex::getOptrType(const string &str) {
	int i = 0;
	for (; i < 16; i++) {
		if (str.c_str() == s_lib[i]) {
			break;
		}
	}

	if (i <= 5)		return "DIVIDING";
	else if (i <= 9)	return "CACULATING";
	else if (i <= 15)	return "RELATION";
	else return "ERROR";
}

int Lex::getOptrPointer(const string &str) {

	if (str == "DIVIDING")		return 2;
	else if (str == "CACULATING")	return 3;
	else if (str == "RELATION")	return 4;
	else return -1;
}
int getIdPos(vector<string> ids, string str) {		//���ұ�ʶ���ڱ���λ�õĺ���
	vector<string>::iterator it = ids.begin();
	int i = 0;
	for (; it < ids.end(); it++,i++) {
		if (*it == str) {
			return i;
		}
	}
	ids.push_back(str);
	return -1;
}


string getPiece(int start, int end, const string srcString) {		//��һ���ַ�����start��endλ�ý�ȡ����Ƭ��
	return string(srcString.begin() + start, srcString.begin() + end);
}

int Lex::run(int row) {
	int word_num = 1;
	for (unsigned int current_point = 0; current_point < current_line.length(); current_point++) {
		unsigned int current_end = current_point + 1;
		char current_char = current_line.c_str()[current_point];
		string current_string = "";
		if (isalpha(current_char)) {		//�������ĸ���Ǿͽ����жϹؼ��ֻ��߱�ʶ���ĳ���
			for (; current_point < current_line.length() 
				&&(isalnum(current_line[current_end])||current_line[current_end]=='_'); current_end++) {
			};//�������ϱ�ʶ�����߹ؼ��ֱ�׼���ַ�һ��ǰ����ֱ�����������Ϲ涨��
			current_string = getPiece(current_point, current_end, current_line);

			if (isKey(current_string)) {
				cout << current_string << "		(" << getKeyPointer << "," << current_string << ")			KEY			" << "(" << row << ", " << word_num << ")" << endl;
			}
			else if (isIde(current_string)) {
				id_lib.push_back(current_string);
				cout << current_string << "		(" << getIdePointer << "," << current_string << ")			IDENTIFY		" << "(" << row << ", " << word_num << ")" << endl;
			}
			else {
				cout << current_string << "		 	Error			Error			" << "(" << row << ", " << word_num << ")" << endl;

			}
			current_point = current_end - 1;		//-1����Ϊ�����current_point��Ҫ�����ѭ��һ�ֽ���ʱ+1
			word_num++;
		}
		else if (isdigit(current_char)) {	//��������֣��Ǿͽ����ж��Ƿ��ǳ������߱�ʶ���ĳ���			
			for (; (current_end < current_line.length()) &&
				(isalnum(current_line[current_end])); current_end++) {
			};
			current_string = getPiece(current_point, current_end, current_line);

			if (isNum(current_string)) {
				ci_lib.push_back(current_string);
				cout << current_string << "		(" << getNumPointer << "," << current_string << ")			NUMBER			" << "(" << row << ", " << word_num << ")" << endl;
			}
			else {
				cout << current_string << "		Error  			Error			" << "(" << row << ", " << word_num << ")" << endl;
			}
			current_point = current_end - 1;
			word_num++;
		}
		else if (current_char == '/' && current_line[current_end] == '*') {		//??�˴���Ҫ�Ż�������Ҫʶ��������к���ע�����
			for (; current_end + 1 < current_line.length() &&
				!(current_line[current_end] == '*'&&current_line[current_end + 1] == '/'); current_end++);
			current_string = getPiece(current_point, current_end + 2, current_line);

			cout << current_string << "		(" << "COMMENT" << "," << current_string << ")			COMMENT		" << "(" << row << ", " << word_num << ")" << endl;

			current_point = current_end +1 ;
			word_num++;
		}
		else if (isOptr(string(1,current_char))) {	//�����һ�����ţ��Ǿͽ����ж��Ƿ��ǺϷ����ŵĳ���
			for (; current_end < current_line.length() && isOptr(string(1, current_line[current_end]))
				&& getOptrType(string(1, current_line[current_end])) != "DIVIDING"; current_end++);

			current_string = getPiece(current_point, current_end, current_line);
			if (current_end - 1 < current_line.length()) {
				string type = getOptrType(current_string);
				if(type != "ERROR")
				cout << current_string << "		(" << getOptrPointer(type) << "," << current_string << ")			" << type << "		(" << row << ", " << word_num << ")" << endl;
				else
					cout << current_string << "		ERROR			ERROR			" << "(" << row << ", " << word_num << ")" << endl;
			}
			else
				cout << current_string << "		Error			Error			" << "(" << row << ", " << word_num << ")" << endl;

			current_point = current_end -1 ;
			word_num++;

		}
		else if (current_char == '"' || current_char == '\''){	//˵���������ַ���	
			for (current_end=current_point+2 ; current_end < current_line.length() && (current_char != current_line[current_end-1]); current_end++);
			current_string = getPiece(current_point, current_end, current_line);

			if (current_char == '"'&&current_end < current_line.length()) {
				cout << current_string << "		STRING			STRING			(" << row << ", " << word_num << ")" << endl;
			}
			else if (current_char == '\''&&isChar(current_string)){
				
				cout << " " << current_string << " CHAR at " << current_point << " at " << row << " row " << endl;
			}
			else {
				cout << " " << current_string << " ERROR at " << current_point << " at " << row << " row " << endl;
			}

			current_point = current_end - 1;
			word_num++;
		}
		else if (current_char != ' ') {		//������������ǣ�����Ҳ���ǿո���ô
			current_string = string(1,current_char);
			cout << current_string << "		Error			Error			" << "(" << row << ", " << word_num << ")" << endl;
			word_num++;
		}
		
	}
	row += 1;
	return(row);
}

TireNode * Lex::BuildKeyLib(const string key_lib[]) {
	TireNode *root = new TireNode();
	for (int i = 0; i < 8; i++) {		//??�˴���֪��������ַ��������Ԫ�ظ�����ֻ���Լ�������һ��
		TreeAdd(root, key_lib[i].c_str(), i);
	}
	return root;
}


int main(){

	Lex lexical;
	string filename;
	ifstream infile;
//	ofstream outfile;
	cout << "input the filename of the test file: ";
	cin >> filename;
//	filename = "test.txt";
	infile.open(filename.c_str());
	while (!infile) {
		cout << "error" << filename <<" and try again "<<endl;
		cin >> filename;
		infile.open(filename.c_str());
	}
	KeyRoot = lexical.BuildKeyLib(k_lib);	//����һ���ؼ��ֵ��ֵ������ڲ�ѯ

	int ROW = 1;
	while (getline(infile, current_line)) {		//���濪ʼһ��һ�е�ɨ��ÿ����
		ROW = lexical.run(ROW);
	}

	infile.close();
	

	system("pause");
	return 0;
}