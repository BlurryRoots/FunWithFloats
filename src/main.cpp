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
        << f.GetFlags() << std::endl;
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
        zero,
        max,
        min,
        f,
        g,
        keks;

    zero.SetValue( 0 );

    max.SetSignBit( false );
    max.SetExponent( 127 );
    max.SetMantissa( 2.0 );

    min.SetSignBit( false );
    min.SetExponent( -126 );
    min.SetMantissa( 1.0 );

    f = -3.1415f;

    g.SetValue( "11111111111111111111111111111111" );

    keks = f;

    tryPrintFloat( std::cout, "zero", zero );

    tryPrintFloat( std::cout, "max", max );

    tryPrintFloat( std::cout, "min", min );

    tryPrintFloat( std::cout, "f", f );

    tryPrintFloat( std::cout, "g", g );

    tryPrintFloat( std::cout, "keks", keks );

    return 0;
}
