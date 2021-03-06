/**************************************************************************************************
*
* \file Strategy_Benchmark.cpp
* \brief C++ Training - Programming Task for the Strategy Design Pattern
*
* Copyright (C) 2015-2020 Klaus Iglberger - All Rights Reserved
*
* This file is part of the C++ training by Klaus Iglberger. The file may only be used in the
* context of the C++ training or with explicit agreement by Klaus Iglberger.
*
**************************************************************************************************/

#define BENCHMARK_OO_SOLUTION 1
#define BENCHMARK_STRATEGY_SOLUTION 1
#define BENCHMARK_STD_FUNCTION_SOLUTION 1
#define BENCHMARK_BOOST_FUNCTION_SOLUTION 0
#define BENCHMARK_MANUAL_FUNCTION_SOLUTION 0
#define BENCHMARK_TYPE_ERASURE_SOLUTION 0
#define BENCHMARK_TYPE_ERASURE_SBO_SOLUTION 0
#define BENCHMARK_TYPE_ERASURE_DYNO_SOLUTION 0


#include <chrono>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <random>
#include <type_traits>
#include <vector>
#if BENCHMARK_BOOST_FUNCTION_SOLUTION
#  include <boost/function.hpp>
#endif
#if BENCHMARK_TYPE_ERASURE_DYNO_SOLUTION
#  include <dyno.hpp>
using namespace dyno::literals;
#endif


struct Vector3D
{
   double x{};
   double y{};
   double z{};
};

Vector3D operator+( const Vector3D& a, const Vector3D& b )
{
   return Vector3D{ a.x+b.x, a.y+b.y, a.z+b.z };
}


#if BENCHMARK_OO_SOLUTION
namespace object_oriented_solution {

   struct Shape
   {
      Shape()
      {}

      virtual ~Shape() {}

      virtual void translate( const Vector3D& v ) = 0;
   };


   struct Circle : public Shape
   {
      Circle( double rad )
         : Shape()
         , radius( rad )
         , center()
      {}

      ~Circle() {}

      void translate( const Vector3D& v ) override
      {
         center = center + v;
      }

      double radius;
      Vector3D center;
   };


   struct Square : public Shape
   {
      Square( double s )
         : Shape()
         , side( s )
         , center()
      {}

      ~Square() {}

      void translate( const Vector3D& v ) override
      {
         center = center + v;
      }

      double side;
      Vector3D center;
   };


   using Shapes = std::vector< std::unique_ptr<Shape> >;

   void translate( Shapes& shapes, const Vector3D& v )
   {
      for( const auto& s : shapes )
      {
         s->translate( v );
      }
   }

} // namespace object_oriented_solution
#endif


#if BENCHMARK_STRATEGY_SOLUTION
namespace strategy_solution {

   struct Circle;
   struct Square;

   struct TranslateStrategy
   {
      virtual ~TranslateStrategy() {}

      virtual void translate( Circle& circle, const Vector3D& v ) const = 0;
      virtual void translate( Square& square, const Vector3D& v ) const = 0;
   };


   struct Shape
   {
      Shape() = default;

      virtual ~Shape() {}

      virtual void translate( const Vector3D& v ) = 0;
   };


   struct Circle : public Shape
   {
      Circle( double r, std::unique_ptr<TranslateStrategy>&& ts )
         : radius( r )
         , strategy( std::move(ts) )
      {}

      ~Circle() {}

      void translate( const Vector3D& v ) override { strategy->translate( *this, v ); }

      double radius;
      Vector3D center{};
      std::unique_ptr<TranslateStrategy> strategy;
   };


   struct Square : public Shape
   {
      Square( double s, std::unique_ptr<TranslateStrategy>&& ts )
         : side( s )
         , strategy( std::move(ts) )
      {}

      ~Square() {}

      void translate( const Vector3D& v ) override { strategy->translate( *this, v ); }

      double side;
      Vector3D center{};
      std::unique_ptr<TranslateStrategy> strategy;
   };


   struct ConcreteTranslateStrategy : public TranslateStrategy
   {
      virtual ~ConcreteTranslateStrategy() {}

