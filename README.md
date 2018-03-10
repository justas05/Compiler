Documentation
=============

In total the documentation burden is (at most) 1000 words
plus one diagram. Assessment of the documentation is not relative
to compiler functionality, it just requires a description
of the compiler as-is, and a realistic assessment of the
compiler's strengths and weaknesses.


AST
===

Overview Diagram
----------------

Overview of the ast that is built by the compiler.

![my-ast.png](/res/my-ast.png)


Description
-----------

### Description of the structure

I used a pure abstract Node class as an entry point to the AST.
I then had a TranslationUnit class which contained the external declarations and function
definitions in a vector. All other classes used linked lists instead of vectors to store
multiple elements, for example, the statement class had a pointer to a next statement instead
of having a separate StatementList class. This meant that I did not have to write an extra List
class and made it easier to inherit from the right classes. I used the grammar to separated the
classes properly, as an Expression is completely different to a Type or a Statement or a Function.
This meant that I could separate the member functions and not have to declare all of them in the
Node class, as that would lead to a lot of empty definitions. These classes were mostly abstract too
but contained a few function definitions that would throw exceptions so that I did not have to
define these functions when I did not need them. I also had a few specific member functions
(eg. for UnaryExpression) for which I had to use dynamic and static pointer casting to access
them.


Strengths
---------

The class hierarchy and structure is very logical as it closely matches the grammar.
This also that it was easy to know what member variables a class needed and the
inheritance was very logical too. All the member functions are well separated
and only appear where they are actually used.

All the general base classes, that are mostly abstract as well, are in the bison union,
which means that I can use and assign all of those classes directly, and be more
specific in the member variables of those classes so that they only contain the types I need.

Limitations 
-----------

The Type class is not very useful as it does not capture arrays correctly and store
all their information when using a multidimensional array for example. It also does not
enable me to extract this information correctly as it will only give me the primitive type of
the array.

As I did not want all the classes to contain functions that they do not need, classes like
UnaryOperator have member functions. To access these I have to use dynamic casts and with my
linked lists, I always have to check for nullptr before doing anything with it.


Variable binding
================

General approach
----------------

I did not use many registers because they are a limited resource, and instead I decided
only to use registers $2 and $3 for performing operations and rarely use registers $t0 and $t1
when loading the address of a value, for example for pointers. I also used registers $4, $5, $6, $7
for passing values to functions. I used a frame pointer for the current function to access the
stack. This frame is split into multiple parts, first, enough space to store and pass values
when making function calls, then space for all declared variables in the function, and lastly enough
space to store temporary results from expressions. It also stores the previous frame pointer and the
return address at the end of the frame. Every time I perform any operations, I store
the result of that operation in that temporary space in the frame. I keep track of the stack
position and temporary expression stack position using the Bindings class. This class also includes
a map of variables bindings, which binds the identifier to its type and stack position
relative to the frame pointer. The Bindings class is passed by value to account for scopes and
variable shadowing. 

Strengths
---------

The Bindings class stores the type of the identifier so that I can look
it up and perform the right operation in the Expression class. Storing the type, however, also
means that I do not have to store the type of an Expression, but can just deduce it.

By only using two registers for operations, I do not have to worry about having no more
registers available, and which registers will not be overwritten after a function call.
The temporary expression result will always be in the current frame of the function even after
a function call.

Limitations
-----------

As I am only using two registers to perform operations, I have to include loads and
stores to access the temporary results of the operations. I also store results of an operation
when I do not need the result anymore. This means that the code will run much slower.

When counting the variables that are being assigned in the current function, I assume that
they are all integers. I also always store the return value $31 even when there is no function
call. This means that the frame is always much larger than it needs to be.


Reflection
==========

Strengths
---------

The Type class was not structured well for handling arrays, however, it does work well
with the other types and makes it easy to store and load variables using the
right operations. It also made it easy to identify pointers for pointer arithmatic and store
values in arrays without padding.

Function calls work well as they follow the MIPS ABI, and leave enough space for the called
function to store the parameters in the previous frame. The return address is also always stored,
which make recursive calls possible. Function declarations also work because they do not have
to emit assembly.

Scope for Improvement
---------------------

I would like to work on multidimensional arrays as they do not fully work. To do
that I would have to add information such as the initial array in the type so that I can assign
arrays inside the array to a pointer and perform the right pointer arithmetic.

I would also like to reduce the number of dynamic and static casts by adding more
intermediate classes. This would also make the whole AST design more extensible and it would
be easier to possibly add structures in the future. This would also make classes more specialized
and maintainable.


Functionality
=============

Which of these features does your compiler support (insert
an `x` to check a box):

- [x] Local variables
- [x] Integer arithmetic
- [x] While
- [x] IfElse
- [x] For
- [x] Function calls
- [x] Arrays
- [x] Pointers
- [x] Strings
- [ ] Structures
- [ ] Floating-point

Note that all features will be tested, regardless of what
appears here. This is documentation of what you expect to work,
versus what doesn't work.
