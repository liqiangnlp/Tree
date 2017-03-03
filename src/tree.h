/* NiuTrans - SMT platform
 * Copyright (C) 2011, NEU-NLPLab (http://www.nlplab.com/). All rights reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA
 */

/*
 * $Id:
 * tree.h
 *
 * $Version:
 * 1.1.0
 *
 * $Created by:
 * Qiang Li (email: liqiangneu@gmail.com)
 *
 * $Last Modified by:
 *
 */

#ifndef _TREE_H_
#define _TREE_H_

#include <cassert>
#include <memory>
#include <stdexcept>
#include <iterator>
#include <string>
#include <vector>
#include <set>
#include <queue>
#include <algorithm>
#include <cstddef>

//////////////////////////////////////////////////////////////////////////
/// TreeNode
//////////////////////////////////////////////////////////////////////////
template< class T >
class _TreeNode
{
public:
    _TreeNode(          );
    _TreeNode( const T& );

    _TreeNode<T> *parent     ;
    _TreeNode<T> *firstChild ;
    _TreeNode<T> *lastChild  ;
    _TreeNode<T> *prevSibling;
    _TreeNode<T> *nextSibling;
    T            data        ;
};

template< class T >
_TreeNode< T >::_TreeNode()
                    : parent(      0 ),
				      firstChild(  0 ),
				      lastChild(   0 ),
				      prevSibling( 0 ),
				      nextSibling( 0 ){}

template< class T >
_TreeNode< T >::_TreeNode( const T& value )
                    : parent(      0 ),
				      firstChild(  0 ),
				      lastChild(   0 ),
				      prevSibling( 0 ),
				      nextSibling( 0 ),
				      data(    value ){}


//////////////////////////////////////////////////////////////////////////
/// Tree
//////////////////////////////////////////////////////////////////////////
template< class T, class TreeNodeAllocator_ = std::allocator< _TreeNode<T> > >
class Tree
{
protected:
    typedef _TreeNode< T > TREE_NODE;

public:
	typedef T value_type;

	class iteratorBase     ;
	class preOrderIterator ;
	class postOrderIterator;
	class siblingIterator  ;
	class leafIterator     ;

	Tree(                                      );
	Tree( const T&                             );
	Tree( const iteratorBase&                  );
	Tree( const Tree< T, TreeNodeAllocator_ >& );
	
	~Tree();

    Tree< T, TreeNodeAllocator_ >& operator=( const Tree< T, TreeNodeAllocator_ >& );

    class iteratorBase
	{
	public:
		typedef T                               value_type       ;
		typedef T*                              pointer          ;
		typedef T&                              reference        ;
		typedef size_t                          size_type        ;
		typedef ptrdiff_t                       difference_type  ;
		typedef std::bidirectional_iterator_tag iterator_category;

		iteratorBase(             );
		iteratorBase( TREE_NODE * );

		reference operator*( ) const;
        pointer   operator->() const;

        void skipChildren(           );
		void skipChildren( bool skip );

		size_t numberOfChildren() const;

        siblingIterator begin() const;
		siblingIterator end(  ) const;

        TREE_NODE *node;

	protected:
		bool skipCurrentChildren;

	};

	// Depth-first iterator, first accessing the node, then its children
	class preOrderIterator : public iteratorBase
	{
	public:

		preOrderIterator(                        );
		preOrderIterator( TREE_NODE *            );
        preOrderIterator( const iteratorBase&    );
		preOrderIterator( const siblingIterator& );

		bool operator==( const preOrderIterator& ) const;
		bool operator!=( const preOrderIterator& ) const;

        preOrderIterator& operator++(        );
        preOrderIterator& operator--(        );
		preOrderIterator  operator++( int    );
		preOrderIterator  operator--( int    );
		preOrderIterator& operator+=( size_t );
		preOrderIterator& operator-=( size_t );
	};

	// Depth-first iterator, first accessing the children, then the node
	class postOrderIterator : public iteratorBase
	{
	public:
		postOrderIterator(                        );
		postOrderIterator( TREE_NODE *            );
		postOrderIterator( const iteratorBase&    );
		postOrderIterator( const siblingIterator& );

		bool operator==( const postOrderIterator& ) const;
		bool operator!=( const postOrderIterator& ) const;

		postOrderIterator& operator++(        );
		postOrderIterator& operator--(        );
		postOrderIterator  operator++( int    );
		postOrderIterator  operator--( int    );
		postOrderIterator& operator+=( size_t );
		postOrderIterator& operator-=( size_t );

		void descendAll();
	};

	// BreadthFirstQueuedIterator
	class breadthFirstQueuedIterator : public iteratorBase
	{
	public:
		breadthFirstQueuedIterator(                     );
		breadthFirstQueuedIterator( TREE_NODE *         );
		breadthFirstQueuedIterator( const iteratorBase& );

		bool operator==( const breadthFirstQueuedIterator& ) const;
		bool operator!=( const breadthFirstQueuedIterator& ) const;

		breadthFirstQueuedIterator& operator++(        );
		breadthFirstQueuedIterator  operator++( int    );
		breadthFirstQueuedIterator& operator+=( size_t );

	private:
		std::queue< TREE_NODE * > traversalQueue;
	};

//	typedef preOrderIterator           ITERATOR;
//	typedef breadthFirstQueuedIterator BREADTH_FIRST_ITERATOR;

	// FixedDepthIterator
	class fixedDepthIterator : public iteratorBase
	{
	public:
		fixedDepthIterator(                           );
		fixedDepthIterator( TREE_NODE *               );
		fixedDepthIterator( const iteratorBase &      );
		fixedDepthIterator( const siblingIterator&    );
		fixedDepthIterator( const fixedDepthIterator& );

		bool operator==( const fixedDepthIterator& ) const;
		bool operator!=( const fixedDepthIterator& ) const;

		fixedDepthIterator& operator++(        );
		fixedDepthIterator& operator--(        );
		fixedDepthIterator  operator++( int    );
		fixedDepthIterator  operator--( int    );
		fixedDepthIterator& operator+=( size_t );
		fixedDepthIterator& operator-=( size_t );

		TREE_NODE *topNode;
	};

	// SiblingIterator
	class siblingIterator : public iteratorBase
	{
	public:

		siblingIterator(                        );
		siblingIterator( TREE_NODE *            );
		siblingIterator( const siblingIterator& );
		siblingIterator( const iteratorBase&    );

		bool operator==( const siblingIterator& ) const;
		bool operator!=( const siblingIterator& ) const;

		siblingIterator& operator++(        );
		siblingIterator& operator--(        );
		siblingIterator  operator++( int    );
		siblingIterator  operator--( int    );
		siblingIterator& operator+=( size_t );
		siblingIterator& operator-=( size_t );

		TREE_NODE *rangeFirst() const;
		TREE_NODE *rangeLast( ) const;
		
		TREE_NODE *parent;
		
	private:
		void setParent();
	};

    // LeafIterator
	class leafIterator : public iteratorBase
	{
	public:
		leafIterator(                                 );
		leafIterator( TREE_NODE *, TREE_NODE *top = 0 );
		leafIterator( const siblingIterator&          );
		leafIterator( const iteratorBase&             );

        bool operator==( const leafIterator& ) const;
		bool operator!=( const leafIterator& ) const;

		leafIterator& operator++(        );
		leafIterator& operator--(        );
		leafIterator  operator++( int    );
		leafIterator  operator--( int    );
		leafIterator& operator+=( size_t );
		leafIterator& operator-=( size_t );

	private:
		TREE_NODE *topNode;
	};


	inline preOrderIterator begin() const;
	inline preOrderIterator end(  ) const;

	postOrderIterator beginPost() const;
	postOrderIterator endPost(  ) const;

	fixedDepthIterator beginFixed( const iteratorBase&, size_t ) const;
    fixedDepthIterator endFixed(   const iteratorBase&, size_t ) const;

	
    breadthFirstQueuedIterator beginBreadthFirst() const;
	breadthFirstQueuedIterator endBreadthFirst(  ) const;

    siblingIterator beginSibling( const iteratorBase& ) const;
	siblingIterator endSibling(   const iteratorBase& ) const;

	leafIterator beginLeaf() const;
	leafIterator endLeaf(  ) const;

    leafIterator beginLeaf( const iteratorBase& top ) const;
	leafIterator endLeaf(   const iteratorBase& top ) const;

    template< typename iter > static iter parent( iter );

	template< typename iter > iter previousSibling( iter ) const;
    template< typename iter > iter nextSibling(     iter ) const;
	template< typename iter > iter nextAtSameDepth( iter ) const;

	void clear();   // clear all nodes of the tree.

	template< typename iter > iter erase( iter );

	void eraseChildren( const iteratorBase& );

	template< typename iter > iter appendChild(  iter );
	template< typename iter > iter prependChild( iter );

	template< typename iter > iter appendChild(  iter, const T& );
	template< typename iter > iter prependChild( iter, const T& );

	template< typename iter > iter appendChild(  iter, iter );
	template< typename iter > iter prependChild( iter, iter );

	template< typename iter > iter appendChildren(  iter, siblingIterator, siblingIterator );
	template< typename iter > iter prependChildren( iter, siblingIterator, siblingIterator );

    preOrderIterator setHead( const T& );

	template< typename iter > iter insert( iter           , const T& );
	siblingIterator                insert( siblingIterator, const T& );

	template< typename iter > iter insertSubtree( iter, const iteratorBase& );

	template< typename iter > iter insertAfter( iter, const T& );

	template< typename iter > iter insertSubtreeAfter( iter, const iteratorBase& );

    template< typename iter > iter replace( iter, const T&            );

	template< typename iter > iter replace( iter, const iteratorBase& );

	siblingIterator                replace( siblingIterator,
		                                    siblingIterator,
							                siblingIterator,
							                siblingIterator );

	template< typename iter > iter flatten( iter );

	template< typename iter > iter reparent( iter, siblingIterator, siblingIterator );
    template< typename iter > iter reparent( iter, iter );

    template< typename iter > iter wrap( iter, const T& );

	template< typename iter > iter moveAfter(  iter, iter );
	template< typename iter > iter moveBefore( iter, iter );
    siblingIterator                moveBefore( siblingIterator,
		                                       siblingIterator );
	template< typename iter > iter moveOntop(  iter, iter );

	void merge( siblingIterator                      , 
		        siblingIterator                      , 
				siblingIterator                      , 
				siblingIterator                      , 
				bool            duplicateLeaves=false  );

	void sort( siblingIterator              ,
		       siblingIterator              ,
		       bool               deep=false  );
	template< class StrictWeakOrdering > 
	void sort( siblingIterator              ,
	           siblingIterator              ,
	           StrictWeakOrdering           ,
			   bool               deep=false  );

