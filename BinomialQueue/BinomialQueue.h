// By Mark Allen Weiss
// Last modified by Wei Shi 04/25/2015

#ifndef BINOMIAL_QUEUE_H
#define BINOMIAL_QUEUE_H

#include <iostream>
#include "../HashTable/MyHashMap.h"
#include <vector>
#include <stdexcept>
using namespace std;
/*
README:

WEI SHI HOMEWORK 4 

i.		Completed every part.

ii.		No bugs

iii.	IMPORTANT: This program assumes no duplicates.

			1. Open terminal and type make clean(to delete old object files) just in case.
			2. Type make all.
			3. Type: "./testBinomialQueue.exe" or "./testBinomialQueue" depending on terminal or IDE.
			5. Follow the prompts.

iv.		input file : document/words.txt		



*/
// Binomial queue class
//
// CONSTRUCTION: with no parameters
//
// ******************PUBLIC OPERATIONS*********************
// void insert( x )       --> Insert x
// deleteMin( )           --> Return and remove smallest item
// Comparable findMin( )  --> Return smallest item
// bool isEmpty( )        --> Return true if empty; else false
// void makeEmpty( )      --> Remove all items
// void merge( rhs )      --> Absorb rhs into this heap
// ******************ERRORS********************************
// Throws underflow_error as warranted

template <typename Comparable>
class BinomialQueue
{

  private:
    struct BinomialNode
    {
        Comparable    element;

        BinomialNode *parent;
        BinomialNode *leftChild;
        BinomialNode *nextSibling;

        BinomialNode( const Comparable & e, BinomialNode *p, BinomialNode *lt, BinomialNode *rt )
          : element{ e }, parent{ p }, leftChild{ lt }, nextSibling{ rt } { }
        
        BinomialNode( Comparable && e, BinomialNode *p, BinomialNode *lt, BinomialNode *rt )
          : element{ std::move( e ) }, parent{ p }, leftChild{ lt }, nextSibling{ rt } { }
    };

    const static int DEFAULT_TREES = 1;


		//Each insert, deleteMin, and merge, the hashTable is updated.
		myHashTable< Comparable, BinomialNode* > hashTable_;
    vector<BinomialNode *> theTrees_;  // An array of tree roots
    int currentSize_;                  // Number of items in the priority queue
		unsigned int work_ctr_=0;
    
    /**
     * Find index of tree containing the smallest item in the priority queue.
     * The priority queue must not be empty.
     * Return the index of tree containing the smallest item.
     */
    int findMinIndex( ) const;
		int findIndexOfRoot(const Comparable & key) const;

    //Return the capacity.
    int capacity( ) const;

    //Return the result of merging equal-sized t1 and t2.
    BinomialNode * combineTrees( BinomialNode *t1, BinomialNode *t2 );


    //Make a binomial tree logically empty, and free memory.
    void makeEmpty( BinomialNode * & t );


    //Internal method to clone subtree.
    BinomialNode * clone( BinomialNode * t ) const;

		//adds every node paired with its pointer starting from from t to the hashTable.
		void toHash(BinomialNode * t);

		//finds the node containing key, and returns a pointer to it.
		BinomialNode* find( const Comparable & key) const;

		//always percolates up. Used only with remove.
		BinomialNode* moveToRoot(BinomialNode * t);




  public:
    BinomialQueue( );
    BinomialQueue( const Comparable & item );
    BinomialQueue( const BinomialQueue & rhs );
    BinomialQueue( BinomialQueue && rhs );

    ~BinomialQueue( );

    BinomialQueue & operator=( const BinomialQueue & rhs );
    BinomialQueue & operator=( BinomialQueue && rhs );
    
    /**
     * Return true if empty; false otherwise.
     */
    bool isEmpty( ) const;


    /**
     * Returns minimum item.
     * Throws UnderflowException if empty.
     */
    const Comparable & findMin( ) const;

    /**
     * Insert item x into the priority queue; allows duplicates.
     */
    void insert( const Comparable & x );


    /**
     * Insert item x into the priority queue; allows duplicates.
     */
    void insert( Comparable && x );

    
    /**
     * Remove the smallest item from the priority queue.
     * Throws UnderflowException if empty.
     */
    void deleteMin( );


    /**
     * Remove the minimum item and place it in minItem.
     * Throws UnderflowException if empty.
     */
    void deleteMin( Comparable & minItem );


    /**
     * Make the priority queue logically empty.
     */
    void makeEmpty( );


    /**
     * Merge rhs into the priority queue.
     * rhs becomes empty. rhs must be different from this.
     * Exercise 6.35 needed to make this operation more efficient.
     */
    void merge( BinomialQueue & rhs );
 

		bool contains(const Comparable & key) const;
		bool contains(Comparable && key) const;

		const unsigned int& size() const;

		//Test function: returns parent pointer of node containing key.
		bool parentOf(const Comparable & key) const;

		bool remove(const Comparable & key);
		bool remove(Comparable && key);

		void resetWorkCtr();

		unsigned int getWorkCount();


};









#include "BinomialQueue.cpp"

#endif
