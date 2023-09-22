#include <stdio.h>
#include <stdlib.h>

typedef struct cell cell;

// Definition of cell type
struct cell{
	cell * right;
	cell * left;
	char value;
};

// Definition of instruction type
typedef struct{
	char val_write;
	char direction;
	int new_state;
}instruction;

// Creates a new cell in the right with the value given, pointing to NULL to the right and the previous cell to the left
void new_cell(cell* position, char value){
	position->right = malloc(sizeof(cell));
	position->right->left= position;
	position->right->value = value;
	position->right->right = NULL;
}


// Prints the actual tape

void print_tape(cell* firstcell){
	while (firstcell!=NULL){
		printf("%c ",(firstcell->value));
		firstcell = firstcell->right;
	}
	printf("\n");
}

// Prints a mark on the current position
void print_position_mark(cell* firstcell, cell* position){
	while (firstcell!=position){
                printf("  ");
                firstcell = firstcell->right;
        }
	printf("^\n");
}

int main(){
	printf("\n\n");
	printf("****************************************\n");
	printf("******PROJECT 1 BY BELTRAN HIDALGO******\n");
	printf("****************************************\n");

	// Ask for file name
	char fname[128];
	printf("Input file: ");
	scanf("%123s",fname);

	// Open the file for reading
        FILE *file = fopen(fname, "r");
        if (file == NULL) {
                perror("Error opening file");
                return 1;
        }


	//
	//CREATION, READING AND FILLING OF THE TURING MACHINE TAPE
	//

	printf("Writing tape...\n");

	// Create first cell
	cell * firstcell = malloc(sizeof(cell));
	// Fill left cell, with vaule A, pointing to NULL left and right
	firstcell->left = NULL;
	firstcell->right = NULL;
	firstcell->value = 'A';
	// Position will move through the cells
	cell * position = firstcell;
	// Reading tape and creating new cells for storing the values
	char value;
        while((value = fgetc(file))!='\n'){
		new_cell(position, value);
		position = position->right;
	}

	printf("Initial tape:  ");
	print_tape(firstcell);

	//
	//CREATION, READING AND FILLING OF THE INSTRUCTIONS
	//

	// Reads the number of states
        int n_states;
        fscanf(file,"%d", &n_states);
        while(fgetc(file)!='\n');
        // Reads the first state
        int actual_state;
        fscanf(file,"%d", &actual_state);
        while(fgetc(file)!='\n');
        // Reads last state
        int final_state;
        fscanf(file,"%d", &final_state);
        while(fgetc(file)!='\n');


	// Create the istructions matrix of size n_states x 255 storing instruction types
	instruction ** instructions = malloc(n_states*sizeof(instruction*));
	for (int i=0; i<n_states; i++){
		instructions[i] = malloc(255*sizeof(instruction));
	}

	// Read the values of the instruction from the file and store it in the instruction matrix, read also the position to store it in the matrix
	int c_state;
        char read_val;
        char val_write;
        char direction;
        int new_state;
        int char_to_int;
        while (fscanf(file, "(%d,%c)->(%c,%c,%d)", &c_state, &read_val, &val_write, &direction, &new_state)==5){
		char_to_int = (int) read_val;
                instructions[c_state][char_to_int].val_write = val_write;
		instructions[c_state][char_to_int].direction = direction;
		instructions[c_state][char_to_int].new_state = new_state;
		while(fgetc(file)!='\n');
        }



	//
	//TURING MACHINE IMPLEMENTATION
	//

	// Option of watching the full simulation
	char answer;
	printf("Do you want to see the full process?    y/n:   ");
	getchar();
	scanf("%c", &answer);
	while((answer!='y')&&(answer!='n')){
		printf("Please, answer with y or n, do you want to see the full process?:   ");
		scanf("%c", &answer);
	}
	if (answer=='y'){
		printf("SIMULATION, IN DETAIL\n");
		printf("-------------------------\n");
	}
	// Position starts from first cell
	position = firstcell;
	int tape_char_to_int;
	tape_char_to_int = (int) position->value;
	int x = 1;
	// Loop while machine doesn't get to the final state
	while (actual_state!=final_state){
		// Get the value of the actual position of the tape to int, we will use it and the actual state for searching for the instructions
		tape_char_to_int = (int) position->value;
		// Print the actual state, full tape and instruction to execute
		if (answer=='y'){
			printf("State %d\n", actual_state);
			print_tape(firstcell);
			print_position_mark(firstcell,position);
			printf("Write %c, Move %c, New State %d\n", instructions[actual_state][tape_char_to_int].val_write, instructions[actual_state][tape_char_to_int].direction, instructions[actual_state][tape_char_to_int].new_state);
			printf("-------------------------\n");
		}
		// The value of the actual position changes to the value the istruction says
		position->value = instructions[actual_state][tape_char_to_int].val_write;
		// The next position moves to the direction the instruction says
		// If it says left, we asume there will be a cell in the left as the description of the project says the tape is infinite to the right only
		if (instructions[actual_state][tape_char_to_int].direction=='L'){
			position = position->left;
		}else{
			// If there is no cell to the right, we create it with B vaule for blank cell
			if (position->right == NULL){
				new_cell(position,'B');
				position = position->right;
			}else{
				position = position->right;
			}
		}
		// The state changes to the one the instruction says
		actual_state = instructions[actual_state][tape_char_to_int].new_state;
	}
	if (answer=='y'){
		print_tape(firstcell);
		print_position_mark(firstcell, position);
		printf("-------------------------\n\n");
	}
	printf("Final tape contents:  ");
	print_tape(firstcell);
	printf("************END OF EXECUTION************\n");
	printf("\n\n");


	// Dellocate memory and close file

	for (int i = 0; i < n_states; i++) {
    		free(instructions[i]);
	}
	free(instructions);

	while(firstcell->right!=NULL){
		free(firstcell->left);
		firstcell = firstcell->right;
	}
	free(firstcell);

        fclose(file);
}
