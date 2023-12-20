#include <iostream>
#include <unordered_map>
#include <string>
#include <fstream>
#include <sstream>
#include <regex>
#include <filesystem>


/// usage : synthasm [input-file] [output-file-path]\n
///\n This CLI is written in the
///
///\n This is going to be a command line interface which gets a txt file as an input this saves the final '.hasm' file in the given directory,
///\n If no directory is given then it is going to save it in the same directory as the input file.
///
///PLEASE NOTE THAT THIS ASSEMBLER DOESN'T GIVE A FLYING FUCK FOR TYPES OR VALUES OR VARIABLES,PLEASE LOOK AFTER THEM YOURSELF.
///
///
///\n=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
///\nINSTRUCTIONS
///\n=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
///\n
///\n-INSTRUCTIONS : (OPERAND IS THE VALUE WHICH IS AT THE ADDRESS IN THE INSTRUCTION())
///\n
///\n	-OPERAND IS REQUIRED :
///\n		-DIRECT AND INDIRECT ADDRESSING
///\n			=> 1 X X X | 9 X X X -> LFA -> LOAD       -> LOAD THE VALUE FROM THE ADDRESS INTO DR.
///\n			=> 2 X X X | A X X X -> STR -> STORE      -> STORES THE PRESENT VALUE OF AC IN THE OPERAND ADDRESS.
///\n			=> 3 X X X | B X X X -> JMP -> JUMP_COND  -> JUMP CONDITIONALLY TO THE GIVEN ADDRESS.
///\n			=> 4 X X X | C X X X -> CAL -> CALL	      -> CALL A SUBROUTINE.
///\n			=> 5 X X X | D X X X -> RET -> RETURN	  -> RETURN FROM A SUBROUTINE.
///\n			=> 6 X X X | E X X X -> LDI -> LOAD IMM   -> LOAD IMMEDIATE VALUE INTO THE AC, (THE HIGHEST 4 BITS WILL BE FILLED WITH 0).
///\n			=> 7 X X X | F X X X -> SWP -> SWAP	      -> SWAPS THE VALUES OF AC AND THE OPERAND.
///\n
///\n
///\n	-OPERAND IS NOT REQUIRED : INSTRUCTION FORMAT 0-X-X-X OR 8-X-X-X
///\n
///\n		-INSTRUCTIONS MANIPULATING DATA :
///\n
///\n			-INSTRUCTIONS ON THE STACK :
///\n				=> 8 0 X X -> PSH  -> PUSH ONTO THE STACK		                -> (STACK TOP <- DR, SP <- SP + 1)
///\n				=> 8 1 X X -> POP  -> POP FROM THE STACK		                -> POP THE VALUE FROM THE STACK TOP AND PLACE IT IN THE AC REGISTER.
///\n				=> 8 2 X X -> ADS  -> ADD THE TOP TWO ELEMENTS IN THE STACK     -> (STACK TOP <- STACK TOP + STACK TOP AFTER A POP)
///\n				=> 8 3 X X -> SDS  -> SUBTRACT ON THE STACK		                -> (STACK TOP <- STACK TOP - STACK TOP AFTER A POP)
///\n				=> 8 4 X X -> MDS  -> MULTIPLY ON THE STACK		                -> (STACK TOP <- STACK TOP * STACK TOP AFTER A POP)
///\n				=> 8 5 X X -> DDS  -> DIVIDE ON THE STACK		                -> (STACK TOP <- STACK TOP / STACK TOP AFTER A POP)
///\n               => 8 6 X X -> LFS  -> LOAD FROM STACK TOP                       -> (AC <- STACK TOP)
///\n
///\n			-INSTRUCTIONS ON ACCUMULATOR AND DATA REGISTER :
///\n				=> 0 0 X X -> LFD  -> LOAD FROM DR(AC <- DR) 		            -> LOAD THE VALUE OF THE DATA REGISTER INTO AC.
///\n				=> 0 1 X X -> LDC  -> LOAD DATA COMPLIMENT(AC <- ~DR)	        -> LOAD THE COMPLIMENT OF THE VALUE OF DR INTO AC.
///\n				=> 0 2 X X -> ADD  -> ADD(DR + AC) 			                    -> ADD THE VALUE OF DR AND AC AND STORE IT IN AC.
///\n				=> 0 3 X X -> MUL  -> MULTIPLY(DR * AC) 		                -> MULTIPLY THE VALUE OF DR WITH AC.
///\n				=> 0 4 X X -> SUB  -> SUBTRACT(AC <- DR + (~AC + 1))	        -> SUBTRACT AC FROM DR.
///\n				=> 0 5 X X -> DIV  -> DIVIDE(AC <- REMAINDER OF AC/DR, QR <- AC/DR) -> DIVIDE AC WITH DR.
///\n				=> 0 6 X X -> CMP  -> COMPARE THE VALUES OF DR AND AC           -> WILL SET THREE BITS BASED ON THE COMPARISON(SMALLER, EQUAL, GREATER).
///\n				=> 0 7 X X -> AND  -> BITWISE AND(AC <- AC && DR)   	        -> BITWISE AND WITH AC.
///\n				=> 0 8 X X -> BOR  -> BITWISE OR(AC <- AC || DR)	            -> BITWISE OR WITH AC.
///\n				=> 0 9 X X -> XOR  -> BITWISE XOR(AC <- AC ///\n DR) 	        -> BITWISE XOR WITH AC.
///\n				=> 0 A X X -> SHR  -> SHIFT AC TO RIGHT 		                -> SHIFT THE AC VALUE TO THE RIGHT.
///\n				=> 0 B X X -> SHL  -> SHIFT AC TO THE LEFT		                -> SHIFT THE AC VALUE TO THE LEFT.
///\n			    => 0 D X X -> RND  -> RANDOM NUMBER IN AC                       -> LOAD A RANDOM NUMBER IN AC.
///\n               => 0 E X X -> IBO  -> INCREMENT BY ONE                          -> AC <= AC+1
///\n
///\n			-INSTRUCTIONS TO SET THE COMPARISON BITS :
///\n               => 8 7 X X -> CSC ->  COMPARISON STATUS BIT -> COMPARE REGISTER CARRY(SETS THE COMPARE REGISTER THE PRESENT VALUE OF THE CARRY FLIP FLOP)
///\n				=> 8 8 X X -> CSO ->  COMPARISON STATUS BIT -> COMPARE REGISTER ONE(SETS THE COMPARE ONE WITHOUT CHECKING ANY OTHER STUFF)
///\n				=> 8 9 X X -> CSL ->  COMPARISON STATUS BIT -> COMPARE REGISTER(LESSER)(COMPARE SET LESSER)
///\n				=> 8 A X X -> CSE ->  COMPARISON STATUS BIT -> COMPARE REGISTER(EQUAL)
///\n				=> 8 B X X -> CSG ->  COMPARISON STATUS BIT -> COMPARE REGISTER(GREATER)
///\n
///\n			-EXTRA
///\n				=> 0 C X X -> SSI -> SET SERIAL INPUT TO 1		-> SET THE SERIAL INPUTS FOR THE SHIFT OPERATIONS.(IT WILL BE 0 BY DEFAULT EVERY TIME)
///\n				=> 0 F X X -> HLT -> PROGRAM HALT 			    -> HALT THE EXECUTION.
///\n
///\n		-INSTRUCTIONS RELATED I/O :
///\n			=> 8 C X X -> LOAD THE VALUE FROM INPUT REGISTER INTO THE DATA REGISTER.
///\n			=> 8 D X X -> PLACE THE VALUE OF AC IN THE OUTPUT REGISTER.
///\n           => 8 E X X -> SET THE INTERRUPT ENABLE ON.
///\n           => 8 F X X -> SET THE INTERRUPT ENABLE OF.
///\n
///\n \n*************************************************************
///\n FORMAT IN WHICH THE CODE SHOULD BE PROVIDED :
///\n *** a simple 'hasm' code that calculates 15! and outputs the number.
///\n
///\n .data:
///\n   num = 1;
///\n   max = 16;
///\n   output = 1;
///\n   tmp = 0;
///\n
///\n _loop:
///\n   lfs;
///\n   mds;
///\n   ibo;
///\n   str tmp;
///\n   lfa tmp;
///\n   psh;
///\n   lfa max;
///\n   cmp;
///\n   jmp loop;
///\n   ret;
///\n
///\n
///\n _main:
///\n   ief;
///\n   cso;
///\n   call loop;
///\n   ret;
///\n
///\n
///\n .instructions:
///\n   lfa output;
///\n   psh;
///\n   lfa num;
///\n   psh;
///\n   call main;
///\n   out output;
///\n   hlt;
///\n


