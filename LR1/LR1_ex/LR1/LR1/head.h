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

	friend bool operator == (const formula &a, const formula  &b) {		//操作符重载，规定用于find函数查找时比较的标准
		return a.theLeft == b.theLeft && a.theRight == b.theRight;
	}
	friend bool operator == (const formula &a, char c) {
		return a.theLeft == c;
	}

public:
	set<char> lookaheads;	//展望符集

	//string replaceAll(const string &str, const string from, const string to);
	//string showStr() const;		//表示不改变传入的参数，对返回值不做限制,只能使用对象中的const成员，对非const成员是不能使用的
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
	set<char> Vt;			//用set可能会造成自动排序，和样例的顺序不符合
	set<char> Symbol;
	friend bool operator == (const item&a, const item &b) {		//重载等号，说明比较的标准
		if (a.forms.size() != b.forms.size())	return false;	//先是比较大小
		else {
			for ( auto f : a.forms) {		//然后对于每个产生式
				auto it = find(b.forms.begin(), b.forms.end(), f);	//看对方有没有此产生式
				if ( it ==b.forms.end()) {
					return false;
				}
				else if (f.lookaheads != (*it).lookaheads) {	//在比较候选集
						return false;
				}
			}
			return true;
		}
	}

public:
	void add(const string &form);			//以状态为单位，将该状态的所有产生式的符号整理收入对应的集中
//	void show() const;

};


//构造LR1表
class LR1 {
private:
	item Grammer;
	vector<char> in_words;		//输入字符串的栈

	vector<item> normal_set;	//规范族集
	map<char, set<char>> FIRST;		//first集
	map<char, set<char>> FOLLOW;		//follow集

	map<pair<int, char>, int> GOTO;		//goto数组，状态遇上符号，进入另一个状态
	map<pair<int, char>, pair<string, int>> ACTION;	//action数组

	vector<int> status;		//状态栈
	vector<char> parse;		//分析栈

	item closure(item i);		//求该项目的闭包
	item go_to(const item &i, char X);		//求I经过X到达的项目集
	void dfa_items();	//求项目集状态机？？？
	void showStrStack();		//显示输入栈
	void showStatusStack();
	void showParseStack();

public:
	set<char> first(const string &str);	//求first集
	//void follow();		//求follow集

	void add(const string &str);		//添加产生式
	void build();		//构造Action.goto表
	void showTable();	//打印LR分析表
	// debug();
	void loadStr(const string &str);		//读取输入串
	void analyze();		//LR(1)分析
	void showGrammer();	//显示输入的文法产生式
	//void drawGrapgh();		//画出DFA
	//void generateDot();		//
	void run(const string& str);
};

#endif









