#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;
//global variables
const int SIZE = 81; // constant variable
int displaymenu();
//structure Costco
struct Costco
{
	char name[SIZE];
	int quantity;
	float wholesale;
	float retail;
};
//function prototypes
void addrecord();
void displayall();
int initialdataread();
void displayrecord();
void changerecord();
void getitem(Costco&);
void report();
void displayitem(const Costco&);

int main()
{
	int choice;
	initialdataread();
	choice = displaymenu();
	
	while (choice != 6) //menu loop to ask the user which option he/she would like to choose
	{
		switch (choice)
		{
		case 1: displayall();
			break;
		case 2: displayrecord();
			break;
		case 3: changerecord();
			break;
		case 4: addrecord();
			break;
		case 5: report();

		default: if (choice != 6)
			cout << "Wrong choice" << endl;
		}
		choice = displaymenu();
	}
	return 0;
}

int displaymenu() // function definition for displaymenu
{
	int choice;
	string menu = "\n\n\t1-\tdisplay all, \t\t2-\tdisplay a record\n\t3-\tchange record";
	menu += ", \t\t-4-\tadd a record \n\t5-\tprepare report,\t\t6-\texit\n\n";
	cout << menu; // this line will output the menu
	cin >> choice;
	return choice; // the function will essentially display the menu 
}

int initialdataread()//function definition for initialdataread
{
	Costco temp;
	ofstream out;
	ifstream in;
	in.open("data.txt");
	out.open("inventory.dat", ios::out | ios::binary);
	if (in && out)
	{
		in >> temp.name;
		while (!in.eof())
		{
			in >> temp.quantity >> temp.wholesale >> temp.retail;
			out.write((char*)&temp, sizeof(temp));
			in >> temp.name;
		}
		in.close();
		out.close();
	}
	else
	{
		cout << "Error opening files \n";
		exit(EXIT_FAILURE);
	}
}

void displayall()//function definition for displayall
{
	Costco temp;
	ifstream in("inventory.dat", ios::binary);
	if (in)
	{
		in.read((char*)&temp, sizeof(temp));
		while (!in.eof())
		{
			displayitem(temp);
			in.read((char*)&temp, sizeof(temp));
		}
		in.close();
	}
}// the function will display everything in the text file to the console

void displayrecord()//function definition for display record
{
	Costco temp;
	char todisplay[SIZE];
	cout << "Enter the name of the record you would like to view\n";
	cin >> todisplay;
	ifstream in("inventory.dat", ios::binary);
	if (in)
	{
		in.read((char*)&temp, sizeof(temp));
		while (!in.eof())
		{
			if (strcmp(temp.name, todisplay) == 0)
			{
				displayitem(temp);
				break;
			}
			in.read((char*)&temp, sizeof(temp));
		}
		if (in.eof())
		{
			cout << "Item not in the file \n\n";
		}
		in.close();
	}
}// the function will display the record for the specific item in the text file

void changerecord()//function definition for change record
{
	Costco temp;
	Costco newitem;
	long where;

	char olditem[SIZE];
	cout << "Enter item you would like to replace ";
	cin >> olditem;
	getitem(newitem);

	fstream inOut("inventory.dat", ios::in | ios::out | ios::binary);

	if (inOut)
	{
		inOut.read((char*)&temp, sizeof(temp));
		while (!inOut.eof())
		{
			if (strcmp(temp.name, olditem) == 0)
			{
				cout << "Item found   " << temp.name << endl;
				break;
			}
			inOut.read((char*)&temp, sizeof(temp));
		}
	}
	inOut.clear();

	where = inOut.tellg();
	inOut.seekp(where-sizeof(Costco), ios::beg);
	inOut.write((char*)&newitem, sizeof(Costco));
	inOut.close();
} // this function will change replace one of the items in the text file 

void addrecord() //function definition for addrecord
{
	Costco temp;
	getitem(temp);

	ofstream out("inventory.dat", ios::app | ios::binary);
	if (out)
	{
		out.write((char*)&temp, sizeof(Costco));
	}
	else
	{
		cout << "Error opening the file" << endl;
		exit(EXIT_FAILURE);
	}
}// this function will add another item into the text file

void displayitem(const Costco& c)// function definition for displayitem
{
	cout << "Item name         " << c.name << endl;
	cout << "Quantity          " << c.quantity << endl;
	cout << "Wholesale cost    " << c.wholesale << endl;
	cout << "Retail cost       " << c.retail << endl;
}// this function will display the items in the text file

void getitem(Costco& newitem)// Function definition for getitem
{
	cout << "Enter new item name : ";
	cin >> newitem.name;
	cout << "Enter the new item quantitiy : ";
	cin >> newitem.quantity;
	cout << "Enter the new item wholesale price : ";
	cin >> newitem.wholesale;
	cout << "Enter the new item retail price : ";
	cin >> newitem.retail;
}// This function will ask the user for the characteristics of the new item

void report()//Function definition for report
{
	Costco temp;
	float total_quantity = 0.0, total_retail = 0.0, total_wholesale = 0.0;
	ifstream in("inventory.dat", ios::binary);
	if (in)
	{
		while (in.read((char*)&temp, sizeof(Costco)))
		{
			total_quantity += temp.quantity;
			total_retail += temp.retail;
			total_wholesale = temp.wholesale;
		}
		in.close();
	}
	else
	{
		cout << "Error opening the file" << endl;
		exit(EXIT_FAILURE);
	}

	cout << "\n\nREPORT\n\n";
	cout << "Total quantity of items on stock     " << total_quantity << endl;
	cout << "Total retail amount on stock        $" << total_retail << endl;
	cout << "Total wholesale amount on stock     $" << total_wholesale << endl << endl;
}// This function will report everything in the text file.