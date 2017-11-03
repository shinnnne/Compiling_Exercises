 #include<cstdlib>
 #include<vector>
 #include<string>
 #include"stack.h"
 #include<fstream>
 #include<stdio.h>
#include <iomanip>
 using namespace std;
 
 typedef map<pair<char, char>, string> TypeOfTab;

	vector<string> raw_produces;	//����Ĺ涨�ķ���
	string in_str = "i*i+i#";		//����Ĵ������ַ���

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


 bool LL1::G_add(const formula &form) {		//!!������Ҫע���ֹ�ڲ�ͬ�䷨����ͬ�У��У�����ͬһ�����ս���Ĳ�ͬ����ʽ��ʽ
											//��ʱ����Ҫ��ͬһ���ս���Ĳ�ͬ����ʽ���䵽��ͬһ���ս���ĺ�ѡ����
	 if (form.isEntire) {
		 int flag = 0;
		 if (!Grammer.empty()) {
			 for (auto i : Grammer) {			//!! ��ʱ��Ҫע��string���ָ��ԭ��ֱ����==�Ƚ������ַ��������ݿ��ܻ����
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
		 if (Grammer.empty() || flag == 1) {		//���֮ǰû����¼������ս�����ķ�
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
	 cout<<"���봮��" << in_string<< endl;
	 cout << "�涨�ķ�: " << endl;
	 for (auto prods : Grammer) {
		 cout << prods.produce << endl;
	 }
	 cout << "first����" << endl;
	 for (auto f : FIRST) {
		 cout << f.first << " : { ";
		 for (auto ff : f.second) {
			 cout << ff << " ";
		 }
		 cout << " } " << endl;
	 }

	 cout << "follow����" << endl;
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
		//����������ս��
		if (this->Vt.find(str[0]) != this->Vt.end()){
			return set<char>({ str[0] });
		}
		//������ڷ��ս��
		else  {
			if (FIRST[str[0]].size() != 0)	return FIRST[str[0]];
			//���ֻ��һ�����ս�����Ǿ��ڲ���ʽ��������Ĳ���ʽ������������first�� 
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
	 //����Ǻ�ѡʽ
	 else {
		 formula form = formula(str);
		 //����ǲ���ʽ
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
				 set<char> f = first(string(1, str[i]));		//���ķ����ָ��һλλ�������first����
				 if ((f.find('$') != f.end()) && (i != str.length() - 1)) {		//����ǰi����ÿ���ַ���first���пռ����Ǿ�һֱ������֪����i��û�пռ�ʱ�����Խ���i���first�Ž�ȥ
					 f.erase(f.find('$'));
					 a.insert(f.begin(), f.end());
				 }
				 else {		//���ʽֱ����$������������ַ���first������$��������Լ���һ��Ͳ�����$��������͵�i�����$�������
					 a.insert(f.begin(), f.end());
					 break;		//!!֮ǰһֱû�м�break���³���������ѭ����bug
				 }
			 }
			 return a;
	 }
 }


 //!!�����ж�һ���ַ����Ƿ����Ƶ��ռ��ĺ���,һ�仰�������
void LL1::follow(const formula &form) {

	 if (form.isEntire) {		//����Ϸ�������
		 char current_left = form.theLeft;
			// size = FOLLOW[current_left].size();		//�൱��ÿ�ֵĿ�ʼǰ�ͽ�����ĳߴ�����Ƚϣ����Ƿ��б仯
			 for (auto pro : Grammer) {		//�ٴ�ͷ��ʼһ��ѭ������ʼ��ÿ�θ��ս�����ֵ�����λ�ã�Ȼ�󽫸÷��ս���ֱ������follow��
				 for (auto houxuan : pro.HouXuanJi) {	//�Ƚ���ѡ���еĲ���ʽһ��һ���ķ���
					 if (houxuan.find(string(1, current_left)) != string::npos) {		//����ĳ����ѡ�����ҵ��÷��ս�����ͼ�¼��λ��
						 int pos = houxuan.find(string(1, current_left));
						 string tail_piece;
						 tail_piece = string(houxuan.begin() + pos +1 , houxuan.end());		//���ڵ�βƬ��
						 //cout << tail_piece << endl;
						 set<char> fos = first(tail_piece);
						 if (tail_piece == string(1, ')')) {
							 ;
						 }
						 FOLLOW[current_left].insert(fos.begin(), fos.end());	//�ȴ�������2
						 if (FOLLOW[current_left].find('$') != FOLLOW[current_left].end()) {		//���first�к���'$'���Ǿ��޳�
							 FOLLOW[current_left].erase(FOLLOW[current_left].find('$'));
							 if (FOLLOW[pro.theLeft].size() != 0) {
								 FOLLOW[current_left].insert(FOLLOW[pro.theLeft].begin(), FOLLOW[pro.theLeft].end());		//��ʱ��Ҫ��ԭʼ���ķ������Ƿǵݹ��
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
	 vector<char>::iterator w_it = in_words.end() - 1;	//w_it��ʾָ�򱣴����봮��ջ��ջ��ָ��
	 vector<char>::iterator m_it = mark_stack.end() - 1;	//m_it��ʾָ�����ջ��ջ��

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
		 //��ʼƥ�䣬����У�������һ��ѭ�����滻�����������Ҫ���ü�����������һ���滻��־
		  //�������ջջ���Ƿ��ս�������������������滻
		 if (Vn.find(*m_it) != Vn.end()) {
			 if (Index_Tab.index.find(make_pair(*m_it, *w_it)) != Index_Tab.index.end()) {
				 if ((Index_Tab.index[(make_pair(*m_it, *w_it))]).empty() ){
					 int error_num = 3;	//??������
					 Exception(error_num);
					 return;
					}
				trans = true;	//�´�Ҫ�滻
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
		//������ս��������ԱȲ������Ƿ�ջ
		else if (Vt.find(*m_it) != Vt.end()) {
			if (*w_it == *m_it) {
				mark_stack.pop_back();
				in_words.pop_back();
			}
			else {
				int err_num = 1;
				Exception(err_num);			//??��ʾ����,��������
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
 	//�����ķ�����
	 for (auto str:raw_produces) {
		 formula fo = formula(str);
		// cout << fo.isEntire << endl;
		 G_add(fo);
		 //if (!G_add(formula(*it))) {
			// int error_num = 5;		//?? �﷨��Ӵ���
			// Exception(error_num);
			// break;
		 //}
	 }
	//�����ʶ����ַ�����һ��ջ��
	 this->in_string = in_str;
	 this->in_words.push_back('#');
	 for (int i = in_string.length()-1; i>=0; i--) {		//�Ե������ʽ���
		 in_words.push_back(in_str[i]);
	 }

	//����������Ҫ����
	 //�Ƚ���first��
	 for (auto i : this->Grammer) {
		 //cout <<endl<< i.produce << endl;
		 first(i.produce);
		 //FOLLOW.insert(make_pair(i, follow(string(1, i))));
	 }
	 //����follow��
	 FOLLOW[Grammer[0].theLeft].insert('#');	//��һ����Ĭ�ϵ�һ�е�����ǿ�ʼ���ţ���һ������ʼ���ŵ�follow�з���#


	 //debug1 ����follow��ȱ�٣���,�������д�Ĵ���ֻ���ж�first���к���$�Ż����first����һ���������û�м��룬���¶�û�У�
	 for (auto gg : Grammer) {	//Ϊ�˼��ÿ�����ս����follow����������
		 int size = 0;
		 while (FOLLOW[gg.theLeft].size() != size) {
			 size = FOLLOW[gg.theLeft].size();
			 //��Ϊһ���䶯����������䶯�ģ���������ı��Ҫ�ټ�һ��ѭ�������ж�Ҫ�ı�
			 for (auto form : Grammer) {
				 follow(form);
			 }
		 }
	 }

	 //�ٽ���������
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
	 Vt.insert('#');			//��Ϊ���봮���ܻ���#������$������������Ч�Ľ�������
	 PrintInfo();

	 //�ٿ�ʼ�������봮�������Ԥ��
	 analyze();

	//������
	   
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
