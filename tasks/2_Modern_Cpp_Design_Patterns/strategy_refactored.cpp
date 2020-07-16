/**************************************************************************************************
*
* \file Strategy_Cpp11.cpp
* \brief C++ Training - Programming Task for the Strategy Design Pattern
*
* Copyright (C) 2015-2020 Klaus Iglberger - All Rights Reserved
*
* This file is part of the C++ training by Klaus Iglberger. The file may only be used in the
* context of the C++ training or with explicit agreement by Klaus Iglberger.
*
*
* Task: Refactor the classical Strategy solution by a value semantics based solution. Note that
*       the general behavior should remain unchanged.
*
**************************************************************************************************/

#include <iostream>
#include <memory>
#include <vector>
#include <functional>

// Improvements
// - Does not break interface segregation principle (draw strategy and shapes are not coupled)
// use std::function to
// - abstract and decouple a single function, not several cohesive functions
// - if performance is not the primary concern


class Circle;
class Square;

using CircleDrawStrategy = std::function<void(const Circle& c)>;
using SquareDrawStrategy = std::function<void(const Square& s)>;

struct Draw {
    template<typename T>
    void operator()(const T& shape) const {
        draw(shape);
    }
};

class Shape {
public:
    Shape() = default;
    virtual ~Shape() = default;
    virtual void draw() const = 0;
};

class Circle : public Shape {
 public:
   Circle( double r, CircleDrawStrategy d = Draw{})
      : Shape()
      , radius_( r )
      , draw_( std::move(d) )
   {}

   ~Circle() {}

   void draw() const override { draw_( *this ); }

   double radius() const { return radius_; }

 private:
   double radius_;
   CircleDrawStrategy draw_;
};


class Square : public Shape {
 public:
   Square( double s, SquareDrawStrategy d = Draw{})
      : Shape()
      , side_( s )
      , draw_( std::move(d) )
   {}

   ~Square() {}

   void draw() const override { draw_( *this ); }

   double side() const { return side_; }

 private:
   double side_;
   SquareDrawStrategy draw_;
};

void draw( const Circle& circle ) {
    std::cout << "circle: radius=" << circle.radius() << std::endl;
}

void draw( const Square& square ) {
    std::cout << "square: side=" << square.side() << std::endl;
}


using Shapes = std::vector< std::unique_ptr<Shape> >;

void draw( Shapes const& shapes ) {
   for( auto const& shape : shapes ) {
      shape->draw();
   }
}


int main()
{
   Shapes shapes;

   shapes.push_back( std::make_unique<Circle>(2.3) );
   shapes.push_back( std::make_unique<Square>(1.2) );
   shapes.push_back( std::make_unique<Circle>(4.1) );

   draw( shapes );

   return EXIT_SUCCESS;
}

