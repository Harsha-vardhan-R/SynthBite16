#include <iostream>
#include <unordered_map>
#include <string>

///usage : synthasm input-file-path name-of-output-file-with-path

//This is going to be a command line interface which gets a txt file as an input this saves the final '.hasm' file in the given directory,
//If no directory is given then it is going to save it in the same directory as the input file.


//-INSTRUCTIONS : (OPERAND IS THE VALUE WHICH IS AT THE ADDRESS IN THE INSTRUCTION())
//
//-OPERAND IS REQUIRED :
//-DIRECT AND INDIRECT ADDRESSING
//=> 1 X X X | 9 X X X -> LFA -> LOAD       -> LOAD THE VALUE FROM THE ADDRESS INTO DR.
//=> 2 X X X | A X X X -> STR -> STORE      -> STORES THE PRESENT VALUE OF AC IN THE OPERAND ADDRESS.
//=> 3 X X X | B X X X -> JMP -> JUMP_COND  -> JUMP CONDITIONALLY TO THE GIVEN ADDRESS.
//=> 4 X X X | C X X X -> CAL -> CALL	    -> CALL A SUBROUTINE.
//=> 5 X X X | D X X X -> RET -> RETURN	    -> RETURN FROM A SUBROUTINE.
//=> 6 X X X | E X X X -> LID -> LOAD IMM   -> LOAD IMMEDIATE VALUE INTO THE AC, (THE HIGHEST 4 BITS WILL BE FILLED WITH 0).(E X X X WILL FILL THE HIGHEST 12 BITS OF THE NUMBER)
//=> 7 X X X | F X X X -> SWP -> SWAP	    -> SWAPS THE VALUES OF AC AND THE OPERAND.
//
//
//-OPERAND IS NOT REQUIRED : INSTRUCTION FORMAT 0-X-X-X OR 8-X-X-X
//
//-INSTRUCTIONS MANIPULATING DATA :
//
//-INSTRUCTIONS ON THE STACK :
//=> 8 0 X X -> PSH  -> PUSH ONTO THE STACK		        -> (STACK TOP <- DR, SP <- SP + 1)
//=> 8 1 X X -> POP  -> POP FROM THE STACK		        -> POP THE VALUE FROM THE STACK TOP AND PLACE IT IN THE OUT BUFFER REGISTER.
//=> 8 2 X X -> ADS  -> ADD THE TOP TWO ELEMENTS IN THE STACK -> (STACK TOP <- STACK TOP + STACK TOP AFTER A POP)
//=> 8 3 X X -> SDS  -> SUBTRACT ON THE STACK		    -> (STACK TOP <- STACK TOP - STACK TOP AFTER A POP)
//=> 8 4 X X -> MDS  -> MULTIPLY ON THE STACK		    -> (STACK TOP <- STACK TOP * STACK TOP AFTER A POP)
//=> 8 5 X X -> DDS  -> DIVIDE ON THE STACK		        -> (STACK TOP <- STACK TOP / STACK TOP AFTER A POP)
//
//-INSTRUCTIONS ON ACCUMULATOR AND DATA REGISTER :
//=> 0 0 X X -> LFD  -> LOAD FROM DR(AC <- DR) 		        -> LOAD THE VALUE OF THE DATA REGISTER INTO AC.
//=> 0 1 X X -> LDC  -> LOAD DATA COMPLIMENT(AC <- ~DR)	    -> LOAD THE COMPLIMENT OF THE VALUE OF DR INTO AC.
//=> 0 2 X X -> ADD  -> ADD(DR + AC) 			            -> ADD THE VALUE OF DR AND AC AND STORE IT IN AC.
//=> 0 3 X X -> MUL  -> MULTIPLY(DR * AC) 		            -> MULTIPLY THE VALUE OF DR WITH AC.
//=> 0 4 X X -> SUB  -> SUBTRACT(AC <- DR + (~AC + 1))	    -> SUBTRACT AC FROM DR.
//=> 0 5 X X -> DIV  -> DIVIDE(AC <- REMAINDER OF AC/DR, QR <- AC/DR) -> DIVIDE AC WITH DR.
//=> 0 6 X X -> CMP  -> COMPARE THE VALUES OF DR AND AC     -> WILL SET THREE BITS BASED ON THE COMPARISON(SMALLER, EQUAL, GREATER).
//=> 0 7 X X -> AND  -> BITWISE AND(AC <- AC && DR)   	    -> BITWISE AND WITH AC.
//=> 0 8 X X -> BOR  -> BITWISE OR(AC <- AC || DR)	        -> BITWISE OR WITH AC.
//=> 0 9 X X -> XOR  -> BITWISE XOR(AC <- AC ^ DR) 	        -> BITWISE XOR WITH AC.
//=> 0 A X X -> SHR  -> SHIFT AC TO RIGHT 		            -> SHIFT THE AC VALUE TO THE RIGHT.
//=> 0 B X X -> SHL  -> SHIFT AC TO THE LEFT		        -> SHIFT THE AC VALUE TO THE LEFT.
//
//-INSTRUCTIONS TO SET THE COMPARISON BITS :
//=> 8 8 X X -> CSO ->  COMPARISON STATUS BIT <- '1' (YOU WANT TO USE THIS WHEN THERE IS NO CONDITION TO JUMP)(COMPARE STATUS SET ONE)
//=> 8 9 X X -> CSL ->  COMPARISON STATUS BIT <- COMPARE REGISTER(LESSER)(COMPARE SET LESSER)
//=> 8 A X X -> CSE ->  COMPARISON STATUS BIT <- COMPARE REGISTER(EQUAL)
//=> 8 B X X -> CSG ->  COMPARISON STATUS BIT <- COMPARE REGISTER(GREATER)
//
//-COOL STUFF :
//=> 0 D X X -> (generate random number)
//=> 0 E X X ->
//
//-EXTRA
//=> 0 C X X -> SSI -> SET SERIAL INPUT TO 1		-> SET THE SERIAL INPUTS FOR THE SHIFT OPERATIONS.(IT WILL BE 0 BY DEFAULT EVERY TIME)
//=> 0 F X X -> HLT -> PROGRAM HALT 			-> HALT THE EXECUTION.
//
//-INSTRUCTIONS RELATED I/O :
//=> 8 C X X -> LOAD THE VALUE FROM INPUT REGISTER INTO THE DATA REGISTER.
//=> 8 D X X -> PLACE THE VALUE OF AC IN THE OUTPUT REGISTER.


//In the 7 instructions that require you to give the operand as an address or data,
//  6 of them have two types of addressing modes(direct and indirect)
//  1 of them has an immediate mode.
enum AddressingMode {
    DirectAddress,
    IndirectAddress,
    ImmediateAddress,
    NoAddress,//for the stuff that does not need an operand.
};

//first 4 bits of the instruction in the memory
std::unordered_map<std::string, char> FIRST_BIT_MAP = {
        {"psh" , '8'},
        {"pop" , '8'},
        {"ads" , '8'},
        {"sds" , '8'},
        {"mds" , '8'},
        {"dds" , '8'},

        {"cso" , '8'},
        {"csl" , '8'},
        {"cse" , '8'},
        {"csg" , '8'},

        {"lfd" , '0'},
        {"ldc" , '0'},
        {"add" , '0'},
        {"mul" , '0'},
        {"sub" , '0'},
        {"div" , '0'},
        {"cmp" , '0'},
        {"and" , '0'},
        {"bor" , '0'},
        {"xor" , '0'},
        {"shr" , '0'},
        {"shl" , '0'},


};

int main() {

    for (auto i : FIRST_BIT_MAP) {
        std::cout << i.first << " : " << i.second << std::endl;
    }
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
