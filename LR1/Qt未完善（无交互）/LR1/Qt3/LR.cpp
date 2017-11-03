#include"head.h"
#include "Qt3.h"
#include<QWidget>
#include<QtWidgets/qapplication.h>
#include<qstring.h>


using namespace std;

#define Extension '~'			//假装是拓广文法的开始符号

//~
formula::formula(const string &in_string) {
	this->theLeft = in_string[0];
	this->theRight = string(in_string.begin() + 3, in_string.end());
}

//~
bool item::form_isValid(const string &str) {
	//暂时想到的三个基本满足条件，和LL1中类似
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
	for (unsigned int i = 0; i < str.length(); ++i) {		//开始整理终结符和非终结符
		if (isupper(str[i])) {
			Vn.insert(str[i]);
			Symbol.insert(str[i]);
		}
		else if (!(str[i] == '-' && str[i + 1] == '>' && (i++)) && str[i] != '|') {
			Vt.insert(str[i]);
			Symbol.insert(str[i]);
		}
	}

	for (unsigned int i = 3; i < str.length(); i++) {		//整理候选集,!!第一次在这里除了bug是因为误把i的初值设为0
		unsigned int j;
		for (j = i + 1; j < str.length() && str[j] != '|'; ++j);
		formula form = formula(string(1, str[0]) + "->" + string(str.begin() + i, str.begin() + j));
		if (find(forms.begin(), forms.end(), form) == forms.end()) {			//如果找不到重复的元素就插入进去（去重）
			forms.push_back(form);
		}
		i = j;
	}
}


