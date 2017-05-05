#include <iostream>
using namespace std;
int main()
{
	cout << "Enter a number: " <<
	endl; int value;
	cin >> value;
	if(value < 10)
	{
		cout << "This value is too small";
	}else if(value > 10){
		cout << "This is a big enough number";
	}
	return 0;
}