      void translate( Circle& circle, const Vector3D& v ) const override
      {
         circle.center = circle.center + v;
      }

      void translate( Square& square, const Vector3D& v ) const override
      {
         square.center = square.center + v;
      }
   };

   using Shapes = std::vector< std::unique_ptr<Shape> >;

   void translate( Shapes& shapes, const Vector3D& v )
   {
      for( auto& shape : shapes )
      {
         shape->translate( v );
      }
   }

} // namespace strategy_solution
#endif


#if BENCHMARK_STD_FUNCTION_SOLUTION
namespace std_function_solution {

   struct Shape
   {
      Shape() = default;

      virtual ~Shape() {}

      virtual void translate( const Vector3D& v ) = 0;
   };


   struct Circle : public Shape
   {
      using TranslateStrategy = std::function<void(Circle&, const Vector3D&)>;

      Circle( double r, TranslateStrategy ts )
         : radius( r )
         , strategy( std::move(ts) )
      {}

      ~Circle() {}

      void translate( const Vector3D& v ) override { strategy( *this, v ); }

      double radius;
      Vector3D center;
      TranslateStrategy strategy;
   };

   void translate( Circle& circle, const Vector3D& v )
   {
      circle.center = circle.center + v;
   }


   struct Square : public Shape
   {
      using TranslateStrategy = std::function<void(Square&, const Vector3D&)>;

      Square( double s, TranslateStrategy ts )
         : side( s )
         , strategy( std::move(ts) )
      {}

      ~Square() {}

      void translate( const Vector3D& v ) override { strategy( *this, v ); }

      double side;
      Vector3D center;
      TranslateStrategy strategy;
   };

   void translate( Square& square, const Vector3D& v )
   {
      square.center = square.center + v;
   }


   struct Translate {
      template< typename T >
      void operator()( T& t, const Vector3D& v )
      {
         translate( t, v );
      }
   };


   using Shapes = std::vector< std::unique_ptr<Shape> >;

   void translate( Shapes& shapes, const Vector3D& v )
   {
      for( auto& shape : shapes )
      {
         shape->translate( v );
      }
   }

} // namespace std_function_solution
#endif


#if BENCHMARK_BOOST_FUNCTION_SOLUTION
namespace boost_function_solution {

   struct Shape
   {
      Shape() = default;

      virtual ~Shape() {}

      virtual void translate( const Vector3D& v ) = 0;
   };


   struct Circle : public Shape
   {
      using TranslateStrategy = boost::function<void(Circle&, const Vector3D&)>;

      Circle( double r, TranslateStrategy ts )
         : radius( r )
         , strategy( std::move(ts) )
      {}

      ~Circle() {}

      void translate( const Vector3D& v ) override { strategy( *this, v ); }

      double radius;
      Vector3D center;
      TranslateStrategy strategy;
   };

   void translate( Circle& circle, const Vector3D& v )
   {
      circle.center = circle.center + v;
   }


   struct Square : public Shape
   {
      using TranslateStrategy = boost::function<void(Square&, const Vector3D&)>;

      Square( double s, TranslateStrategy ts )
         : side( s )
         , strategy( std::move(ts) )
      {}

      ~Square() {}

      void translate( const Vector3D& v ) override { strategy( *this, v ); }

      double side;
      Vector3D center;
      TranslateStrategy strategy;
   };

   void translate( Square& square, const Vector3D& v )
   {
      square.center = square.center + v;
   }


   struct Translate {
      template< typename T >
      void operator()( T& t, const Vector3D& v )
      {
         translate( t, v );
      }
   };


   using Shapes = std::vector< std::unique_ptr<Shape> >;

   void translate( Shapes& shapes, const Vector3D& v )
   {
      for( auto& shape : shapes )
      {
         shape->translate( v );
      }
   }

} // namespace boost_function_solution
#endif


#if BENCHMARK_MANUAL_FUNCTION_SOLUTION
namespace manual_function_solution {

   template< typename Fn, size_t N >
   class Function;

