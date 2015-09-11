//Last modified by Wei Shi 04/25/2015

#include "BinomialQueue/BinomialQueue.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;


template<typename binoq>
void parse(binoq & container)
{

	cout << "Reading from document/words.txt...\n" << endl;

	string input_string;

	ifstream instream;

	instream.open("document/words.txt");

	while(instream.good())
	{
		getline(instream, input_string);

		if(input_string != "")
		{
			if(input_string[input_string.length()-1] == ' ')
				input_string.erase( (input_string.length()-1), 1);

			container.insert(input_string);

		}

	}



}


template<typename binoq>
void search(const binoq & container)
{
	string word;

	while(word != "~q")
	{
		cout << "\nEnter a string key to see if it is in the binomial queue or \"~q\" to move to the next exercise." << endl;	
		cin >> word;
		
		if(container.contains(word))
			cout << "The binomial queue contains your word." << endl;
			
		else if(word != "~q") 
			cout << "The binomial queue doesn't contain your word." << endl;

	}


}

template<typename binoq>
void removal(binoq & container)
{

	string word;
	cout << "\nYou'll now be prompt to enter 5 strings to be removed from the binomial queue." << endl;

	bool found;
	for(int i = 0; i< 5; i++)
	{

		cout << "\nEnter a string key to delete or enter \"~q\" to move on to next exercise: " << endl;	
		cin >> word;
		
		if(word != "~q" && container.contains(word))
		{

			container.remove(word);
			cout << "Your word has been removed from the binomial queue." << endl;

 			found = container.contains(word);

			cout << "words.contains(" << word << ") should return 0 : " << found << endl;
			if(!found)
				cout << "The binomial queue no longer contains your item." << endl;



		}

		else if(word == "~q")
			break;

		else 
		{
			cout << "The binomial queue doesn't contain your word, try again or enter \"~q\" to move on to next exercise." << endl;
			i--;
		}


	}

}



int main( )
{

	try
	{
		BinomialQueue<string> words;

		parse(words);

		cout << "Insertion work: " << words.getWorkCount() << endl;

		cout << "\nDeleting the 10 minimum items..." << endl;

		string word;

		for(int i = 0; i < 10; i++)
		{

			words.deleteMin(word);
			cout << "Item deleted: " << word << endl;
		}

		bool repeat = 1;
		while(repeat)
		{
			search(words);
			removal(words);

			cout << "\nDo you want to repeat the exercise with the changed binomial queue? (Y/N)" << endl;
			cin >> word;

			if(word == "N" || word == "n")
				repeat = 0;

		}

		words.resetWorkCtr();


	}


	catch(underflow_error & UFE)
	{

		cout << UFE.what() << endl;


	}









  return 0;
}
