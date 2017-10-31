This directory contains:

myAllocator.c: a first-fit allocator
myAllocator.h: its header file

myAllocatorTest1.c: a test program for my allocator 

malloc.c: a replacement for malloc that uses my allocator
test1.c: a test program that uses this replacement malloc

There are two different testers as some implementations of printf
call malloc to allocate buffer space. This causes test1 to behave
improperly as it uses myAllocator as a malloc replacement. In this
case myAllocatorTest1 will function correctly. The only difference
between the programs is that test1 uses myAllocator as a malloc
replacement and myAllocatorTest1 uses myAllocator directly.

10/28/2017

  myAllocator.c has now been modified to provided an alternative allocation
  algorithm, bestFit. The memory allocator has also been modified to work 
  more efficiently by trying to merge a block with its successor when possible
  instead of always allocating new memory and copying the old block's contents. 
  
  Gabriel Maximilian Felix and I worked together to figure out how to merge a block 
  with it's successor.
  
10/28/2017

  Added resizeRegionExtra. This new memory reallocator now tries merging a block with both
  its predecessor and successor blocks, even trying to merge with both. 

10/31/2017

    Added proper testing for the lab in myAllocatorTest1.c and myTest1.c, to showcase my
    implementation of bestFit allocation and my new memory reallocator. 
  
Makefile: a fairly portable "makefile", targets "all" and "clean"

To test:
 $ myAllocTest - Using allocator directly
 $ myTest - Calling malloc, which calls myAllocator functions 
To compile: 
 $ make 
To clean:
 $ make clean

The cygwin runtime uses malloc() and brk() extensively.  It is
interesting to compare the output of test1 & myAllocatorTest1.  All
those extra allocated regions are being used by cygwin's libraries!