set<char> LR1::first(const string & str) {		//原理同LL1
	if (str.length() == 0) {
		return set<char>({ '$' });
	}
	else if (str.length() == 1) {
		if (Grammer.Vt.find(str[0]) != Grammer.Vt.end() || str[0] == '#') {		//如果是终结符
			return set<char>({ str[0] });
		}
		else {
			if (FIRST[str[0]].size() != 0) return FIRST[str[0]];			//如果first不为空，说明已经求过，不用再求，直接返回
			else {
				for (vector<formula>::iterator it = Grammer.forms.begin(); it != Grammer.forms.end(); ++it) {
					if (*it == str[0]) {		//拿it的左边和
												// 防止直接左递归,即左边的狮子不能出现在右边的式子里
						size_t xPos = it->theRight.find(it->theLeft);
						if (xPos != string::npos) { // 如果找到X->aXb，即出现直接左递归的形式
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
			set<char> f = first(string(1, str[i])); // 逐个符号求first(Xi)集
			if (f.find('$') != f.end() && str.length() - 1 != i) { // 发现@
				f.erase(f.find('$')); // 减去@
				ret.insert(f.begin(), f.end()); // 放入first集合
			}
			else { // 无$或者最后一个Xi，则不需要求下去了
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
	for (vector<char>::iterator it = in_words.end(); it != in_words.begin();) {		//也可以用reverse_iterator， 是反向迭代器，用来反向遍历整个数组
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
	//此处都是展示分析表
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

		int sTop = status.size() - 1;		//状态栈栈顶
		int iTop = in_words.size() - 1;		//输入字符串栈顶
		pair<int, char> p = make_pair(status[sTop], in_words[iTop]);
		if (ACTION.find(p) == ACTION.end()) {	//如果遇到输入的字符没有对应的动作就报错
			processTab->setItem(step, 3, new QStandardItem(QString::fromStdString(string("no exist"))));
			//cout << "error, cant find the action " << endl;
			break;
		}
		pair<string, int> act = ACTION[p];
		if (act.first == "sj") {			//如果检测到是移进动作
			processTab->setItem(step, 3, new QStandardItem(QString::fromStdString(string("shift"))));
			//cout << "shift";
			status.push_back(act.second);
			parse.push_back(in_words[iTop]);
			in_words.pop_back();
		}
		else if (act.first == "rj") {	//如果检测到是归约动作
			formula f = Grammer.forms[act.second];
			processTab->setItem(step, 3, new QStandardItem(QString::fromStdString((string("reduce: ") + f.theLeft +  string("->") + f.theRight))));			//~!!debugged此处如果直接用双引号引用的字符串会出现显示错误
			//cout << "reduce:  " << f.theLeft << "->" << f.theRight; 

			if (f.theRight != "$")		//如果不是空串，需要先将产生式右端的所有字符出栈
				for (unsigned int i = 0; i < f.theRight.size(); i++) {
					status.pop_back();
					parse.pop_back();
				}
			parse.push_back(f.theLeft);	//右端出栈左端进栈
			status.push_back(GOTO[make_pair(status[status.size() - 1], f.theLeft)]);		//归约后需要根据goto表索引到下一步状态的位置
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
	if (I.forms.size() == 0)		return I;			//空是非法的.直接返回就好
	for (unsigned int i = 0; i < I.forms.size(); i++) {		//从该项目的第一个产生式开始求闭包
		formula form = I.forms[i];
		unsigned int DotLoc = 0;		//找停顿点的位置
		if ((DotLoc = form.theRight.find('`')) != string::npos && DotLoc != form.theRight.length() - 1) {	//如果没在最后位置
			char B = form.theRight[DotLoc + 1];
			if (Grammer.Vt.find(B) != Grammer.Vt.end()) {		//如果是终结符
				if (B == '$') {
					swap(I.forms[i].theRight[DotLoc], I.forms[i].theRight[DotLoc + 1]);	//将分隔点的位置后移一位
				}
				continue;		//如果是非空的终结符直接跳过此次循环，因为主要求得是非终结符产生的闭包
			}
			string f = string(form.theRight.begin() + DotLoc + 2, form.theRight.end());	//从分割点后面的一位字符的后面开始，直接为求first，再求展望字符做准备
			set<char> ff = {};
			for (auto look : form.lookaheads) {			//要结合每一个展望符号求first
				set<char> fs = first(f + look);		//对于first（B+展望符）中的每一个终结符b，根据书上的定义的条件2来
				ff.insert(fs.begin(), fs.end());		//求出的first放在ff里，后面要遍历用
			}

			for (vector<formula>::iterator it = Grammer.forms.begin(); it != Grammer.forms.end(); it++) {
				if (*it == B) {		//找左边是B的对应的产生式,这里的等号经过了重载
					formula f = *it;
					if (f.theRight[0] == '$') {
						f.theRight = '`' + f.theRight;
						swap(f.theRight[0], f.theRight[1]);		//!!将分割点的位置后移一位
					}
					else {
						f.theRight = "`" + f.theRight;
					}
					//如果原来的闭包没有，就加入进入
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

item LR1::go_to(const item & iitem, char x) {	//加&是为了直接引用该值，而不用多复制一份而占空间，而const则是为了防止被修改
	item j;
	if (iitem.forms.size() == 0 || x == '$')		return j;		//如果项目为空，就返回空项目

																	//主要是搜集该状态中的哪些产生式能接受x
	for (auto p : iitem.forms) {// 对于iitem中的每个项目
		string right = p.theRight;
		unsigned int DotLoc = right.find('`');	//找到点的位置
		if (right[DotLoc + 1] == x) {
			swap(right[DotLoc], right[DotLoc + 1]);		//!!利用swap将分隔点的位置后移一位
			j.forms.push_back(formula(p.theLeft, right, p.lookaheads));
		}
	}
	//找到之后就对这些产生式求闭包
	return closure(j);
}


//~
void LR1::dfa_items() {
	item ini;
	ini.forms.push_back(formula(Extension, "`" + string(1, Grammer.forms[0].theLeft), { '#' }));		//生成拓广文法
	normal_set.push_back(closure(ini));	//将生成的初始的文法生成闭包，插入到normal_set里面

	size_t size = 0;		//
	while (size != normal_set.size()) {		//循环求闭包直到项目族集不在增大
		size = normal_set.size();
		for (unsigned int i = 0; i < normal_set.size(); i++) {		//对于当前规范族集中的每个项目
			item current_item = normal_set[i];
			for (auto sym : Grammer.Symbol) {		//对于语法集中的每个有效符号（包括终结符和非终结符）
				item next = go_to(current_item, sym);		//进行goto计算
				if (next.forms.size() != 0) {	//生成的goto集若不为空,看这个goto集是否已经存在，不存在就插入
					auto it = find(normal_set.begin(), normal_set.end(), next);			//这里需要规定一种比较item的标准，需要在类中进行重载操作符
					if (it != normal_set.end()) {
						GOTO[make_pair(i, sym)] = it - normal_set.begin();		//用相对位置索引，找到了说明直接将goto集指向那个迭代器位置
					}
					else {//并且生成的goto集不属于项目族集
						normal_set.push_back(next); //那么就将生成的新的闭包插入到项目族集中
						GOTO[make_pair(i, sym)] = normal_set.size() - 1;		//同上，用相对位置索引，将goto集指向最后迭代器的位置
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


//构造ACTION GOTO表
void LR1::build(Qt3 &win) {
	QStandardItemModel *norm_list = new QStandardItemModel();
	win.ui.normal_list->setModel(norm_list);

	dfa_items();		//求项目集状态机dfa		//!!debugged@ 已解决

	for (unsigned int i = 0; i < normal_set.size(); i++) {		//对于规范族集的每个状态
		QStandardItem *n_num = new QStandardItem(QString("[status")+QString::number(i, 10)+QString("]"));		//打印状态号
		norm_list->appendRow(n_num);

		const item & iitem = normal_set[i];
		for (auto form : iitem.forms) {		//对于每个状态下的项目集的每个项
			//以下开始打印每一个状态的所有产生式
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

			//打印结束，开始构建分析表
			unsigned long DotLoc = form.theRight.find('`');
			if (DotLoc < form.theRight.length() - 1) {		///如果不是最后一个,说明可能进入移进操作， 即满足课本的条件1
				char x = form.theRight[DotLoc + 1];
				if (Grammer.Vt.find(x) != Grammer.Vt.end() && GOTO.find(make_pair(i, x)) != GOTO.end()) {	//如果是终结符，并且goto集中存在该状态遇到该终结符的索引记录
					int j = GOTO[make_pair(i, x)];
					ACTION[make_pair(i, x)] = make_pair("sj", j);		//将该转换操作记录下来
				}
			}
			else {
				if (form == formula(Extension, string(1, Grammer.forms[0].theLeft) + '`', {}) && form.lookaheads == set<char>({ '#' })) { // 检验是否是 S'->S.,#，如果是就转入操作 acction[i, #] = acc，即满足课本上的条件3
					ACTION[make_pair(i, '#')] = make_pair("accept", 0);
				}
				else if (form.theLeft != Extension) {		//如果满足课本上的条件2
					string right = form.theRight;
					right.erase(DotLoc, 1);		//删除点,为后面构造完整的产生式用于进行搜索作准备
					for (auto l : form.lookaheads) {
						vector<formula>::iterator it = find(Grammer.forms.begin(), Grammer.forms.end(), formula(form.theLeft, right, set<char>{}));
						if (it != Grammer.forms.end())		//若存在,确定一下规约要依靠的产生式的位置
							ACTION[make_pair(i, l)] = make_pair("rj", it - Grammer.forms.begin());
					}
				}
			}
		}
		if (Grammer.Vt.find('$') != Grammer.Vt.end()) {		//此时应该将最初收入的'$'移除，换成'#'，以便后面构造分析表用
			Grammer.Vt.erase(Grammer.Vt.find('$'));
			Grammer.Symbol.erase(Grammer.Symbol.find('$'));
		}
		Grammer.Vt.insert('#');
		Grammer.Symbol.insert('#');
	}
}

void LR1::showTable(Qt3 &win) {
	//结合了QT的元素
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
	for (const auto & X : Grammer.Vt) {		//对于表中的每一个终结符
		if (X != '#') {
			p_tab->setHorizontalHeaderItem(count++, new QStandardItem(QObject::tr((string(1, X).c_str()))));
			//p_tab->setItem(0, count++, new QStandardItem(QString::fromStdString(string(1, X))));
			//cout << X << "\t";
		}
	}
	p_tab->setHorizontalHeaderItem(count++, new QStandardItem(QObject::tr((string(1, '#').c_str()))));
	//p_tab->setItem(0,count++, new QStandardItem(QString::fromStdString(string(1, '#'))));
	//cout << "#\t"; // #放到最后一列显示，美观
	 //cout << "Vn:";
	 //int firstComma = 0; // 处理第一个逗号
	for (const auto & X : Grammer.Vn) {
		p_tab->setHorizontalHeaderItem(count++, new QStandardItem(QObject::tr((string(1, X).c_str()))));
		//p_tab->setItem(0, count++, new QStandardItem(QString::fromStdString(string(1, X))));
		//cout << X << "\t";
	}
	//cout << endl << endl;
	for (unsigned int i = 0; i<normal_set.size(); ++i) {		//对于正规族集中的每一个状态
																//printf("%s[", i == 0 ? " " : ", ");
		int c_count = 0;														//int firstComma = 0;
		//cout << i << '\t';
		for (const auto & X : Grammer.Vt) {		//对于该状态遇到的每一个终结符
			
			if (X != '#') {
				pair<int, char> p = make_pair(i, X);
				if (ACTION.find(p) != ACTION.end()) {		//如果在action中找到对应的动作
					pair<string, int> res = ACTION[p];
					p_tab->setItem(i,c_count++ , new QStandardItem(QString::fromStdString(res.first)+QString::number(res.second,10)));		//Qt
					//cout << res.first << res.second << "\t";
				}
				else c_count++;
					//cout << " \t";
			}
		}
		pair<int, char> p = make_pair(i, '#');		//专门为#进行索引展示
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

		for (const auto & X : Grammer.Vn) {		//对于每一个非终结符
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



//~用于将输入串的字符串格式转换成一个个字符保存在栈中
void LR1::loadStr(const string &str) {
	in_words.push_back('#');
	status.push_back(0);
	for (int i = str.length() - 1; i >= 0; --i)
		in_words.push_back(str[i]);
}

//
void LR1::run(const string &in_string, Qt3 &win) {

	//分析文法部分
	//string in_string;
	//cin >> in_string;
	//in_string = "";

	showGrammer();		//debugged@没有问题
	//cout << endl;

	build(win);
	showTable(win);
	//cout << endl;

	//drawGrapgh();
	//cout << endl;


	//分析具体的输入串部分
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

	//输入文法
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

	//qt部分

	lr1.run(in_string, w);
	w.show();
	filein.close();
	return a.exec();

}