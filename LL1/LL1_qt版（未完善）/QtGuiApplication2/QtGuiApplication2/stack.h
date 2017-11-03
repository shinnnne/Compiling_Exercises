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
 		string produce;		//����ʽ 
 		set<char> Vn;			//!!ѡ��set�����ݽṹ��set�������ظ�Ԫ�ص����Ըպ�Ϊ������first��follow���ṩ�˼���ı���
 		set<char> Vt;		//�ս���ͷ��ս�� 
 		char theLeft;		//����ʽ���
		string cut(int i, int j){
			return string(produce.begin()+i, produce.begin()+j);	//����i������j 
		}
		set<string> HouXuanJi;
		bool isEntire;			//�����ж���������ʽ�Ƿ�Ϸ����ֽ���� 

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
 		
 		//bool addProd(const string & str);	//��Ӳ���ʽ
		void PrintTab();			//չʾ������ 
 	//	TypeOfTab buildTab();
 	
 }; 
 
 //�����࣬�����ķ����������ַ�����������first����follow�� ���Լ�����ʱ�õ���ջ 
 class LL1{
 	private:
 		vector<formula> Grammer;			//������¼������õ������ָ���ķ�
		map<char, set<char>> FIRST;			//������¼���е��ַ���Ӧ��first����
		map<char, set<char>> FOLLOW;		//������¼���е��ַ���Ӧ��follow����
 		set<char> Vn;						//������¼���еķ��ս����
 		set<char> Vt;						//............�ս����
		string in_string;					//������Ҫʶ������봮
		FenXiTab Index_Tab;
		
		vector<char> in_words;				//���봮��ÿһλ����ʽ�������ջ�У��������ķ���Ԥ��Ҫ��
		vector<char> mark_stack;			//����ջ
		

 	public:
 		friend class FenXiTab;
 		friend class formula;
 		
 		bool G_add(const formula & form);	//���һ���ķ�;
	
		void PrintInfo();		//��ӡ�����Ϣ 
 	//	void PrintText();		//������봮���� 
 	
	// 	void pushStack();		//�����봮����ջ�� 
 	//	void PrintStack();		//���ջ���� 

 		set<char> first(const string &s);
 		void follow(const formula &form);
 		
		void analyze(QtGuiApplication2& w);			//���� 
		void Exception(int num);		//������ 
 		void run(QtGuiApplication2 &w);				//���� 
 	
 	
 }; 

#endif
