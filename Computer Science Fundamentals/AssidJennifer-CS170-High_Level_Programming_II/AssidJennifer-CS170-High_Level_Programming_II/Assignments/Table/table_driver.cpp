// I DO NOT OWN THE FOLLOWING CODE - IT IS BASED OFF OF CODE PROVIDED TO THE CLASS VIA A DRIVER FILE TO TEST FUNCTIONALITY
#include "table.h"
#include <iostream>

void table_test0()
{
        CS170::Table t(2,3);

        t(0,0) = "aaaa";
        t(0,1) = "bbbbbbbb";
        t(0,2) = "cccc";
        t(1,0) = "12";
        t(1,1) = "345678";
        t(1,2) = "90";

        t.Display( std::cout );
}

void table_test1()
{
        CS170::Table t(2,3);

        t(0,0) = "aaaa";
        t(0,1) = "bbbbbbbb";
        t(0,2) = "cccc";
        t(1,0) = "12";
        t(1,1) = "345678";
        t(1,2) = "90";

        t.Display2( std::cout );
}

void table_test2()
{
        CS170::Table t(2,3);

        t(0,0) = "aaaa";
        t(0,1) = "bbbbbbbb";
        t(0,2) = "cccc";
        t(1,0) = "12";
        t(1,1) = "345678";
        t(1,2) = "90";

        std::cout << t << std::endl;

        t.Reverse(1,1);
        std::cout << t << std::endl;

        t(0,2) = "fffffffff";
        std::cout << t << std::endl;
}

void table_test3()
{
        CS170::Table t(2,3);

        t(0,0) = "aaaa";
        t(0,1) = "bbbbbbbb";
        t(0,2) = "cccc";
        t(1,0) = "12";
        t(1,1) = "345678";
        t(1,2) = "90";

        std::cout << t << std::endl;

        t.Reverse(1);

        std::cout << t << std::endl;
}

// copy ctor - use memory debugger to check for double delete and or leaks
void table_test4()
{
        CS170::Table t1(2,3);

        t1(0,0) = "aaaaaaa";
        t1(0,1) = "bbbbb";
        t1(0,2) = "cccccccc";
        t1(1,0) = "11";
        t1(1,1) = "22222222222";
        t1(1,2) = "3";

        CS170::Table t2( t1 );

        std::cout << t1 << std::endl;
        std::cout << t2 << std::endl;
}

// copy ctor - use memory debugger to check for double delete and or leaks
// create copy 
// modify original 
// display both - see if changes are in original only
// same, but modify copy
void table_test5()
{
        CS170::Table t1(2,3);

        t1(0,0) = "aaaaaaa";
        t1(0,1) = "bbbbb";
        t1(0,2) = "cccccccc";
        t1(1,0) = "11";
        t1(1,1) = "22222222222";
        t1(1,2) = "3";

        CS170::Table t2( t1 );

        t1(1,0) = "X";
        t1(1,1) = "YYYYYYYYYY";
        t1(1,2) = "ZZZZZ";

        std::cout << t1 << std::endl;
        std::cout << t2 << std::endl;

        t2(0,0) = "****************";
        t2(0,1) = "&&&&&&&&&";
        t2(0,2) = "->->->";

        std::cout << t1 << std::endl;
        std::cout << t2 << std::endl;
}


