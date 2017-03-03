#include <algorithm>
#include <string>
#include <iostream>
#include "tree.h"

using namespace std;

int main( int, char ** )
{
	Tree<string> tr;
	Tree<string>::preOrderIterator top, one, two, loc, banana;

	top=tr.begin();
	one=tr.insert(top, "one");
	two=tr.appendChild(one, "two");
	tr.appendChild(two, "apple");
	banana=tr.appendChild(two, "banana");
	tr.appendChild(banana,"cherry");
	tr.appendChild(two, "peach");
	tr.appendChild(one,"three");

	loc=find(tr.begin(), tr.end(), "two");

	if(loc!=tr.end()) 
	{
		Tree<string>::siblingIterator sib=tr.beginSibling(loc); 
		
		while(sib!=tr.endSibling(loc)) 
		{
			cout << (*sib) << endl;
			++sib;
		}
		
		cout << endl;

		Tree<string>::preOrderIterator sib2=tr.beginSibling(loc);
		Tree<string>::preOrderIterator end2=tr.endSibling(loc);

		while(sib2!=end2) 
		{
			for(int i=0; i<tr.depth(sib2)-2; ++i) 
				cout << " ";
			cout << (*sib2) << endl;
			++sib2;
		}
	}
}