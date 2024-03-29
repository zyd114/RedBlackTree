#include <iostream>
#include <cstring>
#include <algorithm>
using namespace std;
#define RED 1
#define BLACK 0

template<typename T>
class RedBlackTree {
	public:
		struct node;
		node* root;
		void SolveDoubleRed(node*);
		void SolveDoubleBlack(node*);
		node* Find(T);
		void connect(node*, node*, int);
		void rotate(node*);
	public:
		RedBlackTree() : root(NULL) {}
		void insert(T);
		bool remove(T);
		bool remove(T, node*);
		int size();
		bool empty();
		int get_rank(T, node*);
		int get_rank(T);
		T kth(int, node*);
		T kth(int);
		T get_prev(T);
		T get_succ(T);
		
		void previs(node*);
		void invis(node*);
		void postvis(node*);
		void print();
		void checkNodeSize(node*);
		void checkNodeSize();
};

template<typename T>
struct RedBlackTree<T>::node {
	/*
	 * <X>	X is red.
	 * [X]	X is black.
	 * {X}	X is unknown(red/black).
	 */
	T val;
	bool color;		//1Îªºì£¬0ÎªºÚ 
	node *father, *ch[2];
	#define left ch[0]
	#define right ch[1]
	int siz;
	node(T v = T(), bool col = true, node* f = NULL,
		node* l = NULL, node* r = NULL , int s = 1)
		: val(v), color(col), father(f), siz(s) {
			left = l;
			right = r;
		}
	node* prev() {
		node* x = left;
		if(x == NULL)	return NULL;
		while(x->right != NULL) {
			x = x->right;
		}
		return x;
	}
	node* succ() {
		node* x = right;
		if(x == NULL)	return NULL;
		while(x->left != NULL) {
			x = x->left;
		}
		return x;
	}
	int direction() {
		if(father == NULL)	return 0;
		return father->right == this;
	}
	node* sibling() {
		if(father == NULL)	return NULL;
		return father->ch[direction() ^ 1];
	}
	bool hasSibling() {
		return sibling() != NULL;
	}
	node* uncle() {
		if(father == NULL)	return NULL;
		return father->sibling();
	}
	bool hasUncle() {
		return uncle() != NULL;
	}
	void pushup() {
		siz = (left?left->siz:0) + (right?right->siz:0) + 1;
	}
	void print() {
		cout << "key = " << val << ", color = " << (color ? "Red" : "Black") << endl;
	}
};

template<typename T>
void RedBlackTree<T>::connect(node *x, node *fa, int k) {
	if(x != NULL)	x->father = fa;
	if(fa != NULL) {
		fa->ch[k] = x;
	} else {
		root = x;
	}
}

template<typename T>
void RedBlackTree<T>::rotate(node *x) {
	node* y = x->father;
	node* z = y->father;
	int yson = (y->right == x);
	if(z == NULL) {
		root = x;
		x->father = NULL;
	} else {
		int zson = (z->right == y);
		connect(x, z, zson);
	}
	connect(x->ch[yson^1], y, yson);
	connect(y, x, yson^1);
	y->pushup();
	x->pushup();
}

