
// Last modified by Wei Shi 04/25/2015

#include "../PrimeFunction/prime.cpp"
#include <iostream>
using namespace std;




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//																			Quadratic Hashing Methods
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//																									PUBLIC
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//error: only declarations of constructors can be ‘explicit’

template <typename key, typename value>
myHashTable<key, value>:: myHashTable( unsigned int size = 101 ) : array_( nextPrime( size ) )
{ 

	makeEmpty( );

}

template <typename key, typename value>
bool myHashTable<key, value>:: contains( const key & k ) const
{

  return isActive( findPos( k ) );

}


template <typename key, typename value>
void myHashTable<key, value>:: makeEmpty( )
{
  numOfItems_ = 0;
  for( auto & entry : array_ )
      entry.info = EMPTY;
}

template <typename key, typename value>
bool myHashTable<key, value>:: insert( const typename myHashTable<key, value>::HashEntry & entry )
{
		  // Insert x as active
	unsigned int currentPos = findPos( entry.first );

	if( array_[ currentPos ].info == ACTIVE )
		  return false;

		array_[ currentPos ].first = entry.first;
		array_[ currentPos ].second = entry.second;
		array_[ currentPos ].info = ACTIVE;

		  // Rehash; see Section 5.5
	if( ++numOfItems_ > array_.size( ) / 2 )
		  rehash( );

	return true;
}

template <typename key, typename value>
bool myHashTable<key, value>:: insert( typename myHashTable<key, value>::HashEntry && entry)
{
      // Insert x as active
  unsigned int currentPos = findPos( entry.first );
  if( array_[ currentPos ].info == ACTIVE )
      return false;

  array_[ currentPos ].first = std::move( entry.first );
  array_[ currentPos ].second = std::move( entry.second );
  array_[ currentPos ].info = ACTIVE;


      // Rehash; see Section 5.5
  if( ++numOfItems_ > array_.size( ) / 2 )
      rehash( );


  return true;
}


