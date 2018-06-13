#include<iostream>
#include<cstdlib>
#include<string>
#include<vector>
#include<utility>
#include<sstream>
#include<fstream>

using namespace std;

class Node {
public:
	Node(string *arr, int n) {
		errStrs = vector<string>();

		Node::root = this;

		childs = vector<pair<char, Node*>>();
		error = nullptr;
		hasLeaf = false;

		for (int i = 0; i < n; i++)
			add(arr[i]);

		setErrorNodes();

		//print();
	}

	void printSubstrings(string s) {
		int i = 0;
		Node *cur = root; 
		ofstream out("answer.txt");
		while (i < s.size()) {
			Node *ptr = cur->findNodeForString(string(1, s[i]));
			if (ptr != nullptr) {
				cur = ptr;
				i++;
				continue;
			}
			if (cur->hasLeaf) {
				out << cur->errStrs.at(0) << endl;
				cur = root;
				continue;
			}
			if (cur->error == nullptr) {
				cur = root;
				i++;
				continue;
			}
			vector<string>::iterator it;
			for (it = cur->errStrs.begin(); it != cur->errStrs.end(); it++)
				out << *it << endl;
			cur = cur->error;
		}
		while (cur != nullptr && cur != root) {
			if (cur->hasLeaf) {
				out << cur->errStrs.at(0) << endl;
				return;
			}
			vector<string>::iterator it;
			for (it = cur->errStrs.begin(); it != cur->errStrs.end(); it++)
				out << *it << endl;
			cur = cur->error;
		}
		out.close();
	}

private:
	static Node *root;
	vector<pair<char, Node*>> childs;
	Node *error;
	vector<string> errStrs;
	bool hasLeaf;

	Node() {
		childs = vector<pair<char, Node*>>();
		error = nullptr;
		hasLeaf = false;
	}

	Node(string s) {
		childs = vector<pair<char, Node*>>();
		hasLeaf = false;
		add(s);
		error = nullptr;
	}    

	//add a node corresponding to the string s (without error ptr)
	void add(string s) {
		if (s.size() != 0) {
			bool find = false;
			vector<pair<char, Node*>>::iterator i;
			for (i = childs.begin(); i != childs.end(); i++)
				if (i->first == s[0]) {
					find = true;
					break;
				}
				else if (i->first > s[0])
					break;

			if (find)
				i->second->add(s.substr(1));
			else
				childs.insert(i, pair<char, Node*>(s[0], new Node(s.substr(1))));
		} else
			hasLeaf = true;
	}

	void print(int shift = 0) {
		vector<pair<char, Node*>>::iterator i;
		for (i = childs.begin(); i != childs.end(); i++) {
			for (int i = 0; i < shift; i++)
				cout << " ";

			cout << "(";
			stringstream ss1;
			ss1 << i->second;
			string s1;
			ss1 >> s1;
			cout << s1.substr(5) << ")" << i->first;

			if (i->second->error == nullptr)
				cout << " -----------------" << endl;
			else {
				stringstream ss2;
				ss2 << i->second->error;
				string s2;
				ss2 >> s2;
				cout << "(" << s2.substr(5) << ")" << (i->second->hasLeaf ? " gr" : "");
				vector<string>::iterator it;
				for (it = i->second->errStrs.begin(); it != i->second->errStrs.end(); it++)
					cout << " " << *it;
				cout << endl;
			}

			i->second->print(shift + 5);
		}
	}

	//find a node corresponding to the string s (starts in the current node)
	Node *findNodeForString(string s) {
		if (s.size() == 0)
			return this;

		vector<pair<char, Node*>>::iterator i;
		for (i = childs.begin(); i != childs.end(); i++)
			if (i->first == s[0])
				return i->second->findNodeForString(s.substr(1));
		return nullptr;
	}

	Node *findErrorNode(string s) {
		if (s.size() == 0)
			return nullptr;
		Node *err = root->findNodeForString(s.substr(1));
		if (err == nullptr)
			return findErrorNode(s.substr(1));
		return err;
	}

	void setErrorNodes(string s = "") {
		vector<pair<char, Node*>>::iterator it;
		for (it = childs.begin(); it != childs.end(); it++) 
			if (it->second->hasLeaf) {
				it->second->errStrs.push_back(s + it->first);
				it->second->setErrorNodes(s + it->first);
			}
			else {
				string str = s + it->first;
				string st = str;
				string s1 = st;
				int k = 0;
				for (int i = 0; i < st.size(); i++) {
					if (k != 0 && root->findNodeForString(st.substr(i)) != nullptr) {
						s1 = str.substr(k - 1);
						break;
					}
					for (int j = st.size() - 1; j > i; j--)
						if (root->findNodeForString(st.substr(i, j)) != nullptr && 
							root->findNodeForString(st.substr(i, j))->hasLeaf) {
							it->second->errStrs.push_back(st.substr(i, j));
							st = st.substr(j);
							k += j;
							i = -1;
							break;
						}
				}
				it->second->error = findErrorNode(s1);
				it->second->setErrorNodes(str);
			}
	}
};
Node *Node::root = nullptr;

int main() {
	int n;
	ifstream fin("substrings.txt");

	fin >> n;
	string *arr = new string[n];

	for (int i = 0; i < n; i++) 
		fin >> arr[i];
	fin.close();
	
	Node *root = new Node(arr, n);

	ifstream strIn("string.txt");
	string s;
	strIn >> s;
	strIn.close();

	root->printSubstrings(s);

	return 0;
}
