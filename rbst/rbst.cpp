#include <cstdlib>
#include <map>
#include <vector>
#include <iostream>

#define REP(i,x) for(int i=0 ; i<(int)(x) ; i++)

using namespace std;

class RBST{
public:
	struct Node{
		Node *l, *r;
		int size;
		int value;
		Node(int v):size(1),value(v){
			l = r = NULL;
		}
		Node(Node *c){
			l = c->l;
			r = c->r;
			size = c->size;
			value = c->value;
		}
	};

	static Node* newnode(Node *c){
		return new Node(c);
	}

	static Node* newnode(int v){
		return new Node(v);
	}

	static int count(Node* a){
		if (!a)return 0;
		return a->size;
	}

	static Node* update(Node *a){
		if (!a)return a;
		a->size = count(a->l) + count(a->r) + 1;
		return a;
	}

	static Node* merge(Node *a, Node *b){
		if (!a)return b;
		if (!b)return a;
		if (rand() & ((count(a) + count(b)) < count(a))){
			Node *n = newnode(a);
			n->r = merge(a->r, b);
			return update(n);
		}
		else{
			Node *n = newnode(b);
			n->l = merge(a, b->l);
			return update(n);
		}
	}

	static pair<Node*, Node*> split(Node *c, int k){
		if (!c)return make_pair(c, c);
		Node *n = newnode(c);
		if (k <= count(c->l)){
			pair<Node*, Node*> s = split(c->l, k);
			n->l = s.second;
			return make_pair(s.first, update(n));
		}
		else{
			pair<Node*, Node*> s = split(c->r, k - count(c->l) - 1);
			n->r = s.first;
			return make_pair(update(n), s.second);
		}
	}

	static Node* insert(Node *c, int k, int value){
		pair<Node*, Node*> s = split(c, k);
		Node *n = newnode(value);
		return merge(merge(s.first, n), s.second);
	}

	static void vectorize(Node *c, vector<int> &arr){
		if (!c)return;
		vectorize(c->l, arr);
		arr.push_back(c->value);
		vectorize(c->r, arr);
	}

	static vector<int> vectorize(Node *root){
		vector<int> res(0);
		vectorize(root, res);
		return res;
	}
};

int main(){
	// testcode
	const int M = 1000;
	RBST::Node *root = NULL;
	vector<int> arr;
	vector<vector<int> > oldArr;
	vector<RBST::Node*> oldRbst;
	
	bool hasError = false;
	REP(i, M){
		oldArr.push_back(arr);
		oldRbst.push_back(root);
		int pos = arr.size() == 0 ? 0 : rand() % arr.size();
		if (rand() & 1){
			int val = rand() % 100;
			root = RBST::insert(root, pos, val);
			arr.insert(arr.begin() + pos, val);
			cout << "insert " << val << " to arr[" << pos << "].\n";
		}
		else{
			int ver = rand() % oldArr.size();
			bool res = RBST::vectorize(oldRbst[ver]) == oldArr[ver];
			hasError |= (res == false);
			cout << "version " << ver << " ... " << (res ? "ok" : "ng") << ".\n";
		}
	}
	cout << (hasError ? "some tests failed." : "all tests passed.") << endl;

	return 0;
}