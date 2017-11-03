 #include<cstdlib>
 #include<vector>
 #include<string>
 #include"stack.h"
 #include<fstream>
 #include<stdio.h>
#include <iomanip>
 using namespace std;
 
 typedef map<pair<char, char>, string> TypeOfTab;

	vector<string> raw_produces;	//输入的规定文法集
	string in_str = "i*i+i#";		//输入的待测试字符串

 bool formula::isValid(const string &str){
	// cout << str.length() << endl;
 	if(str.length()<4){
	//	cout << "grammer fatal1"<<endl;
 		return false;
	 }
	if(!(str[1]=='-'&&str[2]=='>')){
	//	cout << "grammer fatal2"<<endl;
		return false;
	}
	if(!isupper(str[0])){
	//	cout << "grammer fatal3"<<endl;
		return false;
	}
	else {
	//	cout << "true" << endl;
		return true;
	}
 }
 
 formula::formula(const string &str){
	// cout << str << endl;
 	this->isEntire =false;
 	if(isValid(str)){
		this->produce = str;
		this->parse();
	}
	else {
	//	cout << "grammer is fatal!" << endl;
	}
 }
 bool formula::parse(){
 	this->theLeft = this->produce[0];
 	for(unsigned int i=0;i<produce.length();i++ ){
 		char c = produce[i];
 		if(isupper(c))	Vn.insert(c);
 		else if((c=='-'&&produce[i+1]=='>')){
 			i++;
		 }
		else if(c!='|') Vt.insert(c);
	 }
	 for(unsigned int i=3;i<produce.length();i++){
	 	unsigned int j;
	 	for(j=i+1;j<produce.length() && produce[j]!='|' ;j++);
	 		char theLeft = produce[0];
	 		this->HouXuanJi.insert( cut(i,j));
	 		i=j;
	 }
	 this->isEntire =true;
 	return true;
 }
 
 void FenXiTab::PrintTab() {
	 
 }


 bool LL1::G_add(const formula &form) {		//!!这里需要注意防止在不同句法（不同行）中，声明同一个非终结符的不同产生式形式
											//此时，就要将同一非终结符的不同产生式补充到该同一非终结符的候选集中
	 if (form.isEntire) {
		 int flag = 0;
		 if (!Grammer.empty()) {
			 for (auto i : Grammer) {			//!! 此时需要注意string类的指向原理，直接用==比较两个字符串的内容可能会出错
				 if (i.theLeft == form.theLeft) {
					 i.HouXuanJi.insert(form.HouXuanJi.begin(), form.HouXuanJi.end());
					 for (auto str : form.HouXuanJi) {
						 i.produce += ('|' + str);
					 }
					 flag = 1;
				 }
			 }
			 Grammer.push_back(form);
		 }
		 if (Grammer.empty() || flag == 1) {		//如果之前没有收录这个非终结符的文法
			 Grammer.push_back(form);
		 }
		 this->Vn.insert(form.Vn.begin(), form.Vn.end());
		 this->Vt.insert(form.Vt.begin(), form.Vt.end());
		 return true;
	 }
	 else
		 return false;
 }

 void LL1::PrintInfo() {
	 cout<<"输入串：" << in_string<< endl;
	 cout << "规定文法: " << endl;
	 for (auto prods : Grammer) {
		 cout << prods.produce << endl;
	 }
	 cout << "first集：" << endl;
	 for (auto f : FIRST) {
		 cout << f.first << " : { ";
		 for (auto ff : f.second) {
			 cout << ff << " ";
		 }
		 cout << " } " << endl;
	 }

	 cout << "follow集：" << endl;
	 for (auto f : FOLLOW) {
		 cout << f.first << " : { ";
		 for (auto ff : f.second) {
			 cout << ff << " ";
		 }
		 cout << " } " << endl;
	 }

	 Index_Tab.PrintTab();

 }


