/**************************************************************************************************
*
* \file Visitor.cpp
* \brief C++ Training - Programming Task for the Visitor Design Pattern
*
* Copyright (C) 2015-2020 Klaus Iglberger - All Rights Reserved
*
* This file is part of the C++ training by Klaus Iglberger. The file may only be used in the
* context of the C++ training or with explicit agreement by Klaus Iglberger.
*
* Task: Refactor the classical Visitor solution by a value semantics based solution. Note that
*       the general behavior should remain unchanged.
*
**************************************************************************************************/

#include <iostream>
#include <vector>
#include <variant>

/// Improvements
/// - Using standard C++ std::visit function rather than a custom Visitor class hierarchy
/// - Removed all abstract base classes
/// - No virtual functions, no pointer indirections, no double dispatch. Allows functions to be inlined.
/// - No need for destructors
/// - No dynamic memory allocation (unique_ptr is removed)
/// - Value semantics rather than pointer semantics. Easy to reason about in terms of determinism and behaviour
/// - Lots of code erased. Very few additions
/// - Follows single responsibility principle
/// - Follows open-close principle on functionality but not types. No couplings:
///         - Can't extend to new types without a recompile, although types don't need to know each other. 
///         - Third parties or customers can extend existing functions without recompile, and without breaking existing code.
/// - Performance overhead: std::variant is slower but an acceptable compromise compared to inflexible enum-based implementation.
///
/// Disadvantages:
/// - Requires  closed set of known types. Use to extend functionality, not types
/// - Abstraction across architecture boundaries
/// - not best performance.

class Circle;
class Square;

using Shape = std::variant<Circle, Square>;

class Circle 
{
 public:
   Circle( double r ) : radius_( r )   {}
   double radius() const { return radius_; }

 private:
   double radius_;
};


class Square 
{
 public:
   Square( double s ) : side_( s ) {}
   double side() const { return side_; }

 private:
   double side_;
};


class Draw
{
 public:
    template<typename T>
    void operator()( const T& c ) const { std::cout << typeid(T).name() << "\n"; }
    /// TODO: Reimplement for each T that we want to support
};


using Shapes = std::vector<Shape>;

void draw( Shapes const& shapes )
{
   for( auto const& shape : shapes )
   {
      std::visit( Draw{}, shape );
   }
}


int main()
{
   Shapes shapes;

   shapes.push_back( Circle( 2.3 ) );
   shapes.push_back( Square( 1.2 ) );
   shapes.push_back( Circle( 4.1 ) );

   draw( shapes );

   return EXIT_SUCCESS;
}