   template< typename R, typename... Args, size_t N >
   class Function<R(Args...),N>
   {
    public:
      template< typename Fn >
      Function( Fn fn )
         : pimpl_{ reinterpret_cast<Concept*>( buffer ) }
      {
         static_assert( sizeof(Fn) <= N, "Given type is too large" );
         new (pimpl_) Model<Fn>( fn );
      }

      Function( Function const& f )
         : pimpl_{ reinterpret_cast<Concept*>( buffer ) }
      {
         f.pimpl_->clone( pimpl_ );
      }

      Function& operator=( Function f )
      {
         pimpl_->~Concept();
         f.pimpl_->clone( pimpl_ );
         return *this;
      }

      ~Function() { pimpl_->~Concept(); }

      R operator()( Args... args ) { return (*pimpl_)( std::forward<Args>( args )... ); }

    private:
      class Concept
      {
       public:
         virtual ~Concept() = default;
         virtual R operator()( Args... ) const = 0;
         virtual void clone( Concept* memory ) const = 0;
      };

      template< typename Fn >
      class Model : public Concept
      {
       public:
         explicit Model( Fn fn )
            : fn_( fn )
         {}

         R operator()( Args... args ) const override { return fn_( std::forward<Args>( args )... ); }
         void clone( Concept* memory ) const override { new (memory) Model( fn_ ); }

       private:
         Fn fn_;
      };

      Concept* pimpl_;

      char buffer[N+8UL];
   };


   struct Shape
   {
      Shape() = default;

      virtual ~Shape() {}

      virtual void translate( const Vector3D& v ) = 0;
   };


   struct Circle : public Shape
   {
      using TranslateStrategy = Function<void(Circle&, const Vector3D&),8UL>;

      Circle( double r, TranslateStrategy ts )
         : radius{ r }
         , strategy{ std::move(ts) }
      {}

      ~Circle() {}

      void translate( const Vector3D& v ) override { strategy( *this, v ); }

      double radius;
      Vector3D center;
      TranslateStrategy strategy;
   };

   void translate( Circle& circle, const Vector3D& v )
   {
      circle.center = circle.center + v;
   }


   struct Square : public Shape
   {
      using TranslateStrategy = Function<void(Square&, const Vector3D&),8UL>;

      Square( double s, TranslateStrategy ts )
         : side{ s }
         , strategy{ std::move(ts) }
      {}

      ~Square() {}

      void translate( const Vector3D& v ) override { strategy( *this, v ); }

      double side;
      Vector3D center;
      TranslateStrategy strategy;
   };

   void translate( Square& square, const Vector3D& v )
   {
      square.center = square.center + v;
   }


   struct Translate {
      template< typename T >
      void operator()( T& t, const Vector3D& v ) const
      {
         translate( t, v );
      }
   };


   using Shapes = std::vector< std::unique_ptr<Shape> >;

   void translate( Shapes& shapes, const Vector3D& v )
   {
      for( auto& shape : shapes )
      {
         shape->translate( v );
      }
   }

} // namespace manual_function_solution
#endif


#if BENCHMARK_TYPE_ERASURE_SOLUTION
namespace type_erasure_solution {

   struct Circle
   {
      double radius{};
      Vector3D center{};
   };

   void translate( Circle& circle, const Vector3D& v )
   {
      circle.center = circle.center + v;
   }


   struct Square
   {
      double side{};
      Vector3D center{};
   };

   void translate( Square& square, const Vector3D& v )
   {
      square.center = square.center + v;
   }


   class Shape
   {
    public:
      template< typename T >
      Shape( const T& shape )
         : pimpl_{ new Model<T>( shape ) }
      {}

      Shape( const Shape& s ) : pimpl_{ s.pimpl_->clone() } {}
      Shape( Shape&& ) = default;
      ~Shape() = default;
      Shape& operator=( const Shape& s ) { Shape tmp( s ); std::swap( pimpl_, tmp.pimpl_ ); return *this; }
      Shape& operator=( Shape&& ) = default;

    private:
      struct Concept
      {
         virtual ~Concept() = default;
         virtual Concept* clone() const = 0;
         virtual void doTranslate( const Vector3D& ) = 0;
      };