template <typename key, typename value>
bool myHashTable<key, value>:: remove( const key & k )
{
  unsigned int currentPos = findPos( k );
  if(  array_[ currentPos ].info != ACTIVE  )
      return false;

	//the book for some reason didn't implement the setting the second to default value.
	//array_[currentPos].second = value{};

  array_[ currentPos ].info = DELETED;
  return true;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//																							BEGIN: operator[]
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//PROBLEM: accessor operator is not being used as an accessor!

template <typename key, typename value>
const value& myHashTable<key, value>::operator[]( const key & k ) const
{	//accessor operator[] changes nothing, the previous cell status remains.

	//cout << "Inside accessor operator[]" << endl;
	if(numOfItems_ == 0)
		throw runtime_error{"Error: Empty array."};

	unsigned int index = findPos( k );
/*
	if(index >= array_.size())
		throw runtime_error{"Error: Index larger than or equal to array size."};
*/

	return array_[index].second;


}
template <typename key, typename value>
const value& myHashTable<key, value>::operator[]( key && k ) const
{	//accessor operator[] changes nothing, the previous cell status remains.


	//cout << "Inside accessor operator[]" << endl;
	if(numOfItems_ == 0)
		throw runtime_error{"Error: Empty array."};

	unsigned int index = findPos( k );
/*
	if(index >= array_.size())
		throw runtime_error{"Error: Index larger than or equal to array size."};
*/

	return array_[index].second;


}

template <typename key, typename value>
value& myHashTable<key, value>::operator[]( const key & k )
{	//mutator operator[] changes the value, making the cell ACTIVE.

	//cout << "Inside mutator operator[]" << endl;
	unsigned int index = findPos( k );
/*
	if(index >= array_.size())
		throw runtime_error{"Error: Index larger than or equal to array size."};
*/
	//findpos returns empty cell or the cell containing key k.


	if(!isActive(index))
	{
		array_[index].info = ACTIVE;

		array_[index].first = k;
		array_[index].second = value{};

		numOfItems_++;

		if( numOfItems_ > array_.size( ) / 2 )
		    rehash( );


	}



	return array_[index].second;

}

template <typename key, typename value>
value& myHashTable<key, value>::operator[]( key && k ) 
{	//mutator operator[] changes the value, making the cell ACTIVE.

	//cout << "Inside mutator operator[]" << endl;
	unsigned int index = findPos( k );
/*
	if(index >= array_.size())
		throw runtime_error{"Error: Index larger than or equal to array size."};
*/
	//findpos returns index of empty cell or the cell containing key k.

	if(!isActive(index))
	{
		array_[index].info = ACTIVE;

		array_[index].first = k;
		array_[index].second = value{};

		numOfItems_++;

		if( numOfItems_ > array_.size( ) / 2 )
		    rehash( );



	}



	return array_[index].second;

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//																							END: operator[]
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////
//
//										size
//
////////////////////////////////////////////////
template <typename key, typename value>
unsigned int myHashTable<key, value>:: size( )
{
	
	return array_.size();
}


////////////////////////////////////////////////
//
//								 numOfItems
//
////////////////////////////////////////////////
template <typename key, typename value>
unsigned int myHashTable<key, value>:: numOfItems()
{
	
	return numOfItems_;
}
////////////////////////////////////////////////
//
//								 numOfActives
//
////////////////////////////////////////////////

template <typename key, typename value>
unsigned int myHashTable<key, value>:: numOfActives()
{
	numOfActives_ = 0;
	for(int i =0; i<array_.size(); i++)
	{
		if(array_[i].info == ACTIVE)
			numOfActives_++;


	}

	return numOfActives_;
}



////////////////////////////////////////////////
//
//								 begin
//
////////////////////////////////////////////////
template <typename key, typename value>
const unsigned int myHashTable<key, value>::begin() const
{

	if(numOfItems_ == 0)
		throw runtime_error{"Error: Empty array."};

	unsigned int begin = 0;

	for(; begin < array_.size(); begin++)
	{
		if(array_[begin].info == ACTIVE)
			return begin;

	}

}

////////////////////////////////////////////////
//
//								 end
//
////////////////////////////////////////////////
template <typename key, typename value>
const unsigned int myHashTable<key, value>::end() const
{


	if(numOfItems_ == 0)
		throw runtime_error{"Error: Empty array."};

	unsigned int end = array_.size()-1;

	for(; end >= 0; end--)
	{

		if(array_[end].info == ACTIVE)
		{

			end = end +1;
			return end;



		}
	}

}
////////////////////////////////////////////////
//
//								 getIndexOf
//
////////////////////////////////////////////////
template <typename key, typename value>
unsigned int myHashTable<key, value>::getIndexOf( const key& k) const
{
	unsigned int index = findPos(k);

	if( !isActive(index) )
	{

		return end();


	}

	return index;



}

////////////////////////////////////////////////
//
//								 getEntryAt
//
////////////////////////////////////////////////
template <typename key, typename value>
const typename myHashTable<key, value>::HashEntry& myHashTable<key, value>::getEntryAt( const unsigned int & index ) const
{
	if( isActive(index) )
		return array_[index];

	else if( array_[index].info != ACTIVE )
		throw runtime_error{"Error: accessing inactive cell."};

	else if( index >= array_.size())
		throw runtime_error{"Error: index out of range."};

	else if( numOfItems_ == 0 )
		throw runtime_error{"Error: empty table."};


			
}


////////////////////////////////////////////////
//
//								 getNext(key)
//
////////////////////////////////////////////////
template <typename key, typename value>
unsigned int myHashTable<key, value>::getNext( const key & k ) const
{


	if(numOfItems_ == 0)
		throw runtime_error{"Error: Empty array."};

	unsigned int next = findPos(k);
	next++;

	for(; next< array_.size(); next++)
	{
		if(array_[next].info == ACTIVE)
			return next;

	}

	if(next >= array_.size())
		return end();

}

////////////////////////////////////////////////
//
//								 getNext(key)
//
////////////////////////////////////////////////
template <typename key, typename value>
unsigned int myHashTable<key, value>::getNext( key && k ) const
{

	if(numOfItems_ == 0)
		throw runtime_error{"Error: Empty array."};

	unsigned int next = findPos(k);
	next++;

	for(; next< array_.size(); next++)
	{
		if(array_[next].info == ACTIVE)
			return next;

	}

	if(next >= array_.size())
		return end();

}
////////////////////////////////////////////////
//
//								 getNext(index)
//
////////////////////////////////////////////////
template <typename key, typename value>
unsigned int myHashTable<key, value>::getNext( unsigned int & index ) const
{

	unsigned int next = index;
	next++;


	while(!(isActive(next)) && ( next!=end() ))
	{
		next++;

	}

	return next;
}

////////////////////////////////////////////////
//
//								 getNext(HashEntry)
//
////////////////////////////////////////////////
template <typename key, typename value>
typename myHashTable<key, value>::HashEntry& myHashTable<key, value>::getNext( const HashEntry & h ) const
{

	if(numOfItems_ == 0)
		throw runtime_error{"Error: Empty array."};

	unsigned int next = findPos(h.first);
	next++;

	for(; next< array_.size(); next++)
	{
		if(array_[next].info == ACTIVE)
			return array_[next];

	}


}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//																									PRIVATE
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename key, typename value>
bool myHashTable<key, value>:: isActive( int currentPos ) const
{ 
	return array_[ currentPos ].info == ACTIVE; 

}

////////////////////////////////////////////////
//
//									findPos
//
////////////////////////////////////////////////
template <typename key, typename value>
size_t myHashTable<key, value>:: findPos( const key & k ) const
{
  int offset = 1; //first offset is 1^2
  size_t currentPos = myhash( k ); //calls hash function of hashed object x.
	//cout << "myhash( " << k << " ): " << currentPos << endl; 

	//if item not in table, then we eventually find empty cell.
	//other wise we increment by 2
  while( array_[ currentPos ].info != EMPTY &&
         array_[ currentPos ].first != k )
  {

      currentPos += offset;  // Compute ith probe
      offset += 2;


      if( currentPos >= array_.size( ) )
          currentPos -= array_.size( );
  }

  return currentPos;
}
////////////////////////////////////////////////
//
//									rehash
//
////////////////////////////////////////////////

template <typename key, typename value>
void myHashTable<key, value>:: rehash( )
{
  vector<HashEntry> oldarray = array_;

  // Create new double-sized, empty table
  array_.resize( nextPrime( 2 * oldarray.size( ) ) );


  for( auto & entry : array_ )
	{
      entry.info = EMPTY;

	}


  numOfItems_ = 0;
  for( auto & entry : oldarray )
      if( entry.info == ACTIVE )
          insert( std::move( entry ));

}
template <typename key, typename value>
size_t myHashTable<key, value>::myhash( const key & x ) const
{

  static hash<key> hf;

  return hf( x ) % array_.size( );

}

/*

template <typename key, typename value>
size_t myHashTable<key, value>:: myhash( const unsigned int & usigned_key ) const
{

  static hash<unsigned int> hf;

	size_t hash_value = hf( usigned_key  ) % array_.size();


  return hash_value;



}


template <typename key, typename value>
size_t myHashTable<key, value>:: myhash( const string & string_key ) const
{

  static hashString hf;

	size_t hash_value = hf( string_key ) % array_.size();


  return hash_value;



}
*/








