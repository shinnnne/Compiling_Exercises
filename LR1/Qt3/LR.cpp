#include"head.h"
#include "Qt3.h"
#include<QWidget>
#include<QtWidgets/qapplication.h>
#include<qstring.h>


using namespace std;

#define Extension '~'			//��װ���ع��ķ��Ŀ�ʼ����

//~
formula::formula(const string &in_string) {
	this->theLeft = in_string[0];
	this->theRight = string(in_string.begin() + 3, in_string.end());
}

//~
bool item::form_isValid(const string &str) {
	//��ʱ�뵽����������������������LL1������
	if (str.length() < 4) return false;
	else if (!isupper(str[0]))		return false;
	else if (!(str[1] == '-'&&str[2] == '>'))	return false;
	else return true;
}


//~
void item::add(const string &str) {
	if (!form_isValid(str)) {
		return;
	}

	//char left = str[0];
	for (unsigned int i = 0; i < str.length(); ++i) {		//��ʼ�����ս���ͷ��ս��
		if (isupper(str[i])) {
			Vn.insert(str[i]);
			Symbol.insert(str[i]);
		}
		else if (!(str[i] == '-' && str[i + 1] == '>' && (i++)) && str[i] != '|') {
			Vt.insert(str[i]);
			Symbol.insert(str[i]);
		}
	}

	for (unsigned int i = 3; i < str.length(); i++) {		//�����ѡ��,!!��һ�����������bug����Ϊ���i�ĳ�ֵ��Ϊ0
		unsigned int j;
		for (j = i + 1; j < str.length() && str[j] != '|'; ++j);
		formula form = formula(string(1, str[0]) + "->" + string(str.begin() + i, str.begin() + j));
		if (find(forms.begin(), forms.end(), form) == forms.end()) {			//����Ҳ����ظ���Ԫ�ؾͲ����ȥ��ȥ�أ�
			forms.push_back(form);
		}
		i = j;
	}
}


set<char> LR1::first(const string & str) {		//ԭ��ͬLL1
	if (str.length() == 0) {
		return set<char>({ '$' });
	}
	else if (str.length() == 1) {
		if (Grammer.Vt.find(str[0]) != Grammer.Vt.end() || str[0] == '#') {		//������ս��
			return set<char>({ str[0] });
		}
		else {
			if (FIRST[str[0]].size() != 0) return FIRST[str[0]];			//���first��Ϊ�գ�˵���Ѿ��������������ֱ�ӷ���
			else {
				for (vector<formula>::iterator it = Grammer.forms.begin(); it != Grammer.forms.end(); ++it) {
					if (*it == str[0]) {		//��it����ߺ�
												// ��ֱֹ����ݹ�,����ߵ�ʨ�Ӳ��ܳ������ұߵ�ʽ����
						size_t xPos = it->theRight.find(it->theLeft);
						if (xPos != string::npos) { // ����ҵ�X->aXb��������ֱ����ݹ����ʽ
							if (xPos == 0) continue; // X->Xb
							else { // X->aXb
								string a = string(it->theRight.begin(), it->theRight.begin() + xPos);
								if (first(a) == set<char>{'$'}) continue;
							}
						}
						set<char> f = first(it->theRight);
						FIRST[str[0]].insert(f.begin(), f.end());
					}
				}
				return FIRST[str[0]];
			}
		}

	}
	else { // first(X1X2X3X4)...
		set<char> ret;
		for (unsigned int i = 0; i<str.length(); ++i) {
			set<char> f = first(string(1, str[i])); // ���������first(Xi)��
			if (f.find('$') != f.end() && str.length() - 1 != i) { // ����@
				f.erase(f.find('$')); // ��ȥ@
				ret.insert(f.begin(), f.end()); // ����first����
			}
			else { // ��$�������һ��Xi������Ҫ����ȥ��
				ret.insert(f.begin(), f.end());
				break;
			}
		}
		return ret;
	}

}

//~
void LR1::add(const string &str) {
	Grammer.add(str);
}

//~
string LR1::showStatusStack() {
	string ret ="";
	for (vector<int>::iterator it = status.begin(); it != status.end(); it++) {
		//it--;
		char tmp[2];
		sprintf(tmp, "%d", *it);
		ret += tmp ;
	}
	return string(ret);
}
//~
string LR1::showParseStack() {
	string ret = "";
	for (vector<char>::iterator it = parse.begin(); it != parse.end(); it++) {
		ret+= *it;
	}
	return string(ret);
}
//~
string LR1::showStrStack() {
	string ret;
	for (vector<char>::iterator it = in_words.end(); it != in_words.begin();) {		//Ҳ������reverse_iterator�� �Ƿ�����������������������������
		it--;
		ret+= *it;
	}
	return string(ret);
}

