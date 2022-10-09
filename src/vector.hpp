#pragma once

#include "algorithm.hpp"
#include "iterator.hpp"
#include "type_traits.hpp"
#include <algorithm>
#include <iostream>
#include <iterator>
#include <memory>

namespace ft {

template < typename T, class Allocator = std::allocator< T > > class vector {

    /* -------------------------------- Iterator -------------------------------- */

    template < typename U > class Iterator {
    public:
        typedef U &                             reference;
        typedef U *                             pointer;
        typedef std::ptrdiff_t                  difference_type;
        typedef std::random_access_iterator_tag iterator_category;

    private:
        pointer _p;

    public:
        Iterator( pointer p ) : _p( p ) {}
        Iterator( const Iterator< T > &other ) : _p( other.operator->() ) {}

        Iterator        operator+( difference_type n ) const { return _p + n; }
        Iterator        operator-( difference_type n ) const { return _p - n; }
        difference_type operator-( const Iterator &other ) const { return _p - other._p; }

        Iterator operator++() {
            _p++;
            return *this;
        }
        Iterator operator--() {
            _p--;
            return *this;
        }
        Iterator operator++( int ) {
            Iterator copy( *this );
            _p++;
            return copy;
        }
        Iterator operator--( int ) {
            Iterator copy( *this );
            _p--;
            return copy;
        }

        void operator+=( difference_type n ) { _p += n; };
        void operator-=( difference_type n ) { _p -= n; };

        bool operator==( const Iterator &other ) const { return ( _p == other._p ); };
        bool operator!=( const Iterator &other ) const { return ( _p != other._p ); };
        bool operator>( const Iterator &other ) const { return ( _p > other._p ); };
        bool operator<( const Iterator &other ) const { return ( _p < other._p ); };
        bool operator>=( const Iterator &other ) const { return ( _p >= other._p ); };
        bool operator<=( const Iterator &other ) const { return ( _p <= other._p ); };

        reference operator*() { return *_p; }
        reference operator[]( difference_type i ) { return _p[i]; }
        pointer   operator->() const { return ( _p ); };
    };

    /* ------------------------------ Member types ------------------------------ */

public:
    typedef T                                        value_type;
    typedef Allocator                                allocator_type;
    typedef typename allocator_type::reference       reference;
    typedef typename allocator_type::const_reference const_reference;
    typedef typename allocator_type::pointer         pointer;
    typedef typename allocator_type::const_pointer   const_pointer;
    typedef Iterator< value_type >                   iterator;
    typedef Iterator< const value_type >             const_iterator;
    typedef ft::reverse_iterator< iterator >         reverse_iterator;
    typedef ft::reverse_iterator< const_iterator >   const_reverse_iterator;
    typedef std::size_t                              size_type;

    /* ------------------------------- Attributes ------------------------------- */

protected:
    allocator_type _allocator;
    pointer        _data;
    size_type      _capacity;
    size_type      _size;

    /* ------------------------------ Construction ------------------------------ */

public:
    explicit vector( const allocator_type &alloc = allocator_type() )
        : _allocator( alloc ),
          _data( 0 ),
          _capacity( 0 ),
          _size( 0 ) {}

    explicit vector( size_type n, const value_type &val = value_type(), const allocator_type &alloc = allocator_type() )
        : _allocator( alloc ),
          _data( 0 ),
          _capacity( 0 ),
          _size( 0 ) {
        assign( n, val );
    }

    template < class U > vector( U first, U last, const allocator_type &alloc = allocator_type() );
    vector( const vector &other ) : _allocator( other._allocator ), _data( 0 ), _capacity( 0 ), _size( 0 ) {
        assign( other.begin(), other.end() );
    }

    virtual ~vector() {
        clear();
        _allocator.deallocate( _data, _capacity );
    }

    vector &operator=( const vector &other ) {
        assign( other.begin(), other.end() );
        return *this;
    }

    /* -------------------------------- Capacity -------------------------------- */

    size_type size() const { return _size; }

    size_type max_size() const { return _allocator.max_size(); }

    void resize( size_type n, value_type val = value_type() ) {
        if ( n > _size ) {
            insert( end(), n - _size, val );
        } else {
            erase( begin() + n, end() );
        }
    }

    size_type capacity() const { return _capacity; }

    bool empty() const { return !_size; }

    void reserve( size_type n ) {
        if ( n > _capacity ) {
            size_type len( 1 );
            while ( len < n ) { len <<= 1; }
            pointer tmp = _allocator.allocate( len );
            for ( size_type i( 0 ); i < _size; i++ ) { _allocator.construct( tmp + i, _data[i] ); }
            size_type size = _size;
            clear();
            _allocator.deallocate( _data, _capacity );
            _size     = size;
            _capacity = len;
            _data     = tmp;
        }
    }

    /* -------------------------------- Iterators ------------------------------- */

    iterator               begin() { return _data; }
    const_iterator         begin() const { return _data; }
    iterator               end() { return _data + _size; }
    const_iterator         end() const { return _data + _size; }
    reverse_iterator       rbegin() { return _data + _size - 1; }
    const_reverse_iterator rbegin() const { return _data + _size - 1; };
    reverse_iterator       rend() { return _data; }
    const_reverse_iterator rend() const { return _data; }
    const_iterator         cbegin() const { return _data; }
    const_iterator         cend() const { return _data + _size; }
    const_reverse_iterator crbegin() const { return _data + _size - 1; }
    const_reverse_iterator crend() const { return _data; };

    /* ----------------------------- Element access ----------------------------- */

    reference         operator[]( size_type i ) { return i >= _size ? *end() : _data[i]; }
    const_reference   operator[]( size_type i ) const { return operator[]( i ); }
    reference         at( size_type i );
    const_reference   at( size_type i ) const;
    reference         front() { return *begin(); }
    const_reference   front() const { return front(); }
    reference         back() { return *end(); }
    const_reference   back() const { return back(); }
    value_type *      data() { return _data; }
    const value_type *data() const { return data(); }

    /* -------------------------------- Modifiers ------------------------------- */

    template < class U >
    void assign( U first, U last, typename ft::enable_if< !ft::is_integral< U >::value, U >::type * = 0 ) {
        clear();
        insert( begin(), first, last );
    }

    void assign( size_type n, const value_type &val ) {
        clear();
        insert( begin(), n, val );
    }

    void push_back( const value_type &val ) { insert( end(), val ); }

    void pop_back() {
        if ( _size ) {
            _allocator.destroy( _data + _size - 1 );
            _size--;
        }
    }

    iterator insert( iterator position, const value_type &val ) {
        insert( position, 1, val );
        return position;
    }

    void insert( iterator position, size_type n, const value_type &val ) {
        size_type i = position - begin();
        reserve( _size + n );
        position = begin() + i;
        for ( iterator it = position; it < position + n; it++ ) {
            if ( it - begin() < _size ) { it[n] = *it; }
            *it = val;
        }
        _size += n;
    }

    template < typename U >
    void insert( iterator position,
                 U        first,
                 U        last,
                 typename ft::enable_if< !ft::is_integral< U >::value, U >::type * = 0 ) {
        size_type i = position - begin();
        reserve( _size + ( last - first ) );
        position = begin() + i;
        for ( U it = first; it != last; it++, position++ ) {
            if ( position - begin() < _size ) { position[last - first] = *position; }
            *position = *it;
        }
        _size += last - first;
    }

    iterator erase( iterator position ) { return erase( position, position + 1 ); }

    iterator erase( iterator first, iterator last ) {
        size_type i = last - first;
        for ( iterator it = first; it != end(); it++ ) {
            if ( it >= last ) { it[-i] = *it; }
            _allocator.destroy( it.operator->() );
        }
        _size -= i;
        return first;
    }

    void swap( vector &other ) {
        ft::swap( _data, other._data );
        ft::swap( _capacity, other._capacity );
        ft::swap( _size, other._size );
    }

    void clear() {
        for ( size_type i = 0; i < _size; i++ ) { _allocator.destroy( _data + i ); }
        _size = 0;
    }

    /* -------------------------------- Allocator ------------------------------- */

    allocator_type get_allocator() const { return _allocator; }

    /* -------------------------------------------------------------------------- */
};
}
