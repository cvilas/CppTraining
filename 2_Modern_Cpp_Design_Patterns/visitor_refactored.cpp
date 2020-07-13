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
/// - Follows open-close principle on both types as well as functions. No couplings:
///         - Types don't need to know each other. 
///         - Draw() does not need to know all types. 
///         - Third parties or customers can extend existing implementation without recompile, and without breaking existing code.
/// - Performance overhead: Very acceptable compromise with inflexible enum-based implementation.

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
