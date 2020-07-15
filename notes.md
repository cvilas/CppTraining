# My notes from the course

## General
- No locking required for reading const data from multiple threads
- Value semantics is easier to work with compared to reference/pointer semantics. That's why STL string operations 
work with deep copies. You always know the state.
- prefer an unqualified call to std::swap() - ie. call swap() instead of std::swap(). Why? Read: http://ericniebler.com/2014/10/21/customization-point-design-in-c11-and-beyond/

- TODO: Review rule of 6 (CppCoreGuideline 21) - if you define one, you should define all.
- TODO: Review SOLID principles.
    - Single responsibility principle (SRP) is performing a cohesive set of things; it is *not* about doing just one thing.
- TODO: Understand decltype(auto) vs auto for return type
- TODO: Review how Type erasure works in the expression templates. This is the meat of the workshop
- TODO: Review how all design patterns are implemented in modern c++ in the given exercises

## Visitor Pattern

- A new way to implement Visitor pattern compared to traditional GoF implementation. See visitor_refactored.cpp
- std::variant and std::visit work well with each other, and without the need for inheritance hierarchy. Use free functions instead.
- Note how state machine is implemented using std::variant. Problem is that we cannot extend it to more states. Here's 
where type-erasure based approach comes in.

## Command Pattern

- Use std::function instead. Again, value semantics. It can be copied, moved.

## Strategy Pattern

- Again, use free functions instead of inheritance hierarchy of strategies. See strategy_refactored.cpp

## Prototype

- Command-like pattern with a clone() function to get a copy


## "Inheritance is the base class of evil"

- There is no base class in standard C++/STL. Why should I need one?
- Prefer free functions over inheritance hierarchy containing virtual functions
    - inheritance introduces tight coupling which is intrusive and difficult to extend
    - inheritance is about adding types, not operations
    - inheritance introduces overheads: heap allocation (to create derived classes operated upon with base class pointers), 
    virtual functions (cannot be inlined), have to deal with null pointers
    
## Type erasure

- used in new programming languages (rust, go) to achieve dynamic polymorphism
- CoreCppGuideline T.49 - Avoid type erasure where possible. Read this and understand why. Then learn how to use type erasure properly

- TODO

## Expression Templates

- Use this to implement high performance math library without creating temporaries
- We return proxies that are evaluated at the point where they are required, rather than returning evaluated values