set<char> LL1::first(const string &str){

	if ((str.length() == 1)) {
		//如果本身是终结符
		if (this->Vt.find(str[0]) != this->Vt.end()){
			return set<char>({ str[0] });
		}
		//如果属于非终结符
		else  {
			if (FIRST[str[0]].size() != 0)	return FIRST[str[0]];
			//如果只是一个非终结符，那就在产生式里查找它的产生式，并进行生成first集 
			for (auto g : Grammer) {
				if (g.theLeft == str[0]) {
					set<char> f = first(g.produce);
					FIRST[str[0]].insert(f.begin(), f.end());
					return FIRST[str[0]];
				}
			}
		}
	}
	 else if (str.length() == 0) {
		 return set<char>({ '$' });
	 }
	 //如果是候选式
	 else {
		 formula form = formula(str);
		 //如果是产生式
		 if (form.isEntire) {
			 if (FIRST[form.theLeft].size() != 0)
				 return FIRST[form.theLeft];
			 else {
				 for (auto h : form.HouXuanJi) {
					 set<char> f = first(h);
					 FIRST[form.theLeft].insert(f.begin(), f.end());
				 }
				 return FIRST[form.theLeft];
			 }
		 }
			 set<char> a;
			 for (unsigned int i = 0; i < str.length(); i++) {
				 set<char> f = first(string(1, str[i]));		//将文法串分割成一位位，逐个求first集合
				 if ((f.find('$') != f.end()) && (i != str.length() - 1)) {		//若在前i项中每个字符的first都有空集，那就一直继续，知道第i项没有空集时，可以将第i项的first放进去
					 f.erase(f.find('$'));
					 a.insert(f.begin(), f.end());
				 }
				 else {		//表达式直接是$的情况，所有字符的first都存在$的情况，以及第一项就不存在$的情况，和第i项不存在$的情况。
					 a.insert(f.begin(), f.end());
					 break;		//!!之前一直没有加break导致程序陷入死循环的bug
				 }
			 }
			 return a;
	 }
 }


 //!!加入判断一个字符串是否能推到空集的函数,一句话可以完成
