FunWithFloats
================================

What's this?
------------

This is not intented to be of any real practial use. I created the class `FloatingPoint` mainly to help me understand how computers deal with floating point numbers. 

If you happen to be interested in floating point numbers, you may find this little class quite useful.


Here's a little expample of a 32bit floating point number.
`FloatingPoint` is by default instantiated with 8bits exponent and a 23bit mantissa, plus 1bit for the sign.

 ```c++
    #include <iostream>
    #include "FloatingPoint.hpp"
    
    int main()
    {
        FloatingPoint f;
        f = 1.618f;
        
        std::cout << "Value: " << f.GetValue();
        std::cout << "Bits: " << f.GetValue( true );
        
        return 0;
    }
 ```
 
 See `main.cpp` for further examples.