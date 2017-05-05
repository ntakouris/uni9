
#include <iostream>
#include <string>

using namespace std;

class Product{
private:
	string name;
	double price;
	int score;
public:
	void read(){
    	cout << "Please enter the model name: ";
    	getline(cin, name);
    	cout << "Please enter the price: ";
    	cin >> price;
    	cout << "Please enter the score: ";
    	cin >> score;

    	string remainder; /* read remainder of line */
    	getline(cin, remainder);
	}

	friend void print(Product p);

	bool operator>(const Product& other){
    	return (score / price) > (other.score / other.price);
	}
};

void print(Product p){
    	cout << "Product{ Model: " << p.name << " (Score: " << p.score << " Price: " << p.price << " }" << endl;
}

int main()
{
	Product best;
	bool more = true;
	while (more)
	{
    	Product next;
    	next.read();

    	if (next > best)
    	{
        	best = next;
    	}

    	cout << "More data? (y/n) ";
    	string answer;
    	getline(cin, answer);

    	if (answer != "y"){ more = false;}
	}
	cout << "The best value is ";
	print(best);

	return 0;
}