//~
void LR1::analyze(Qt3 &win) {
	//cout << "analyze: \n";
	bool success = false;
	/*cout << "step\t";
	cout << "staS\t\t";
	cout << "anaS\t\t";
	cout << "strS\t\t";
	cout << "action\t\t" << endl;*/
	//�˴�����չʾ������
	QStandardItemModel *processTab = new QStandardItemModel();
	//processTab->setHorizontalHeaderItem(0, new QStandardItem(QObject::tr("step")));
	processTab->setHorizontalHeaderItem(0, new QStandardItem(QObject::tr("sta_stack")));
	processTab->setHorizontalHeaderItem(1, new QStandardItem(QObject::tr("ana_stack")));
	processTab->setHorizontalHeaderItem(2, new QStandardItem(QObject::tr("str_stack")));
	processTab->setHorizontalHeaderItem(3, new QStandardItem(QObject::tr("action")));

	win.ui.process_tab->setModel(processTab);

	int step = 0;
	while (!success) {
		//processTab->setItem(step, 0, new QStandardItem(QString::fromStdString(string(1, ('0' + step)))));
		// << step << "\t";
		processTab->setItem(step, 0, new QStandardItem(QString::fromStdString(showStatusStack())));
		//showStatusStack();
		//cout << "\t\t";
		processTab->setItem(step, 1, new QStandardItem(QString::fromStdString(showParseStack())));
		//showParseStack();
		//cout << "\t\t";
		processTab->setItem(step, 2, new QStandardItem(QString::fromStdString(showStrStack())));
		//showStrStack();
		//cout << "\t\t";

		int sTop = status.size() - 1;		//״̬ջջ��
		int iTop = in_words.size() - 1;		//�����ַ���ջ��
		pair<int, char> p = make_pair(status[sTop], in_words[iTop]);
		if (ACTION.find(p) == ACTION.end()) {	//�������������ַ�û�ж�Ӧ�Ķ����ͱ���
			processTab->setItem(step, 3, new QStandardItem(QString::fromStdString(string("no exist"))));
			//cout << "error, cant find the action " << endl;
			break;
		}
		pair<string, int> act = ACTION[p];
		if (act.first == "sj") {			//�����⵽���ƽ�����
			processTab->setItem(step, 3, new QStandardItem(QString::fromStdString(string("shift"))));
			//cout << "shift";
			status.push_back(act.second);
			parse.push_back(in_words[iTop]);
			in_words.pop_back();
		}
		else if (act.first == "rj") {	//�����⵽�ǹ�Լ����
			formula f = Grammer.forms[act.second];
			processTab->setItem(step, 3, new QStandardItem(QString::fromStdString((string("reduce: ") + f.theLeft +  string("->") + f.theRight))));			//~!!debugged�˴����ֱ����˫�������õ��ַ����������ʾ����
			//cout << "reduce:  " << f.theLeft << "->" << f.theRight; 

			if (f.theRight != "$")		//������ǿմ�����Ҫ�Ƚ�����ʽ�Ҷ˵������ַ���ջ
				for (unsigned int i = 0; i < f.theRight.size(); i++) {
					status.pop_back();
					parse.pop_back();
				}
			parse.push_back(f.theLeft);	//�Ҷ˳�ջ��˽�ջ
			status.push_back(GOTO[make_pair(status[status.size() - 1], f.theLeft)]);		//��Լ����Ҫ����goto����������һ��״̬��λ��
		}
		else if (act.first == "accept") {
			success = true;
			processTab->setItem(step, 3, new QStandardItem(QString::fromStdString(string("success"))));
			//cout << "success\t"; 
		}
		step++;
		//cout << endl;
	}
	if (!success) {
		processTab->setItem(step, 3, new QStandardItem(QString::fromStdString(string("error"))));
		//cout << "error\n";
	}
	//cout << '\n';

}


