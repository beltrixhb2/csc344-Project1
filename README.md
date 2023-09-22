# csc344-Project1
Implementation of a Turing Machine in C
In this assignment you will implement a Turing Machine (TM) in C. A TM consists of:

    -An infinite tape, divided into cells
    -A read/write head which traverses along the tape, capable of reading the current cell, writing a new value in the current cell, and moving left or right
    -A state register
    -A finite table of instructions which, given the current state of the machine, and the value in the tape cell currently being read, retrieves an instruction which tells       the machine to:
        >Write some (possibly the same) item into the cell
        >Move the head left or right one cell
        >Change to some (possibly the same) state

Our TM tape will be one-way infinite to the right, meaning that the tape has a beginning. In order to let our machine recognize when it reaches the beginning of the tape, you should ensure that the tape begins with the character ‘A’ in its leftmost cell. By convention, we won’t use ‘A’ anywhere else.

A TM tape is made up of cells. Computationally, represent the tape as a linked data structure of cells, wherein a cell (most conveniently represented as a struct) has a character representing what is written in that cell of the tape, and pointers to previous and next cells (think: doubly linked list).

The initial contents of the tape (to be written immediately after the ‘A’ cell) will be provided by an input file. Your program should allocate just enough cells for the input. If you need more cells during processing you should allocate new blank cells on the fly. We’ll establish the convention that a tape cell containing the character ‘B’ is a blank cell.

The read/write head of your TM should be a pointer to some cell of the tape. Begin machine execution with the read/write head pointing to the leftmost cell on the tape (the ‘A’). As you move the read/write head, you will update the pointer to the current cell seen by the read/write head.

The set of instructions will be provided in the input file in the form: (CurrentState,ReadVal)->(WriteVal,MoveDirection,NewState). The first pair represents a configuration of the machine wherein an instruction, the triple after the arrow, should be executed.

In order to represent the instruction table in your program, create a two-dimensional array (see the C FAQ!), where the row indices are states and the columns indices are all standard ASCII characters which may be read from or written to the tape (i.e., there should always be 128 columns, representing ASCII 0-127). Given the current state and the current character being read by the read/write head, the machine looks up an instruction in this array. For example, if the machine is in state 1, reading a ‘B’ from the tape it should access [1][‘B’] in the array. The instruction at that position will tell the machine what value to write, what direction to move, and what the new state should be. (In the below example, this would be to write a ‘B’, move left, and change to state 2). Instructions should be represented using a struct.

Below is a sample input file which adds two unary numbers together. Given an initial tape input of 111B1111, representing the numbers 3 and 4 separated by a blank, the machine walks to the end of the tape, changing the middle B to a 1, and changing the last 1 to a B. The result is 1111111, unary 7. The program assumes the machine starts at the beginning of the tape (on the ‘A’ cell).

111B1111
6
0
5
(0,A)->(A,R,1)
(1,1)->(1,R,1)
(1,B)->(1,R,2)
(2,1)->(1,R,2)
(2,B)->(B,L,3)
(3,1)->(B,L,4)
(4,1)->(1,L,4)
(4,A)->(A,R,5)

The input file has the following specification:
Line 1: Initial tape content (written after ‘A’ cell)
Line 2: Number of states
Line 3: Start state
Line 4: End state
Remaining lines represent the state machine. Each line is has 5 parts:
(CurrentState,ReadVal)->(WriteVal,MoveDirection,NewState)
