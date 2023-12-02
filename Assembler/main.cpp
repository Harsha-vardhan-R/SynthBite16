#include <iostream>
#include <unordered_map>
#include <string>
#include <fstream>
#include <sstream>
#include <regex>
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
///\n-INSTRUCTIONS MANIPULATING DATA ://positions of the 'data:' and 'instructions:'
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
///\ndata:
///\n    name 0xhhhh;
///\n    name2 0xhhhh;
///\n    :
///\n    name3 &name2;        //this is going to store the address of name2 in the memory. for eg : \name2 :pointer should always be declared after the original declaration.
///\n
///\n
///\ninstructions:
///\n
///\n
///\n
///\n
///\n
///\nsubroutine1:
///\n
///\n
///\n
///\n
///\n
///\n
///\n
///\n





//"v3.0 hex words addressed"

struct [[maybe_unused]] synthasm{};
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
const std::unordered_map<std::string, char> FIRST_BIT_MAP = {
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
const std::unordered_map<std::string, char> SECOND_BIT_MAP = {
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

//we are going to leave the first 256 words free, 'cause we may need to write a bootloader or some shit.
unsigned short PRESENT_ADDRESS = 0x102;
unsigned short DATA_START_ADDRESS = 0x0;//need to change this while running based on the amount of instructions.

std::string short_to_nibble(unsigned short number) {
    std::ostringstream four_nibbles;
    four_nibbles << std::setw(4) << std::setfill('0') << std::hex << number;
    return four_nibbles.str();
}

std::string short_to_nibble_three(unsigned short number) {
    std::ostringstream four_nibbles;
    four_nibbles << std::setw(3) << std::setfill('0') << std::hex << number;
    return four_nibbles.str();
}

int main(int argc, char* argv[]) {

    //Check for correct number of arguments.
    if (argc < 2 || argc > 3) {
        std::cerr << "Usage: " << argv[0] << " <input_filename> [output_filename]" << std::endl;
        return -1;
    }

    //Checking for the extension name of the input file.
    //I do not even know why i am doing this shit!
    std::string InputFileName = argv[1];
    if (std::filesystem::path(InputFileName).extension() != ".hasm") {
        std::cerr << "The input file must have the extension '.hasm'" << std::endl;
        //return -1;
    }

    //creating the output file
    std::string output_file_name;
    if (argc == 2) {
        output_file_name = std::filesystem::path(InputFileName).parent_path().string() + "/out.octet-stream";
    } else if (argc == 3) {
        if (std::filesystem::path(argv[2]).extension() != ".octet-stream") {
            std::cerr << "The output file name should have .octet-stream as the extension" << std::endl;
            return -1;
        }
        output_file_name = argv[2];
    }

    //Opening the input file.
    std::ifstream InFile(InputFileName);
    if (!InFile.is_open()) {
        std::cerr << "Couldn't open the specified file" << std::endl;
        return -1;
    }

    //Creating the output file
    std::ofstream OutputFile(output_file_name, std::ios::binary | std::ios::out);
    if (!OutputFile.is_open()) {
        std::cerr << "Failed to create the output file" << std::endl;
        return -1;
    }


    //Reading the input file to a string to tokenize the string.
    std::stringstream InputFileStream;
    InputFileStream << InFile.rdbuf();
    std::string InputData = InputFileStream.str();
    InFile.close();

    //Vector of tokens.
    std::vector<std::string> tokens;
    std::regex pattern(R"(\s+|:|=|;)");
    //Creating the vector by tokenizing using regex.
    std::sregex_token_iterator tokenIterator(InputData.begin(), InputData.end(), pattern, -1);
    std::sregex_token_iterator end;//empty iter(matches with an exhausted iterator)
    while (tokenIterator != end) {
        if (!tokenIterator->str().empty()) {
            tokens.push_back(tokenIterator->str());
        }
        tokenIterator++;
    }

    ///Process of assembling :
    ///First we are going to iterate over the data and create a symbol tree for it.
    /// we do not know how many instructions will be present, so we need to store instructions after storing the data.
    ///at 0x101 we are going to store the address from which the instructions are starting which we are going to jump to indirectly from the bootstrap.
    ///the status bit for the jump will be set in the boot strap.

    //Start assembling!
    //positions of the 'data:' and 'instructions:'
    std::tuple<unsigned short , unsigned short> Data, Instructions;
    //positions of other subroutines (where it starts and where it ends).
    std::vector<std::tuple<std::string, unsigned short, unsigned short>> Subroutines;
    int index1 = 0;
    for (auto const& token : tokens) {
        if (token == ".data") {
            Data = std::make_tuple(index1, 0);
        } else if (token == ".instructions") {
            Instructions = std::make_tuple(index1, 0);
            Data = std::make_tuple(std::get<0>(Data), index1);
        }
        index1++;
    }
    Instructions = std::make_tuple(std::get<0>(Instructions), index1);
    //Setting the subroutines starting and ending positions.
    int SubroutineNumber = 0;
    index1 = 0;
    for (auto& token : tokens) {
        if (token.at(0) == '_') {
            token.erase(0, 1);
            Subroutines.emplace_back(token, index1, 0);
            if (SubroutineNumber != 0) {
                Subroutines[SubroutineNumber-1] = std::make_tuple(std::get<0>(Subroutines[SubroutineNumber-1]), std::get<1>(Subroutines[SubroutineNumber-1]), index1);
            } else {
                Instructions = std::make_tuple(std::get<0>(Instructions), index1);
            }
            SubroutineNumber++;
        }
    }

    //Stores the variable name and the address at which it is stored.
    std::unordered_map<std::string, unsigned short> DataAddressMap;
    //Stores the data part of the code in a sequential order, which is later used to write into the output file.
    std::vector<std::string> MachineStringData;
    for (unsigned short TokenIndex = std::get<0>(Data) + 1; TokenIndex < std::get<0>(Instructions); TokenIndex += 2) {
        //if the value starts with a '&' we are going to stem it and search for the stemmed symbol.
        if (tokens[TokenIndex + 1].at(0) == '&') {
            tokens[TokenIndex + 1].erase(0, 1);
            try {
                auto value = DataAddressMap.at(tokens[TokenIndex + 1]);
                DataAddressMap.emplace(tokens[TokenIndex], PRESENT_ADDRESS);
                MachineStringData.emplace_back(short_to_nibble(value));
            } catch (std::out_of_range& e) {
                std::cerr << "Symbol '" << tokens[TokenIndex + 1] << "' is not found in the present scope to indirectly address, \n please note you need to have the variable declared before referencing it\n" << e.what() << std::endl;
                return -1;
            }
        } else {
            DataAddressMap.emplace(tokens[TokenIndex], PRESENT_ADDRESS);
            auto value = std::stoi(tokens[TokenIndex + 1]);
            if (value < -32768 || value > 32767) {
                std::cerr << "The value : " << value << ", is too small or too big to be assigned" << " for the variable : '" << tokens[TokenIndex] << "' , values should be in the range : [0, 65,535]" << std::endl;
                return -1;
            }
            MachineStringData.emplace_back(short_to_nibble(value));
        }
        PRESENT_ADDRESS += 1;
    }



    //Here we are going to store the instructions in the sequence.
    std::vector<std::string> MachineInstruction;
    std::stringstream present;
    for (unsigned short InstructionIndex = std::get<0>(Instructions)+1; InstructionIndex <= std::get<1>(Instructions);) {
        std::string* Instruction = &tokens[InstructionIndex];
        try {
            char first_nibble = FIRST_BIT_MAP.at(*Instruction);
            char second_nibble = SECOND_BIT_MAP.at(*Instruction);
            if (second_nibble != '\0') {
                present << first_nibble << second_nibble << "00";
                MachineInstruction.emplace_back(present.str());
                present.clear();
                //because this does not have an address.
                InstructionIndex += 1;
            } else {
                std::string* InstructionAddress = &tokens[InstructionIndex + 1];
                try {
                    std::string Address = short_to_nibble_three(DataAddressMap.at(*InstructionAddress));
                    MachineInstruction.emplace_back(present.str());
                    present.clear();
                } catch(const std::out_of_range& err) {
                    std::cout << "The variable name : '" << *InstructionAddress << "', is not found in the data section" << std::endl;
                    return -1;
                }
                InstructionIndex += 2;
            }
        } catch (const std::out_of_range& e) {
            std::cerr << "No instruction matching with the given instruction name : '" << *Instruction << std::endl;
            return -1;
        }
    }





    unsigned short CURRENT_FILLER = 0x0;
    //    for (auto i : tokens) {
//        std::cout << i << "\n";
//    }

    ///Writing the bootloader to the output file.
    for (unsigned short i = 0x0; i <= 0x100; i++) {
        OutputFile << short_to_nibble(0) << " ";
        if (i % 8 == 0 && i != 0) {
            OutputFile << "\n";
        }
    }

    ///Filling the remaining space till 0x100.


    ///setting the address to jump at the first position at 0x101.


    ///Writing the data from 0x101.


    ///writing the instructions and subroutines.
    for (const auto& i : DataAddressMap) {
        OutputFile <<  i.first << " " << short_to_nibble_three(i.second) << "\n";
    }

    for (auto i : MachineStringData) {
        OutputFile << i << "\n";
    }

    std::cout << "Saving at : " << output_file_name << "\n";
    OutputFile.close();
    return 1;
}
