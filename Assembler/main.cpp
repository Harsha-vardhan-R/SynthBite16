#include <iostream>
#include <unordered_map>
#include <string>
#include <fstream>
#include <filesystem>

/// usage : synthasm [input-file] [output-file-path]\n
///
///\n This is going to be a command line interface which gets a txt file as an input this saves the final '.hasm' file in the given directory,
///\n If no directory is given then it is going to save it in the same directory as the input file.
///
///PLEASE NOTE THAT THIS ASSEMBLER DOESN'T GIVE A FLYING FUCK FOR TYPES OR VALUES OR VARIABLES,PLEASE LOOK AFTER THEM YOURSELF.
///
///
///\n-INSTRUCTIONS : (OPERAND IS THE VALUE WHICH IS AT THE ADDRESS IN THE INSTRUCTION())
///\n
///\n-OPERAND IS REQUIRED :
///\n-DIRECT AND INDIRECT ADDRESSING
///\n=> 1 X X X | 9 X X X -> LFA -> LOAD       -> LOAD THE VALUE FROM THE ADDRESS INTO DR.
///\n=> 2 X X X | A X X X -> STR -> STORE      -> STORES THE PRESENT VALUE OF AC IN THE OPERAND ADDRESS.
///\n=> 3 X X X | B X X X -> JMP -> JUMP_COND  -> JUMP CONDITIONALLY TO THE GIVEN ADDRESS.
///\n=> 4 X X X | C X X X -> CAL -> CALL	    -> CALL A SUBROUTINE.
///\n=> 5 X X X | D X X X -> RET -> RETURN	    -> RETURN FROM A SUBROUTINE.
///\n=> 6 X X X | E X X X -> LID -> LOAD IMM   -> LOAD IMMEDIATE VALUE INTO THE AC, (THE HIGHEST 4 BITS WILL BE FILLED WITH 0).(E X X X WILL FILL THE HIGHEST 12 BITS OF THE NUMBER)
///\n=> 7 X X X | F X X X -> SWP -> SWAP	    -> SWAPS THE VALUES OF AC AND THE OPERAND.
///\n
///\n
///\n-OPERAND IS NOT REQUIRED : INSTRUCTION FORMAT 0-X-X-X OR 8-X-X-X
///\n
///\n-INSTRUCTIONS MANIPULATING DATA :
///\n
///\n-INSTRUCTIONS ON THE STACK :
///\n=> 8 0 X X -> PSH  -> PUSH ONTO THE STACK		        -> (STACK TOP <- DR, SP <- SP + 1)
///\n=> 8 1 X X -> POP  -> POP FROM THE STACK		        -> POP THE VALUE FROM THE STACK TOP AND PLACE IT IN THE OUT BUFFER REGISTER.
///\n=> 8 2 X X -> ADS  -> ADD THE TOP TWO ELEMENTS IN THE STACK -> (STACK TOP <- STACK TOP + STACK TOP AFTER A POP)
///\n=> 8 3 X X -> SDS  -> SUBTRACT ON THE STACK		    -> (STACK TOP <- STACK TOP - STACK TOP AFTER A POP)
///\n=> 8 4 X X -> MDS  -> MULTIPLY ON THE STACK		    -> (STACK TOP <- STACK TOP * STACK TOP AFTER A POP)
///\n=> 8 5 X X -> DDS  -> DIVIDE ON THE STACK		        -> (STACK TOP <- STACK TOP / STACK TOP AFTER A POP)
///\n
///\n-INSTRUCTIONS ON ACCUMULATOR AND DATA REGISTER :
///\n=> 0 0 X X -> LFD  -> LOAD FROM DR(AC <- DR) 		        -> LOAD THE VALUE OF THE DATA REGISTER INTO AC.
///\n=> 0 1 X X -> LDC  -> LOAD DATA COMPLIMENT(AC <- ~DR)	    -> LOAD THE COMPLIMENT OF THE VALUE OF DR INTO AC.
///\n=> 0 2 X X -> ADD  -> ADD(DR + AC) 			            -> ADD THE VALUE OF DR AND AC AND STORE IT IN AC.
///\n=> 0 3 X X -> MUL  -> MULTIPLY(DR * AC) 		            -> MULTIPLY THE VALUE OF DR WITH AC.
///\n=> 0 4 X X -> SUB  -> SUBTRACT(AC <- DR + (~AC + 1))	    -> SUBTRACT AC FROM DR.
///\n=> 0 5 X X -> DIV  -> DIVIDE(AC <- REMAINDER OF AC/DR, QR <- AC/DR) -> DIVIDE AC WITH DR.
///\n=> 0 6 X X -> CMP  -> COMPARE THE VALUES OF DR AND AC      -> WILL SET THREE BITS BASED ON THE COMPARISON(SMALLER, EQUAL, GREATER).
///\n=> 0 7 X X -> AND  -> BITWISE AND(AC <- AC && DR)   	    -> BITWISE AND WITH AC.
///\n=> 0 8 X X -> BOR  -> BITWISE OR(AC <- AC || DR)	        -> BITWISE OR WITH AC.
///\n=> 0 9 X X -> XOR  -> BITWISE XOR(AC <- AC ^ DR) 	        -> BITWISE XOR WITH AC.
///\n=> 0 A X X -> SHR  -> SHIFT AC TO RIGHT 		            -> SHIFT THE AC VALUE TO THE RIGHT.
///\n=> 0 B X X -> SHL  -> SHIFT AC TO THE LEFT		            -> SHIFT THE AC VALUE TO THE LEFT.
///\n
///\n-INSTRUCTIONS TO SET THE COMPARISON BITS :
///\n=> 8 8 X X -> CSO ->  COMPARISON STATUS BIT <- '1' (YOU WANT TO USE THIS WHEN THERE IS NO CONDITION TO JUMP)(COMPARE STATUS SET ONE)
///\n=> 8 9 X X -> CSL ->  COMPARISON STATUS BIT <- COMPARE REGISTER(LESSER)(COMPARE SET LESSER)
///\n=> 8 A X X -> CSE ->  COMPARISON STATUS BIT <- COMPARE REGISTER(EQUAL)
///\n=> 8 B X X -> CSG ->  COMPARISON STATUS BIT <- COMPARE REGISTER(GREATER)
///\n
///\n-COOL STUFF :
///\n=> 0 D X X -> RND -> LOAD A RANDOM VALUE INTO THE ACCUMULATOR.
///\n=> 0 E X X ->
///\n
///\n-EXTRA
///\n=> 0 C X X -> SSI -> SET SERIAL INPUT TO 1		-> SET THE SERIAL INPUTS FOR THE SHIFT OPERATIONS.(IT WILL BE 0 BY DEFAULT EVERY TIME)
///\n=> 0 F X X -> HLT -> PROGRAM HALT 			-> HALT THE EXECUTION.
///\n
///\n-INSTRUCTIONS RELATED I/O :
///\n=> 8 C X X -> LOAD THE VALUE FROM INPUT REGISTER INTO THE DATA REGISTER.
///\n=> 8 D X X -> PLACE THE VALUE OF AC IN THE OUTPUT REGISTER.