//~
item LR1::closure(item I) {
	if (I.forms.size() == 0)		return I;			//���ǷǷ���.ֱ�ӷ��ؾͺ�
	for (unsigned int i = 0; i < I.forms.size(); i++) {		//�Ӹ���Ŀ�ĵ�һ������ʽ��ʼ��հ�
		formula form = I.forms[i];
		unsigned int DotLoc = 0;		//��ͣ�ٵ��λ��
		if ((DotLoc = form.theRight.find('`')) != string::npos && DotLoc != form.theRight.length() - 1) {	//���û�����λ��
			char B = form.theRight[DotLoc + 1];
			if (Grammer.Vt.find(B) != Grammer.Vt.end()) {		//������ս��
				if (B == '$') {
					swap(I.forms[i].theRight[DotLoc], I.forms[i].theRight[DotLoc + 1]);	//���ָ����λ�ú���һλ
				}
				continue;		//����Ƿǿյ��ս��ֱ�������˴�ѭ������Ϊ��Ҫ����Ƿ��ս�������ıհ�
			}
			string f = string(form.theRight.begin() + DotLoc + 2, form.theRight.end());	//�ӷָ������һλ�ַ��ĺ��濪ʼ��ֱ��Ϊ��first������չ���ַ���׼��
			set<char> ff = {};
			for (auto look : form.lookaheads) {			//Ҫ���ÿһ��չ��������first
				set<char> fs = first(f + look);		//����first��B+չ�������е�ÿһ���ս��b���������ϵĶ��������2��
				ff.insert(fs.begin(), fs.end());		//�����first����ff�����Ҫ������
			}

			for (vector<formula>::iterator it = Grammer.forms.begin(); it != Grammer.forms.end(); it++) {
				if (*it == B) {		//�������B�Ķ�Ӧ�Ĳ���ʽ,����ĵȺž���������
					formula f = *it;
					if (f.theRight[0] == '$') {
						f.theRight = '`' + f.theRight;
						swap(f.theRight[0], f.theRight[1]);		//!!���ָ���λ�ú���һλ
					}
					else {
						f.theRight = "`" + f.theRight;
					}
					//���ԭ���ıհ�û�У��ͼ������
					vector<formula>::iterator iit = find(I.forms.begin(), I.forms.end(), f);
					if (iit != I.forms.end()) {
						iit->lookaheads.insert(ff.begin(), ff.end());
					}
					else {
						f.lookaheads.insert(ff.begin(), ff.end());
						I.forms.push_back(f);
					}
				}
			}
		}
	}
	return I;
}

item LR1::go_to(const item & iitem, char x) {	//��&��Ϊ��ֱ�����ø�ֵ�������öิ��һ�ݶ�ռ�ռ䣬��const����Ϊ�˷�ֹ���޸�
	item j;
	if (iitem.forms.size() == 0 || x == '$')		return j;		//�����ĿΪ�գ��ͷ��ؿ���Ŀ

																	//��Ҫ���Ѽ���״̬�е���Щ����ʽ�ܽ���x
	for (auto p : iitem.forms) {// ����iitem�е�ÿ����Ŀ
		string right = p.theRight;
		unsigned int DotLoc = right.find('`');	//�ҵ����λ��
		if (right[DotLoc + 1] == x) {
			swap(right[DotLoc], right[DotLoc + 1]);		//!!����swap���ָ����λ�ú���һλ
			j.forms.push_back(formula(p.theLeft, right, p.lookaheads));
		}
	}
	//�ҵ�֮��Ͷ���Щ����ʽ��հ�
	return closure(j);
}


//~
void LR1::dfa_items() {
	item ini;
	ini.forms.push_back(formula(Extension, "`" + string(1, Grammer.forms[0].theLeft), { '#' }));		//�����ع��ķ�
	normal_set.push_back(closure(ini));	//�����ɵĳ�ʼ���ķ����ɱհ������뵽normal_set����

	size_t size = 0;		//
	while (size != normal_set.size()) {		//ѭ����հ�ֱ����Ŀ�弯��������
		size = normal_set.size();
		for (unsigned int i = 0; i < normal_set.size(); i++) {		//���ڵ�ǰ�淶�弯�е�ÿ����Ŀ
			item current_item = normal_set[i];
			for (auto sym : Grammer.Symbol) {		//�����﷨���е�ÿ����Ч���ţ������ս���ͷ��ս����
				item next = go_to(current_item, sym);		//����goto����
				if (next.forms.size() != 0) {	//���ɵ�goto������Ϊ��,�����goto���Ƿ��Ѿ����ڣ������ھͲ���
					auto it = find(normal_set.begin(), normal_set.end(), next);			//������Ҫ�涨һ�ֱȽ�item�ı�׼����Ҫ�����н������ز�����
					if (it != normal_set.end()) {
						GOTO[make_pair(i, sym)] = it - normal_set.begin();		//�����λ���������ҵ���˵��ֱ�ӽ�goto��ָ���Ǹ�������λ��
					}
					else {//�������ɵ�goto����������Ŀ�弯
						normal_set.push_back(next); //��ô�ͽ����ɵ��µıհ����뵽��Ŀ�弯��
						GOTO[make_pair(i, sym)] = normal_set.size() - 1;		//ͬ�ϣ������λ����������goto��ָ������������λ��
					}
				}
			}
		}
	}
}
//~
void LR1::showGrammer() {
	//cout << "Grammer:\n";
	for (const auto & f : Grammer.forms) {
		//cout << f.theLeft << "->" << f.theRight << endl;
		//cout  << "->" << f.theRight << endl;
	}
	//cout << endl;
}