///\n output type : "v3.0 hex words addressed"

struct [[maybe_unused]] synthasm{};
//*************************************************************

////In the 7 instructions that require you to give the operand as an address or data,
////  6 of them have two types of addressing modes(direct and indirect)
////  1 of them has an immediate mode.
//enum AddressingMode {
//    DirectAddress,
//    IndirectAddress,
//    ImmediateAddress,
//    NoAddress,//for the stuff that does not need an operand.
//};

//first 4 bits of the instruction in the memory
const std::unordered_map<std::string, char> FIRST_BIT_MAP = {
        {"psh" , '8'},
        {"pop" , '8'},
        {"ads" , '8'},
        {"sds" , '8'},
        {"mds" , '8'},
        {"dds" , '8'},
        {"lfs" , '8'},

        {"csc" , '8'},
        {"cso" , '8'},
        {"csl" , '8'},
        {"cse" , '8'},
        {"csg" , '8'},

        {"inp" , '8'},
        {"out" , '8'},
        {"ien" , '8'},
        {"ief" , '8'},

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
        {"hlt" , '0'},
        {"ssi" , '0'},
        {"ibo" , '0'},
        {"ssi" , '0'},

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
        {"lfs" , '6'},

        {"csc" , '7'},
        {"cso" , '8'},
        {"csl" , '9'},
        {"cse" , 'a'},
        {"csg" , 'b'},

        {"inp" , 'c'},
        {"out" , 'd'},
        {"ien" , 'e'},
        {"ief" , 'f'},

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
        {"hlt" , 'f'},
        {"ssi" , 'c'},
        {"ibo" , 'e'},
        {"ssi" , 'c'},

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
unsigned short PRESENT_ADDRESS = 0x101;
unsigned short DATA_START_ADDRESS = 0x0;//need to change this while running based on the amount of instructions.
unsigned short RAM_FORMAT_LENGTH = 8;
unsigned short TRACKING_ADDRESS = 0X101;

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


    //****************************************************************
    //START ASSEMBLING!
    //****************************************************************

    //NEED TO IMPROVE , NO NEED FOR TWO LOOPS.

    std::stringstream OutputInOrder;


    //positions of the 'data:' and 'instructions:'
    std::tuple<unsigned short , unsigned short> Data, Instructions;
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
    //stores <token_name, start_index, end_index>
    std::vector<std::tuple<std::string, unsigned short, unsigned short>> Subroutines;
    //stores the token name to its address map.
    std::unordered_map<std::string, unsigned short> SubroutineMap;
    int SubroutineNumber = 0;
    index1 = 0;
    for (auto& token : tokens) {
        if (token.at(0) == '_') {
            token.erase(0, 1);
            Subroutines.emplace_back(token, index1, 0);
            if (SubroutineNumber != 0) {
                Subroutines[SubroutineNumber-1] = std::make_tuple(std::get<0>(Subroutines[SubroutineNumber-1]), std::get<1>(Subroutines[SubroutineNumber-1]), index1);
            }
            SubroutineNumber++;
        }
        index1++;
    }

    unsigned short DataEnd;
    if (Subroutines.empty()) {
        DataEnd = std::get<0>(Instructions);
    } else {
        DataEnd = std::get<1>(Subroutines[0]);
    }

    //ASSEMBLING DATA
    //Stores the variable name and the address at which it is stored.
    std::unordered_map<std::string, unsigned short> DataAddressMap;
    //Stores the data part of the code in a sequential order, which is later used to write into the output file.
    std::vector<std::string> MachineStringData;
    for (unsigned short TokenIndex = std::get<0>(Data) + 1; TokenIndex < DataEnd; TokenIndex += 2) {
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
    //the last subroutine's ending index will not be set, so we need to set it.
    if (!Subroutines.empty()) {
        Subroutines[Subroutines.size()-1] = std::make_tuple(std::get<0>(Subroutines[Subroutines.size()-1]), std::get<1>(Subroutines[Subroutines.size()-1]), std::get<0>(Instructions));
    }

    ///the first two instructions at 0x000 are setting the conditional bit to 1 and jumping to the address at which instructions start indirectly.
    OutputInOrder << "0x000: 8800 b100 ";
    for (unsigned short i = 0x2; i < 0x100; i++) {
        if (i % RAM_FORMAT_LENGTH == 0) {
            OutputInOrder << "\n";
            OutputInOrder << "0x" << std::setw(3) << std::setfill('0') << std::hex << DATA_START_ADDRESS << ": ";
            DATA_START_ADDRESS += RAM_FORMAT_LENGTH;
        }
        OutputInOrder << short_to_nibble(0) << " ";
    }

    std::stringstream SubroutinesNInstructions;
    //PRESENT_ADDRESS++;
    auto new_o = 0x101;
    for (const auto& i : MachineStringData) {
        if (new_o % RAM_FORMAT_LENGTH == 0) {
            SubroutinesNInstructions << "\n";
            SubroutinesNInstructions << "0x" << std::setw(4) << std::hex << new_o << ": ";
        }
        SubroutinesNInstructions << i << " ";
        new_o++;
    }


    //ASSEMBLING_SUBROUTINES
    std::unordered_map<std::string, unsigned short> SubroutineAddressMap;
    for (int SubroutineNumberIndex = 0; SubroutineNumberIndex < Subroutines.size(); SubroutineNumberIndex++) {
        auto Subroutine = Subroutines[SubroutineNumberIndex];
        //address of this subroutine with one free word left at the beginning.
        SubroutineAddressMap.emplace(std::get<0>(Subroutine), PRESENT_ADDRESS);
        auto latest_return = PRESENT_ADDRESS;
        if (PRESENT_ADDRESS % RAM_FORMAT_LENGTH == 0) {
            SubroutinesNInstructions << "\n";
            SubroutinesNInstructions << "0x" << std::setw(3) << std::hex << PRESENT_ADDRESS << ": ";
        }
        SubroutinesNInstructions << "0000 ";
        PRESENT_ADDRESS++;
        for (unsigned short SubroutineInstructionAddress = std::get<1>(Subroutine) + 1;
             SubroutineInstructionAddress < std::get<2>(Subroutine);) {
            try {
                if (PRESENT_ADDRESS % RAM_FORMAT_LENGTH == 0) {
                    SubroutinesNInstructions << "\n";
                    SubroutinesNInstructions << "0x" << std::setw(3) << std::hex << PRESENT_ADDRESS << ": ";
                }
                char first_hex = FIRST_BIT_MAP.at(tokens[SubroutineInstructionAddress]);
                char second_hex = SECOND_BIT_MAP.at(tokens[SubroutineInstructionAddress]);
                if (first_hex != '0' && first_hex != '8') {
                    //for immediate addressing mode
                    auto &instn = tokens[SubroutineInstructionAddress];
                    if (instn == "lid" || instn == "lid%") {
                        SubroutinesNInstructions << first_hex
                                                 << short_to_nibble(std::stoi(tokens[SubroutineInstructionAddress + 1]))
                                                 << " ";
                        SubroutineInstructionAddress += 2;
                    } else if (instn == "lfa" || instn == "str" || instn == "lfa%" || instn == "str%") {
                        SubroutinesNInstructions << first_hex << short_to_nibble_three(
                                DataAddressMap.at(tokens[SubroutineInstructionAddress + 1])) << " ";
                        SubroutineInstructionAddress += 2;
                    } else if (instn == "ret" || instn == "ret%") {
                        SubroutinesNInstructions << first_hex << short_to_nibble_three(latest_return) << " ";
                        SubroutineInstructionAddress++;
                    } else {
                        SubroutinesNInstructions << first_hex << short_to_nibble_three(
                                SubroutineAddressMap.at(tokens[SubroutineInstructionAddress + 1])) << " ";
                        SubroutineInstructionAddress += 2;
                    }
                } else {
                    SubroutinesNInstructions << first_hex << second_hex << "00 ";
                    ++SubroutineInstructionAddress;
                }
            } catch (std::out_of_range &ree) {
                std::cerr << "Don't know, what : '" << tokens[SubroutineInstructionAddress] << "' means!!";
                return -1;
            }
            PRESENT_ADDRESS++;
        }
    }

    auto InstructionStart = PRESENT_ADDRESS;

    //Here we are going to store the instructions in the sequence.
    //ASSEMBLING_INSTRUCTIONS
    //Instructions are always going to be the last part of the file.
    for (unsigned short InstructionIndex = std::get<0>(Instructions)+1; InstructionIndex < tokens.size();) {
        std::string* Instruction = &tokens[InstructionIndex];
        try {
            char first_nibble = FIRST_BIT_MAP.at(*Instruction);
            char second_nibble = SECOND_BIT_MAP.at(*Instruction);
            if (PRESENT_ADDRESS % RAM_FORMAT_LENGTH == 0) {
                SubroutinesNInstructions << "\n";
                SubroutinesNInstructions << "0x" << std::setw(3) << std::hex << PRESENT_ADDRESS << ": ";
            }
            //if we can just get the whole word from the first two bits.
            if (first_nibble == '0' || first_nibble == '8') {
                SubroutinesNInstructions << first_nibble << second_nibble << "00 ";
                //because this does not have an address.
                InstructionIndex += 1;
            } else {//if the address is needed.
                try {
                    std::string* InstructionAddress = &tokens[InstructionIndex + 1];
                    if (*Instruction == "lid" || *Instruction == "lid%") {
                        SubroutinesNInstructions << first_nibble << std::setw(3) << std::hex << std::stoi(*InstructionAddress) << " ";
                    } else if (*Instruction == "lfa" || *Instruction == "str" || *Instruction == "lfa%" || *Instruction == "str%") {
                        SubroutinesNInstructions << first_nibble <<  std::setw(3) << std::hex << DataAddressMap.at(*InstructionAddress) << " ";
                    } else if (*Instruction == "ret" || *Instruction == "ret%") {///if return is used in the main instructions then we are
                        SubroutinesNInstructions << first_nibble << short_to_nibble_three(std::stoi(*InstructionAddress)) << " ";
                    } else {
                        SubroutinesNInstructions << first_nibble << SubroutineAddressMap.at(*InstructionAddress) << " ";
                    }
                } catch(const std::out_of_range& err) {
                    std::cout << "The symbol : '" << tokens[InstructionIndex+1] << "', is not found in the present scope" << std::endl;
                    return -1;
                }
                InstructionIndex += 2;
            }
        } catch (const std::out_of_range& e) {
            std::cerr << "No instruction matching with the given instruction name : '" << *Instruction << std::endl;
            return -1;
        }
        PRESENT_ADDRESS += 1;
    }

    for (;PRESENT_ADDRESS <= 0xfff; PRESENT_ADDRESS++) {
        if (PRESENT_ADDRESS % RAM_FORMAT_LENGTH == 0) {
            SubroutinesNInstructions << "\n";
            SubroutinesNInstructions << "0x" << std::setw(3) << std::hex << PRESENT_ADDRESS << ": ";
        }
        SubroutinesNInstructions << std::setw(4) << std::hex << "0000 ";
    }


    //Set the value at which we are jumping.
    OutputInOrder << "\n0x100: " << std::setw(4) << std::hex << InstructionStart << " ";
    OutputInOrder << SubroutinesNInstructions.str();
    OutputFile << OutputInOrder.str();
    std::cout << "Saving at : " << output_file_name << "\n";
    OutputFile.close();
    return 1;
}
