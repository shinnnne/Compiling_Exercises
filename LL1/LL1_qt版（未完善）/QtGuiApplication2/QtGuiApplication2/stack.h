#ifndef STACK_H
#define STACK_H
	
#include<iostream>
#include<string>
#include<cstdio>
#include<set>
#include<map>
#include "QtGuiApplication2.h"
using namespace std;
typedef map<pair<char, char>, string> TypeOfTab;

 class formula{
 	private:
 		string produce;		//产生式 
 		set<char> Vn;			//!!选用set的数据结构，set不容许重复元素的特性刚好为后面求first和follow等提供了极大的便利
 		set<char> Vt;		//终结符和非终结符 
 		char theLeft;		//产生式左端
		string cut(int i, int j){
			return string(produce.begin()+i, produce.begin()+j);	//包含i不包含j 
		}
		set<string> HouXuanJi;
		bool isEntire;			//用来判断整个产生式是否合法并分解完毕 

 	public:
 		
 		friend class LL1;
 		bool isValid(const string &str);
 		formula(const string &str);
		
 		bool parse();
 	
 };
 
 class FenXiTab{
 	private:	
 		map<pair<char, char>, string> index;
 		
 	public:
 		friend class LL1;
 		
 		//bool addProd(const string & str);	//添加产生式
		void PrintTab();			//展示分析表 
 	//	TypeOfTab buildTab();
 	
 }; 
 
 //整个类，包括文法规则，输入字符串，分析表，first集和follow集 ，以及分析时用到的栈 
 class LL1{
 	private:
 		vector<formula> Grammer;			//用来记录被整理好的输入的指定文法
		map<char, set<char>> FIRST;			//用来记录所有的字符对应的first集合
		map<char, set<char>> FOLLOW;		//用来记录所有的字符对应的follow集合
 		set<char> Vn;						//用来记录所有的非终结符集
 		set<char> Vt;						//............终结符集
		string in_string;					//完整的要识别的输入串
		FenXiTab Index_Tab;
		
		vector<char> in_words;				//输入串以每一位的形式倒序存在栈中，方便后面的分析预测要用
		vector<char> mark_stack;			//符号栈
		

 	public:
 		friend class FenXiTab;
 		friend class formula;
 		
 		bool G_add(const formula & form);	//添加一条文法;
	
		void PrintInfo();		//打印相关信息 
 	//	void PrintText();		//输出输入串内容 
 	
	// 	void pushStack();		//将输入串送入栈中 
 	//	void PrintStack();		//输出栈内容 

 		set<char> first(const string &s);
 		void follow(const formula &form);
 		
		void analyze(QtGuiApplication2& w);			//分析 
		void Exception(int num);		//错误处理 
 		void run(QtGuiApplication2 &w);				//运行 
 	
 	
 }; 

#endif