//����ACTION GOTO��
void LR1::build(Qt3 &win) {
	QStandardItemModel *norm_list = new QStandardItemModel();
	win.ui.normal_list->setModel(norm_list);

	dfa_items();		//����Ŀ��״̬��dfa		//!!debugged@ �ѽ��

	for (unsigned int i = 0; i < normal_set.size(); i++) {		//���ڹ淶�弯��ÿ��״̬
		QStandardItem *n_num = new QStandardItem(QString("[status")+QString::number(i, 10)+QString("]"));		//��ӡ״̬��
		norm_list->appendRow(n_num);

		const item & iitem = normal_set[i];
		for (auto form : iitem.forms) {		//����ÿ��״̬�µ���Ŀ����ÿ����
			//���¿�ʼ��ӡÿһ��״̬�����в���ʽ
			string l_set = "";
			int count = 0;
			for (set<char>::iterator it = form.lookaheads.begin(); it != form.lookaheads.end();it++) {
				count++;
				if(count!=1)
					l_set += '\/';
				l_set += *it;
			}
			QStandardItem *n_pro = new QStandardItem(QString::fromStdString(string(string(1,form.theLeft)+" => "+form.theRight+", "+l_set)));
			norm_list->appendRow(n_pro);

			//��ӡ��������ʼ����������
			unsigned long DotLoc = form.theRight.find('`');
			if (DotLoc < form.theRight.length() - 1) {		///����������һ��,˵�����ܽ����ƽ������� ������α�������1
				char x = form.theRight[DotLoc + 1];
				if (Grammer.Vt.find(x) != Grammer.Vt.end() && GOTO.find(make_pair(i, x)) != GOTO.end()) {	//������ս��������goto���д��ڸ�״̬�������ս����������¼
					int j = GOTO[make_pair(i, x)];
					ACTION[make_pair(i, x)] = make_pair("sj", j);		//����ת��������¼����
				}
			}
			else {
				if (form == formula(Extension, string(1, Grammer.forms[0].theLeft) + '`', {}) && form.lookaheads == set<char>({ '#' })) { // �����Ƿ��� S'->S.,#������Ǿ�ת����� acction[i, #] = acc��������α��ϵ�����3
					ACTION[make_pair(i, '#')] = make_pair("accept", 0);
				}
				else if (form.theLeft != Extension) {		//�������α��ϵ�����2
					string right = form.theRight;
					right.erase(DotLoc, 1);		//ɾ����,Ϊ���湹�������Ĳ���ʽ���ڽ���������׼��
					for (auto l : form.lookaheads) {
						vector<formula>::iterator it = find(Grammer.forms.begin(), Grammer.forms.end(), formula(form.theLeft, right, set<char>{}));
						if (it != Grammer.forms.end())		//������,ȷ��һ�¹�ԼҪ�����Ĳ���ʽ��λ��
							ACTION[make_pair(i, l)] = make_pair("rj", it - Grammer.forms.begin());
					}
				}
			}
		}
		if (Grammer.Vt.find('$') != Grammer.Vt.end()) {		//��ʱӦ�ý���������'$'�Ƴ�������'#'���Ա���湹���������
			Grammer.Vt.erase(Grammer.Vt.find('$'));
			Grammer.Symbol.erase(Grammer.Symbol.find('$'));
		}
		Grammer.Vt.insert('#');
		Grammer.Symbol.insert('#');
	}
}