	template< typename iter >
	bool equal( const iter&,
		        const iter&,
				const iter&  ) const;
	template< typename iter, class BinaryPredicate >
	bool equal( const iter&          ,
		        const iter&          ,
				const iter&          ,
				      BinaryPredicate  ) const;
	
	template< typename iter >
    bool equalSubTree( const iter&,
		               const iter& ) const;
    template< typename iter, class BinaryPredicate >
	bool equalSubTree( const iter&          ,
		               const iter&          ,
          				     BinaryPredicate  ) const;

	Tree subTree( siblingIterator,
		          siblingIterator  ) const;
    void subTree( Tree&,
		          siblingIterator,
				  siblingIterator  ) const;

    void swap( siblingIterator );
	void swap( preOrderIterator, preOrderIterator );

    size_t size(                     ) const;
	size_t size( const iteratorBase& ) const;

	bool empty() const;

	static int depth( const iteratorBase&  );
	static int depth( const iteratorBase&,
		              const iteratorBase&  );

	int maxDepth(                     ) const;
	int maxDepth( const iteratorBase& ) const;

	static size_t numberOfChildren( const iteratorBase& );

	size_t numberOfSiblings( const iteratorBase& ) const;

    bool isInSubTree( const iteratorBase&,
		              const iteratorBase&,
					  const iteratorBase&  ) const;

    bool isValid( const iteratorBase& ) const;

	preOrderIterator lowestCommonAncestor( const iteratorBase&,
		                                   const iteratorBase&  ) const;

    size_t index( siblingIterator it ) const;

	static siblingIterator child( const iteratorBase&, size_t );

    siblingIterator sibling( const iteratorBase&, size_t );

	void debug_verify_consistency() const;

	class iteratorBaseLess
	{
	public:
		bool operator()( const typename Tree< T, TreeNodeAllocator_ >::iteratorBase& one,
			             const typename Tree< T, TreeNodeAllocator_ >::iteratorBase& two ) const
		{
			return one.node < two.node;
		}
	};


	TREE_NODE *head, *feet;

private:
    TreeNodeAllocator_ alloc_;
	void headInitialise();

	void copy( const Tree< T, TreeNodeAllocator_ >& other );

	template< class StrictWeakOrdering >
	class compareNodes
	{
	public:
		compareNodes( StrictWeakOrdering inComp ) : comp( inComp ){};

		bool operator()( const TREE_NODE *a, const TREE_NODE *b )
		{
		    return comp( a->data, b->data );
		}

	private:
		StrictWeakOrdering comp;
	};

};

//////////////////////////////////////////////////////////////////////////
/// Tree 
//////////////////////////////////////////////////////////////////////////
template< class T, class TreeNodeAllocator_ >
Tree< T, TreeNodeAllocator_ >::Tree()
{
    headInitialise();
}

template< class T, class TreeNodeAllocator_ >
Tree< T, TreeNodeAllocator_ >::Tree( const T& x )
{
    headInitialise();
	setHead( x );
}

template< class T, class TreeNodeAllocator_ >
Tree< T, TreeNodeAllocator_ >::Tree( const iteratorBase& other )
{
    headInitialise();
	setHead( ( *other ) );
	replace( begin(), other );
}

template< class T, class TreeNodeAllocator_ >
Tree< T, TreeNodeAllocator_ >::Tree( const Tree< T,TreeNodeAllocator_ >& other )
{
   clear();
   preOrderIterator it = other.begin();
   preOrderIterator to = begin();

   while( it != other.end() )
   {
       to = insert( to, ( *it ) );
	   it.skipChildren();
	   ++it;
   }
   to = begin();
   it = other.begin();

   while( it != other.end() )
   {
       to = replace( to, it );
	   to.skipChildren();
	   it.skipChildren();
	   ++to;
	   ++it;
   }
}

template< class T, class TreeNodeAllocator_ >
Tree< T, TreeNodeAllocator_ >::~Tree()
{
     clear();
	 alloc_.destroy( head );
	 alloc_.destroy( feet );
	 alloc_.deallocate( head, 1 );
	 alloc_.deallocate( feet, 1 );
}

template< class T, class TreeNodeAllocator_ >
void Tree< T, TreeNodeAllocator_ >::headInitialise()
{
    head = alloc_.allocate( 1, 0 );
	feet = alloc_.allocate( 1, 0 );
	alloc_.construct( head, _TreeNode< T >() );
	alloc_.construct( feet, _TreeNode< T >() );

    head->parent      = 0   ;
	head->firstChild  = 0   ;
	head->lastChild   = 0   ;
	head->prevSibling = 0   ;
	head->nextSibling = feet;

	feet->parent      = 0   ;
	feet->firstChild  = 0   ;
	feet->lastChild   = 0   ;
	feet->prevSibling = head;
	feet->nextSibling = 0   ;
}

template< class T, class TreeNodeAllocator_ >
void Tree< T, TreeNodeAllocator_ >::copy( const Tree< T, TreeNodeAllocator_ >& other )
{
	clear();
	preOrderIterator it = other.begin();
	preOrderIterator to = begin();

    while( it != other.end() )
	{
		to = insert( to, ( *it ) );
		it.skipChildren();
		++it;
	}
	to = begin();
	it = other.begin();

	while( it != other.end() )
	{
        to.replace( to, it );
		to.skipChildren();
		it.skipChildren();
		++to;
		++it;
	}
}

//////////////////////////////////////////////////////////////////////////
// IteratorBase
//////////////////////////////////////////////////////////////////////////
template< class T, class TreeNodeAllocator_ >
Tree< T, TreeNodeAllocator_ >::iteratorBase::iteratorBase()
: node( 0 ), skipCurrentChildren( false )
{
}

template< class T, class TreeNodeAllocator_ >
Tree< T, TreeNodeAllocator_ >::iteratorBase::iteratorBase( TREE_NODE *tn )
: node( tn ), skipCurrentChildren( false )
{
}

template< class T, class TreeNodeAllocator_ >
T& Tree< T, TreeNodeAllocator_ >::iteratorBase::operator*() const
{
    return node->data;
}

template< class T, class TreeNodeAllocator_ >
T* Tree< T, TreeNodeAllocator_ >::iteratorBase::operator->() const
{
    return &( node->data );
}

template< class T, class TreeNodeAllocator_ >
void Tree< T, TreeNodeAllocator_ >::iteratorBase::skipChildren()
{
    skipCurrentChildren = true;
}

template< class T, class TreeNodeAllocator_ >
void Tree< T, TreeNodeAllocator_ >::iteratorBase::skipChildren( bool skip )
{
    skipCurrentChildren = skip;
}

template< class T, class TreeNodeAllocator_ >
size_t Tree< T, TreeNodeAllocator_ >::iteratorBase::numberOfChildren() const
{
    TREE_NODE *pos = node->firstChild;
	if( pos == 0 )
		return 0;

	size_t ret = 1;
	while( pos !=node->lastChild )
	{
	    ++ret;
		pos = pos->nextSibling;
	}
	return ret;
}

template< class T, class TreeNodeAllocator_ >
typename Tree< T, TreeNodeAllocator_ >::siblingIterator Tree< T, TreeNodeAllocator_ >::iteratorBase::begin() const
{
    if( node->firstChild == 0 )
		return end();

	siblingIterator ret( node->firstChild );
	ret.parent = this->node;
	return ret;
}

template< class T, class TreeNodeAllocator_ >
typename Tree< T, TreeNodeAllocator_ >::siblingIterator Tree< T, TreeNodeAllocator_ >::iteratorBase::end() const
{
    siblingIterator ret( 0 );
	ret.parent = node;
	return ret;
}

// PreOrderIterator
template< class T, class TreeNodeAllocator_ >
Tree< T, TreeNodeAllocator_ >::preOrderIterator::preOrderIterator()
: iteratorBase( 0 )
{
}

template< class T, class TreeNodeAllocator_ >
Tree< T, TreeNodeAllocator_ >::preOrderIterator::preOrderIterator( TREE_NODE *tn )
: iteratorBase( tn )
{
}

template< class T, class TreeNodeAllocator_ >
Tree< T, TreeNodeAllocator_ >::preOrderIterator::preOrderIterator( const iteratorBase& other )
: iteratorBase( other.node )
{
}

template< class T, class TreeNodeAllocator_ >
Tree< T, TreeNodeAllocator_ >::preOrderIterator::preOrderIterator( const siblingIterator& other )
: iteratorBase( other.node )
{
    if( this->node == 0 )
	{
        if( other.rangeLast() != 0 )
		{
			this->node = other.rangeLast();
		}
		else
		{
			this->node = other.parent;
		}
		this->skipChildren();
		++( *this );
	}
}

template< class T, class TreeNodeAllocator_ >
bool Tree< T, TreeNodeAllocator_ >::preOrderIterator::operator==( const preOrderIterator& other ) const
{
	if( other.node == this->node )
		return true;
	else
		return false;
}

template< class T, class TreeNodeAllocator_ >
bool Tree< T, TreeNodeAllocator_ >::preOrderIterator::operator!=( const preOrderIterator& other ) const
{
    if( other.node != this->node )
		return true;
	else
		return false;
}

template< class T, class TreeNodeAllocator_ >
typename Tree< T, TreeNodeAllocator_ >::preOrderIterator& Tree< T, TreeNodeAllocator_ >::preOrderIterator::operator++()
{
    assert( this->node != 0 );
	if( !this->skipCurrentChildren && this->node->firstChild != 0 )
	{
        this->node = this->node->firstChild;
	}
	else
	{
		this->skipCurrentChildren = false;
		while( this->node->nextSibling == 0 )
		{
			this->node = this->node->parent;
			if( this->node == 0 )
			{
				return *this;
			}
		}
		this->node = this->node->nextSibling;
	}
	return *this;
}

template < class T, class TreeNodeAllocator_ >
typename Tree< T, TreeNodeAllocator_ >::preOrderIterator& Tree< T, TreeNodeAllocator_ >::preOrderIterator::operator--()
{
	assert( this->node != 0 );
	if( this->node->prevSibling )
	{
		this->node = this->node->prevSibling;
		while( this->node->lastChild )
		{
			this->node = this->node->lastChild;
		}
	}
	else
	{
		this->node = this->node->parent;
		if( this->node == 0 )
		{
			return *this;
		}
	}
	return *this;
}

template < class T, class TreeNodeAllocator_ >
typename Tree< T, TreeNodeAllocator_ >::preOrderIterator Tree< T, TreeNodeAllocator_ >::preOrderIterator::operator++( int )
{
   preOrderIterator copy = *this;
   ++( *this );
   return copy;
}

template< class T, class TreeNodeAllocator_ >
typename Tree< T, TreeNodeAllocator_ >::preOrderIterator Tree< T, TreeNodeAllocator_ >::preOrderIterator::operator--( int )
{
    preOrderIterator copy = *this;
	--( *this );
	return copy;
}

