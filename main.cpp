#include "src/map.hpp"
#include "src/rb_tree.hpp"
#include "src/stack.hpp"
#include "src/vector.hpp"

#include <cstdlib>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
//#include <type_traits>
#include <utility>
#include <vector>

template < typename T >
std::ostream &operator<<( std::ostream &os, const ft::vector< T > &v ) {
    os << "size: " << v.size() << std::endl;
    os << "capacity: " << v.capacity() << std::endl;
    os << "[";
    for ( typename ft::vector< T >::const_iterator it = v.begin();
          it != v.end();
          it++ ) {
        if ( it != v.begin() ) { os << ", "; }
        os << *it;
    }
    os << "]";
    return os;
}

template < typename T >
std::ostream &operator<<( std::ostream &os, const std::vector< T > &v ) {
    os << "size: " << v.size() << std::endl;
    os << "capacity: " << v.capacity() << std::endl;
    os << "[";
    for ( typename std::vector< T >::const_iterator it = v.begin();
          it != v.end();
          it++ ) {
        if ( it != v.begin() ) { os << ", "; }
        os << *it;
    }
    os << "]";
    return os;
}

template < typename T, typename U >
std::ostream &operator<<( std::ostream &os, const std::pair< T, U > &p ) {
    os << p.first << " " << p.second;
    return os;
}

template < typename T, typename U > struct cmp {
    bool operator()( const std::pair< T, U > &a,
                     const std::pair< T, U > &b ) const {
        return a.first < b.first;
    }
};

template < typename K, typename V > struct getter {
    typedef V return_type;
    V         operator()( std::pair< K, V > p ) const { return p.second; }
};

#define TESTED_NAMESPACE ft

int main() {
    srand( time( 0 ) );

    typedef typename TESTED_NAMESPACE::map< int, int > map_type;
    
    map_type m;
    
    m[4] = 8;
    m[9] = 15;
    
    std::cout << *m.upper_bound(9) << std::endl;
    std::cout << (m.upper_bound(9) == m.end()) << std::endl;
}
