#include<bits/stdc++.h>
using namespace std;

signed main()
{
	int n = 12, s = 1000;
	cout << "photo num, X: ";
	cin >> n >> s;
	freopen("code.txt", "w", stdout);
	for (int i = 1; i <= n; i++)
	{
		printf(".album img:nth-child(%d) {\n", i);
		printf("    transform: rotateY(%fdeg) translateZ(%dpx);\n", 1.0 * (i - 1) * (360.0 / n), s);
		puts("}");
		puts("");
	}
	return 0;
}