void LR1::showTable(Qt3 &win) {
	//�����QT��Ԫ��
	QStandardItemModel *p_tab = new QStandardItemModel();
	//p_tab->setHorizontalHeaderItem(0, new QStandardItem(QObject::tr("ACTION")));
	//p_tab->setHorizontalHeaderItem(Grammer.Vt.size(), new QStandardItem(QObject::tr("GOTO")));
	//p_tab->setHorizontalHeaderItem(2, new QStandardItem(QObject::tr("in_words")));
	//p_tab->setHorizontalHeaderItem(3, new QStandardItem(QObject::tr("produce")));

	//cout << "Analyze_Tab" << endl;
	//cout << "Vt\t";
	//cout << "step\t";
	win.ui.parse_tab->setModel(p_tab);
	int count = 0;
	for (const auto & X : Grammer.Vt) {		//���ڱ��е�ÿһ���ս��
		if (X != '#') {
			p_tab->setHorizontalHeaderItem(count++, new QStandardItem(QObject::tr((string(1, X).c_str()))));
			//p_tab->setItem(0, count++, new QStandardItem(QString::fromStdString(string(1, X))));
			//cout << X << "\t";
		}
	}
	p_tab->setHorizontalHeaderItem(count++, new QStandardItem(QObject::tr((string(1, '#').c_str()))));
	//p_tab->setItem(0,count++, new QStandardItem(QString::fromStdString(string(1, '#'))));
	//cout << "#\t"; // #�ŵ����һ����ʾ������
	 //cout << "Vn:";
	 //int firstComma = 0; // �����һ������
	for (const auto & X : Grammer.Vn) {
		p_tab->setHorizontalHeaderItem(count++, new QStandardItem(QObject::tr((string(1, X).c_str()))));
		//p_tab->setItem(0, count++, new QStandardItem(QString::fromStdString(string(1, X))));
		//cout << X << "\t";
	}
	//cout << endl << endl;
	for (unsigned int i = 0; i<normal_set.size(); ++i) {		//���������弯�е�ÿһ��״̬
																//printf("%s[", i == 0 ? " " : ", ");
		int c_count = 0;														//int firstComma = 0;
		//cout << i << '\t';
		for (const auto & X : Grammer.Vt) {		//���ڸ�״̬������ÿһ���ս��
			
			if (X != '#') {
				pair<int, char> p = make_pair(i, X);
				if (ACTION.find(p) != ACTION.end()) {		//�����action���ҵ���Ӧ�Ķ���
					pair<string, int> res = ACTION[p];
					p_tab->setItem(i,c_count++ , new QStandardItem(QString::fromStdString(res.first)+QString::number(res.second,10)));		//Qt
					//cout << res.first << res.second << "\t";
				}
				else c_count++;
					//cout << " \t";
			}
		}
		pair<int, char> p = make_pair(i, '#');		//ר��Ϊ#��������չʾ
		if (ACTION.find(p) != ACTION.end()) {
			pair<string, int> res = ACTION[p];
			//cout << res.first;
			if (res.first != "accept") {
				p_tab->setItem(i, c_count++, new QStandardItem(QString::fromStdString(res.first) + QString::number(res.second, 10)));
			}
			//show += res.second;
			else p_tab->setItem(i, c_count++, new QStandardItem(QString("accept")));
				//cout << res.second << '\t';
			//else cout << "\t";
		}
		else c_count++;
			//cout << " \t";

		for (const auto & X : Grammer.Vn) {		//����ÿһ�����ս��
			pair<int, char> p = make_pair(i, X);
			if (GOTO.find(p) != GOTO.end())
				p_tab->setItem(i, c_count++, new QStandardItem(QString::number(GOTO[make_pair(i, X)],10)));
				//	cout << GOTO[make_pair(i, X)] << "\t";
			else		c_count++;
				// cout << " \t";
		}

		//cout << endl;
	}

}



//~���ڽ����봮���ַ�����ʽת����һ�����ַ�������ջ��
void LR1::loadStr(const string &str) {
	in_words.push_back('#');
	status.push_back(0);
	for (int i = str.length() - 1; i >= 0; --i)
		in_words.push_back(str[i]);
}

//
void LR1::run(const string &in_string, Qt3 &win) {

	//�����ķ�����
	//string in_string;
	//cin >> in_string;
	//in_string = "";

	showGrammer();		//debugged@û������
	//cout << endl;

	build(win);
	showTable(win);
	//cout << endl;

	//drawGrapgh();
	//cout << endl;


	//������������봮����
	//cin >> in_string;
	if (in_string == "#" || in_string.size() == 0)return;
	loadStr(in_string);
	analyze(win);
	//cout << endl;
}



int main(int argc, char *argv[]) {
	LR1 lr1;
	QApplication a(argc, argv);
	Qt3 w;

	//�����ķ�
	ifstream filein;
	ofstream fileout;
	string g_filename("grammer.txt");
	filein.open(g_filename);
	while (!filein) {
		//cout << "fail opening file" << endl;
		//cin >> g_filename;
		filein.open(g_filename);
	}
	string current_line;
	while (getline(filein, current_line)) {
		lr1.add(current_line);
	}

	string in_string = "i+i*i";

	//qt����

	lr1.run(in_string, w);
	w.show();
	filein.close();
	return a.exec();

}