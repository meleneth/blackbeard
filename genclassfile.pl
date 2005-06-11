#!/usr/bin/perl

if($#ARGV!=1){
#usage
	print "Usage:\n $ARGV[0] filename classname\n\nFills out a default class with a header in the files filename.cpp \nand filename.hpp\n";
	die;
}
$infile=$ARGV[0];
$classname=$ARGV[1];

open (EP,">$infile.hpp");

print EP <<"HERE";
#ifndef ___${infile}_inc
#define ___${infile}_inc 1

#include"SDL.h"

class ${classname} {
    public:
        // Public data members go here.
        ${classname}::${classname}(int bar); // Constructor
        ${classname}::~${classname}(); // Destructor
        int Foo(void);
    private:
        // Private members go here.
    protected:
        // Protected members go here.
};

#endif
HERE

close(EP);

open (EP, ">$infile.cpp");
print EP <<"HERE";
#include "${infile}.hpp"

// Public data members go here.
${classname}::${classname}(int bar) // Constructor
{
}
    
${classname}::~${classname}() // Destructor
{
}

int ${classname}::Foo(void)
{
}

// Private members go here.
// Protected members go here.
HERE

close EP;