///\n\n*************************************************************
///\nFORMAT IN WHICH THE CODE SHOULD BE PROVIDED :

///\nall the data SHOULD be of a hex value in the form of '0xhhhh'and should have a value of 2 bytes.
///\n.data:
///\n    name : 0xhhhh;
///\n    name2 : 0xhhhh;
///\n    :
///\n    name3 : %name2;        //this is going to store the address of name2 in the memory. for eg : \nname2 :
///\n
///\n
///\n
///\n
///\n
///\n
///\n
///\n
///\n
///\n
///\n
///\n
///\n
///\n
///\n
///\n
///\n




//"v3.0 hex words addressed"

struct synthasm{};
//*************************************************************

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

        {"lfa" , '1'},{"lfa%" , '9'},
        {"str" , '2'},{"str%" , 'a'},
        {"jmp" , '3'},{"jmp%" , 'b'},
        {"cal" , '4'},{"cal%" , 'c'},
        {"ret" , '5'},{"ret%" , 'd'},
        {"ldi" , '6'},{"ldi%" , 'e'},
        {"swp" , '7'},{"swp%" , 'f'},
};

//second 4 bits of the instruction in the memory
std::unordered_map<std::string, char> SECOND_BIT_MAP = {
        {"psh" , '0'},
        {"pop" , '1'},
        {"ads" , '2'},
        {"sds" , '3'},
        {"mds" , '4'},
        {"dds" , '5'},

        {"cso" , '8'},
        {"csl" , '9'},
        {"cse" , 'a'},
        {"csg" , 'b'},

        {"lfd" , '0'},
        {"ldc" , '1'},
        {"add" , '2'},
        {"mul" , '3'},
        {"sub" , '4'},
        {"div" , '5'},
        {"cmp" , '6'},
        {"and" , '7'},
        {"bor" , '8'},
        {"xor" , '9'},
        {"shr" , 'a'},
        {"shl" , 'b'},

        //As these instructions do not have a second bit dedicated to the functionality.
        {"lfa" , '\0'},{"lfa%" , '\0'},
        {"str" , '\0'},{"str%" , '\0'},
        {"jmp" , '\0'},{"jmp%" , '\0'},
        {"cal" , '\0'},{"cal%" , '\0'},
        {"ret" , '\0'},{"ret%" , '\0'},
        {"ldi" , '\0'},{"ldi%" , '\0'},
        {"swp" , '\0'},{"swp%" , '\0'},
};

//we are going to leave the first 256 words free, 'cause we may need to write a bootloader or something.
auto PRESENT_DATA_ADDRESS = 0x100;



int main(int argc, char* argv[]) {

    if (argc == 1) {
        printf("Please provide the file that needs to be assembled");
        return -1;
    }

    std::string output_file_name;

    if (argc == 2) {
        output_file_name = argv[1] + ""
    }

    for (auto i : SECOND_BIT_MAP) {
        std::cout << i.first << " : " << i.second << std::endl;
    }
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
