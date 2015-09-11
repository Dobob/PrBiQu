// By Mark Allen Weiss
// Last modified by Wei Shi 04/25/2015

template <typename Comparable>
BinomialQueue<Comparable>::BinomialQueue( ) : theTrees_( DEFAULT_TREES )//calls one parameter constructor for vector, so it always start with size of 1.
{
  for( auto & root : theTrees_ )
      root = nullptr;

  currentSize_ = 0;
}


template <typename Comparable>
BinomialQueue<Comparable>::BinomialQueue( const Comparable & item ) : theTrees_( 1 ), currentSize_{ 1 }
{ 

	theTrees_[ 0 ] = new BinomialNode{ item, nullptr, nullptr, nullptr }; 


}

template <typename Comparable>
BinomialQueue<Comparable>::BinomialQueue( const BinomialQueue & rhs ): theTrees_( rhs.theTrees_.size( ) ),currentSize_{ rhs.currentSize_ }
{ 
  for( int i = 0; i < rhs.theTrees_.size( ); ++i )
      theTrees_[ i ] = clone( rhs.theTrees_[ i ] );
}


template <typename Comparable>
BinomialQueue<Comparable>::BinomialQueue( BinomialQueue && rhs ): theTrees_{ std::move( rhs.theTrees_ ) }, currentSize_{ rhs.currentSize_ }
{ 

}

template <typename Comparable>
BinomialQueue<Comparable>::~BinomialQueue( )
{ 

	makeEmpty( ); 

}
template <typename Comparable>
BinomialQueue<Comparable> & BinomialQueue<Comparable>::operator=( const BinomialQueue & rhs )
{
    BinomialQueue copy = rhs;
    std::swap( *this, copy );
    return *this;
}

template <typename Comparable>
BinomialQueue<Comparable> & BinomialQueue<Comparable>::operator=( BinomialQueue && rhs )
{
	std::swap( currentSize_, rhs.currentSize_ );
	std::swap( theTrees_, rhs.theTrees_ );

	return *this;
}

template <typename Comparable>
bool BinomialQueue<Comparable>::isEmpty( ) const
{ 

	return currentSize_ == 0; 

}