////////////////////////////////////////////////////////////////////////////////
// this function requires working copy ctor (returns by value)
CS170::Table makeTable( unsigned int num_rows, unsigned int num_cols, unsigned int offset )
{
    std::string as[] = {
        "Morbi", "et", "risus", "faucibus", "lorem", "pulvinar", "interdum",
        "eu", "at", "urna", "Maecenas", "laoreet", "tellus", "ut",
        "tincidunt", "eleifend", "Nullam", "sit", "amet", "ipsum", "eu",
        "arcu", "interdum", "elementum", "vel", "suscipit", "est", "Aliquam",
        "erat", "volutpat", "Nullam", "purus", "tellus", "malesuada", "non",
        "odio", "in", "semper", "accumsan", "turpis", "Proin", "sit", "amet",
        "diam", "ut", "lacus", "porta", "lacinia", "Etiam", "tincidunt",
        "nibh", "est", "a", "laoreet", "risus", "aliquet", "eu", "Nullam",
        "fermentum", "consequat", "est", "pellentesque", "gravida", "Nullam",
        "eu", "finibus", "dui", "ac", "luctus", "sapien", "Maecenas",
        "fringilla", "tincidunt", "libero", "ac", "vehicula", "Donec",
        "varius", "arcu", "quis", "auctor", "fringilla", "Phasellus", "sit",
        "amet", "ultricies", "orci", "non", "dapibus", "orci", "Phasellus",
        "massa", "odio", "euismod", "eu", "dapibus", "tempor", "eleifend",
        "non", "nisl", "Maecenas", "dictum", "et", "elit", "ut", "auctor",
        "Integer", "turpis", "nisi", "laoreet", "eget", "est", "et", "auctor",
        "tristique", "augue", "Etiam", "quis", "sapien", "rutrum", "ornare",
        "eu", "iaculis", "mauris"
    };
    unsigned int as_size = sizeof(as)/sizeof(as[0]);

    CS170::Table ret_val( num_rows, num_cols );
    for( unsigned int r=0; r<num_rows; ++r ) {
        for( unsigned int c=0; c<num_cols; ++c ) {
            ret_val( r,c ) = as[ (offset + r*num_cols + c) % as_size ];
        }
    }
    return ret_val;
}


// reverse (transpose) 4x4 table
void table_test6()
{
        CS170::Table t = makeTable( 4,4,22 );
        std::cout << t << std::endl;

        t.Reverse();
        std::cout << t << std::endl;

}

// reverse (transpose) 5x3 table
void table_test7()
{
        CS170::Table t = makeTable( 5,3,2 );
        std::cout << t << std::endl;

        t.Reverse();
        std::cout << t << std::endl;

}

// multiple reverses - elements, rows, tables
void table_test8()
{
        CS170::Table t = makeTable( 5,3,2 );

        for ( int i=0; i<5; ++i ) {
            for ( int j=0; j<3; ++j ) {
                t.Reverse( i,j );
            }
        }
        std::cout << t << std::endl;

        // reverse the whole table (transpose)
        t.Reverse();
        std::cout << t << std::endl;

        // reverse all rows
        t.Reverse(0);
        t.Reverse(1);
        t.Reverse(2);
        std::cout << t << std::endl;

        // reverse the whole table (transpose)
        t.Reverse();
        std::cout << t << std::endl;
}

// operator* (tensor product)
void table_test9()
{
    std::cout << "--------------------------\n";
    CS170::Table a(2,2);
    CS170::Table b(2,2);
    a(0,0) = "-"; a(0,1) = "+"; 
    a(1,0) = "~"; a(1,1) = "!"; 

    b(0,0) = "1"; b(0,1) = "2";
    b(1,0) = "3"; b(1,1) = "4";

    std::cout << a << std::endl;
    std::cout << b << std::endl;

    CS170::Table r1 = a*b; // this is copy ctor!
    std::cout << r1 << std::endl;

    CS170::Table r2 = b*a; // this is copy ctor!
    std::cout << r2 << std::endl;
}


// operator* (tensot product)
void table_test10()
{
    CS170::Table a(2,2);
    CS170::Table b(2,3);
    a(0,0) = "a"; a(0,1) = "b"; 
    a(1,0) = "c"; a(1,1) = "d"; 

    b(0,0) = "1"; b(0,1) = "2"; b(0,2) = "3";
    b(1,0) = "4"; b(1,1) = "5"; b(1,2) = "6";

    std::cout << a << std::endl;
    std::cout << b << std::endl;

    CS170::Table r1 = a*b;
    std::cout << r1 << std::endl;

    CS170::Table r2 = b*a;
    std::cout << r2 << std::endl;
}

