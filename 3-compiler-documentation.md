Documentation
=============

In total the documentation burden is (at most) 1000 words
plus one diagram. Assesment of the documentation is not relative
to compiler functionality, it just requires a description
of the compiler as-is, and a realistic assessment of the
compiler's strengths and weaknesses.


AST
===

Overview Diagram
----------------

_Add a diagram of your AST, which is designed to *usefully* communicate
the *important* properties of the AST._

![my-ast.png](my-ast.png)


Description
-----------

_Describe the structure and organisation of your AST in 200 words
or fewer_.

I used a pure abstract Node class as an entry point to the AST.
I then had a TranslationUnit class which contained the external declarations and function
definitions in a vector. All other classes used linked lists instead of vectors to store
multiple elements, for example, the statement class had a pointer to a next statement instead
of having a seperate StatementList class. This meant that I did not have to write an extra List
class and made it easier to inherit from the right classes. I used the grammar to seperated the
classes properly, as an Expression is completely different to a Type or a Statement or a Function.
This meant that I could seperate the member functions and not have to declare all of them in the
Node class, as that would lead to a lot of empty definitions. These classes were mostly abstract too
but contained a few function definitions that would throw exceptions so that I did not have to
define these functions when I did not need them. I also had a few specific member functions
(eg. for UnaryExpression) for which I had to use dynamic and static pointer casting to access
them.


Strengths
---------

_Give two strengths or capabilites of your AST, using 50 words or less for each one_.

### Strength 1

The class hierarchy and structure is very logical as it closely matches the grammar.
This also that it was easy to know what member variables a class needed and the
inheritance was very logical too. All the member functions are well seperated
and only appear where they are actually used.

### Strength 2



Limitations
-----------

_Give two limitations of your AST, using 50 words or less for each one_.

### Limitation 1

The Type class is not very useful as it does not capture arrays correctly and store
all their information when using a multidimensional array for example. It also does not
enable me to extract this information correctly as it will only give me the primitive type of
the array.

### Limitation 2

_50 words or less_


Variable binding
================

General approach
----------------

_Describe your overall approach to mapping variable, parameters, etc.
into registers or memory locations at exection time, using 200 words
or less_.

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
relateive to the frame pointer. The Bindings class is passed by value to account for scopes and
variable shadowing. 

Strengths
---------

_Give two strengths or capabilites of your binding approach, using 50 words or less for each one_.

### Strength 1

_50 words or less_

### Strength 2

_50 words or less_

Limitations
-----------

_Give two limitations of your binding approach, using 50 words or less for each one_.

### Limitation 1

_50 words or less_

### Limitation 2

_50 words or less_


Reflection
==========

Strengths
---------

_What two aspects of your compiler do you think work well (beyond
those identified in the AST and binding parts)?_

### Strength 1

The Type class was not structured well for handling arrays, however, it does work well
with the other types and makes it easy to store and load variables using the
right operations. It also made it easy to identify pointers for pointer arithmatic and store
values in arrays without padding.

### Strength 2

_50 words or fewer_

Scope for Improvment
---------------------

_What parts of your compiler do you think could be improved?_

- _This is not looking for things like "It should support more C constructs". What
  would you need to do or change in order to support those things?_

### Improvement 1

_50 words or fewer_

### Improvement 2

_50 words or fewer_


Functionality (not assessed)
============================

Which of these features does your compiler support (insert
an `x` to check a box):

1 - [x] Local variables
2 - [x] Integer arithmetic
3 - [x] While
4 - [x] IfElse
5 - [x] For
6 - [x] Function calls
7 - [x] Arrays
8 - [x] Pointers
9 - [x] Strings
10 - [ ] Structures
11 - [ ] Floating-point

Note that all features will be tested, regardless of what
appears here. This is documentation of what you expect to work,
versus what doesn't work.


Feedback (not assessed)
=======================

_What aspects of your compiler would you like feedback on.
Be specific, as "what could I have done differently" is
too general to answer._

### Feedback 1

Using multidimensional arrays should work, however, for some reason I cannot get the address of
and element inside it

### Feedback 2

_20 words or fewer_


