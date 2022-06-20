# Skiplist

Goals: Working with dynamic arrays, pointers, doubly linked lists

For this assignment, you will write a Skip List data structure to store integers. When searching a 
Skip List, items can be found in `O(log n)` time. No duplicates are allowed.

A `SkipList` can have multiple levels. `SkipList` of depth 1 is similar to a doubly linked list. All
elements are inserted into a single doubly linked list. 

When a `SkipList` has multiple levels, all elements are inserted at level = 0. 50% of those
inserted at level = 0, are also inserted at level = 1. 50% of those inserted at level = 1 are also 
inserted at level = 2, and so on. 

There are multiple different types of implementations of Skip List data structure that you might 
find on the internet, make sure you are following the assignment specifications. 

The above picture shows a `SkipList` with a depth of 5, Level-0 to Level-4

At `L0`, all items are inserted: 10, 15, 35, 70, 120 

Some of those items (based on a coin toss, 50% chance using `(rand() % 2) == 1)` are
inserted at level = 1, i.e. at L1. Some of those items are inserted at L2, and so on.

`SkipList` has front and rear guards, in our case `INT_MIN` and `INT_MAX` as the first and the
last items at each of the levels. This makes it easier to add and find items.

Skip List is made up of private `SNode` elements. Each `SNode` has `SNode*` of `next`, `prev`,
`upLevel` and `downLevel`, as well as `data` which holds the actual integer value stored at the
node. 
front and rear guards are special `SNode*` objects to make the implementation easier. The
pointers to these guards are kept in arrays, named `frontGuards` and `rearGuards`

## Compile and Run

```
g++  -g -std=c++11 -Wall -Wextra assignment4.cpp skiplist.cpp -o assignment4
./assignment4
```