// operator+ (outer product?) - same size
void table_test11()
{
    CS170::Table a(2,2);
    CS170::Table b(2,2);
    a(0,0) = "-"; a(0,1) = "+"; 
    a(1,0) = "~"; a(1,1) = "!"; 

    b(0,0) = "1"; b(0,1) = "2";
    b(1,0) = "3"; b(1,1) = "4";

    CS170::Table r1 = a+b; // this is copy ctor!
    std::cout << r1 << std::endl;

    CS170::Table r2 = b+a; // this is copy ctor!
    std::cout << r2 << std::endl;
}

// operator+ - one dim is the same
void table_test12()
{
    CS170::Table a(2,2);
    CS170::Table b(2,3);
    a(0,0) = "a"; a(0,1) = "b"; 
    a(1,0) = "c"; a(1,1) = "d"; 

    b(0,0) = "1"; b(0,1) = "2"; b(0,2) = "3";
    b(1,0) = "4"; b(1,1) = "5"; b(1,2) = "6";

    CS170::Table r = a+b; // this is copy ctor!
    std::cout << r << std::endl;
}

// operator+ - one dim is the same
void table_test13()
{
    CS170::Table a(2,2);
    CS170::Table b(3,2);
    a(0,0) = "a"; a(0,1) = "b"; 
    a(1,0) = "c"; a(1,1) = "d"; 

    b(0,0) = "1"; b(0,1) = "2";
    b(1,0) = "3"; b(1,1) = "4";
    b(2,0) = "5"; b(2,1) = "6";

    CS170::Table r = a+b; // this is copy ctor!
    std::cout << r << std::endl;
}

// operator+ - one dim is the same
void table_test14()
{
    CS170::Table a(2,2);
    CS170::Table b(3,2);
    a(0,0) = "a"; a(0,1) = "b"; 
    a(1,0) = "c"; a(1,1) = "d"; 

    b(0,0) = "1"; b(0,1) = "2";
    b(1,0) = "3"; b(1,1) = "4";
    b(2,0) = "5"; b(2,1) = "6";

    CS170::Table r = b+a; // this is copy ctor!
    std::cout << r << std::endl;
}

// operator+ - table_testing default constructed potition
void table_test15()
{
    CS170::Table a(1,4);
    CS170::Table b(6,1);
    a(0,0) = "a"; a(0,1) = "b"; a(0,2) = "c"; a(0,3) = "d"; 

    b(0,0) = "0";
    b(1,0) = "1";
    b(2,0) = "2";
    b(3,0) = "3";
    b(4,0) = "4";
    b(5,0) = "5";

    CS170::Table r1 = a+b; // this is copy ctor!
    std::cout << r1 << std::endl;

    CS170::Table r2 = b+a; // this is copy ctor!
    std::cout << r2 << std::endl;
}

// operator+ - one table is smaller in both dimensions
void table_test16()
{
    CS170::Table a(2,2);
    CS170::Table b(3,3);
    a(0,0) = "a"; a(0,1) = "b"; 
    a(1,0) = "c"; a(1,1) = "d"; 

    b(0,0) = "1"; b(0,1) = "2"; b(0,2) = "3";
    b(1,0) = "4"; b(1,1) = "5"; b(1,2) = "6";
    b(2,0) = "7"; b(2,1) = "8"; b(2,2) = "9";

    CS170::Table r1 = a+b; // this is copy ctor!
    std::cout << r1 << std::endl;

    CS170::Table r2 = b+a; // this is copy ctor!
    std::cout << r2 << std::endl;
}

// assignment operator (note - it was not used in any of the previous table_tests)