template<typename T>
void RedBlackTree<T>::insert(T v) {
	node *x = root, *fa = NULL;
	while(x != NULL) {
		x->siz++;
		fa = x;
		if(v < x->val) {
			x = x->left;
		} else {
			x = x->right;
		}
	}
	x = new node(v, true, fa);
	if(fa == NULL) {
		root = x;
	} else if(v < fa->val) {
		fa->left = x;
	} else {
		fa->right = x;
	}
	SolveDoubleRed(x);
}
template<typename T>
void RedBlackTree<T>::SolveDoubleRed(node* x) {
	if(x == root || x->father->color == BLACK) {
		return;
	}
	node* p = x->father;
	if(p == root) {
    	// Case 3: Parent is root and is RED
    	//   Paint parent to BLACK.
    	//    <P>         [P]
    	//     |   ====>   |
    	//    <N>         <N>
    	p->color = BLACK;
    	return;
	}
	if(x->hasUncle() && x->uncle()->color == RED) {
		// Case 4: Both parent and uncle are RED
		//   Paint parent and uncle to BLACK;
		//   Paint grandparent to RED;
		//   Maintain grandparent recursively.
		//        [G]             <G>
		//        / \             / \
		//      <P> <U>  ====>  [P] [U]
		//      /               /
		//    <N>             <N>
		p->color = BLACK;			//parent -> black
		x->uncle()->color = BLACK;	//uncle  -> black
		p->father->color = RED;		//grandparent -> red
		SolveDoubleRed(p->father);
		return;
	}
	// Case 5 & 6: parent is RED, uncle is BLACK(or NULL)
	if(x->direction() != p->direction()) {
        // Case 5: Current node is the opposite direction as parent
        //   Step 1. Rotate x to parent's position.
        //   Step 2. Goto Case 6.
        //      [G]                 [G]
        //      / \    rotate(P)    / \
        //    <P> [U]  ========>  <N> [U]
        //      \                 /
        //      <N>             <P>
		rotate(x);
		x = p;			//Now P is the child of double red.
		p = x->father;	//reset p to x's father
	}
    // Case 6: Current node is the same direction as parent
    //   Step 1. Rotate parent to grandparent's position
    //   Step 2. Paint parent (before rotate) to BLACK;
    //           Paint grandparent (before rotate) to RED.
    //        [G]                 <P>               [P]
    //        / \    rotate(G)    / \    repaint    / \
    //      <P> [U]  ========>  <N> [G]  ======>  <N> <G>
    //      /                         \                 \
    //    <N>                         [U]               [U]
    rotate(p);				//rotate x's parent
    p->color = BLACK;
    x->sibling()->color = RED;	//repaint
}

#define col(a) (a == RED ? "Red" : "Black")

template<typename T>
void RedBlackTree<T>::previs(node* x) {
	if(x == NULL)	return;
	printf("%d %s %d\n", x->val, col(x->color), x->siz);
	previs(x->left);
	previs(x->right);
}
template<typename T>
void RedBlackTree<T>::invis(node* x) {
	if(x == NULL)	return;
	invis(x->left);
	printf("%d %s %d\n", x->val, col(x->color), x->siz);
	invis(x->right);
}
template<typename T>
void RedBlackTree<T>::postvis(node* x) {
	if(x == NULL)	return;
	postvis(x->left);
	postvis(x->right);
	printf("%d %s %d\n", x->val, col(x->color), x->siz);
}
template<typename T>
void RedBlackTree<T>::print() {
	printf("------pre-vis------\n");
	previs(root);
	printf("------in-vis------\n");
	invis(root);
	printf("------post-vis------\n");
	postvis(root);
}

template<typename T>
void RedBlackTree<T>::checkNodeSize(node* x) {
	int before = x->siz;
	if(x->left)	checkNodeSize(x->left);
	if(x->right)	checkNodeSize(x->right);
	x->pushup();
	if(x->siz != before) {
		printf("node of key %d : size changed from %d to %d\n", x->val, before, x->siz);
	}
}

template<typename T>
void RedBlackTree<T>::checkNodeSize() {
	checkNodeSize(root);
}

template<typename T>
bool RedBlackTree<T>::remove(T v) {
	return remove(v, root);
}