template< class T, class TreeNodeAllocator_ >
typename Tree< T, TreeNodeAllocator_ >::preOrderIterator& Tree< T, TreeNodeAllocator_ >::preOrderIterator::operator+=( size_t num )
{
    while( num > 0 )
	{
		++( *this );
		--num;
	}
	return ( *this );
}

template< class T, class TreeNodeAllocator_ >
typename Tree< T, TreeNodeAllocator_ >::preOrderIterator& Tree< T, TreeNodeAllocator_ >::preOrderIterator::operator-=( size_t num )
{
    while( num > 0 )
	{
		--( *this );
		--num;
	}
	return ( *this );
}

// PostOrderIterator
template< class T, class TreeNodeAllocator_ >
Tree< T, TreeNodeAllocator_ >::postOrderIterator::postOrderIterator()
: iteratorBase( 0 )
{
}

template< class T, class TreeNodeAllocator_ >
Tree< T, TreeNodeAllocator_ >::postOrderIterator::postOrderIterator( TREE_NODE *tn )
: iteratorBase( tn )
{
}

template< class T, class TreeNodeAllocator_ >
Tree< T, TreeNodeAllocator_ >::postOrderIterator::postOrderIterator( const iteratorBase& other )
: iteratorBase( other.node )
{
}

template< class T, class TreeNodeAllocator_ >
Tree< T, TreeNodeAllocator_ >::postOrderIterator::postOrderIterator( const siblingIterator& other)
: iteratorBase( other.node )
{
	if( this->node == 0 )
	{
		if( other.rangeLast() != 0 )
		{
			this->node = other.rangeLast();
		}
		else
		{
			this->node = other.parent;
		}
		this->skip_children();
		++( *this );
	}
}

template< class T, class TreeNodeAllocator_ >
bool Tree< T, TreeNodeAllocator_ >::postOrderIterator::operator==( const postOrderIterator& other ) const
{
	if( other.node == this->node )
		return true;
	else
		return false;
}

template< class T, class TreeNodeAllocator_ >
bool Tree< T, TreeNodeAllocator_ >::postOrderIterator::operator!=( const postOrderIterator& other ) const
{
	if( other.node != this->node )
		return true;
	else
		return false;
}

template< class T, class TreeNodeAllocator_ >
typename Tree< T, TreeNodeAllocator_ >::postOrderIterator& Tree< T, TreeNodeAllocator_ >::postOrderIterator::operator++()
{
    assert( this->node != 0 );
	if( this->node->nextSibling == 0 )
	{
		this->node = this->node->parent;
		this->skipCurrentChildren = false;
	}
	else
	{
		this->node = this->node->nextSibling;
		if( this->skipCurrentChildren )
		{
			this->skipCurrentChildren = false;
		}
		else
		{
			while( this->node->firstChild )
			{
				this->node = this->node->firstChild;
			}
		}	
	}
	return *this;
}

template< class T, class TreeNodeAllocator_ >
typename Tree< T, TreeNodeAllocator_ >::postOrderIterator& Tree< T, TreeNodeAllocator_ >::postOrderIterator::operator--()
{
	assert( this->node != 0 );
	if( this->skipCurrentChildren || this->node->lastChild == 0 )
	{
		this->skipCurrentChildren = false;
		while( this->node->prevSibling == 0 )
		{
			this->node = this->node->parent;
		}
		this->node = this->node->prevSibling;
	}
	else
	{
		this->node = this->node->lastChild;
	}
	return *this;
}

template< class T, class TreeNodeAllocator_ >
typename Tree< T, TreeNodeAllocator_ >::postOrderIterator Tree< T, TreeNodeAllocator_ >::postOrderIterator::operator++( int )
{
    postOrderIterator copy = *this;
	++( *this );
	return copy;
}

template< class T, class TreeNodeAllocator_ >
typename Tree< T, TreeNodeAllocator_ >::postOrderIterator Tree< T, TreeNodeAllocator_ >::postOrderIterator::operator--( int )
{
    postOrderIterator copy = *this;
	--( *this );
	return copy;
}

template< class T, class TreeNodeAllocator_ >
typename Tree< T, TreeNodeAllocator_ >::postOrderIterator& Tree< T, TreeNodeAllocator_ >::postOrderIterator::operator+=( size_t num )
{
    while( num > 0 )
	{
		++( *this );
		--num;
	}
	return ( *this );
}

template< class T, class TreeNodeAllocator_ >
typename Tree< T, TreeNodeAllocator_ >::postOrderIterator& Tree< T, TreeNodeAllocator_ >::postOrderIterator::operator-=( size_t num )
{
    while( num > 0 )
	{
		--( *this );
		--num;
	}
	return ( *this );
}

template< class T, class TreeNodeAllocator_ >
void Tree< T, TreeNodeAllocator_ >::postOrderIterator::descendAll()
{
	assert( this->node != 0 );
	while( this->node->firstChild )
	{
		this->node = this->node->firstChild;
	}
}

// BreadthFirstQueuedIterator
template< class T, class TreeNodeAllocator_ >
Tree< T, TreeNodeAllocator_ >::breadthFirstQueuedIterator::breadthFirstQueuedIterator()
: iteratorBase()
{
}

template< class T, class TreeNodeAllocator_ >
Tree< T, TreeNodeAllocator_ >::breadthFirstQueuedIterator::breadthFirstQueuedIterator( TREE_NODE *tn )
: iteratorBase( tn )
{
    traversalQueue.push( tn );
}

template< class T, class TreeNodeAllocator_ >
Tree< T, TreeNodeAllocator_ >::breadthFirstQueuedIterator::breadthFirstQueuedIterator( const iteratorBase& other )
: iteratorBase( other.node )
{
	traversalQueue.push( other.node );
}

template< class T, class TreeNodeAllocator_ >
bool Tree< T, TreeNodeAllocator_ >::breadthFirstQueuedIterator::operator==( const breadthFirstQueuedIterator& other ) const
{
	if( other.node == this->node )
		return true;
	else
		return false;
}

template< class T, class TreeNodeAllocator_ >
bool Tree< T, TreeNodeAllocator_ >::breadthFirstQueuedIterator::operator!=( const breadthFirstQueuedIterator& other ) const
{
	if( other.node != this->node )
		return true;
	else
		return false;
}

template< class T, class TreeNodeAllocator_ >
typename Tree< T, TreeNodeAllocator_ >::breadthFirstQueuedIterator& Tree< T, TreeNodeAllocator_ >::breadthFirstQueuedIterator::operator++()
{
	assert( this->node != 0 );

    siblingIterator sib = this->begin();
	while( sib != this->end() )
	{
		traversalQueue.push( sib.node );
		++sib;
	}
	traversalQueue.pop();
	if( traversalQueue.size() > 0 )
		this->node = traversalQueue.front();
	else
		this->node = 0;
	return ( *this );
}

template< class T, class TreeNodeAllocator_ >
typename Tree< T, TreeNodeAllocator_ >::breadthFirstQueuedIterator Tree< T, TreeNodeAllocator_ >::breadthFirstQueuedIterator::operator++( int )
{
    breadthFirstQueuedIterator copy = *this;
	++( *this );
	return copy;
}

template< class T, class TreeNodeAllocator_ >
typename Tree< T, TreeNodeAllocator_ >::breadthFirstQueuedIterator& Tree< T, TreeNodeAllocator_ >::breadthFirstQueuedIterator::operator+=( size_t num )
{
   while( num > 0 )
   {
       ++( *this );
	   --num;
   }
   return ( *this );
}

// FixedDepthIterator
template< class T, class TreeNodeAllocator_ >
Tree< T, TreeNodeAllocator_ >::fixedDepthIterator::fixedDepthIterator()
: iteratorBase()
{
}

template< class T, class TreeNodeAllocator_ >
Tree< T, TreeNodeAllocator_ >::fixedDepthIterator::fixedDepthIterator( TREE_NODE *tn )
: iteratorBase( tn ), topNode( 0 )
{
}

template< class T, class TreeNodeAllocator_ >
Tree< T, TreeNodeAllocator_ >::fixedDepthIterator::fixedDepthIterator( const iteratorBase& other )
: iteratorBase( other.node ), topNode( 0 )
{
}

template< class T, class TreeNodeAllocator_ >
Tree< T, TreeNodeAllocator_ >::fixedDepthIterator::fixedDepthIterator( const siblingIterator& other )
: iteratorBase( other.node ), topNode( 0 )
{
}

template< class T, class TreeNodeAllocator_ >
Tree< T, TreeNodeAllocator_ >::fixedDepthIterator::fixedDepthIterator( const fixedDepthIterator& other )
: iteratorBase( other.node ), topNode( other.topNode )
{
}

template< class T, class TreeNodeAllocator_ >
bool Tree< T, TreeNodeAllocator_ >::fixedDepthIterator::operator==( const fixedDepthIterator& other ) const
{
	if( other.node == this->node && other.topNode == topNode )
		return true;
	else
		return false;
}

template< class T, class TreeNodeAllocator_ >
bool Tree< T, TreeNodeAllocator_ >::fixedDepthIterator::operator!=( const fixedDepthIterator& other ) const
{
	if( other.node != this->node || other.topNode != topNode )
		return true;
	else
		return false;
}

template< class T, class TreeNodeAllocator_ >
typename Tree< T, TreeNodeAllocator_ >::fixedDepthIterator& Tree< T, TreeNodeAllocator_ >::fixedDepthIterator::operator++()
{
	assert( this->node != 0 );

	if( this->node->nextSibling )
	{
        this->node = this->node->nextSibling;
	}
	else
	{
		int relativeDepth = 0;
upper:
		do
		{
			if( this->node == this->topNode )
			{
				this->node = 0;
				return *this;
			}
            this->node = this->node->parent;
			if( this->node == 0 )
				return *this;
			--relativeDepth;
		}while( this->node->nextSibling == 0 );
lower:
		this->node = this->node->nextSibling;
		while( this->node->firstChild == 0 )
		{
			if( this->node->nextSibling == 0 )
				goto upper;
			this->node = this->node->nextSibling;
			if( this->node == 0 )
				return *this;
		}
		while( relativeDepth < 0 && this->node->firstChild != 0 )
		{
			this->node = this->node->firstChild;
			++relativeDepth;
		}
		if( relativeDepth < 0 )
		{
            if( this->node->nextSibling == 0 )
				goto upper;
			else
				goto lower;
		}
	}
	return *this;
}