// assignment operator - use memory debugger to check for double delete and or leaks
void table_test17()
{
        CS170::Table t1(2,3);

        t1(0,0) = "aaaaaaa"; t1(0,1) = "bbbbb";       t1(0,2) = "cccccccc";
        t1(1,0) = "dd";      t1(1,1) = "eeeeeeeeeee"; t1(1,2) = "fffff";

        CS170::Table t2(3,2);

        t2(0,0) = "1111111"; t2(0,1) = "22222";       
        t2(1,0) = "22";      t2(1,1) = "44444444444";
        t2(2,0) = "333333";  t2(2,1) = "6666666";

        t1 = t2;
        std::cout << t1 << std::endl;
        std::cout << t2 << std::endl;

        t2(1,0) = "777777777";      t2(1,1) = "000000000000";
        std::cout << t1 << std::endl;
        std::cout << t2 << std::endl;

        t1(2,0) = "999999999999";  t1(2,1) = "888";
        std::cout << t1 << std::endl;
        std::cout << t2 << std::endl;
}

// assignment operator - use memory debugger to check for double delete and or leaks
// assign 
// modify original 
// display both - see if changes are in original only
// same, but modify copy
void table_test18()
{
        CS170::Table t1(2,3);

        t1(0,0) = "aaaaaaa"; t1(0,1) = "bbbbb";       t1(0,2) = "cccccccc";
        t1(1,0) = "11";      t1(1,1) = "22222222222"; t1(1,2) = "3";

        CS170::Table t2(3,2);

        t2(0,0) = "aaaaaaa"; t2(0,1) = "bbbbb";       
        t2(1,0) = "11";      t2(1,1) = "22222222222";
        t2(2,0) = "******";  t2(2,1) = "+++++++";

        t2 = t1;

        t1(1,0) = "X";
        t1(1,1) = "YYYYYYYYYY";
        t1(1,2) = "ZZZZZ";

        std::cout << t1 << std::endl;
        std::cout << t2 << std::endl;

        t2(0,0) = "****************";
        t2(0,1) = "&&&&&&&&&";
        t2(0,2) = "->->->";

        std::cout << t1 << std::endl;
        std::cout << t2 << std::endl;
}

// assignment operator - stress table_test + self assignment table_test
void table_test19()
{
        CS170::Table t1(2,3);

        t1(0,0) = "aaaaaaa"; t1(0,1) = "bbbbb";       t1(0,2) = "cccccccc";
        t1(1,0) = "dd";      t1(1,1) = "eeeeeeeeeee"; t1(1,2) = "fffff";

        CS170::Table t2(3,2);

        t2(0,0) = "1111111"; t2(0,1) = "22222";       
        t2(1,0) = "22";      t2(1,1) = "44444444444";
        t2(2,0) = "333333";  t2(2,1) = "6666666";

        for ( int i=0; i<10000; ++i ) { // swaps
            CS170::Table temp = t1;
            t1 = t2;
            t2 = temp;
        }
        std::cout << t1 << std::endl;
        std::cout << t2 << std::endl;

        t2 = t1;
        std::cout << t1 << std::endl;
        std::cout << t2 << std::endl;

        for ( int i=0; i<10000; ++i ) {
            t2 = t2; // self-assignment 
        }
        std::cout << t2 << std::endl;
}


// stress table_test
void table_test20()
{
    CS170::Table t1 = makeTable( 4,8,5 );
    CS170::Table t2 = makeTable( 6,16,32 );
    CS170::Table r1 = t1+t2;

    CS170::Table t3 = makeTable( 6,3,15 );
    CS170::Table t4 = makeTable( 2,8,32 );
    CS170::Table r2 = t3*t4;
}

// stress table_test
// no output - checking for crashes
// takes several seconds
void table_test21()
{
    CS170::Table t1 = makeTable( 400, 800, 5 );
    CS170::Table t2 = makeTable( 1200,1600,32 );
    CS170::Table r1 = t1+t2;
    CS170::Table r2 = t2+t1;

    CS170::Table t3 = makeTable( 60, 30,15 );
    CS170::Table t4 = makeTable( 200,80,32 );
    CS170::Table r3 = t3*t4;
    CS170::Table r4 = t4*t3;
}
