#include <stdlib.h>
#include <iostream>
#include <vector>

using namespace std;

int main()
{
	vector<int> vec; int i;
	cout << "vector size = " << vec.size() << endl;
	
	for(i = 0; i < 7; i++){
		vec.push_back(rand());
	}
	
	cout << "extended vector size = " << vec.size() << endl;
	
	vector<int>::iterator v = vec.begin();
	while( v != vec.end()) {
		cout << "value of v = " << *v << endl;
		v++;
	}
	
	vec.resize(5);
	
	cout << "reduced vector size = " << vec.size() << endl;
	
	return 0;
}