template <typename Comparable>
const Comparable & BinomialQueue<Comparable>::findMin( ) const
{
  if( isEmpty( ) )
      throw underflow_error{"underflow_error: "};

  return theTrees_[ findMinIndex( ) ]->element;
}
////////////////////////////////////////////////
//
//								insert(ref)
//
////////////////////////////////////////////////
template <typename Comparable>
void BinomialQueue<Comparable>::insert( const Comparable & x )
{ 
	BinomialQueue itemToInsert{x};
	//merge(itemToInsert);

	//work_ctr_+= 1; //addition is technically an assignment since it returns a value equal to two addends that is then assigned to the left hand variable.
  currentSize_ += itemToInsert.currentSize_; //update size.


	//checks to see if current vector can contain combined number of items.
	//if not, then resize theTrees_.

	//5 + vector resize() + 1 + loop
  if( currentSize_ > capacity( ) )
  {

    int oldNumTrees = theTrees_.size( ); //previous theTrees_ size
    int newNumTrees = max( theTrees_.size( ), itemToInsert.theTrees_.size( ) ) + 1;

    theTrees_.resize( newNumTrees );

		work_ctr_+=4; // last 4 lines of code

		//sets everything from theTrees_.size( ) forward to nullptr.
    for( int i = oldNumTrees; i < newNumTrees; ++i )// ++i => i = i + 1
		{
      theTrees_[ i ] = nullptr;
			work_ctr_+=3;
		}
  }

  BinomialNode *carry = nullptr;

	//BQ itemToInsert only contains one item at slot 0. After it is inserted, it becomes empty.
  BinomialNode *t2 = itemToInsert.theTrees_[ 0 ];

	//the BQ containing item to insert always exist, so whichCase start at 2.
	//then afterward, it is never incremented, so we omit checking to see if t2 == nullptr since it will always be nullptr.
  int whichCase = 2;

	//work_ctr_+=5; //last 3 lines of code plus 2 assignment in for loop

  for( int i = 0, j = 1; j <= currentSize_; ++i, j *= 2 )
  {
		//work_ctr_+=3; // loop compare, assign, and arithmetic.

    BinomialNode *t1 = theTrees_[ i ]; 



//YOU DON'T WANT TO REMOVE whichCase ADDITIONS! The program loops. t2 will become nullptr after case 3, when two same slot items are combined to form a carry, and the current slot points to nullptr!

		//0 : signifies that theTrees_[i] is empty or nullptr.
		//1 : signifies that theTrees_[i] points to a tree.
    whichCase += t1 == nullptr ? 0 : 1;

		//0 : signifies that carry is empty or nullptr.
		//4 : signifies that carry points to a tree.
    whichCase += carry == nullptr ? 0 : 4;

		//work_ctr_+=5; //last 3 lines of code
    switch( whichCase )
    {
			//IMPORTANT: although pointers in the tree change, the copied pointers stored in hashTable_ remains pointing to the memory locations for the nodes, so we don't have to change hashTable_ pointers when we change node pointers in the tree.

			//CASE 1 and 2 are impossible, since itemToInsert always exist as a BQ.

		  case 2: // Only itemToInsert : add itemToInsert binomial tree to hashTable_
//cout << "CASE 2" << endl;
		    theTrees_[ i ] = t2; //copies t2 into theTrees_[i]

				//we need to add every node and its pointer to the hashTable_.
				toHash(t2);

		    itemToInsert.theTrees_[ i ] = nullptr; //deletes itemToInsert.theTrees_[i]

				work_ctr_+=3; // 2 assign, 1 compare.
		    break;

		  case 4: // Only carry : itemToInsert and this at i is empty, and we already have the carried tree in hashTable_ 
//cout << "CASE 4" << endl;
		    theTrees_[ i ] = carry;
		    carry = nullptr;

				work_ctr_+=4; // 2 assign, 2 compare that includes case 2.
		    break;

		  case 3: // this and itemToInsert : t1 is already in our hash table. We only need to add t2 to our hash table 
				//t1 is already in our hashTable_ because for it to be in this instance of bq, it must have been inserted some time in the past.
				//Hence it must be contained in the hashTable_ of this same instance since insertion uses hashTable_-dependent merge.
//cout << "CASE 3" << endl;
		    carry = combineTrees( t1, t2 );
				toHash(t2);
		    theTrees_[ i ] = itemToInsert.theTrees_[ i ] = nullptr;

				work_ctr_+=10; //combineTrees does 4 work. 2 assign and including case 2 and 4 comparisons
		    break;

		  case 5: // this and carry : the carry must be created in case 3, so our hashTable_ contains everything already 
//cout << "CASE 5" << endl;
		    carry = combineTrees( t1, carry );
		    theTrees_[ i ] = nullptr;

				work_ctr_+=10; // case 2, 4, 3 comparsions so 3 compares, 4 combineTrees work, 2 assign, 1 access
		    break;


				//CASE 6: impossible to have itemToInsert and carry at the same time because itemToInsert contains only one item at slot 0, and no carry is possible from a slot before slot 0.

				//CASE 7: impossible to have itemToInsert, this tree, and carry at the same time for the same reason as case 6.

    }
		//after the first index, itemToInsert BQ contains nullptr for all other slots.
		t2 = nullptr;
		//since whichCase is declared outside of the loop, we need to set it to 0 before the next iteration.
		whichCase = 0;

		work_ctr_+=10; // outside of switch 
  }


		work_ctr_+=9;

}
////////////////////////////////////////////////
//
//								insert(move)
//
////////////////////////////////////////////////
template <typename Comparable>
void BinomialQueue<Comparable>::insert( Comparable && x )
{ 

	//work_ctr_ += 3; //one item constructor does 3 assignments and one access
	BinomialQueue itemToInsert{ std::move( x ) }; 
	//merge(itemToInsert);

	//work_ctr_+= 1; //addition is technically an assignment since it returns a value equal to two addends that is then assigned to the left hand variable.
	currentSize_ += itemToInsert.currentSize_; //update size.


	//checks to see if current vector can contain combined number of items.
	//if not, then resize theTrees_.

	//5 + vector resize() + 1 + loop
	if( currentSize_ > capacity( ) )
	{

	  int oldNumTrees = theTrees_.size( ); //previous theTrees_ size
	  int newNumTrees = max( theTrees_.size( ), itemToInsert.theTrees_.size( ) ) + 1;

	  theTrees_.resize( newNumTrees );

		work_ctr_+=4; // last 4 lines of code

		//sets everything from theTrees_.size( ) forward to nullptr.
	  for( int i = oldNumTrees; i < newNumTrees; ++i )// ++i => i = i + 1
		{
	    theTrees_[ i ] = nullptr;
			work_ctr_+=3;
		}
	}

	BinomialNode *carry = nullptr;

	//BQ itemToInsert only contains one item at slot 0. After it is inserted, it becomes empty.
	BinomialNode *t2 = itemToInsert.theTrees_[ 0 ];

	//the BQ containing item to insert always exist, so whichCase start at 2.
	//then afterward, it is never incremented, so we omit checking to see if t2 == nullptr since it will always be nullptr.
	int whichCase = 2;

	//work_ctr_+=5; //last 3 lines of code plus 2 assignment in for loop

	for( int i = 0, j = 1; j <= currentSize_; ++i, j *= 2 )
	{
		//work_ctr_+=3; // loop compare, assign, and arithmetic.

	  BinomialNode *t1 = theTrees_[ i ]; 



//YOU DON'T WANT TO REMOVE whichCase ADDITIONS! The program loops. t2 will become nullptr after case 3, when two same slot items are combined to form a carry, and the current slot points to nullptr!

		//0 : signifies that theTrees_[i] is empty or nullptr.
		//1 : signifies that theTrees_[i] points to a tree.
	  whichCase += t1 == nullptr ? 0 : 1;

		//0 : signifies that carry is empty or nullptr.
		//4 : signifies that carry points to a tree.
	  whichCase += carry == nullptr ? 0 : 4;

		//work_ctr_+=5; //last 3 lines of code
	  switch( whichCase )
	  {
			//IMPORTANT: although pointers in the tree change, the copied pointers stored in hashTable_ remains pointing to the memory locations for the nodes, so we don't have to change hashTable_ pointers when we change node pointers in the tree.

			//CASE 1 and 2 are impossible, since itemToInsert always exist as a BQ.

			case 2: // Only itemToInsert : add itemToInsert binomial tree to hashTable_
//cout << "CASE 2" << endl;
			  theTrees_[ i ] = t2; //copies t2 into theTrees_[i]

				//we need to add every node and its pointer to the hashTable_.
				toHash(t2);

			  itemToInsert.theTrees_[ i ] = nullptr; //deletes itemToInsert.theTrees_[i]

				work_ctr_+=3; // 2 assign, 1 compare.
			  break;

			case 4: // Only carry : itemToInsert and this at i is empty, and we already have the carried tree in hashTable_ 
//cout << "CASE 4" << endl;
			  theTrees_[ i ] = carry;
			  carry = nullptr;

				work_ctr_+=4; // 2 assign, 2 compare that includes case 2.
			  break;

			case 3: // this and itemToInsert : t1 is already in our hash table. We only need to add t2 to our hash table 
				//t1 is already in our hashTable_ because for it to be in this instance of bq, it must have been inserted some time in the past.
				//Hence it must be contained in the hashTable_ of this same instance since insertion uses hashTable_-dependent merge.
//cout << "CASE 3" << endl;
			  carry = combineTrees( t1, t2 );
				toHash(t2);
			  theTrees_[ i ] = itemToInsert.theTrees_[ i ] = nullptr;

				work_ctr_+=10; //combineTrees does 4 work. 2 assign and including case 2 and 4 comparisons
			  break;

			case 5: // this and carry : the carry must be created in case 3, so our hashTable_ contains everything already 
//cout << "CASE 5" << endl;
			  carry = combineTrees( t1, carry );
			  theTrees_[ i ] = nullptr;

				work_ctr_+=10; // case 2, 4, 3 comparsions so 3 compares, 4 combineTrees work, 2 assign, 1 access
			  break;

				//CASE 6: impossible to have itemToInsert and carry at the same time because itemToInsert contains only one item at slot 0, and no carry is possible from a slot before slot 0.

				//CASE 7: impossible to have itemToInsert, this tree, and carry at the same time for the same reason as case 6.

	  }
		//after the first index, itemToInsert BQ contains nullptr for all other slots.
		t2 = nullptr;
		//since whichCase is declared outside of the loop, we need to set it to 0 before the next iteration.
		whichCase = 0;

		work_ctr_+=10; // outside of switch 
	}


		work_ctr_+=9;


	
}
////////////////////////////////////////////////
//
//							deleteMin( )
//
////////////////////////////////////////////////
template <typename Comparable>
void BinomialQueue<Comparable>::deleteMin( )
{
  Comparable x;
  deleteMin( x );
}
////////////////////////////////////////////////
//
//							deleteMin(item)
//
////////////////////////////////////////////////
template <typename Comparable>
void BinomialQueue<Comparable>::deleteMin( Comparable & minItem )
{
	//breaks H into:
	//H1: H without the binomial tree containing the smallest root.
	//H2: The series of binomial trees obtained by removing the root from the binomial tree with the smallest root.
	
	
/*
For bq H0:

1. find binomial tree with smallest root, bk.

2. remove the tree with that smallest root:
	 H0 - bk = H1

3. remove root of bk:
	 bk - root = b0, b1, ..., bk-1 = H2

4. Merge H1 and H2.

*/


  if( isEmpty( ) )
      throw underflow_error{"underflow_error: "};

  int minIndex = findMinIndex( ); //first find index of minimum root in theTrees_.

  minItem = theTrees_[ minIndex ]->element; //store the minimum item in referenced variable minItem from parameter.


  BinomialNode *minTree = theTrees_[ minIndex ];
  BinomialNode *minTreeWithoutRoot = minTree->leftChild;

	hashTable_.remove(minTree->element);

  delete minTree;

  //Construct H2(minTreeWithoutRoot is only a pointer, we need a bq to merge)

	//deletedQueue is to be the bq without the root from minTree.
  BinomialQueue deletedQueue;

	//The index correspond to the number of subtrees of a binomial tree at index i, e.g., index 8 contains a root node with 8 children.
	//hence if the root is deleted, we have 8 children that can be placed in an array of size 8, or 8+1 in this case.
	//why minIndex + 1?
  deletedQueue.theTrees_.resize( minIndex + 1 );

	//size of a bq is equal to the number of nodes. Each index i contains 2^i nodes.
	//we know the vector is full since each binomial tree must have children that are also binomial trees.
	//Since we used the children of minTree, and height difference between adjacent children is 1 and there are minIndex children
	//we know there are no gaps or jumps between the height of each children greater than 1. 
  deletedQueue.currentSize_ = ( 1 << minIndex ) - 1;

	//now we put items from minTreeWithoutRoot into deletedQueue starting at the largest index minus 1:
  for( int j = minIndex - 1; j >= 0; --j )
  {
			//we need to set each b0, b1, ..., bk-1 parent pointer to nullptr.
			minTreeWithoutRoot->parent = nullptr;

      deletedQueue.theTrees_[ j ] = minTreeWithoutRoot;

			//note that we only need the parent pointer, i.e. pointers to root nodes of b0, b1, ..., bk-1
      minTreeWithoutRoot = minTreeWithoutRoot->nextSibling;

      deletedQueue.theTrees_[ j ]->nextSibling = nullptr;
  }

  // Construct H1
  theTrees_[ minIndex ] = nullptr;

	//we have just removed from H0 the deletedQueue and its former root, hence the plus 1.
  currentSize_ -= deletedQueue.currentSize_ + 1;

  merge( deletedQueue );



}

