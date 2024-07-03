# Block-based doubly linked list
Note: *This code was developed in 2022 as a project for the Algorithms and Data Structures course at the Gdańsk University of Technology. The author of the task's requirements is Dr. Tytus Pikies. The detailed requirements of the project are available [here](/additional_files/task_description.txt).*

## Idea
### Basic assumptions
The aim of the project was to develop a data structure which operates as a doubly linked list but allows storing more than one object in a single node. The two exemplary nodes are presented below (for simplicity, stored objects are just positive integers).

```
      A                 B
+-----------+     +-----------+
|prev block |     |     A     |     -- pointer to the previous node (or null if it is the first node)
|     B     |     |next block |     -- pointer to the next node (or null if it is the last node)
|     3     |     |     4     |     -- number of currently stored objects
+-----------+     +-----------+
|     1     |     |     4     |     -- first object in the node
|     2     |     |     5     |     -- second object in the node
|     3     |     |     6     |     -- third object in the node
|           |     |     7     |     -- fourth object in the node
|           |     |           |
|           |     |           |
+-----------+     +-----------+
```

The objects stored in the data structure are accessible through iterators, which point to particular objects on the list. There are:
* 2 "fixed" iterators:
   * `BEG` - pointing to the first object in the first node of the list,
   * `END` - pointing to the last object in the last node of the list.
* 10 "free" iterators:
   * they may point to any object in any node of the list,
   * they are labeled from `0` to `9`.

The possible actions are:
1. Initialization of the list with blocks of a given size (e.g., 64B), 
2. Addition of an object to the list,
3. Removal of an object from the list,
4. Addition of a new "free" iterator,
5. Shift of a "free" iterator,
6. Visualization of an object pointed by a given iterator,
7. Visualization of the whole list.

When visualizing the whole list the program demonstrates how the list is divided into blocks.\
For example:
```
New block: 1 2 3 4 New block: 5 10
```

### Interface details
The communication with the data structure is performed through the following commands:

```
   I x             -- The parameters x is the block size.
                      Use it to initialize the block-based list.
                      In the case of the simple list you can ignore it, or use however you like.
   i x y           -- Initialize the x-th iterator to the position y.
                      The value of y can be either [BEG]inning, or [END], or a number.
                      If it is a number i', then the iterator should be a copy of the i'-th iterator.
                      An iterator can be initialized many times and there should be no memory leaks.
                      The iterators (except BEG and END) are labeled from 0 to 9.
   + x             -- Move the x-th iterator forward.
                      If moving forward is impossible, then the iterator should point to the last element.
   - x             -- Move the x-th iterator backward.
                      If moving backward is impossible, then the iterator should point to the first element.
   .A x y          -- Add the number y before the position x.
                      As previously, x can be either [BEG]inning, [END], or a number of iterator.
                      In the case of an empty list, add the number to the list and initialize BEG and END appropriately.
   A. x y          -- Add the number y after the position x.
                      As previously, x can be either [BEG]inning, [END], or a number of iterator.
                      In the case of an empty list, add the number to the list and initialize BEG and END appropriately.
   R x             -- Remove an element given by the position x.
                      Similarly, x can either [BEG]inning or [END] of list, or a number of iterator.
                      If an element is removed, then all iterators pointing at it should start pointing to the next element.
                      If this is not possible (the element at the end was removed) they should point to the new last element.
                      If this is also not possible (the last element was removed) the pointer should become uninitialized.
   P p             -- Print the number on the position given by p.
                      The parameter p can be of two types.
                      It can be [ALL] which means to print from head to tail.
                      Or it may be a number of iterator -- in the last case print the value pointed.
```

## Compilation
The following commands can be used to compile the project using CMake:

```
mkdir build
cd build
cmake ..
cmake --build .
```

## Examples
### Example 1
Example 1 demonstrates a imple addition and removal of objects to the list. To run it on your own use `./block-based-double-linked-list < ../examples/example1.in` in the `build` directory.

Example's 1 input:
```
I 64
.A BEG 10
P ALL
.A BEG 9
P ALL
.A BEG 8
P ALL
.A BEG 7
P ALL
.A BEG 6
P ALL
R BEG
P ALL
R BEG 
P ALL
R BEG
P ALL
R BEG
P ALL
.A BEG 5
P ALL
.A BEG 4
P ALL
.A BEG 3
P ALL
.A BEG 2
P ALL
.A BEG 1
P ALL
```

Example's 1 output:
```
New block: 10 
New block: 9 10 
New block: 8 9 10 
New block: 7 8 9 10 
New block: 6 7 8 9 10 
New block: 7 8 9 10 
New block: 8 9 10 
New block: 9 10 
New block: 10 
New block: 5 10 
New block: 4 5 10 
New block: 3 4 5 10 
New block: 2 3 4 5 10 
New block: 1 2 3 4 New block: 5 10
```

### Example 2
Example 2 demonstrates a case of simple operations on an iterator. To run it on your own use `./block-based-double-linked-list < ../examples/example2.in` in the `build` directory.

Example's 2 input:
```
I 64
A. BEG 1
A. END 2
A. END 3
A. END 4
A. END 6
P ALL
i 0 BEG
+ 0
+ 0
R 0 
P ALL
A. 0 5
P ALL
.A 0 3
P ALL
```

Example's 2 output:
```
New block: 1 2 3 4 6 
New block: 1 2 4 6 
New block: 1 2 4 5 6 
New block: 1 2 3 4 New block: 5 6
```

### Example 3
Example 3 demonstrates the process of maximizing the occupancy rate of the blocks. To run it on your own use `./block-based-double-linked-list < ../examples/example3.in` in the `build` directory.


Example's 3 input:
```
I 64
.A BEG 10
P ALL
.A BEG 9
P ALL
.A BEG 8
P ALL
.A BEG 7
P ALL
.A BEG 6
P ALL
.A BEG 5
P ALL
.A BEG 4
P ALL
.A BEG 3
P ALL
.A BEG 2
P ALL
.A BEG 1
P ALL
```

Example's 3 output:
```
New block: 10 
New block: 9 10 
New block: 8 9 10 
New block: 7 8 9 10 
New block: 6 7 8 9 10 
New block: 5 6 7 8 New block: 9 10 
New block: 4 5 6 7 8 New block: 9 10 
New block: 3 4 5 6 New block: 7 8 9 10 
New block: 2 3 4 5 6 New block: 7 8 9 10 
New block: 1 2 3 4 New block: 5 6 New block: 7 8 9 10
```