template< class T, class TreeNodeAllocator_ >
typename Tree< T, TreeNodeAllocator_ >::fixedDepthIterator& Tree< T, TreeNodeAllocator_ >::fixedDepthIterator::operator--()
{
	assert( this->node != 0 );

	if( this->node->prevSibling )
	{
		this->node = this->node->prevSibling;
	}
	else
	{
		int relativeDepth = 0;
upper:
		do 
		{
            if( this->node == this->topNode )
			{
				this->node = 0;
				return *this;
			}
			this->node = this->node->parent;
			if( this->node == 0 )
				return *this;
			--relativeDepth;
		} while( this->node->prevSibling == 0 );
lower:
		this->node = this->node->prevSibling;
		while( this->node->lastChild == 0 )
		{
			if( this->node->prevSibling == 0 )
				goto upper;
			this->node = this->node->prevSibling;
			if( this->node == 0 )
				return *this;
		}
		while( relativeDepth < 0 && this->node->lastChild != 0 )
		{
            this->node = this->node->lastChild;
			++relativeDepth;
		}
		if( relativeDepth < 0 )
		{
			if( this->node->prevSibling == 0 )
				goto upper;
			else
				goto lower;
		}
	}
	return *this;
}

template< class T, class TreeNodeAllocator_ >
typename Tree< T, TreeNodeAllocator_ >::fixedDepthIterator Tree< T, TreeNodeAllocator_ >::fixedDepthIterator::operator++( int )
{
	fixedDepthIterator copy = *this;
	++( *this );
	return copy;
}

template< class T, class TreeNodeAllocator_ >
typename Tree< T, TreeNodeAllocator_ >::fixedDepthIterator Tree< T, TreeNodeAllocator_ >::fixedDepthIterator::operator--( int )
{
	fixedDepthIterator copy = *this;
	--( *this );
	return copy;
}

template< class T, class TreeNodeAllocator_ >
typename Tree< T, TreeNodeAllocator_ >::fixedDepthIterator& Tree< T, TreeNodeAllocator_ >::fixedDepthIterator::operator+=( size_t num )
{
	while( num > 0 )
	{
		++( *this );
		--num;
	}
	return ( *this );
}

template< class T, class TreeNodeAllocator_ >
typename Tree< T, TreeNodeAllocator_ >::fixedDepthIterator& Tree< T, TreeNodeAllocator_ >::fixedDepthIterator::operator-=( size_t num )
{
	while( num > 0 )
	{
		--( *this );
		--num;
	}
	return ( *this );
}

// SiblingIterator
template< class T, class TreeNodeAllocator_ >
Tree< T, TreeNodeAllocator_ >::siblingIterator::siblingIterator()
: iteratorBase()
{
	setParent();
}

template< class T, class TreeNodeAllocator_ >
Tree< T, TreeNodeAllocator_ >::siblingIterator::siblingIterator( TREE_NODE *tn )
: iteratorBase( tn )
{
    setParent();
}

template< class T, class TreeNodeAllocator_ >
Tree< T, TreeNodeAllocator_ >::siblingIterator::siblingIterator( const iteratorBase& other )
: iteratorBase( other.node )
{
	setParent();
}

template< class T, class TreeNodeAllocator_ >
Tree< T, TreeNodeAllocator_ >::siblingIterator::siblingIterator( const siblingIterator& other )
: iteratorBase( other ), parent( other.parent )
{
}

template< class T, class TreeNodeAllocator_ >
bool Tree< T, TreeNodeAllocator_ >::siblingIterator::operator==( const siblingIterator& other ) const
{
	if( other.node == this->node )
	{
		return true;
	}
	else
	{
		return false;
	}
}

template< class T, class TreeNodeAllocator_ >
bool Tree< T, TreeNodeAllocator_ >::siblingIterator::operator!=( const siblingIterator& other ) const
{
    if( other.node != this->node )
	{
		return true;
	}
	else
	{
        return false;
	}
}

template< class T, class TreeNodeAllocator_ >
typename Tree< T, TreeNodeAllocator_ >::siblingIterator& Tree< T, TreeNodeAllocator_ >::siblingIterator::operator++()
{
    if( this->node )
		this->node = this->node->nextSibling;
	return *this;
}

template< class T, class TreeNodeAllocator_ >
typename Tree< T, TreeNodeAllocator_ >::siblingIterator& Tree< T, TreeNodeAllocator_ >::siblingIterator::operator--()
{
    if( this->node )
		this->node = this->node->prevSibling;
	else
	{
		assert( parent );
		this->node = parent->lastChild;
	}

	return *this;
}

template< class T, class TreeNodeAllocator_ >
typename Tree< T, TreeNodeAllocator_ >::siblingIterator Tree< T, TreeNodeAllocator_ >::siblingIterator::operator++( int )
{
    siblingIterator copy = *this;
	++( *this );
	return copy;
}

template< class T, class TreeNodeAllocator_ >
typename Tree< T, TreeNodeAllocator_ >::siblingIterator Tree< T, TreeNodeAllocator_ >::siblingIterator::operator--( int )
{
    siblingIterator copy = *this;
	--( *this );
	return copy;
}

template< class T, class TreeNodeAllocator_ >
typename Tree< T, TreeNodeAllocator_ >::siblingIterator& Tree< T, TreeNodeAllocator_ >::siblingIterator::operator+=( size_t num )
{
	while( num > 0 )
	{
		++( *this );
		--num;
	}
	return ( *this );
}

template< class T, class TreeNodeAllocator_ >
typename Tree< T, TreeNodeAllocator_ >::siblingIterator& Tree< T, TreeNodeAllocator_ >::siblingIterator::operator-=( size_t num )
{
    while( num > 0 )
	{
		--( *this );
		--num;
	}
	return ( *this );
}

template< class T, class TreeNodeAllocator_ >
typename Tree< T, TreeNodeAllocator_ >::TREE_NODE *Tree< T, TreeNodeAllocator_ >::siblingIterator::rangeFirst() const
{
    TREE_NODE *tmp = parent->firstChild;
	return tmp;
}

template< class T, class TreeNodeAllocator_ >
typename Tree< T, TreeNodeAllocator_ >::TREE_NODE *Tree< T, TreeNodeAllocator_ >::siblingIterator::rangeLast() const
{
    return parent->lastChild;
}

template< class T, class TreeNodeAllocator_ >
void Tree< T, TreeNodeAllocator_ >::siblingIterator::setParent()
{
	parent = 0;
	if( this->node == 0 )
		return;
	if( this->node->parent != 0 )
		parent = this->node->parent;
}

// LeafIterator
template< class T, class TreeNodeAllocator_ >
Tree< T, TreeNodeAllocator_ >::leafIterator::leafIterator()
: iteratorBase( 0 ), topNode( 0 )
{
}

template< class T, class TreeNodeAllocator_ >
Tree< T, TreeNodeAllocator_ >::leafIterator::leafIterator( TREE_NODE *tn, TREE_NODE *top )
: iteratorBase( tn ), topNode( top )
{
}

template< class T, class TreeNodeAllocator_ >
Tree< T, TreeNodeAllocator_ >::leafIterator::leafIterator( const iteratorBase& other )
: iteratorBase( other.node ), topNode( 0 )
{
}

template< class T, class TreeNodeAllocator_ >
Tree< T, TreeNodeAllocator_ >::leafIterator::leafIterator( const siblingIterator& other )
: iteratorBase( other.node ), topNode( 0 )
{
    if( this->node == 0 )
	{
	    if( other.rangeLast() != 0 )
			this->node = other.rangeLast();
		else
			this->node = other.parent;
		++( *this );
	}
}

template< class T, class TreeNodeAllocator_ >
bool Tree< T, TreeNodeAllocator_ >::leafIterator::operator==( const leafIterator& other ) const
{
	if( other.node == this->node && other.topNode == this->topNode )
	{
		return true;
	}
	else
	{
		return false;
	}
}

template< class T, class TreeNodeAllocator_ >
bool Tree< T, TreeNodeAllocator_ >::leafIterator::operator!=( const leafIterator& other ) const
{
    if( other.node != this->node )
	{
		return true;
	}
	else
	{
		return false;
	}
}

template< class T, class TreeNodeAllocator_ >
typename Tree< T, TreeNodeAllocator_ >::leafIterator& Tree< T, TreeNodeAllocator_ >::leafIterator::operator++()
{
	assert( this->node != 0 );
	if( this->node->firstChild != 0 )
	{
		while( this->node->firstChild )
		{
			this->node = this->node->first_child;
		}
	}
	else
	{
	    while( this->node->nextSibling == 0 )
		{
			if( this->node->parent == 0 )
				return *this;
			this->node = this->node->parent;
			if( topNode != 0 && this->node == topNode )
				return *this;
		}
		this->node = this->node->nextSibling;
		while( this->node->firstChild )
			this->node = this->node->firstChild;
	}
	return *this;
}

template< class T, class TreeNodeAllocator_ >
typename Tree< T, TreeNodeAllocator_ >::leafIterator& Tree< T, TreeNodeAllocator_ >::leafIterator::operator--()
{
    assert( this->node != 0 );
	while( this->node->prevSibling == 0 )
	{
		if( this->node->parent == 0 )
			return *this;
		this->node = this->node->parent;
		if( topNode != 0 && this->node == topNode )
			return *this;
	}
	this->node = this->node->prevSibling;

	while( this->node->lastChild )
	{
		this->node = this->node->lastChild;
	}

	return *this;
}

template< class T, class TreeNodeAllocator_ >
typename Tree< T, TreeNodeAllocator_ >::leafIterator Tree< T, TreeNodeAllocator_ >::leafIterator::operator++( int )
{
    leafIterator copy = *this;
	++( *this );
	return copy;
}

template< class T, class TreeNodeAllocator_ >
typename Tree< T, TreeNodeAllocator_ >::leafIterator Tree< T, TreeNodeAllocator_ >::leafIterator::operator--( int )
{
    leafIterator copy = *this;
	--( *this );
	return copy;
}

template< class T, class TreeNodeAllocator_ >
typename Tree< T, TreeNodeAllocator_ >::leafIterator& Tree< T, TreeNodeAllocator_ >::leafIterator::operator+=( size_t num )
{
    while( num > 0 )
	{
		++( *this );
		--num;
	}
	return ( *this );
}

template< class T, class TreeNodeAllocator_ >
typename Tree< T, TreeNodeAllocator_ >::leafIterator& Tree< T, TreeNodeAllocator_ >::leafIterator::operator-=( size_t num )
{
	while( num > 0 )
	{
		--( *this );
		--num;
	}
	return ( *this );
}

//////////////////////////////////////////////////////////////////////////
/// Methods for Tree Class
//////////////////////////////////////////////////////////////////////////
template< class T, class TreeNodeAllocator_ >
typename Tree< T, TreeNodeAllocator_ >::preOrderIterator Tree< T, TreeNodeAllocator_ >::begin() const
{
	return preOrderIterator( head->nextSibling );
}

