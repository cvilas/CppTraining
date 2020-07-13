/**************************************************************************************************
*
* \file Function.cpp
* \brief C++ Training - Programming Task for Type Erasure
*
* Copyright (C) 2015-2020 Klaus Iglberger - All Rights Reserved
*
* This file is part of the C++ training by Klaus Iglberger. The file may only be used in the
* context of the C++ training or with explicit agreement by Klaus Iglberger.
*
* Task: Implement a simplified std::function to demonstrate the type erasure design pattern.
*
**************************************************************************************************/

#include <cstdlib>
#include <iostream>
#include <memory>

//--Function.h-------------------------------------------------------------------------------------

template< typename Fn >
class Function;

// specialisation for function types - called if Fn above is function type
template<typename R, typename... Args>
class Function<R(Args...)>
{
public:
	template<typename Fn>
	Function(Fn fn) : pimpl{ std::make_unique<Model<Fn>>(fn) }
	{}
	~Function() = default;
	Function(Function&&) = default;
	Function& operator=(Function&&) = default;

	Function(const Function& f) 
		: pimpl{f.pimpl->clone()} {

	}
	
	Function& operator=(const Function& f)
	{
		using std::swap;// prefer an unqualified call to swap
		// Temporary swap idiom
		Function tmp(f);
		swap(pimpl, tmp.pimpl);
		return *this;
	}

	R operator()(Args... args) const { return pimpl->invoke(args...); }
private:
	// Hiding an inheritance based polymorphism here so that our users don't have to deal with the complexity of callables. And we apply a requirement that 
	// it be a callable by defining the class Function the way we have. We achieve type erasure by hiding it here in 
    // the private section
    
    // Concept sets requirements for the callable
	struct Concept
	{
		virtual ~Concept() = default;
		virtual R invoke(Args...) const = 0;
		virtual std::unique_ptr<Concept> clone() const = 0;
	};

	template< typename Fn>
	struct Model : public Concept
	{
		Model(Fn f) : fn(f){}
		R invoke(Args... args) const override { return fn(args...); }
		std::unique_ptr<Concept> clone() const override {return std::make_unique<Model>(*this);}
		Fn fn;
	};

	std::unique_ptr<Concept> pimpl;	
};

//--Main.cpp---------------------------------------------------------------------------------------


int foo()
{
   return 1;
}

struct Foo {
   int operator()() const {
      return 2;
   }
};


int main()
{
   {
      Function<int(void)> f( foo );
      auto const res = f();
      std::cerr << "\n res = " << res << "\n\n";
   }

   {
      Function<int(void)> f( Foo{} );
      auto const res = f();
      std::cerr << "\n res = " << res << "\n\n";
   }

   {
      Function<int(void)> f( [](){ return 3; } );
      auto const res = f();
      std::cerr << "\n res = " << res << "\n\n";
   }

   return EXIT_SUCCESS;
}
