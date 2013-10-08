#ifndef _FLOATINGPOINT_
#define _FLOATINGPOINT_

#include <string>
#include <sstream>
#include <cstdlib>
#include <cfloat>
#include <functional>
#include <exception>
#include <stdexcept>

/*
 * Class simulating memory for a floating point number
 * */
class FloatingPoint
{
    private:

        const std::size_t exponentBits;
        const std::size_t mantissaBits;
        const int excess;

        long double maxValue;
        long double minValue;

        bool sign;
        bool* const exponent;
        bool* const mantissa;

        bool isNotANumber;
        bool isZero;
        bool isInfinite;

        unsigned int Power2( unsigned int someN ) const
        {
            return ( 1 << someN );
        }

        template<class T>
        T Crop( T someValue, T someMin, T someMax ) const
        {
            someValue =
                ( someValue < someMin ? someMin : someValue );
            someValue =
                ( someValue > someMax ? someMax : someValue );

            return someValue;
        }

        template<class T>
        void SetBits(
            bool* someBitFiled,
            std::size_t someSize,
            std::function<T(T)> powFunc,
            T someValue )
        {
            for( std::size_t i = 0; i < someSize; ++i )
            {
                T pow = powFunc( i );

                someBitFiled[i] = someValue >= pow;
                if( someBitFiled[i] )
                {
                    someValue -= pow;
                }
            }
        }

        void UpdateFlags()
        {
            int expo = this->GetExponent();
            float mantissa = this->GetMantissa();

            this->isZero =
                (expo == -127);
            this->isInfinite =
                (expo == 128);
            this->isNotANumber =
                (this->isZero || this->isInfinite)
                && mantissa != 1.0f;
        }

        void CheckMemory( const FloatingPoint& someOther )
        {
            if( someOther.exponentBits != this->exponentBits )
            {
                throw new std::range_error( 
                    "Expontent bits are different!" 
                );
            }
            else if( someOther.mantissaBits != this->mantissaBits )
            {
                throw new std::range_error(
                    "Mantissa bits are different!"
                );
            }
        }

        void Cleanup()
        {
            delete [] this->exponent;
            delete [] this->mantissa;            
        }

    public:

        FloatingPoint(
            std::size_t someExponentBits = 8,
            std::size_t someMantissaBits = 23 )
            : exponentBits( someExponentBits ),
              mantissaBits( someMantissaBits ),
              excess( (1 << (someExponentBits-1))-1 ),
              sign( true ),
              exponent( new bool[someExponentBits] ),
              mantissa( new bool[someMantissaBits] ),
              isNotANumber( false ),
              isZero( false ),
              isInfinite( false )
        {
            if( this->GetBitSize() > 64 )
            {
                this->Cleanup();

                throw new std::range_error(
                    "Maximum 64bits supported!"
                );
            }

            this->sign = false;

            //max value
            for( std::size_t i = 0; i < this->exponentBits; ++i )
            {
                this->exponent[i] = true;
            }
            this->exponent[this->exponentBits-1] = false;

            for( std::size_t i = 0; i < this->mantissaBits; ++i )
            {
                this->mantissa[i] = true;
            }

            this->maxValue = this->GetValue();

            //min value
            for( std::size_t i = 0; i < this->exponentBits; ++i )
            {
                this->exponent[i] = false;
            }
            this->exponent[this->exponentBits-1] = true;

            for( std::size_t i = 0; i < this->mantissaBits; ++i )
            {
                this->mantissa[i] = false;
            }

            this->minValue = this->GetValue();

            this->UpdateFlags();
        }

        ~FloatingPoint()
        {
            this->Cleanup();
        }

        long double GetMaximum()
        {
            return this->maxValue;
        }

        long double GetMinimum()
        {
            return this->minValue;
        }

        int GetBitSize()
        {
            return 
                1 + this->exponentBits + this->mantissaBits;
        }

        void operator=( const FloatingPoint& someOther )
        {
            try
            {
                this->CheckMemory( someOther );
            }
            catch( std::exception* ex )
            {
                throw ex;
            }

            this->sign = someOther.sign;

            for( std::size_t i = 0; i < this->exponentBits; ++i )
            {
                this->exponent[i] = someOther.exponent[i];
            }

            for( std::size_t i = 0; i < this->mantissaBits; ++i )
            {
                this->mantissa[i] = someOther.mantissa[i];
            }

            this->UpdateFlags();
        }

        void SetSignBit( bool someValue )
        {
            this->sign = someValue;
        }

        void SetExponentBit( std::size_t n, bool someValue )
        {
            this->exponent[n] = someValue;

            this->UpdateFlags();
        }

        void SetExponent( int someExponent )
        {
            int withExcess =
                this->excess +
                this->Crop<int>(
                    someExponent,
                    -1 * this->excess,
                    this->excess + 1
                );

            this->SetBits<int>(
                this->exponent,
                this->exponentBits,
                [=]( int i ) -> int
                {
                    return
                        this->Power2( (this->exponentBits-1) - i);
                },
                withExcess
            );

            this->UpdateFlags();
        }

