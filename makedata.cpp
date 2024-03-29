#include<stdio.h>
#include<cstring>
#include<algorithm>
#include<time.h>
using namespace std;
int Rand(int l,int r) {
	return ((rand()<<15) | rand()) % (r-l+1) + l;
}
int siz, vis[105];
#define Maxx 5
bool check(int op, int x) {
	if(op != 1 && siz == 0)	return false;
	if(op == 2 && vis[x] == 0)	return false;
	if(op == 4 && x > siz)	return false;
	return true;
}
int GenerateX(int op) {
	int ans = Rand(1, Maxx);
	if(op == 2) {
		if(siz == 0)	return -1;
		while(vis[ans] == 0) {
			ans = Rand(1, Maxx);
		}
		return ans;
	}
	return ans;
}
int main() {
	freopen("data.txt","w",stdout);
	srand(time(0));
	int T = 1;
//	printf("%d\n",T);
	while(T--) {
		siz = 0;
		int n = 10;
		printf("%d\n", n);
		while(n--) {
			int op = Rand(1, 6);
			int x = GenerateX(op);
			while(!check(op, x)) {
				op = Rand(1, 6);
				x = GenerateX(op);
			}
			if(op == 1) {
				siz++;
				vis[x]++;
			} else if(op == 2) {
				siz--;
			}
			printf("%d %d\n", op, x);
		}
	}
	return 0;
}
