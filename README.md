# unit_test
Dead Simple Unit Testing for C++, 1 header, 2 Macro's

I tried to get GTest working nicely but that did not work like I wanted. <br/>
After that I tried CxxTest and Boost.Test but they where to bloated. <br/>
As a final attempt I tried Catch2 witch was sort of nice.

So The final result was that I implemented my own unit testing. <br/>
This is a dead simple implementation of the auto registering factory pattern. <br/>
http://derydoca.com/2019/03/c-tutorial-auto-registering-factory/#

                       ___HOW_IT_WORKS___
 1. Every TEST macro gets converted to a class.
 2. Then a global variable of the same type as this class is created,
 3. This then directly calls the constructor before the main gets called
 4. The constructor registers itself at this header only library
 5. Add the macro RUN_ALL_TEST() to your main and all tests will be fired.