template< class T, class TreeNodeAllocator_ >
typename Tree< T, TreeNodeAllocator_ >::preOrderIterator Tree< T, TreeNodeAllocator_ >::end() const
{
    return preOrderIterator( feet );
}

template< class T, class TreeNodeAllocator_ >
typename Tree< T, TreeNodeAllocator_ >::postOrderIterator Tree< T, TreeNodeAllocator_ >::beginPost() const
{
    TREE_NODE *tmp = head->nextSibling;
	if( tmp != feet )
	{
		while( tmp->firstChild )
		{
			tmp = tmp->firstChild;
		}
	}
	return postOrderIterator( tmp );
}

template< class T, class TreeNodeAllocator_ >
typename Tree< T, TreeNodeAllocator_ >::postOrderIterator Tree< T, TreeNodeAllocator_ >::endPost() const
{
    return postOrderIterator( feet );
}

template< class T, class TreeNodeAllocator_ >
typename Tree< T, TreeNodeAllocator_ >::fixedDepthIterator Tree< T, TreeNodeAllocator_ >::beginFixed( const iteratorBase& pos, size_t dp ) const
{
	typename Tree< T, TreeNodeAllocator_ >::fixedDepthIterator ret;
	ret.topNode = pos.node;

    TREE_NODE *tmp = pos.node;
	size_t curdepth = 0;

	while( curdepth < dp )
	{
        while( tmp->firstChild == 0 )
		{
			if( tmp->nextSibling == 0 )
			{
				do
				{
					if( tmp == ret.topNode )
					{
						throw std::range_error( "tree: beginFixed out of range" );
					}
					tmp = tmp->parent;
					if( tmp == 0 )
					{
						throw std::range_error( "tree: beginFixed out of range" );
					}
					--curdepth;
				}while( tmp->nextSibling == 0 );
			}
			tmp = tmp->nextSibling;
		}
		tmp = tmp->firstChild;
		++curdepth;
	}
	ret.node = tmp;
	return ret;
}

template< class T, class TreeNodeAllocator_ >
typename Tree< T, TreeNodeAllocator_ >::fixedDepthIterator Tree< T, TreeNodeAllocator_ >::endFixed( const iteratorBase& pos, size_t dp ) const
{
	assert( 1 == 0 ); //////////////////////////////////////////////////////////////////////////
	TREE_NODE *tmp = pos.node;
    size_t curdepth = 1;
	while( curdepth < dp )
	{
		while( tmp->firstChild == 0 )
		{
			tmp = tmp->nextSibling;
			if( tmp == 0 )
			{			
				throw std::range_error( "tree: endFixed out of range" );
			}
		}
		tmp = tmp->firstChild;
		++curdepth;
	}
	return tmp;
}

template< class T, class TreeNodeAllocator_ >
typename Tree< T, TreeNodeAllocator_ >::breadthFirstQueuedIterator Tree< T, TreeNodeAllocator_ >::beginBreadthFirst() const
{
    return breadthFirstQueuedIterator( head->nextSibling );
}

template< class T, class TreeNodeAllocator_ >
typename Tree< T, TreeNodeAllocator_ >::breadthFirstQueuedIterator Tree< T, TreeNodeAllocator_ >::endBreadthFirst() const
{
	return breadthFirstQueuedIterator();
}

template< class T, class TreeNodeAllocator_ >
typename Tree< T, TreeNodeAllocator_ >::siblingIterator Tree< T, TreeNodeAllocator_ >::beginSibling( const iteratorBase& pos ) const
{
	assert( pos.node != 0 );
	if( pos.node->firstChild == 0 )
	{
	    return endSibling( pos );
	}
	return pos.node->firstChild;
}

template< class T, class TreeNodeAllocator_ >
typename Tree< T, TreeNodeAllocator_ >::siblingIterator Tree< T, TreeNodeAllocator_ >::endSibling( const iteratorBase& pos ) const
{
    siblingIterator ret( 0 );
	ret.parent = pos.node;
	return ret;
}

template< class T, class TreeNodeAllocator_ >
typename Tree< T, TreeNodeAllocator_ >::leafIterator Tree< T, TreeNodeAllocator_ >::beginLeaf() const
{
	TREE_NODE *tmp = head->nextSibling;
	if( tmp != feet )
	{
	    while( tmp->firstChild )
		{
			tmp = tmp->firstChild;
		}
	}
	return leafIterator( tmp );
}

template< class T, class TreeNodeAllocator_ >
typename Tree< T, TreeNodeAllocator_ >::leafIterator Tree< T, TreeNodeAllocator_ >::endLeaf() const
{
	return leafIterator( feet );
}

template< class T, class TreeNodeAllocator_ >
typename Tree< T, TreeNodeAllocator_ >::leafIterator Tree< T, TreeNodeAllocator_ >::beginLeaf( const iteratorBase& top ) const
{
	TREE_NODE *tmp = top.node;
	while( tmp->firstChild )
	{
		tmp = tmp->firstChild;
	}
	return leafIterator( tmp, top.node );
}

template< class T, class TreeNodeAllocator_ >
typename Tree< T, TreeNodeAllocator_ >::leafIterator Tree< T, TreeNodeAllocator_ >::endLeaf( const iteratorBase& top ) const
{
	return leafIterator( top.node, top.node );
}

template< class T, class TreeNodeAllocator_ >
template< typename iter >
iter Tree< T, TreeNodeAllocator_ >::parent( iter position )
{
	assert( position.node != 0 );
	return iter( position.node->parent );
}

template< class T, class TreeNodeAllocator_ >
template< typename iter >
iter Tree< T, TreeNodeAllocator_ >::previousSibling( iter position ) const
{
	assert( position.node != 0 );
	iter ret( position );
	ret.node = position.node->prevSibling;
	return ret;
}

template< class T, class TreeNodeAllocator_ >
template< typename iter >
iter Tree< T, TreeNodeAllocator_ >::nextSibling( iter position ) const
{
	assert( position.node != 0 );
	iter ret( position );
	ret.node = position.node->nextSibling;
	return ret;
}

template< class T, class TreeNodeAllocator_ >
template< typename iter >
iter Tree< T, TreeNodeAllocator_ >::nextAtSameDepth( iter position ) const
{
    //////////////////////////////////////////////////////////////////////////
	typename Tree< T, TreeNodeAllocator_ >::fixedDepthIterator tmp( position.node );
	
	++tmp;
	return iter( tmp );
}

template< class T, class TreeNodeAllocator_ >
void Tree< T, TreeNodeAllocator_ >::clear()
{
	if( head )
	{
		while( head->nextSibling != feet )
		{
			erase( preOrderIterator( head->nextSibling ) ); 
		}
	}
}

template< class T, class TreeNodeAllocator_ >
template< class iter >
iter Tree< T, TreeNodeAllocator_ >::erase( iter it )
{
	TREE_NODE *cur = it.node;
	assert( cur != head );
	iter ret = it;
	ret.skipChildren();
	++ret;
	eraseChildren( it );
	if( cur->prevSibling == 0 )
	{
		cur->parent->firstChild = cur->nextSibling;
	}
	else
	{
		cur->prevSibling->nextSibling = cur->nextSibling;
	}

	if( cur->nextSibling == 0 )
	{
		cur->parent->lastChild = cur->prevSibling;
	}
	else
	{
		cur->nextSibling->prevSibling = cur->prevSibling;
	}

	alloc_.destroy( cur );
	alloc_.deallocate( cur, 1 );
	return ret;
}


template< class T, class TreeNodeAllocator_ >
void Tree< T, TreeNodeAllocator_ >::eraseChildren( const iteratorBase& it )
{
	if( it.node == 0 )
		return;

	TREE_NODE *cur  = it.node->firstChild;
	TREE_NODE *prev = 0                  ;

	while( cur != 0 )
	{
		prev = cur;
		cur  = cur->nextSibling;
		eraseChildren( preOrderIterator( prev ) );
		alloc_.destroy( prev );
		alloc_.deallocate( prev, 1 );
	}
	it.node->firstChild = 0;
	it.node->lastChild  = 0;
}

template< class T, class TreeNodeAllocator_ >
template< typename iter >
iter Tree< T, TreeNodeAllocator_ >::appendChild( iter position )
{
    assert( position.node != head );
	assert( position.node != feet );
	assert( position.node );

	TREE_NODE *tmp = alloc_.allocate( 1, 0 );
	alloc_.construct( tmp, _TreeNode< T >() );

    tmp->firstChild = 0;
	tmp->lastChild  = 0;

	tmp->parent = position.node;

	if( position.node->lastChild != 0 )
	{
	    position.node->lastChild->nextSibling = tmp;
	}
	else
	{
	    position.node->firstChild = tmp;
	}

	tmp->prevSibling = position.node->lastChild;
	position.node->lastChild = tmp;
	tmp->nextSibling = 0; 
	return tmp;
}

template< class T, class TreeNodeAllocator_ >
template< typename iter >
iter Tree< T, TreeNodeAllocator_ >::prependChild( iter position )
{
	assert( position.node != head );
	assert( position.node != feet );
	assert( position.node );

	TREE_NODE *tmp = alloc_.allocate( 1, 0 );
	alloc_.construct( tmp, _TreeNode< T >() );
	
	tmp->firstChild = 0;
	tmp->lastChild  = 0;

	tmp->parent = position.node;
	if( position.node->firstChild != 0 )
	{
	    position.node->firstChild->prevSibling = tmp;
	}
	else
	{
		position.node->lastChild = tmp;
	}

    tmp->nextSibling = position.node->firstChild;
	position.node->firstChild = tmp;
	tmp->prevSibling = 0;
	return tmp;
}


template< class T, class TreeNodeAllocator_ >
template< class iter >
iter Tree< T, TreeNodeAllocator_ >::appendChild( iter position, const T& x )
{
	//////////////////////////////////////////////////////////////////////////
	// note 

	assert( position.node != head );
	assert( position.node != feet );
	assert( position.node );

	TREE_NODE* tmp = alloc_.allocate( 1, 0 );
	alloc_.construct( tmp, x );

	tmp->firstChild = 0;
	tmp->lastChild  = 0;

	tmp->parent = position.node;

	if( position.node->lastChild != 0 )
	{
		position.node->lastChild->nextSibling = tmp;
	}
	else
	{
		position.node->firstChild = tmp;
	}
	tmp->prevSibling = position.node->lastChild;
	position.node->lastChild = tmp;
	tmp->nextSibling = 0;
	return tmp;
}

