#ifndef TIRE_TREE
#define TIRE_TREE

#include<iostream>
#include<cstdlib>
#include<cstdio>
#include<string>

using namespace std;

struct TireNode {
	TireNode *next[26] = {};
	bool finished;
	int location;		//??为什么要安插一个location变量
	TireNode() {
		finished = false;
		for (int i = 0; i < 26; i++) {
			next[i] = nullptr;
		}
	}
};


void TreeAdd(TireNode * root, const string str, int loc) {
	TireNode *p = root;
	for (unsigned int i = 0; i < str.length(); i++) {
		if (p->next[str[i] - 'a'] == nullptr) {
			TireNode *q = new TireNode();
			p->next[str[i] - 'a'] = q;
		}
		p = p->next[str[i] - 'a'];
	}
	p->finished = true;
	p->location = loc;
}

int TreeFind(TireNode *root, const string &str) {
	TireNode *p = root;
	for (unsigned int i = 0; i < str.length(); i++) {
		if ((str[i] - 'a') < 0 || (str[i] - 'a') > 25) {
			return -1;
		}
		p = p->next[str[i] - 'a'];
		if (p == nullptr)
			return -1;
	}
	if (p->finished == true) {
		return p->location;
	}
	else return -1;
}

void TreeFree(TireNode *p) {
	if (p == nullptr)	return;
	for (int i = 0; i < 26; i++) {
		TreeFree(p->next[i]);
	}
	delete p;
}


#endif