template<typename T>
bool RedBlackTree<T>::remove(T v, node* x) {
	if(x == NULL)	return false;
	if(v != x->val) {
		int branch = (v > x->val);	//v > x->val : branch = 1, goto right child
		if(x->ch[branch] != NULL) {
			x->siz--;
			bool ans = remove(v, x->ch[branch]);
			x->pushup();
			return ans;
		}
		return false;
	}
	if(x == root && x->siz == 1) {
		root = NULL;
		return true;
	}
	if(x->left != NULL && x->right != NULL) {
    	// Case 1: If the node is strictly internal
    	//   Step 1. Find the successor S with the smallest key
    	//           and its parent P on the right subtree.
    	//   Step 2. Swap the data (key and value) of S and N,
    	//           S is the node that will be deleted in place of N.
    	//   Step 3. N = S, goto Case 2, 3
    	//     |                    |
    	//     N                    S
    	//    / \                  / \
    	//   L  ..   swap(N, S)   L  ..
    	//       |   =========>       |
    	//       P                    P
    	//      / \                  / \
    	//     S  ..                N  ..
//		node* succ = x->succ();
		x->siz--;
		node* rt = x->right;
		rt->siz--;
		while(rt->left) {
			rt = rt->left;
			rt->siz--;
		}
		node* succ = rt;
		swap(x->val, succ->val);
		x = succ;
	}
	if(x->left == NULL && x->right == NULL) {
    	// Case 2: Current node is a leaf
    	//   Step 1. Unlink and remove it.
    	//   Step 2. If N is BLACK, maintain N;
    	//           If N is RED, do nothing.
    	
		// The maintain operation won't change the node itself,
    	//  so we can perform maintain operation before unlink the node.
    	x->siz = 0;
    	if(x->color == BLACK) {
			SolveDoubleBlack(x);
		}
		x->father->ch[x->direction()] = NULL;
		x->father->pushup();
		return true;
	}
    // Case 3: Current node has a single left or right child
    //   Step 1. Replace N with its child
    //   Step 2. If N is BLACK, maintain N
    node* replacement = (x->left != NULL ? x->left : x->right);
    if(x == root) {
    	root = replacement;
	} else {
		node* parent = x->father;
		parent->ch[x->direction()] = replacement;
		replacement->father = parent;
		parent->pushup();
	}
	if(x->color == BLACK) {
		if(replacement->color == RED) {
			replacement->color = BLACK;
		} else {
			SolveDoubleBlack(replacement);
		}
	}
    return true;
}

template<typename T>
void RedBlackTree<T>::SolveDoubleBlack(node* x) {
	if(x == root)	return;
	node* sibling = x->sibling();
	if(sibling->color == RED) {
    	// Case 1: Sibling is RED, parent and nephews must be BLACK
    	//   Step 1. Rotate N's sibling to P's position
    	//   Step 2. Paint S to BLACK, P to RED
    	//   Step 3. Goto Case 2, 3, 4, 5
    	//      [P]                   <S>               [S]
    	//      / \    l-rotate(P)    / \    repaint    / \
    	//    [N] <S>  ==========>  [P] [D]  ======>  <P> [D]
    	//        / \               / \               / \
    	//      [C] [D]           [N] [C]           [N] [C]
    	node* parent = x->father;
    	//Step 1
		rotate(sibling);
		//Step 2
		sibling->color = BLACK;
		parent->color = RED;
		sibling = x->sibling();		//update sibling after rotation
	}
	//close nephew: sibling's child with the same direction as x
	int xdir = x->direction();		//the direction of x
	node* closeNephew = sibling->ch[xdir];
	node* distantNephew = sibling->ch[xdir ^ 1];
	//NIL nodes are always black
	bool closeBlack = (closeNephew == NULL) || (closeNephew->color == BLACK);
	bool distantBlack = (distantNephew == NULL) || (distantNephew->color == BLACK);
	if(closeBlack && distantBlack) {
		if(x->father->color == RED) {
        	// Case 2: Sibling and nephews are BLACK, parent is RED
        	//   Swap the color of P and S
        	//      <P>             [P]
        	//      / \             / \
        	//    [N] [S]  ====>  [N] <S>
        	//        / \             / \
        	//      [C] [D]         [C] [D]
        	sibling->color = RED;
        	x->father->color = BLACK;
		} else {
        	// Case 3: Sibling, parent and nephews are all black
        	//   Step 1. Paint S to RED
        	//   Step 2. Recursively maintain P
        	//      [P]             [P]
        	//      / \             / \
        	//    [N] [S]  ====>  [N] <S>
        	//        / \             / \
        	//      [C] [D]         [C] [D]
        	sibling->color = RED;
        	SolveDoubleBlack(x->father);
		}
	} else {
		bool closeRed = (closeNephew != NULL) && (closeNephew->color == RED);
		if(closeRed && distantBlack) {
	        // Case 4: Sibling is BLACK, close nephew is RED,
	        //         distant nephew is BLACK
	        //   Step 1. Rotate close nephew to sibling's position
	        //   Step 2. Swap the color of close nephew and sibling
	        //   Step 3. Goto case 5
	        //                            {P}                {P}
	        //      {P}                   / \                / \
	        //      / \    r-rotate(S)  [N] <C>   repaint  [N] [C]
	        //    [N] [S]  ==========>        \   ======>        \
	        //        / \                     [S]                <S>
	        //      <C> [D]                     \                  \
	        //                                  [D]                [D]
			
			//Step 1
			rotate(closeNephew);
			//Step 2
			closeNephew->color = BLACK;
			sibling->color = RED;
        	// Update sibling and nephews after rotation
			sibling = x->sibling();
			xdir = x->direction();
			closeNephew = sibling->ch[xdir];
			distantNephew = sibling->ch[xdir ^ 1];
		}
    	// Case 5: Sibling is BLACK, close nephew is unknown,
    	//         distant nephew is RED
    	//      {P}                   [S]                 {S}
    	//      / \    l-rotate(P)    / \    repaint     /  \
    	//    [N] [S]  ==========>  {P} <D>  =======>  [P]  [D]
    	//        / \               / \                / \
    	//      {C} <D>           [N] {C}            [N] {C}
		//   Step 1. Rotate sibling to P's position
		//   Step 2. Swap the color of parent and sibling.
		//			 Paint distant nephew to BLACK if it is not null.
		
		//Step 1
		rotate(sibling);
		//Step 2
		sibling->color = x->father->color;
		x->father->color = BLACK;
		if(distantNephew != NULL) {
			distantNephew->color = BLACK;
		}
	}
}

