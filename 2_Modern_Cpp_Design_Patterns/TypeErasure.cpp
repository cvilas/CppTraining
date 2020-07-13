/**************************************************************************************************
*
* \file TypeErasure.cpp
* \brief C++ Training - Programming Task for Type Erasure
*
* Copyright (C) 2015-2020 Klaus Iglberger - All Rights Reserved
*
* This file is part of the C++ training by Klaus Iglberger. The file may only be used in the
* context of the C++ training or with explicit agreement by Klaus Iglberger.
*
* Task: Implement the 'Shape' class by means of Type Erasure. 'Shape' may require all types to
*       provide a free 'draw()' function that draws them to the screen.
*
**************************************************************************************************/

#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <memory>

// Defining this makes even integer behave like a shape. Follow the rest of this page
void draw(int i)
{
    std::cout << "integer: " << i << std::endl;
}

// This follows the develop in Function.cpp for an abstract callable 
// Compare this with the variant approach. Again fully value-based approach. But we don't need to know all types anymore.
// We can now extend with new types

class Shape
{
public:
    template<typename shape_rep>
    Shape(shape_rep shape) : pimpl_{std::make_unique<Model<shape_rep>>(shape)}
    {}
	~Shape() = default;
	Shape(Shape&&) = default;
	Shape& operator=(Shape&&) = default;

	Shape(const Shape& f) 
		: pimpl_{f.pimpl_->clone()} {

	}
	
	Shape& operator=(const Shape& f)
	{
		using std::swap;// prefer an unqualified call to swap
		// Temporary swap idiom
		Shape tmp(f);
		swap(pimpl_, tmp.pimpl_);
		return *this;
	}
    
    // void draw() const { pimpl_->doDraw(); }
    
private:
    
    // Sets requirements on Shape. Anything that satifies the following requirements set in Concept is a Shape. So now
    // we don't need a class inheritance anymore. This is the core of dynamic polymorphism.
    // We can even define `void draw(int i)` and suddenly integer becomes a Shape. This demonstrates very lose coupling
    struct Concept
    {
        virtual ~Concept() = default;
        virtual void doDraw() const = 0; //!< made the requirement that any kind of shape needs a draw function, but it's not coupled to Shape class
        virtual std::unique_ptr<Concept> clone() const = 0;
    };
    
    template<typename shape_rep>
    struct Model : public Concept
    {
        explicit Model(shape_rep s) : shape_(s){}
        void doDraw() const override { draw(shape_); }
		std::unique_ptr<Concept> clone() const override {return std::make_unique<Model>(*this);}
        shape_rep shape_;
    };
    
    std::unique_ptr<Concept> pimpl_;
    
    friend void draw(Shape const & shape);
};

void draw( Shape const& shape )
{
    shape.pimpl_->doDraw();
}


using Shapes = std::vector<Shape>;

void draw( Shapes const& shapes )
{
   for( auto const& shape : shapes )
   {
      draw( shape );
   }
}


struct Circle
{
   double radius;
};

void draw( Circle const& circle )
{
   std::cout << "circle: radius=" << circle.radius << std::endl;
}


struct Square
{
   double side;
};

void draw( Square const& square )
{
   std::cout << "square: side=" << square.side << std::endl;
}



int main()
{
   Shapes shapes;

   shapes.push_back( Circle{ 2.3 } );
   shapes.push_back( Square{ 1.2 } );
   shapes.push_back( Circle{ 4.1 } );
   shapes.push_back(42);
   
   draw( shapes );

   return EXIT_SUCCESS;
}