      template< typename T >
      struct Model : public Concept
      {
         explicit Model( const T& shape ) : shape_( shape ) {}
         Concept* clone() const override { return new Model( *this ); }
         void doTranslate( const Vector3D& v ) override { translate( shape_, v ); }
         T shape_;
      };

      friend void translate( Shape& s, const Vector3D& v )
      {
         s.pimpl_->doTranslate( v );
      }

      std::unique_ptr<Concept> pimpl_;
   };

   using Shapes = std::vector<Shape>;

   void translate( Shapes& shapes, const Vector3D& v )
   {
      for( auto& shape : shapes )
      {
         translate( shape, v );
      }
   }

} // namespace type_erasure_solution
#endif


#if BENCHMARK_TYPE_ERASURE_SBO_SOLUTION
namespace type_erasure_sbo_solution {

   struct Circle
   {
      double radius{};
      Vector3D center{};
   };

   void translate( Circle& circle, const Vector3D& v )
   {
      circle.center = circle.center + v;
   }


   struct Square
   {
      double side{};
      Vector3D center{};
   };

   void translate( Square& square, const Vector3D& v )
   {
      square.center = square.center + v;
   }


   template< size_t Length, size_t Alignment >
   class Shape
   {
    private:
      struct Concept
      {
         virtual ~Concept() = default;
         virtual void clone( Concept* ) const = 0;
         virtual void doTranslate( const Vector3D& ) = 0;
      };

      template< typename T >
      struct Model : public Concept
      {
         explicit Model( const T& shape ) : shape_( shape ) {}
         void clone( Concept* ptr ) const override { new (ptr) Model( *this ); }
         void doTranslate( const Vector3D& v ) override { translate( shape_, v ); }
         T shape_;
      };

    public:
      template< typename T >
      Shape( const T& shape )
         // data_ intentionally not initialized
      {
         new ( pimpl() ) Model<T>( shape );
      }

      Shape( const Shape& s ) { s.pimpl()->clone( pimpl() ); }
      ~Shape() { pimpl()->~Concept(); }
      Shape& operator=( const Shape& s ) { pimpl()->~Concept(); s.pimpl()->clone( pimpl() ); return *this; }

    private:
      Concept* pimpl()
      {
         return reinterpret_cast<Concept*>( &data_ );
      };

      const Concept* pimpl() const
      {
         return reinterpret_cast<const Concept*>( &data_ );
      };

      friend void translate( Shape& s, const Vector3D& v )
      {
         s.pimpl()->doTranslate( v );
      }

      std::aligned_storage_t<Length+8UL,Alignment> data_;
   };

   using Shapes = std::vector<Shape<sizeof(Circle),16UL>>;

   void translate( Shapes& shapes, const Vector3D& v )
   {
      for( auto& shape : shapes )
      {
         translate( shape, v );
      }
   }

} // namespace type_erasure_sbo_solution
#endif


#if BENCHMARK_TYPE_ERASURE_DYNO_SOLUTION

struct ShapeConcept : decltype( dyno::requires_(
   dyno::CopyConstructible{},
   dyno::MoveConstructible{},
   dyno::Destructible{},
   "translate"_s = dyno::function<void (dyno::T&, const Vector3D&)>
)) { };

template <typename T>
auto const dyno::default_concept_map<ShapeConcept, T> = dyno::make_concept_map(
   "translate"_s = []( T& self, const Vector3D& v ) { translate( self, v ); }
);

namespace type_erasure_dyno_solution {

   struct Circle
   {
      double radius;
      Vector3D center{};
   };

   void translate( Circle& circle, const Vector3D& v )
   {
      circle.center = circle.center + v;
   }


   struct Square
   {
      double side;
      Vector3D center{};
   };

   void translate( Square& square, const Vector3D& v )
   {
      square.center = square.center + v;
   }

   class Shape
   {
    public:
      template <typename T>
      Shape( T x ) : poly_{x} { }

      void translate( const Vector3D& v )
      { poly_.virtual_( "translate"_s )( poly_, v ); }