template <typename Comparable>
void BinomialQueue<Comparable>::makeEmpty( )
{
  currentSize_ = 0;
  for( auto & root : theTrees_ )
  	makeEmpty( root );
}
////////////////////////////////////////////////
//
//									merge
//
////////////////////////////////////////////////
template <typename Comparable>
void BinomialQueue<Comparable>::merge( BinomialQueue & rhs )
{

  if( this == &rhs )    // Avoid aliasing problems(the same place in memory being accessed by different names.
    return;

  currentSize_ += rhs.currentSize_; //update size.

	//checks to see if current vector can contain combined number of items.
	//if not, then resize theTrees_.
  if( currentSize_ > capacity( ) )
  {
    int oldNumTrees = theTrees_.size( );
    int newNumTrees = max( theTrees_.size( ), rhs.theTrees_.size( ) ) + 1;

    theTrees_.resize( newNumTrees );
		//sets everything from theTrees_.size( ) forward to nullptr.
    for( int i = oldNumTrees; i < newNumTrees; ++i )
      theTrees_[ i ] = nullptr;
  }

	//carry over to the next, more significant tree.
  BinomialNode *carry = nullptr;

	//j is used for the size counter.
	//why j*=2? How do we know currentSize_ is a multiple of 2?
  for( int i = 0, j = 1; j <= currentSize_; ++i, j *= 2 )
  {
    BinomialNode *t1 = theTrees_[ i ]; //Since in decreasing order, theTrees_[0] contains the largest binomial tree.

		//*t2=rhs.theTrees_[ i ] if i < rhs.theTrees_.size( ), else nullptr.
		//this is to ensure that rhs.theTrees_[ i ] can be accessed and that we are not accessing outside the vector's range.
    BinomialNode *t2 = i < rhs.theTrees_.size( ) ? rhs.theTrees_[ i ] : nullptr;

		//if t1 == nullptr, then whichCase = 0. 
		//0 : signifies that theTrees_[i] is empty or nullptr.
		//1 : signifies that theTrees_[i] points to a tree.
    int whichCase = t1 == nullptr ? 0 : 1;

		//0 : signifies that rhs.theTrees_[ i ] is empty or nullptr.
		//2 : signifies that rhs.theTrees_[ i ] points to a tree.
    whichCase += t2 == nullptr ? 0 : 2;

		//0 : signifies that carry is empty or nullptr.
		//4 : signifies that carry points to a tree.
    whichCase += carry == nullptr ? 0 : 4;
		
    switch( whichCase )
    {
			//IMPORTANT: although pointers in the tree change, the copied pointers stored in hashTable_ remains pointing to the memory locations for the nodes, so we don't have to change hashTable_ pointers when we change node pointers in the tree.

		  case 0: /* No trees : no need to update hashTable_ */

		  case 1: /* Only this : no need to update hashTable_ */
		    break;

		  case 2: /* Only rhs : add rhs binomial tree to hashTable_*/
		    theTrees_[ i ] = t2; //copies t2 into theTrees_[i]

				//we need to add every node and its pointer to the hashTable_.
				toHash(t2);

		    rhs.theTrees_[ i ] = nullptr; //deletes rhs.theTrees_[i]

		    break;

		  case 4: /* Only carry : rhs and this at i is empty, and we already have the carried tree in hashTable_ */
		    theTrees_[ i ] = carry;
		    carry = nullptr;
		    break;

		  case 3: /* this and rhs : t1 is already in our hash table. We only need to add t2 to our hash table */
				//t1 is already in our hashTable_ because for it to be in this instance of bq, it must have been inserted some time in the past.
				//Hence it must be contained in the hashTable_ of this same instance since insertion uses hashTable_-dependent merge.
		    carry = combineTrees( t1, t2 );
				toHash(t2);
		    theTrees_[ i ] = rhs.theTrees_[ i ] = nullptr;

		    break;

		  case 5: /* this and carry : the carry must be created in case 3, so our hashTable_ contains everything already */
		    carry = combineTrees( t1, carry );
		    theTrees_[ i ] = nullptr;
		    break;

		  case 6: /* rhs and carry: hashTable_ contains pointers for all nodes in carry, we only need to add rhs to hashTable_ */
		    carry = combineTrees( t2, carry );
				toHash(t2);
		    rhs.theTrees_[ i ] = nullptr;

		    break;

		  case 7: /* All three */
		    theTrees_[ i ] = carry; //sets current index to contain carry tree
		    carry = combineTrees( t1, t2 ); //combine t1 and t2 to have it carry over to next tree height size.
				toHash(t2);
		    rhs.theTrees_[ i ] = nullptr;
		    break;

    }
  }

  for( auto & root : rhs.theTrees_ )
		root = nullptr;
  rhs.currentSize_ = 0;


}  
////////////////////////////////////////////////
//
//								contains(ref)
//
////////////////////////////////////////////////
template <typename Comparable>
bool BinomialQueue<Comparable>::contains(const Comparable & key) const
{
	BinomialNode * temp = find(key);

	if(temp != nullptr)
		return true;

	return false;


}
////////////////////////////////////////////////
//
//								contains(move)
//
////////////////////////////////////////////////
template <typename Comparable>
bool BinomialQueue<Comparable>::contains(Comparable && key) const
{

	BinomialNode * temp = find(key);

	if(temp != nullptr)
		return true;

	return false;


}
////////////////////////////////////////////////
//
//					  			size
//
////////////////////////////////////////////////
template <typename Comparable>
const unsigned int& BinomialQueue<Comparable>::size() const
{


	return currentSize_;

}
////////////////////////////////////////////////
//
//									parentOf
//
////////////////////////////////////////////////
template <typename Comparable>
bool BinomialQueue<Comparable>::parentOf(const Comparable & key) const
{

	BinomialNode* temp = find(key);

	if(temp != nullptr && temp->parent != nullptr)
	{
		if(temp->parent->element > temp->element)
			return false;

		cout << "Parent of " << key << " is " << temp->parent->element << endl;
		return true;

	}

	


}
////////////////////////////////////////////////
//
//								remove(ref)
//
////////////////////////////////////////////////
template <typename Comparable>
bool BinomialQueue<Comparable>::remove(const Comparable & key)
{
  if( isEmpty( ) )
      throw underflow_error{"underflow_error: "};


	BinomialNode* nodeToDelete = find(key);

	if(nodeToDelete == nullptr)
		return false;

	//moveToRoot returns the root node, which contains the key to be removed.
	nodeToDelete = moveToRoot(nodeToDelete);

	//cout << "nodeToDelete->element: " << nodeToDelete->element << endl;
	//the index will never be -1.
  int index = findIndexOfRoot(nodeToDelete->element); 

	//tempTreeWithoutRoot points to b0, b1, ..., bk-1.
  BinomialNode *tempTreeWithoutRoot = nodeToDelete->leftChild;

	hashTable_.remove(nodeToDelete->element);

  delete nodeToDelete;

  //Construct H2
  BinomialQueue deletedQueue;


  deletedQueue.theTrees_.resize( index + 1 );
 
  deletedQueue.currentSize_ = ( 1 << index ) - 1;

  for( int j = index - 1; j >= 0; --j )
  {
			tempTreeWithoutRoot->parent = nullptr;

      deletedQueue.theTrees_[ j ] = tempTreeWithoutRoot;

      tempTreeWithoutRoot = tempTreeWithoutRoot->nextSibling;

      deletedQueue.theTrees_[ j ]->nextSibling = nullptr;
  }

  // Construct H1
  theTrees_[ index ] = nullptr;

  currentSize_ -= deletedQueue.currentSize_ + 1;

  merge( deletedQueue );

	return true;
}
////////////////////////////////////////////////
//
//								remove(move)
//
////////////////////////////////////////////////
template <typename Comparable>
bool BinomialQueue<Comparable>::remove(Comparable && key)
{
  if( isEmpty( ) )
      throw underflow_error{"underflow_error: "};


	BinomialNode* nodeToDelete = find(key);

	if(nodeToDelete == nullptr)
		return false;

	//moveToRoot returns the root node, which contains the key to be removed.
	nodeToDelete = moveToRoot(nodeToDelete);

	//the index will never be -1.
  int index = findIndexOfRoot(nodeToDelete->element); 

	//tempTreeWithoutRoot points to b0, b1, ..., bk-1.
  BinomialNode *tempTreeWithoutRoot = nodeToDelete->leftChild;

	//cout << "nodeToDelete->element: " << nodeToDelete->element << endl;
	hashTable_.remove(nodeToDelete->element);

  delete nodeToDelete;

  //Construct H2
  BinomialQueue deletedQueue;


  deletedQueue.theTrees_.resize( index + 1 );
 
  deletedQueue.currentSize_ = ( 1 << index ) - 1;

  for( int j = index - 1; j >= 0; --j )
  {
			tempTreeWithoutRoot->parent = nullptr;

      deletedQueue.theTrees_[ j ] = tempTreeWithoutRoot;

      tempTreeWithoutRoot = tempTreeWithoutRoot->nextSibling;

      deletedQueue.theTrees_[ j ]->nextSibling = nullptr;
  }

  // Construct H1
  theTrees_[ index ] = nullptr;

  currentSize_ -= deletedQueue.currentSize_ + 1;

  merge( deletedQueue );

	return true;
}