template< class T, class TreeNodeAllocator_ >
template< class iter >
iter Tree< T, TreeNodeAllocator_ >::prependChild( iter position, const T& x )
{
	assert( position.node != head );
	assert( position.node != feet );
	assert( position.node );

    TREE_NODE* tmp = alloc_.allocate( 1, 0 );
	alloc_.construct( tmp, x );

	tmp->firstChild = 0;
	tmp->lastChild  = 0;
	
	tmp->parent = position.node;

	if( position.node->firstChild != 0 )
	{
	    position.node->firstChild->prevSibling = tmp;
	}
	else
	{
	    position.node->lastChild = tmp;
	}

	tmp->nextSibling = position.node->firstChild;
	position.node->firstChild = tmp;
	tmp->prevSibling = 0;
	return tmp;
}

template< class T, class TreeNodeAllocator_ >
template< class iter >
iter Tree< T, TreeNodeAllocator_ >::appendChild( iter position, iter other )
{
	assert( position.node != head );
	assert( position.node != feet );
	assert( position.node );

	siblingIterator aargh = appendChild( position, value_type() );
	return replace( aargh, other );
}

template< class T, class TreeNodeAllocator_ >
template< class iter >
iter Tree< T, TreeNodeAllocator_ >::prependChild( iter position, iter other )
{
    assert( position.node != head );
	assert( position.node != feet );
	assert( position.node );

	siblingIterator aargh = prependChild( position, value_type() );
	return replace( aargh, other );
}

template< class T, class TreeNodeAllocator_ >
template< class iter >
iter Tree< T, TreeNodeAllocator_ >::appendChildren( iter position, siblingIterator from, siblingIterator to )
{
	assert( position.node != head );
	assert( position.node != feet );
	assert( position.node );

	iter ret = from;

	while( from != to )
	{
		insertSubtree( position.end(), from );
		++from;
	}
	return ret;
}

template< class T, class TreeNodeAllocator_ >
template< class iter >
iter Tree< T, TreeNodeAllocator_ >::prependChildren( iter position, siblingIterator from, siblingIterator to )
{
	assert( position.node != head );
	assert( position.node != feet );
	assert( position.node );

	iter ret = from;

	while( from != to )
	{
	    insertSubtree( position.begin(), from );
		++from;
	}
	return ret;
}

template< class T, class TreeNodeAllocator_ >
typename Tree< T, TreeNodeAllocator_ >::preOrderIterator Tree< T, TreeNodeAllocator_ >::setHead( const T& x )
{
	assert( head->nextSibling == feet );
	return insert( preOrderIterator( feet ), x );
}

template< class T, class TreeNodeAllocator_ >
template< class iter >
iter Tree< T, TreeNodeAllocator_ >::insert( iter position, const T& x )
{
	if( position.node == 0 )
		position.node = feet;

	TREE_NODE* tmp = alloc_.allocate( 1, 0 );
	alloc_.construct( tmp, x );

	tmp->firstChild = 0;
	tmp->lastChild  = 0;

	tmp->parent      = position.node->parent     ;
	tmp->nextSibling = position.node             ;
	tmp->prevSibling = position.node->prevSibling;

	position.node->prevSibling = tmp;

	if( tmp->prevSibling == 0 )
	{
		if( tmp->parent )
		{
			tmp->parent->firstChild = tmp;
		}
	}
	else
		tmp->prevSibling->nextSibling = tmp;

	return tmp;
}

template< class T, class TreeNodeAllocator_ >
typename Tree< T, TreeNodeAllocator_ >::siblingIterator Tree< T, TreeNodeAllocator_ >::insert( siblingIterator position, const T& x )
{
	TREE_NODE *tmp = alloc_.allocate( 1, 0 );
	alloc_.construct( tmp, x );

	tmp->firstChild = 0;
	tmp->lastChild  = 0;

	tmp->nextSibling = position.node;
	if( position.node == 0 )
	{
		tmp->parent      = position.parent     ;
		tmp->prevSibling = position.rangeLast();

		tmp->parent->lastChild = tmp;
	}
	else
	{
		tmp->parent      = position.node->parent     ;
		tmp->prevSibling = position.node->prevSibling;

		position.node->prevSibling = tmp;
	}

	if( tmp->prevSibling == 0 )
	{
		if( tmp->parent )
			tmp->parent->firstChild = tmp;
	}
	else
		tmp->prevSibling->nextSibling = tmp;

	return tmp;
}

template< class T, class TreeNodeAllocator_ >
template< class iter >
iter Tree< T, TreeNodeAllocator_ >::insertSubtree( iter position, const iteratorBase &subtree )
{
	iter it = insert( position, value_type() );
	return replace( it, subtree );
}

template< class T, class TreeNodeAllocator_ >
template< class iter >
iter Tree< T, TreeNodeAllocator_ >::insertAfter( iter position, const T& x )
{
	TREE_NODE *tmp = alloc_.allocate( 1, 0 );
	alloc_.construct( tmp, x );
	tmp->firstChild = 0;
	tmp->lastChild  = 0;

	tmp->parent = position.node->parent;
	tmp->prevSibling = position.node;
	tmp->nextSibling = position.node->nextSibling;

    position.node->nextSibling = tmp;

	if( tmp->nextSibling == 0 )
	{
		if( tmp->parent )
		{
			tmp->parent->lastChild = tmp;
		}
	}
	else
	{
		tmp->nextSibling->prevSibling = tmp;
	}
	return tmp;
}

template< class T, class TreeNodeAllocator_ >
template< class iter >
iter Tree< T, TreeNodeAllocator_ >::insertSubtreeAfter( iter position, const iteratorBase& subtree )
{
	iter it = insertAfter( position, value_type() );
	return replace( it, subtree );
}

template< class T, class TreeNodeAllocator_ >
template< class iter >
iter Tree< T, TreeNodeAllocator_ >::replace( iter position, const T& x )
{
	position.node->data = x;
	return position;
}

template< class T, class TreeNodeAllocator_ >
template< class iter >
iter Tree< T, TreeNodeAllocator_ >::replace( iter position, const iteratorBase& from )
{
	assert( position.node != head );

	TREE_NODE *currentFrom = from.node   ;
	TREE_NODE *startFrom   = from.node   ;
	TREE_NODE *currentTo   = position.node;

	eraseChildren( position );  

	TREE_NODE* tmp = alloc_.allocate( 1, 0 );
	alloc_.construct( tmp, ( *from ) );

	tmp->firstChild = 0;
	tmp->lastChild  = 0;

	if( currentTo->prevSibling == 0 )
	{
		if( currentTo->parent != 0 )
			currentTo->parent->firstChild = tmp;
	}
	else
	{
		currentTo->prevSibling->nextSibling = tmp;
	}

	tmp->prevSibling = currentTo->prevSibling;
	if( currentTo->nextSibling == 0 )
	{
		if( currentTo->parent != 0 )
			currentTo->parent->lastChild = tmp;
	}
	else
	{
		currentTo->nextSibling->prevSibling = tmp;
	}

	tmp->nextSibling = currentTo->nextSibling;
	tmp->parent      = currentTo->parent     ;

	alloc_.destroy( currentTo );
	alloc_.deallocate( currentTo, 1 );

	currentTo = tmp;

	TREE_NODE *last = from.node->nextSibling;

	preOrderIterator toit = tmp;

	do 
	{
		assert( currentFrom != 0 );
		if( currentFrom->firstChild != 0 )
		{
			currentFrom = currentFrom->firstChild;
			toit = appendChild( toit, currentFrom->data ); 
		}
		else
		{
			while( currentFrom->nextSibling == 0 && currentFrom != startFrom )
			{
				currentFrom = currentFrom->parent;
				toit = parent( toit );  
				assert( currentFrom != 0 );
			}
			currentFrom = currentFrom->nextSibling;
			if( currentFrom != last )
			{
				toit = appendChild( parent( toit ), currentFrom->data );
			}
		}
	} while ( currentFrom != last );

	return currentTo;
}

template< class T, class TreeNodeAllocator_ >
typename Tree< T, TreeNodeAllocator_ >::siblingIterator Tree< T, TreeNodeAllocator_ >::replace( siblingIterator origBegin,									                   
																							    siblingIterator origEnd  ,
                                                                                                siblingIterator newBegin ,
							                                                                    siblingIterator newEnd     )
{
    TREE_NODE *origFirst = origBegin.node;
	TREE_NODE *newFirst  = newBegin.node ;
    TREE_NODE *origLast  = origFirst;
	while( ( ++origBegin ) != origEnd )
	{
	    origLast = origLast->nextSibling;
	}

	TREE_NODE *newLast   = newFirst;
	while( ( ++newBegin ) != newEnd )
	{
		newLast = newLast->nextSibling;
	}

	bool first = true;
	preOrderIterator ret;
	while( 1==1 )
	{
		preOrderIterator tt = insertSubtree( preOrderIterator( origFirst ), preOrderIterator( newFirst ) );
		if( first )
		{
			ret = tt;
			first = false;
		}
		if( newFirst == newLast )
		{
			break;
		}
		newFirst = newFirst->nextSibling;
	}

    bool last = false;
	TREE_NODE *next = origFirst;
	while( 1 == 1 )
	{
		if( next == origLast )
		{
			last = true;
		}
		next = next->nextSibling;
		erase( ( preOrderIterator )origFirst );
		if( last )
		{
			break;
		}
		origFirst = next;
	}
	return ret;
}

template< class T, class TreeNodeAllocator_ >
template< typename iter >
iter Tree< T, TreeNodeAllocator_ >::flatten( iter position )
{
	if( position.node->firstChild == 0 )
	{
		return position;
	}

	TREE_NODE *tmp = position.node->firstChild;
	while( tmp )
	{
        tmp->parent = position.node->parent;
		tmp = tmp->nextSibling;
	}
	if( position.node->nextSibling )
	{
        position.node->lastChild->nextSibling = position.node->nextSibling;
		position.node->nextSibling->prevSibling = position.node->lastChild;
	}
	else
	{
		position.node->parent->lastChild = position.node->lastChild;
	}
	position.node->nextSibling = position.node->firstChild;
	position.node->nextSibling->prevSibling = position.node;
	position.node->firstChild = 0;
	position.node->lastChild  = 0;
	return position;
}

template< class T, class TreeNodeAllocator_ >
template< typename iter >
iter Tree< T, TreeNodeAllocator_ >::reparent( iter position, siblingIterator begin, siblingIterator end )
{
	TREE_NODE *first = begin.node;
	TREE_NODE *last  = first;

	assert( first != position.node );

	if( begin == end )
	{
		return begin;
	}
	while( ( ++begin ) != end )
	{
		last = last->nextSibling;
	}

    if( first->prevSibling == 0 )
	{
		first->parent->firstChild = last->nextSibling;
	}
	else
	{
		first->prevSibling->nextSibling = last->nextSibling;
	}

	if( last->nextSibling == 0 )
	{
		last->parent->lastChild = first->prevSibling;
	}
	else
	{
		last->nextSibling->prevSibling = first->prevSibling;
	}

	if( position.node->firstChild == 0 )
	{
        position.node->firstChild = first;
		position.node->lastChild  = last ;
		first->prevSibling = 0;
	}
	else
	{
		position.node->lastChild->nextSibling = first;
		first->prevSibling = position.node->lastChild;
		position.node->lastChild = last;
	}
	last->nextSibling = 0;

    TREE_NODE *pos = first;
	for( ; ; )
	{
        pos->parent = position.node;
		if( pos == last )
		{
			break;
		}
		pos = pos->nextSibling;
	}
	return first;
}

