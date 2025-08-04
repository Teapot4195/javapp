# Java++

Welcome to libbase, `java.base` but in c++! (you should not question how this came
to be)

As complete as possible java 19 Lang API, provided in C++. 

In order to emulate some early method-binding behavior, a lateinit function is
provided, if using the `alloc` templated function as intended. It will
automatically call the  most derived class' lateinit function first.

If you derive the lateinit function, **you are expected to call the parent after
you are done.** If you need to pass arguments between the ctor and lateinit, there
is a global lateinit stack provided as `_G_stack`, make your own frame on there if
you use it.

The lateinit function can also be used when `shared_from_this()` or `weak_from_this()`
must absolutely be used, as lateinit has a valid ptr to self now.

## TODOs:

java.util.function interfaces should be extracted elsewhere somehow

in String (observed in Enum registration), String's overridden equals hides the
templated variant that accepts shared_ptrs from object, investigate how to fix,
preferably in a user friendly way, `.get()` is kind of ugly.
- It has been "fixed" if you override the method you can use the
`DEFINE_SHARED_EQUALS` macro to redefine the templated shared\<\> equals overload.

Most of the class implementations are very naive and less than optimal, 