    private:
      dyno::poly<ShapeConcept> poly_;
   };

   void translate( Shape& shape, const Vector3D& v )
   {
      shape.translate( v );
   }


   using Shapes = std::vector<Shape>;

   void translate( Shapes& shapes, const Vector3D& v )
   {
      for( auto& shape : shapes )
      {
         translate( shape, v );
      }
   }

} // namespace type_erasure_dyno_solution
#endif




int main()
{
   const size_t N    ( 100UL );
   const size_t steps( 2500000UL );

   std::random_device rd{};
   const unsigned int seed( rd() );

   std::mt19937 rng{};
   std::uniform_real_distribution<double> dist( 0.0, 1.0 );

   std::cout << std::endl;

#if BENCHMARK_OO_SOLUTION
   {
      using namespace object_oriented_solution;

      rng.seed( seed );

      Shapes shapes;

      for( size_t i=0UL; i<N; ++i ) {
         if( dist( rng ) < 0.5 )
            shapes.push_back( std::make_unique<Circle>( dist( rng ) ) );
         else
            shapes.push_back( std::make_unique<Square>( dist( rng ) ) );
      }

      std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
      start = std::chrono::high_resolution_clock::now();

      for( size_t s=0UL; s<steps; ++s ) {
         translate( shapes, Vector3D{ dist( rng ), dist( rng ) } );
      }

      end = std::chrono::high_resolution_clock::now();
      const std::chrono::duration<double> elapsedTime( end - start );
      const double seconds( elapsedTime.count() );

      std::cout << " OO solution runtime               : " << seconds << "s\n";
   }
#endif

#if BENCHMARK_STRATEGY_SOLUTION
   {
      using namespace strategy_solution;

      rng.seed( seed );

      Shapes shapes;

      for( size_t i=0UL; i<N; ++i ) {
         if( dist( rng ) < 0.5 )
            shapes.push_back( std::make_unique<Circle>( dist( rng )
                                                      , std::make_unique<ConcreteTranslateStrategy>() ) );
         else
            shapes.push_back( std::make_unique<Square>( dist( rng )
                                                      , std::make_unique<ConcreteTranslateStrategy>() ) );
      }

      std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
      start = std::chrono::high_resolution_clock::now();

      for( size_t s=0UL; s<steps; ++s ) {
         translate( shapes, Vector3D{ dist( rng ), dist( rng ) } );
      }

      end = std::chrono::high_resolution_clock::now();
      const std::chrono::duration<double> elapsedTime( end - start );
      const double seconds( elapsedTime.count() );

      std::cout << " Classic strategy solution runtime : " << seconds << "s\n";
   }
#endif

#if BENCHMARK_STD_FUNCTION_SOLUTION
   {
      using namespace std_function_solution;

      rng.seed( seed );

      Shapes shapes;

      for( size_t i=0UL; i<N; ++i ) {
         if( dist( rng ) < 0.5 )
            shapes.push_back( std::make_unique<Circle>( dist( rng ), Translate{} ) );
         else
            shapes.push_back( std::make_unique<Square>( dist( rng ), Translate{} ) );
      }

      std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
      start = std::chrono::high_resolution_clock::now();

      for( size_t s=0UL; s<steps; ++s ) {
         translate( shapes, Vector3D{ dist( rng ), dist( rng ) } );
      }

      end = std::chrono::high_resolution_clock::now();
      const std::chrono::duration<double> elapsedTime( end - start );
      const double seconds( elapsedTime.count() );

      std::cout << " std::function solution runtime    : " << seconds << "s\n";
   }
#endif

#if BENCHMARK_BOOST_FUNCTION_SOLUTION
   {
      using namespace boost_function_solution;

      rng.seed( seed );

      Shapes shapes;

      for( size_t i=0UL; i<N; ++i ) {
         if( dist( rng ) < 0.5 )
            shapes.push_back( std::make_unique<Circle>( dist( rng ), Translate{} ) );
         else
            shapes.push_back( std::make_unique<Square>( dist( rng ), Translate{} ) );
      }

      std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
      start = std::chrono::high_resolution_clock::now();

      for( size_t s=0UL; s<steps; ++s ) {
         translate( shapes, Vector3D{ dist( rng ), dist( rng ) } );
      }

      end = std::chrono::high_resolution_clock::now();
      const std::chrono::duration<double> elapsedTime( end - start );
      const double seconds( elapsedTime.count() );

      std::cout << " boost::function solution runtime  : " << seconds << "s\n";
   }
#endif

#if BENCHMARK_MANUAL_FUNCTION_SOLUTION
   {
      using namespace manual_function_solution;

      rng.seed( seed );

      Shapes shapes;

      for( size_t i=0UL; i<N; ++i ) {
         if( dist( rng ) < 0.5 )
            shapes.push_back( std::make_unique<Circle>( dist( rng ), Translate{} ) );
         else
            shapes.push_back( std::make_unique<Square>( dist( rng ), Translate{} ) );
      }

      std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
      start = std::chrono::high_resolution_clock::now();

      for( size_t s=0UL; s<steps; ++s ) {
         translate( shapes, Vector3D{ dist( rng ), dist( rng ) } );
      }

      end = std::chrono::high_resolution_clock::now();
      const std::chrono::duration<double> elapsedTime( end - start );
      const double seconds( elapsedTime.count() );

      std::cout << " Manual function solution runtime  : " << seconds << "s\n";
   }
#endif

#if BENCHMARK_TYPE_ERASURE_SOLUTION
   {
      using namespace type_erasure_solution;

      rng.seed( seed );

      Shapes shapes;

      for( size_t i=0UL; i<N; ++i ) {
         if( dist( rng ) < 0.5 )
            shapes.push_back( Circle{ dist( rng ) } );
         else
            shapes.push_back( Square{ dist( rng ) } );
      }

      std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
      start = std::chrono::high_resolution_clock::now();

      for( size_t s=0UL; s<steps; ++s ) {
         translate( shapes, Vector3D{ dist( rng ), dist( rng ) } );
      }

      end = std::chrono::high_resolution_clock::now();
      const std::chrono::duration<double> elapsedTime( end - start );
      const double seconds( elapsedTime.count() );

      std::cout << " Type erasure solution runtime     : " << seconds << "s\n";
   }
#endif

#if BENCHMARK_TYPE_ERASURE_SBO_SOLUTION
   {
      using namespace type_erasure_sbo_solution;

      rng.seed( seed );

      Shapes shapes;

      for( size_t i=0UL; i<N; ++i ) {
         if( dist( rng ) < 0.5 )
            shapes.push_back( Circle{ dist( rng ) } );
         else
            shapes.push_back( Square{ dist( rng ) } );
      }

      std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
      start = std::chrono::high_resolution_clock::now();

      for( size_t s=0UL; s<steps; ++s ) {
         translate( shapes, Vector3D{ dist( rng ), dist( rng ) } );
      }

      end = std::chrono::high_resolution_clock::now();
      const std::chrono::duration<double> elapsedTime( end - start );
      const double seconds( elapsedTime.count() );

      std::cout << " Type erasure SBO solution runtime : " << seconds << "s\n";
   }
#endif

#if BENCHMARK_TYPE_ERASURE_DYNO_SOLUTION
   {
      using namespace type_erasure_dyno_solution;

      rng.seed( seed );

      Shapes shapes;

      for( size_t i=0UL; i<N; ++i ) {
         if( dist( rng ) < 0.5 )
            shapes.push_back( Circle{ dist( rng ) } );
         else
            shapes.push_back( Square{ dist( rng ) } );
      }

      std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
      start = std::chrono::high_resolution_clock::now();

      for( size_t s=0UL; s<steps; ++s ) {
         translate( shapes, Vector3D{ dist( rng ), dist( rng ) } );
      }

      end = std::chrono::high_resolution_clock::now();
      const std::chrono::duration<double> elapsedTime( end - start );
      const double seconds( elapsedTime.count() );

      std::cout << " Type erasure dyno solution runtime: " << seconds << "s\n";
   }
#endif

   std::cout << std::endl;

   return EXIT_SUCCESS;
}