template< class T, class TreeNodeAllocator_ >
template< typename iter > iter Tree< T, TreeNodeAllocator_ >::reparent( iter position, iter from )
{
	if( from.node->firstChild == 0 )
		return position;
    
	return reparent( position, from.node->firstChild, end( from ) );
}

template< class T, class TreeNodeAllocator_ >
template< typename iter > iter Tree< T, TreeNodeAllocator_ >::wrap( iter position, const T& x )
{
	assert( position.node != 0 );

	siblingIterator fr = position;
	siblingIterator to = position;

	++to;

	iter ret = insert( position, x );
	reparent( ret, fr, to );
	return ret;
}

template< class T, class TreeNodeAllocator_ >
template< typename iter > iter Tree< T, TreeNodeAllocator_ >::moveAfter( iter target, iter source )
{
	TREE_NODE *dst = target.node;
	TREE_NODE *src = source.node;
	assert( dst );
	assert( src );

	if( dst == src )
	{
		return source;
	}

	if( dst->nextSibling )
	{
		if( dst->nextSibling == src )
		{
			return source;
		}
	}

	if( src->prevSibling != 0 )
	{
		src->prevSibling->nextSibling = src->nextSibling;
	}
	else
	{
		src->parent->firstChild = src->nextSibling;
	}

	if( src->nextSibling != 0 )
	{
		src->nextSibling->prevSibling = src->prevSibling;
	}
	else
	{
		src->parent->lastChild = src->prevSibling;
	}

	if( dst->nextSibling != 0 )
	{
		dst->nextSibling->prevSibling = src;
	}
	else
	{
		dst->parent->lastChild = src;
	}

	src->nextSibling = dst->nextSibling;
	dst->nextSibling = src             ;
	src->prevSibling = dst             ;
	src->parent      = dst->parent     ;
	return src;
}

template< class T, class TreeNodeAllocator_ >
template< typename iter > iter Tree< T, TreeNodeAllocator_ >::moveBefore( iter target, iter source )
{
	TREE_NODE *dst = target.node;
	TREE_NODE *src = source.node;

	assert( dst );
	assert( src );

    if( dst == src )
	{
		return source;
	}

	if( dst->prevSibling )
	{
		if( dst->prevSibling == src )
		{
			return source;
		}
	}

	if( src->prevSibling != 0 )
	{
		src->prevSibling->nextSibling = src->nextSibling;
	}
	else
	{
		src->parent->firstChild = src->nextSibling;
	}

	if( src->nextSibling != 0 ) 
	{
		src->nextSibling->prevSibling = src->prevSibling;
	}
	else
	{
		src->parent->lastChild = src->prevSibling;
	}

	if( dst->prevSibling != 0 )
	{
		dst->prevSibling->nextSibling = src;
	}
	else
	{
		dst->parent->firstChild = src;
	}

	src->prevSibling = dst->prevSibling;
	dst->prevSibling = src             ;
	src->nextSibling = dst             ;
	src->parent      = dst->parent     ;
	return src;
}

template< class T, class TreeNodeAllocator_ >
typename Tree< T, TreeNodeAllocator_ >::siblingIterator Tree< T, TreeNodeAllocator_ >::moveBefore( siblingIterator target, siblingIterator source )
{
	TREE_NODE *dst = target.node;
	TREE_NODE *src = source.node;
	TREE_NODE *dstPrevSibling   ;

	if( dst == 0 )
	{
		dstPrevSibling = target.parent->lastChild;
		assert( dstPrevSibling );
	}
	else
	{
		dstPrevSibling = dst->prevSibling;
	}

	assert( src );

	if( dst == src )
	{
		return source;
	}

	if( dstPrevSibling )
	{
		if( dstPrevSibling == src )
		{
            return source;
		}
	}

	if( src->prevSibling != 0 )
	{
		src->prevSibling->nextSibling = src->nextSibling;
	}
	else
	{
		src->parent->firstChild = src->nextSibling;
	}

	if( src->nextSibling != 0 )
	{
		src->nextSibling->prevSibling = src->prevSibling;
	}
	else
	{
		src->parent->lastChild = src->prevSibling;
	}

	if( dstPrevSibling != 0 )
	{
		dstPrevSibling->nextSibling = src;
	}
	else
	{
		target.parent->firstChild = src;
	}

	src->prevSibling = dstPrevSibling;
	
	if( dst )
	{
		dst->prevSibling = src;
		src->parent = dst->parent;
	}

    src->nextSibling = dst;
	return src;
}

template< class T, class TreeNodeAllocator_ >
template< typename iter > iter Tree< T, TreeNodeAllocator_ >::moveOntop( iter target, iter source )
{
	TREE_NODE *dst = target.node;
	TREE_NODE *src = source.node;

	assert( dst );
	assert( src );

    if( dst == src )
	{
		return source;
	}

	TREE_NODE *bPrevSibling = dst->prevSibling;
	TREE_NODE *bNextSibling = dst->nextSibling;
	TREE_NODE *bParent      = dst->parent     ;

    erase( target );

	if( src->prevSibling != 0 )
	{
		src->prevSibling->nextSibling = src->nextSibling;
	}
	else
	{
		src->parent->firstChild = src->nextSibling;
	}

	if( src->nextSibling != 0 )
	{
		src->nextSibling->prevSibling = src->prevSibling;
	}
	else
	{
		src->parent->lastChild = src->prevSibling;
	}

    if( bPrevSibling != 0 )
	{
		bPrevSibling->nextSibling = src;
	}
	else
	{
		bParent->firstChild = src;
	}

	if( bNextSibling != 0 )
	{
		bNextSibling->prevSibling = src;
	}
	else
	{
	    bParent->lastChild = src;
	}

	src->prevSibling = bPrevSibling;
	src->nextSibling = bNextSibling;
	src->parent      = bParent;
	return src;
}

template< class T, class TreeNodeAllocator_ >
void Tree< T, TreeNodeAllocator_ >::merge( siblingIterator to1, siblingIterator to2, siblingIterator from1, siblingIterator from2, bool duplicateLeaves )
{
	siblingIterator fnd;
	while( from1 != from2 )
	{
		if( ( fnd = std::find( to1, to2, ( *from1) ) ) != to2 )
		{
			if( from1.begin() == from1.end() )
			{
                if( duplicateLeaves )
				{
					appendChild( parent( to1 ), ( *from1 ) );
				}
			}
			else
			{
				merge( fnd.begin(), fnd.end(), from1.begin(), from1.end(), duplicateLeaves );
			}
		}
		else
		{
			insertSubtree( to2, from1 );
		}
		++from1;
	}
}

template< class T, class TreeNodeAllocator_ >
void Tree< T, TreeNodeAllocator_ >::sort( siblingIterator from, siblingIterator to, bool deep )
{
	std::less< T > comp;
	sort( from, to, comp, deep );
}

template< class T, class TreeNodeAllocator_ >
template< class StrictWeakOrdering >
void Tree< T, TreeNodeAllocator_ >::sort( siblingIterator from, siblingIterator to, StrictWeakOrdering comp, bool deep )
{
    if( from == to )
	{
		return;
	}

	std::multiset< TREE_NODE *, compareNodes<StrictWeakOrdering> > nodes( comp );
	siblingIterator it  = from;
	siblingIterator it2 = to  ;

	while( it != to )
	{
		nodes.insert( it.node );
		++it;
	}
	--it2;

    TREE_NODE *prev = from.node->prevSibling;
	TREE_NODE *next = it2.node->nextSibling ;

	typename std::multiset< TREE_NODE *, compareNodes<StrictWeakOrdering> >::iterator nit = nodes.begin();
	typename std::multiset< TREE_NODE *, compareNodes<StrictWeakOrdering> >::iterator eit = nodes.end();

	if( prev == 0 )
	{
        if( ( *nit )->parent != 0 )
		{
			( *nit )->parent->firstChild = ( *nit );
		}
	}
	else
	{
		prev->nextSibling = ( *nit );
	}

	--eit;

	while( nit != eit )
	{
        ( *nit )->prevSibling = prev;
		if( prev )
		{
			prev->nextSibling = ( *nit );
		}
		prev = ( *nit );
		++nit;
	}
    
	if( prev )
	{
		prev->nextSibling = ( *eit );
	}

	( *eit )->nextSibling = next;
	( *eit )->prevSibling = prev;

    if( next == 0 )
	{
		if( ( *eit )->parent != 0 )
		{
			( *eit )->parent->lastChild = ( *eit );
		}
	}
	else
	{
		next->prevSibling = ( *eit );
	}

	if( deep )
	{
        siblingIterator bcs( *nodes.begin() );
		siblingIterator ecs( *eit );
		++ecs;
        while( bcs != ecs )
		{
			sort( begin( bcs ), end( bcs ), comp, deep );
			++bcs;
		}
	}
}

template< class T, class TreeNodeAllocator_ >
template< typename iter >
bool Tree< T, TreeNodeAllocator_ >::equal( const iter& one_, const iter& two, const iter& three_ ) const
{
	std::equal_to< T > comp;
	return equal( one_, two, three_, comp );
}

template< class T, class TreeNodeAllocator_ >
template< typename iter, class BinaryPredicate >
bool Tree< T, TreeNodeAllocator_ >::equal( const iter& one_, const iter& two, const iter& three_, BinaryPredicate fun ) const
{
    preOrderIterator one(   one_   );
	preOrderIterator three( three_ );

	while( one != two && isValid( three ) )
	{
	    if( !fun( *one, *three ) )
		{
			return false;
		}
		if( one.numberOfChildren() != three.numberOfChildren() )
		{
			return false;
		}
		++one;
		++three;
	}
	return true;
}

template< class T, class TreeNodeAllocator_ >
template< typename iter >
bool Tree< T, TreeNodeAllocator_ >::equalSubTree( const iter& one_, const iter& two_ ) const
{
	std::equal_to< T > comp;
	return equalSubTree( one_, two_, comp );
}

