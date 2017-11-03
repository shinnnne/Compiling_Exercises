#ifndef HEAD_H
#define HEAD_H


#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<string>
#include<vector>
#include<set>
#include<map>
#include<fstream>

using namespace std;


class formula {
	friend class item;
	friend class LR1;
private:
	char theLeft;
	string theRight;

	friend bool operator == (const formula &a, const formula  &b) {		//���������أ��涨����find��������ʱ�Ƚϵı�׼
		return a.theLeft == b.theLeft && a.theRight == b.theRight;
	}
	friend bool operator == (const formula &a, char c) {
		return a.theLeft == c;
	}

public:
	set<char> lookaheads;	//չ������

	//string replaceAll(const string &str, const string from, const string to);
	//string showStr() const;		//��ʾ���ı䴫��Ĳ������Է���ֵ��������,ֻ��ʹ�ö����е�const��Ա���Է�const��Ա�ǲ���ʹ�õ�
	formula(const string &str);
	formula(const char &left, const string &right, const set<char>& llookaheads) {
		theLeft = left;
		theRight = right;
		lookaheads = llookaheads;
	}
};


class item {
	friend class LR1;
	friend class formula;
private:
	bool form_isValid(const string &str);
	vector<formula> forms;
	set<char> Vn;
	set<char> Vt;			//��set���ܻ�����Զ����򣬺�������˳�򲻷���
	set<char> Symbol;
	friend bool operator == (const item&a, const item &b) {		//���صȺţ�˵���Ƚϵı�׼
		if (a.forms.size() != b.forms.size())	return false;	//���ǱȽϴ�С
		else {
			for ( auto f : a.forms) {		//Ȼ�����ÿ������ʽ
				auto it = find(b.forms.begin(), b.forms.end(), f);	//���Է���û�д˲���ʽ
				if ( it ==b.forms.end()) {
					return false;
				}
				else if (f.lookaheads != (*it).lookaheads) {	//�ڱȽϺ�ѡ��
						return false;
				}
			}
			return true;
		}
	}

public:
	void add(const string &form);			//��״̬Ϊ��λ������״̬�����в���ʽ�ķ������������Ӧ�ļ���
//	void show() const;

};


//����LR1��
class LR1 {
private:
	item Grammer;
	vector<char> in_words;		//�����ַ�����ջ

	vector<item> normal_set;	//�淶�弯
	map<char, set<char>> FIRST;		//first��
	map<char, set<char>> FOLLOW;		//follow��

	map<pair<int, char>, int> GOTO;		//goto���飬״̬���Ϸ��ţ�������һ��״̬
	map<pair<int, char>, pair<string, int>> ACTION;	//action����

	vector<int> status;		//״̬ջ
	vector<char> parse;		//����ջ

	item closure(item i);		//�����Ŀ�ıհ�
	item go_to(const item &i, char X);		//��I����X�������Ŀ��
	void dfa_items();	//����Ŀ��״̬��������
	void showStrStack();		//��ʾ����ջ
	void showStatusStack();
	void showParseStack();

public:
	set<char> first(const string &str);	//��first��
	//void follow();		//��follow��

	void add(const string &str);		//��Ӳ���ʽ
	void build();		//����Action.goto��
	void showTable();	//��ӡLR������
	// debug();
	void loadStr(const string &str);		//��ȡ���봮
	void analyze();		//LR(1)����
	void showGrammer();	//��ʾ������ķ�����ʽ
	//void drawGrapgh();		//����DFA
	//void generateDot();		//
	void run(const string& str);
};

#endif