template<typename T>
T RedBlackTree<T>::kth(int k, node* x) {
	if(!(x->left)) {
		if(k == 1)	return x->val;
		return kth(k - 1, x->right);
	}
	if(k <= x->left->siz)	return kth(k, x->left);
	if(k <= x->left->siz + 1)	return x->val;
	return kth(k - x->left->siz - 1, x->right);
}
template<typename T>
T RedBlackTree<T>::kth(int k) {
	return kth(k, root);
}

template<typename T>
T RedBlackTree<T>::get_prev(T v) {
	node *x = root;
	T ans;
	while(x != NULL) {
		if(x->val < v) {
			ans = x->val;
			x = x->right;
		} else {
			x = x->left;
		}
	}
	return ans;
}
template<typename T>
T RedBlackTree<T>::get_succ(T v) {
	node *x = root;
	T ans;
	while(x != NULL) {
		if(x->val > v) {
			ans = x->val;
			x = x->left;
		} else {
			x = x->right;
		}
	}
	return ans;
}

template<typename T>
int RedBlackTree<T>::get_rank(T v, node* x) {
	if(x == NULL)	return 1;
	int lsiz = (x->left != NULL ? x->left->siz : 0);
	if(v <= x->val)	return get_rank(v, x->left);
	return lsiz + 1 + get_rank(v, x->right);
}
template<typename T>
int RedBlackTree<T>::get_rank(T v) {
	return get_rank(v, root);
}
RedBlackTree<int> Tree;
int main() {
	Tree.insert(-0x7fffffff);
	Tree.insert(0x7fffffff);
	int n;
	cin >> n;
	while(n--) {
		int op, x;
		cin >> op >> x;
		if(op == 1) {
			Tree.insert(x);
		} else if(op == 2) {
			Tree.remove(x);
		} else if(op == 3) {
			cout << Tree.get_rank(x) - 1 << endl;
		} else if(op == 4) {
			cout << Tree.kth(x + 1) << endl;
		} else if(op == 5) {
			cout << Tree.get_prev(x) << endl;
		} else {
			cout << Tree.get_succ(x) << endl;
		}
//		Tree.checkNodeSize();
	}
	return 0;
}