template< class T, class TreeNodeAllocator_ >
template< typename iter, class BinaryPredicate >
bool Tree< T, TreeNodeAllocator_ >::equalSubTree( const iter& one_, const iter& two_, BinaryPredicate fun ) const
{
	preOrderIterator one( one_ );
	preOrderIterator two( two_ );

	if( !fun( *one, *two ) )
	{
        return false;
	}

	if( numberOfChildren( one ) != numberOfChildren( two ) )
	{
		return false;
	}

	return equal( begin( one ), end( one ), begin( two ), fun );
}

template< class T, class TreeNodeAllocator_ >
Tree< T, TreeNodeAllocator_ > Tree< T, TreeNodeAllocator_ >::subTree( siblingIterator from, siblingIterator to ) const
{
	Tree tmp;
	tmp.setHead( value_type() );
	tmp.replace( tmp.begin(), tmp.end(), from, to );
	return tmp;
}

template< class T, class TreeNodeAllocator_ >
void Tree< T, TreeNodeAllocator_ >::subTree( Tree& tmp, siblingIterator from, siblingIterator to ) const
{
    tmp.setHead( value_type() );
	tmp.replace( tmp.begin(), tmp.end(), from, to );
}

template< class T, class TreeNodeAllocator_ >
void Tree< T, TreeNodeAllocator_ >::swap( siblingIterator it )
{
	TREE_NODE *nxt = it.node->nextSibling;
	if( nxt )
	{
		if( it.node->prevSibling )
		{
			it.node->prevSibling->nextSibling = nxt;
		}
		else
		{
			it.node->parent->firstChild = nxt;
		}
        nxt->prevSibling = it.node->prevSibling;
		TREE_NODE *nxtnxt = nxt->nextSibling;

		if( nxtnxt )
		{
			nxtnxt->prevSibling = it.node;
		}
		else
		{
			it.node->parent->lastChild = it.node;
		}
		nxt->nextSibling     = it.node;
		it.node->prevSibling = nxt    ;
		it.node->nextSibling = nxtnxt ;
	}
}

template< class T, class TreeNodeAllocator_ >
void Tree< T, TreeNodeAllocator_ >::swap( preOrderIterator one, preOrderIterator two )
{
	if( one.node->nextSibling == two.node )
	{
		swap( one );
	}
	else if( two.node->nextSibling == one.node )
	{
		swap( two );
	}
	else
	{
		TREE_NODE *nxt1 = one.node->nextSibling;
		TREE_NODE *nxt2 = two.node->nextSibling;
		TREE_NODE *pre1 = one.node->prevSibling;
        TREE_NODE *pre2 = two.node->prevSibling;
		TREE_NODE *par1 = one.node->parent     ;
		TREE_NODE *par2 = two.node->parent     ;

		one.node->parent = par2;
		one.node->nextSibling = nxt2;

		if( nxt2 )
		{
			nxt2->prevSibling = one.node;
		}
		else
		{
            par2->lastChild = one.node;
		}

		one.node->prevSibling = pre2;

		if( pre2 )
		{
			pre2->nextSibling = one.node;
		}
		else
		{
			par2->firstChild = one.node;
		}

		two.node->parent = par1;
		two.node->nextSibling = nxt1;
		if( nxt1 )
		{
			nxt1->prevSibling = two.node;
		}
		else
		{
			par1->lastChild = two.node;
		}

		two.node->prevSibling = pre1;

		if( pre1 )
		{
			pre1->nextSibling = two.node;
		}
		else
		{
			par1->firstChild = two.node;
		}
	}
}

template< class T, class TreeNodeAllocator_ >
size_t Tree< T, TreeNodeAllocator_ >::size() const
{
	size_t i = 0;
	preOrderIterator it  = begin();
	preOrderIterator eit = end()  ;

	while( it != eit )
	{
		++i;
		++it;
	}
	return i;
}

template< class T, class TreeNodeAllocator_ >
size_t Tree< T, TreeNodeAllocator_ >::size( const iteratorBase& top ) const
{
	size_t i = 0;
	preOrderIterator it  = top;
	preOrderIterator eit = top;

	eit.skipChildren();
	++eit;
	while( it != eit )
	{
		++i;
		++it;
	}
	return i;
}

template< class T, class TreeNodeAllocator_ >
bool Tree< T, TreeNodeAllocator_ >::empty() const
{
	preOrderIterator it  = begin();
	preOrderIterator eit = end()  ;
	return ( it == eit );
}

template< class T, class TreeNodeAllocator_ >
int Tree< T, TreeNodeAllocator_ >::depth( const iteratorBase& it )
{
	TREE_NODE *pos = it.node;
	assert( pos != 0 );
	
	int ret = 0;
    while( pos->parent != 0 )
	{
		pos = pos->parent;
		++ret;
	}
	return ret;
}

template< class T, class TreeNodeAllocator_ >
int Tree< T, TreeNodeAllocator_ >::depth( const iteratorBase& it, const iteratorBase& root )
{
	TREE_NODE *pos = it.node;

	assert( pos != 0 );
	int ret = 0;

	while( pos->parent != 0 && pos != root.node )
	{
        pos = pos->parent;
		++ret;
	}
	return ret;
}

template< class T, class TreeNodeAllocator_ >
int Tree< T, TreeNodeAllocator_ >::maxDepth() const
{
	int maxd = -1;
	for( TREE_NODE *it = head->nextSibling; it != feet; it = it->nextSibling )
	{
		maxd = std::max( maxd, maxDepth( it ) );
	}
	return maxd;
}

template< class T, class TreeNodeAllocator_ >
int Tree< T, TreeNodeAllocator_ >::maxDepth( const iteratorBase& pos ) const
{
	TREE_NODE *tmp = pos.node;

	if( tmp == 0 || tmp == head || tmp == feet )
	{
		return -1;
	}

    int curdepth = 0;
	int maxdepth = 0;

	while( true )
	{
		while( tmp->firstChild == 0 )
		{
			if( tmp == pos.node )
			{
				return maxdepth;
			}

			if( tmp->nextSibling == 0 )
			{
				do 
				{
					tmp = tmp->parent;

                    if( tmp == 0 )
					{
						return maxdepth;
					}
					--curdepth;
				} while( tmp->nextSibling == 0 );
			}

			if( tmp == pos.node )
			{
                return maxdepth;
			}

			tmp = tmp->firstChild;
			++curdepth;
			maxdepth = std::max( curdepth, maxdepth );
		}
	}
}

template< class T, class TreeNodeAllocator_ >
size_t Tree< T, TreeNodeAllocator_ >::numberOfChildren( const iteratorBase& it )
{
	TREE_NODE *pos = it.node->firstChild;
	if( pos == 0 )
	{
		return 0;
	}

	size_t ret = 1;

	while( ( pos = pos->nextSibling ) )
	{
		++ret;
	}

	return ret;
}

template< class T, class TreeNodeAllocator_ >
size_t Tree< T, TreeNodeAllocator_ >::numberOfSiblings( const iteratorBase& it ) const
{
	TREE_NODE *pos = it.node;

	size_t ret = 0;

	while( pos->nextSibling         && 
		   pos->nextSibling != head && 
		   pos->nextSibling != feet    )
	{
	    ++ret;
		pos = pos->nextSibling;
	}

	pos = it.node;
	while( pos->prevSibling         && 
		   pos->prevSibling != head && 
		   pos->prevSibling != feet    )
	{
		++ret;
		pos = pos->prevSibling;
	}

	return ret;
}

template< class T, class TreeNodeAllocator_ >
bool Tree< T, TreeNodeAllocator_ >::isInSubTree( const iteratorBase& it, const iteratorBase& begin, const iteratorBase& end ) const
{
	preOrderIterator tmp = begin;
	while( tmp != end )
	{
		if( tmp == it )
		{
			return true;
		}
		++tmp;
	}
	return false;
}

template< class T, class TreeNodeAllocator_ >
bool Tree< T, TreeNodeAllocator_ >::isValid( const iteratorBase& it ) const
{
	if( it.node == 0 || it.node == feet || it.node == head ) 
	{
		return false;
	}
	else
	{
		return true;
	}
}

template< class T, class TreeNodeAllocator_ >
typename Tree< T, TreeNodeAllocator_ >::preOrderIterator Tree< T, TreeNodeAllocator_ >::lowestCommonAncestor( const iteratorBase& one, const iteratorBase& two ) const
{
	std::set< preOrderIterator, iteratorBaseLess > parents;

	preOrderIterator walk = one;
	do 
	{
		walk = parent( walk );
		parents.insert( walk );
	} while( isValid( parent( walk ) ) );

	walk = two;
	do 
	{
		walk = parent( walk );
		if( parents.find( walk ) != parents.end() )
		{
			break;
		}
	} while( isValid( parent( walk ) ) );

	return walk;
}

template< class T, class TreeNodeAllocator_ >
size_t Tree< T, TreeNodeAllocator_ >::index( siblingIterator it ) const
{
	size_t ind = 0;
	if( it.node->parent == 0 )
	{
		while( it.node->prevSibling != head )
		{
			it.node = it.node->prevSibling;
			++ind;
		}
	}
	else
	{
		while( it.node->prevSibling != 0 )
		{
			it.node = it.node->prevSibling;
			++ind;
		}
	}
	return ind;
}

template< class T, class TreeNodeAllocator_ >
typename Tree< T, TreeNodeAllocator_ >::siblingIterator Tree< T, TreeNodeAllocator_ >::child( const iteratorBase& it, size_t num )
{
	TREE_NODE *tmp = it.node->firstChild;
	while( num-- )
	{
		assert( tmp != 0 );
		tmp = tmp->nextSibling;
	}
	return tmp;
}

template< class T, class TreeNodeAllocator_ >
typename Tree< T, TreeNodeAllocator_ >::siblingIterator Tree< T, TreeNodeAllocator_ >::sibling( const iteratorBase& it, size_t num )
{
	TREE_NODE *tmp;
	if( it.node->parent == 0 )
	{
        tmp = head->nextSibling;
		while( num )
		{
			tmp = tmp->nextSibling;
			--num;
		}
	}
	else
	{
		tmp = it.node->parent->firstChild;
		while( num )
		{
			assert( tmp != 0 );
			tmp = tmp->nextSibling;
			--num;
		}
	}
	return tmp;
}

template< class T, class TreeNodeAllocator_ >
void Tree< T, TreeNodeAllocator_ >::debug_verify_consistency() const
{
	preOrderIterator it = begin();

	while( it != end() )
	{
		if( it.node->parent != 0 )
		{
			if( it.node->prevSibling == 0 )
			{
				assert( it.node->parent->firstChild == it.node );
			}
			else
			{
                assert( it.node->prevSibling->nextSibling == it.node );
			}

			if( it.node->nextSibling == 0 )
			{
				assert( it.node->parent->lastChild == it.node );
			}
			else
			{
				assert( it.node->nextSibling->prevSibling == it.node );
			}
		}
		++it;
	}
}

#endif
