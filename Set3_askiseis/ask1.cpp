#include <iostream>
#include <string>
#include <cstdlib>

using namespace std;

class student {
public:
	string name;
	string surname;
	int am;
	virtual void message(){
    	cout << "In student" << endl;
	}
};

class undergrad : public student {
public:
	int entrance_order;
	int passed_courses;
	void message(){
    	cout << "In undergrad" << endl;
	}

};

class phd : public student {
public:
	string title;
	string professor;
	int start_month;
	int start_year;
	void message(){
    	cout << "In phd" << endl;
	}

};

class msc : public student {
public:
	string thesis;
	string department;
	int getGrade();

};

int  main()
{
	student * S1 = new student();
	undergrad * U1 = new undergrad();
	msc * M1 = new msc();
	phd * Ph1 = new phd();

	student *people[] = {S1,U1,M1,Ph1};

	for(int i = 0; i <= 3; i++){
    	people[i]->message();
	}

	cin;
	return 0;
}