////////////////////////////////////////////////
//
//								resetWorkCtr
//
////////////////////////////////////////////////
template <typename Comparable>
void BinomialQueue<Comparable>::resetWorkCtr()
{

	work_ctr_ = 0;


}
////////////////////////////////////////////////
//
//								getWorkCount
//
////////////////////////////////////////////////
template <typename Comparable>
unsigned int BinomialQueue<Comparable>::getWorkCount()
{


	return work_ctr_;

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//																									PRIVATE
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////
//
//								findMinIndex
//
////////////////////////////////////////////////
template <typename Comparable>
int BinomialQueue<Comparable>::findMinIndex( ) const
{
  int i;
  int minIndex;

  for( i = 0; theTrees_[ i ] == nullptr; ++i )
      ;

  for( minIndex = i; i < theTrees_.size( ); ++i )
    if( theTrees_[ i ] != nullptr && theTrees_[ i ]->element < theTrees_[ minIndex ]->element )
		{
			//cout << "theTrees_[ i ]->element: " << theTrees_[ i ]->element << endl;
			//cout << "theTrees_[ minIndex ]->element : " << theTrees_[ minIndex ]->element  << endl;

      minIndex = i;


		}

  return minIndex;
}
////////////////////////////////////////////////
//
//						findIndexOfRoot
//
////////////////////////////////////////////////
template <typename Comparable>
int BinomialQueue<Comparable>::findIndexOfRoot(const Comparable & key) const
{
  int index;

	//skip first few empty slots
  for( index = 0; theTrees_[ index ] == nullptr; ++index )
      ;

  for( index; index < theTrees_.size( ); ++index )
	{
		//theTrees_[ index ] != nullptr is needed before dereferencing theTrees_[ index ]
		if(theTrees_[ index ] != nullptr && theTrees_[ index ]->element == key)
			return index;

	}

	return -1;
}
////////////////////////////////////////////////
//
//								capacity
//
////////////////////////////////////////////////
template <typename Comparable>
int BinomialQueue<Comparable>::capacity( ) const
{ 
	return ( 1 << theTrees_.size( ) ) - 1; 


}

////////////////////////////////////////////////
//
//							combineTrees
//
////////////////////////////////////////////////
template <typename Comparable>
typename BinomialQueue<Comparable>::BinomialNode * BinomialQueue<Comparable>::combineTrees( BinomialNode *t1, BinomialNode *t2 )
{
	//we are assuming t1 < t2
	
//cout << "t1 == nullptr: " << (t1 == nullptr) << endl;
//cout << "t2 == nullptr: " << (t2 == nullptr) << endl;
  if( t2->element < t1->element )
    return combineTrees( t2, t1 );

	//we know we are always combining binomial trees of same height.
	//therefore t2 is our largest subtree, and will represent our left most path.
  t2->nextSibling = t1->leftChild;

  t2->parent = t1;

  t1->leftChild = t2;

	//all of the other pointers in the lower depths remain the same, only the pointers to the root of the binomial trees change.

  return t1;

}


template <typename Comparable>
void BinomialQueue<Comparable>::makeEmpty( BinomialNode * & t )
{
  if( t != nullptr )
  {
    makeEmpty( t->leftChild );
    makeEmpty( t->nextSibling );
    delete t;
    t = nullptr;
  }
}

template <typename Comparable>
typename BinomialQueue<Comparable>::BinomialNode * BinomialQueue<Comparable>::clone( BinomialNode * t ) const
{
  if( t == nullptr )
    return nullptr;
  else
    return new BinomialNode{ t->element, clone( t->leftChild ), clone( t->nextSibling ) };
}

////////////////////////////////////////////////
//
//								toHash
//
////////////////////////////////////////////////
template <typename Comparable>
void BinomialQueue<Comparable>::toHash(BinomialNode * t)
{

/*
        Comparable    element;
        BinomialNode *leftChild;
        BinomialNode *nextSibling;
*/

	if(t != nullptr)
	{
		hashTable_[t->element]= t;

		work_ctr_+=3; // last 2 lines of code contains access and assign and compare.

		toHash(t->nextSibling);
		toHash(t->leftChild);

	}


}

////////////////////////////////////////////////
//
//									find
//
////////////////////////////////////////////////
template <typename Comparable>
typename BinomialQueue<Comparable>::BinomialNode* BinomialQueue<Comparable>::find(const Comparable & key) const
{

	BinomialNode* temp = nullptr;

	if(hashTable_.contains(key))
		temp = hashTable_[key];


	return temp;





}


////////////////////////////////////////////////
//
//								moveToRoot
//
////////////////////////////////////////////////

template <typename Comparable>
typename BinomialQueue<Comparable>::BinomialNode* BinomialQueue<Comparable>::moveToRoot(BinomialNode * t)
{

	//order of testing while loop here is very important. We want to make sure t->parent is not nullptr before accessing t->parent->element.
	while(t->parent != nullptr)
	{
		//as a result of changing values in the node instead of the nodes themselves, we change less pointer.
		//However, we do need to update hashTable_.

		Comparable percolatedItem = t->element;
		
		hashTable_[t->parent->element] = t; 

		t->element = t->parent->element;

		t->parent->element = percolatedItem;

		t = t->parent;

	}
	//cout << "t->element: " << t->element << endl;
	//return the new root node.
	return t;



}




























