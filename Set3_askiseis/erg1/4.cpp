#include<iostream>

using namespace std;

class Kouti
{
	private:
		double length;
		double breadth;
		double height;
	
	public:
		Kouti(){
		}
		Kouti(double l, double b, double h) : length (l), breadth (b), height(h) {}
		
		double volume(){
			return length * breadth * height;
		}
		
		Kouti operator+(const Kouti& b)
		{
		Kouti kouti;
		kouti.length = this->length + b.length;
		kouti.breadth = this->breadth + b.breadth;
		kouti.height = this->height + b.height;
		return kouti;
		}
		
		void setLength(double length){
			this->length = length;
		}
		
		void setBreadth(double breadth){
			this->breadth = breadth;
		}
		
		void setHeight(double height){
			this->height = height;
		}
	
};

int main(){
	Kouti ka = Kouti(2.0,3.2,6.0);
	
	cout << "ka: " << ka.volume() << endl;
	
	Kouti kb = Kouti(2.5,4.0,5.0);
	
	cout << "kb: " << kb.volume() << endl;
	
	Kouti kc = ka + kb;
	
	cout << "kc: " << kc.volume() << endl;
	return 0;	
}