void LL1::follow(const formula &form) {

	 if (form.isEntire) {		//如果合法，继续
		 char current_left = form.theLeft;
			// size = FOLLOW[current_left].size();		//相当于每轮的开始前和结束后的尺寸情况比较，看是否有变化
			 for (auto pro : Grammer) {		//再从头开始一个循环，开始找每次该终结符出现的所有位置，然后将该非终结符分别按序求出follow集
				 for (auto houxuan : pro.HouXuanJi) {	//先将候选集中的产生式一条一条的分析
					 if (houxuan.find(string(1, current_left)) != string::npos) {		//若在某个候选集中找到该非终结符，就记录下位置
						 int pos = houxuan.find(string(1, current_left));
						 string tail_piece;
						 tail_piece = string(houxuan.begin() + pos +1 , houxuan.end());		//相邻的尾片段
						 //cout << tail_piece << endl;
						 set<char> fos = first(tail_piece);
						 if (tail_piece == string(1, ')')) {
							 ;
						 }
						 FOLLOW[current_left].insert(fos.begin(), fos.end());	//先处理条件2
						 if (FOLLOW[current_left].find('$') != FOLLOW[current_left].end()) {		//如果first中含有'$'，那就剔除
							 FOLLOW[current_left].erase(FOLLOW[current_left].find('$'));
							 if (FOLLOW[pro.theLeft].size() != 0) {
								 FOLLOW[current_left].insert(FOLLOW[pro.theLeft].begin(), FOLLOW[pro.theLeft].end());		//此时就要求原始的文法必须是非递归的
							 }
						 }
						 
					 }
				 }
			 }
		 //cout << current_left << " : ";
		 //for (auto i : FOLLOW[current_left]) {
			// cout << " " << i ;
		 //}
		 //cout << endl;
	 }
	 return ;
 }


 //
 void LL1::analyze() {
	 mark_stack.push_back('#');
	 mark_stack.push_back(Grammer[0].theLeft);

	 int step = 0;
	 //int m_point, w_point;
	 bool trans = false;
	 string next_substitute, next_pro;
	 vector<char>::iterator w_it = in_words.end() - 1;	//w_it表示指向保存输入串的栈的栈顶指针
	 vector<char>::iterator m_it = mark_stack.end() - 1;	//m_it表示指向符号栈的栈顶

	 while (*m_it!='#') {

		 cout << step;
		 cout << "\t\t\t";
		 for (auto m : mark_stack) {
			 cout << m;
		 }
		 cout << "\t\t\t";
		 for (int i = in_words.size() - 1; i > 0; i--) {
			 cout << in_words[i] << ' ';
		 }
		 if (in_words.size() < 5) {
			 cout << "\t";
		 }
		 cout << "\t";
		 if (trans) {
			 trans = false;	
			 cout.width(16);
			 cout << next_pro<<"\t\t\t";
		 }
		 step++;
		 //开始匹配，如果有，将在下一次循环中替换并输出，所以要设置几个变量，和一个替换标志
		  //如果符号栈栈顶是非终结符，进入索引并设置替换
		 if (Vn.find(*m_it) != Vn.end()) {
			 if (Index_Tab.index.find(make_pair(*m_it, *w_it)) != Index_Tab.index.end()) {
				 if ((Index_Tab.index[(make_pair(*m_it, *w_it))]).empty() ){
					 int error_num = 3;	//??错误处理
					 Exception(error_num);
					 return;
					}
				trans = true;	//下次要替换
				next_substitute = Index_Tab.index[(make_pair(*m_it, *w_it))];
				next_pro = string(1,*m_it) + "-" + ">" + next_substitute ;
				mark_stack.pop_back();
				if (next_substitute.length() > 1) {
					for (int i = next_substitute.length()-1; i >= 0; i--) {
						mark_stack.push_back(next_substitute[i]);
					}
				}
				else if (next_substitute.length() == 1 && next_substitute[0] == '$') {
				}
				else if (next_substitute.length() == 1 && Vt.find(next_substitute[0]) != Vt.end()) {
					mark_stack.push_back(next_substitute[0]);
				}
			}
		}
		//如果是终结符，进入对比并决定是否弹栈
		else if (Vt.find(*m_it) != Vt.end()) {
			if (*w_it == *m_it) {
				mark_stack.pop_back();
				in_words.pop_back();
			}
			else {
				int err_num = 1;
				Exception(err_num);			//??提示出错,完善这里
				return;
			}
		}
		w_it = in_words.end() - 1;
		m_it = mark_stack.end() - 1;
		cout << endl;
	}
	 cout << step << "\t\t\t" << *m_it << "\t\t\t" << *w_it << "\t\t\t    " << next_pro <<"\t\t"<< endl;
}

 void LL1::Exception(int error_num) {
	 switch (error_num){
		 case 1: cout << "error" << endl; break;
		 case 2: cout << "error" << endl; break;
	 }
 }


 void LL1::run(){
 	//输入文法规则
	 for (auto str:raw_produces) {
		 formula fo = formula(str);
		// cout << fo.isEntire << endl;
		 G_add(fo);
		 //if (!G_add(formula(*it))) {
			// int error_num = 5;		//?? 语法添加错误
			// Exception(error_num);
			// break;
		 //}
	 }
	//输入待识别的字符串到一个栈里
	 this->in_string = in_str;
	 this->in_words.push_back('#');
	 for (int i = in_string.length()-1; i>=0; i--) {		//以倒序的形式存放
		 in_words.push_back(in_str[i]);
	 }

	//进入程序的主要部分
	 //先建立first集
	 for (auto i : this->Grammer) {
		 //cout <<endl<< i.produce << endl;
		 first(i.produce);
		 //FOLLOW.insert(make_pair(i, follow(string(1, i))));
	 }
	 //建立follow集
	 FOLLOW[Grammer[0].theLeft].insert('#');	//第一步，默认第一行的左边是开始符号，第一步将开始符号的follow中放入#


	 //debug1 发现follow都缺少）项,问题出在写的代码只有判断first集中含有$才会加入first，而一般情况下则没有加入，导致都没有）
	 for (auto gg : Grammer) {	//为了检测每个非终结符的follow集不在增大
		 int size = 0;
		 while (FOLLOW[gg.theLeft].size() != size) {
			 size = FOLLOW[gg.theLeft].size();
			 //因为一个变动会引起整体变动的，所以如果改变就要再加一个循环，所有都要改变
			 for (auto form : Grammer) {
				 follow(form);
			 }
		 }
	 }

	 //再建立分析表
	 FenXiTab Tab;
	 for (auto G : Grammer) {
		 for (auto houxuan : G.HouXuanJi) {
			 for (auto term : first(houxuan)) {
				 if (term != '$') {
					 Tab.index.insert(make_pair(make_pair(G.theLeft, term), houxuan));
				 }
				 else {
					 for (auto b : FOLLOW[G.theLeft]) {
						 Tab.index.insert(make_pair(make_pair(G.theLeft, b), houxuan));
					 }
				 }
			 }
		 }
	 }
	 this->Index_Tab = Tab;
	 if (Vt.find('$') != Vt.end()) {
		 Vt.erase(Vt.find('$'));
	 }
	 Vt.insert('#');			//因为输入串可能会有#而不是$，这样才能有效的进行索引
	 PrintInfo();

	 //再开始根据输入串进入分析预测
	 analyze();

	//输出结果
	   
 }
 

 int main(){
 	string g_filename;
 	ifstream infile;
 	ofstream outfile;
 	
 	//cin>>g_filename;
	g_filename ="grammer.txt";
	infile.open(g_filename);
	while(!infile){
 		cout<<"fail opening the g_file !"<<endl;
 		cin>>g_filename;
	 }
	 string current_line;

	 while(getline(infile, current_line)){
	//	 cout << "success" << endl;
		 cout << current_line << endl;
	 	raw_produces.push_back(current_line);
	 }
 	
	 LL1 ll_parser;
	 ll_parser.run();
 	
	 infile.close();
	 system("pause");
 	return 0;
 }