        void SetMantissaBit( std::size_t n, bool someValue )
        {
            this->mantissa[n] = someValue;

            this->UpdateFlags();
        }

        void SetMantissa( float someValue )
        {
            float withOutLeadingOne =
                this->Crop<float>(
                    someValue,
                    1.0,
                    2.0
                ) - 1.0;

            this->SetBits<float>(
                this->mantissa,
                this->mantissaBits,
                [=]( float i ) -> float
                {
                    return
                        1.0 / (float)this->Power2( i+1 );
                },
                withOutLeadingOne
            );

            this->UpdateFlags();
        }

        void SetValue( const std::string& someBitValue )
        {
            if( 
                someBitValue.size()
                != (1 + this->exponentBits + this->mantissaBits)
            )
            {
                throw new std::range_error( "Number of bits are different!" );
            }

            this->sign = atoi( someBitValue.substr( 0, 1 ).c_str() ) == 1;

            for( std::size_t i = 0; i < this->exponentBits; ++i )
            {
                this->exponent[i] = atoi( 
                    someBitValue.substr( 1+i, 1 ).c_str() 
                ) == 1;
            }

            for( std::size_t i = 0; i < this->mantissaBits; ++i )
            {
                this->mantissa[i] = atoi( 
                    someBitValue.substr( 
                        1 + this->exponentBits+i,
                        1 
                    ).c_str() 
                ) == 1;
            }

            this->UpdateFlags();
        }

        void operator=( const std::string& someBitValue )
        {
            this->SetValue( someBitValue );
        }

        void SetValue( long double someValue )
        {
            if( someValue == 0 )
            {
                this->SetExponent( -1 * this->excess );
                this->SetMantissa( 1.0 );
            }
            else
            {
                //check sign
                this->sign = someValue < 0;
                if( this->sign )
                {
                    someValue *= -1;
                }

                int expoCounter = 0;

                //while value > 2
                //div 2 add 1 to expo
                while( someValue > 2 )
                {
                    ++expoCounter;
                    someValue /= 2.0;
                }

                //while value < 1
                //mul 2 sub 1 from expo
                while( someValue < 1 )
                {
                    --expoCounter;
                    someValue *= 2.0;
                }

                this->SetExponent( expoCounter );
                this->SetMantissa( someValue );
            }

            this->UpdateFlags();
        }

        void operator=( float someValue )
        {
            this->SetValue( someValue );
        }

        int GetSign() const
        {
            return ( this->sign ? -1 : 1 );
        }

        int GetExponent() const
        {
            int exp = 0;
            for( std::size_t i = 0; i < this->exponentBits; ++i )
            {
                exp += ( this->exponent[i] ?
                    this->Power2( (this->exponentBits-1) - i )
                    : 0
                );
            }
            exp -= this->excess;

            return exp;
        }

        int GetExcess() const
        {
            return this->excess;
        }

        float GetMantissa() const
        {
            float m = 1;
            for( std::size_t i = 0; i < this->mantissaBits-1; ++i )
            {
                m += ( this->mantissa[i] ?
                    1.0f / this->Power2( i+1 )
                    : 0
                );
            }

            return m;
        }

        long double GetValue() const
        {
            if( this->isNotANumber )
            {
                throw new std::range_error( "Is NAN!" );
            }
            else if( this->isInfinite )
            {
                std::stringstream ss;
                ss << "Is " << (this->sign ? "-" : "+") << "infinite!";
                throw new std::range_error( ss.str() );
            }

            if( this->isZero )
            {
                return 0;
            }
            else if( this->isInfinite )
            {
                //not really infinte isn't it?!
                return this->sign * FLT_MAX;
            }

            //mantissa
            long double m = this->GetMantissa();
            int exp = this->GetExponent();
            if( exp > 0 )
            {
                for( int i = 0; i < exp; ++i )
                {
                    m *= 2.0f;
                }
            }
            else if( exp < 0 )
            {
                for( int i = exp; i < 0; ++i )
                {
                    m /= 2.0f;
                }
            }

            return this->GetSign() * m;
        }

        std::string GetFlags() const
        {
            std::stringstream ss;

            ss << "Sign: " << this->sign
               <<  " Zero: " << this->isZero
               << " IsInfinite: " << this->isInfinite
               << " NAN: " << this->isNotANumber;

            return ss.str();
        }

        std::string ToString( bool inBits = false ) const
        {
            if( this->isNotANumber )
            {
                throw new std::range_error( "Is NAN!" );
            }

            //
            std::stringstream ss;
            if( inBits )
            {
                ss << "[" << this->sign << "]";

                ss << "[";
                for( std::size_t i = 0; i < this->exponentBits; ++i )
                {
                    ss << this->exponent[i];
                }
                ss << "]";

                ss << "[";
                for( std::size_t i = 0; i < this->mantissaBits; ++i )
                {
                     ss << this->mantissa[i];
                }
                ss << "]";
            }
            else
            {
                ss << this->GetValue();
            }

            //
            return ss.str();
        }
};

#endif
