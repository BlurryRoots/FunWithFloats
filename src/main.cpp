#include <iostream>
#include "FloatingPoint.hpp"

void printFloat(
    std::ostream& out,
    const char* name,
    FloatingPoint& f )
{
    out << name << ":" << std::endl;
    out << f.ToString() << std::endl
        << f.ToString(true) << std::endl
        << f.GetFlags() << std::endl
        << "MAX: +-" << f.GetMaximum() << " && "
        << "MIN: +-" << f.GetMinimum() << std::endl;
    out << std::endl;
}

void tryPrintFloat(
    std::ostream& out,
    const char* name,
    FloatingPoint& f )
{
    try
    {
        printFloat( out, name, f );
    }
    catch( std::exception* ex )
    {
        std::cout << "Exception: " << ex->what() << std::endl << std::endl;
    }
}

int main()
{
    FloatingPoint
        eightBits( 3, 4 ),
        sixteenBits( 5, 10 ),
        thirtyTwoBits( 8, 23 ),
        sixtyFourBits( 11, 52 ),
        f,
        g,
        keks;

    eightBits = 7.7;

    sixteenBits = 1337.7331;

    f = -3.1415;

    g.SetValue( "11111111111111111111111111111111" );

    tryPrintFloat( std::cout, "eightBits", eightBits );

    tryPrintFloat( std::cout, "sixteenBits", sixteenBits );

    tryPrintFloat( std::cout, "thirtyTwoBits", thirtyTwoBits );

    tryPrintFloat( std::cout, "sixtyFourBits", sixtyFourBits );

    tryPrintFloat( std::cout, "f", f );

    tryPrintFloat( std::cout, "g", g );

    g.SetSignBit( true );
    g.SetMantissa( 1.0 );

    tryPrintFloat( std::cout, "g", g );

    return 0;
}
