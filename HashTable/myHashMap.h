
// Last modified by Wei Shi 04/25/2015

#ifndef MY_HASH_MAP_H
#define MY_HASH_MAP_H

//#include "HashFunction/stringHash.h"
#include <vector>
#include <algorithm>
#include <string>
#include <typeinfo>
#include <stdexcept>
using namespace std;

/*
README 
i.		Completed every part.

ii.		No bugs

iii.	IMPORTANT: 1. For the sake of this homework. 
								 		This program only works for keys of unsigned int and string type.

								 2. This program does not detect copies of strings in the second object parameter(in this case vector<string>).

			1. Open terminal and type make clean(to delete old object files) just in case.
			2. Type make all.
			3. For probe testing, type: ./testProbe documents/words.txt 
			3. For word puzzle using STL's unordered map, type: ./testMaps documents/words.txt 
			4. For word puzzle using my own hash table, type: ./testMyHashMap documents/words.txt <initial hash table size>
			5. Follow the prompts.
			6. The analysis is contained in folder called "Analysis", which are in format:

						 OpenDocument text
					

iv.		input file : document/words.txt		


			Output files used for probe testing:
	
			output files : document/lower.txt
										 document/upper.txt
*/

template <typename key, typename value>
class myHashTable
{
  public: enum EntryType { ACTIVE, EMPTY, DELETED };

  private:
    struct HashEntry
    {
        key first;
				value second;
        EntryType info;

				HashEntry( const key & k = key{ }, const value & v = value{ }, EntryType cellStatus = EMPTY )
				  : first{ k }, second{ v }, info{ cellStatus } { }
				
				HashEntry( key && k, value && v, EntryType cellStatus = EMPTY )
				  : first{ std::move( k ) }, second{ std::move( v ) }, info{ cellStatus } { }
    };


    //array of HashEntry, or the hash table itself.
    vector<HashEntry> array_;
		unsigned int numOfItems_ = 0;
		unsigned int numOfActives_ = 0;


    bool isActive( int currentPos ) const;

    size_t findPos( const key & x ) const;

    void rehash( );

    size_t myhash( const key & x ) const;

/*
		//hash function for unsigned int
    size_t myhash( const unsigned int & usigned_key ) const;
		//hash function for string
    size_t myhash( const string & string_key ) const;
*/
  public:
		//cannot use int & size if we want a default value for the parameter.
    explicit myHashTable( unsigned int size ); 

    bool contains( const key & k ) const;

    void makeEmpty( );

		//insert functions are primarily used for rehashing.
		//There is no need for client to use insert over operator[].
    bool insert(  const HashEntry & entry );
    bool insert( HashEntry && entry  );

		//returns first active cell index in array_
   	const unsigned int begin( ) const;

		//returns (last active cell index) + 1 in array_
   	const unsigned int end( ) const;

		//accepts key
		//getNext functions returns next active cell index after key.
    HashEntry& getNext( const HashEntry & h ) const;
    unsigned int getNext( const key & k ) const;
    unsigned int getNext( key && k ) const;
		unsigned int getNext( unsigned int & index ) const;

    bool remove( const key & k );

    const HashEntry& getEntryAt( const unsigned int & index ) const;

		//returns end() if item doesn't exist. Otherwise returns index of key k.
    unsigned int getIndexOf( const key& k) const;

		const value& operator[]( const key & k ) const;
		const value& operator[]( key && k ) const;

		value& operator[]( const key & k );
		value& operator[]( key && k );

    //void print();

    unsigned int size();
    unsigned int numOfItems();
    unsigned int numOfActives();

};

#include "myHashMap.cpp"

#endif
