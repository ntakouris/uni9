#include<iostream>

//#define M 2

using namespace std;
int main(){
	
	int M;
	
	cout << "Enter size (M): " <<endl;
	cin >> M;	
	
	cout<<"First set of numbers:"<<endl;
	int first[M][M], second[M][M];
	int i,j;
	for( i=0;i<2;i++){
		cout<<"Enter two integers: "<<i+1<<endl;
		for( j=0;j<2;j++){
			cin>>first[i][j];
		}
	}
	cout<<"\n\n\nSecond set of numbers: "<<endl; 
	
	for( i=0;i<M;i++){
		cout<< "Enter two more integers:" <<i+1<<endl; 
	
		for( j=0;j<M;j++){
			cin>>second[i][j];
		}
	}
	for( i=0;i<M;i++){
		for( j=0;j<M;j++){
			first[i][j]=first[i][j]+second[i][j];
		}
	}
	
	for( i=0;i<M;i++){
		for( j=0;j<M;j++){
			cout << first[i][j];
		}
		cout << endl;
	}
	return 0;
}

