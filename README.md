# SynthBite16


A 16-Bit computer(C.I.S.C) with 38 Instructions, two addressing modes and a register stack!

![FINAL_CIRC__](https://github.com/Harsha-vardhan-R/SynthBite16/assets/112687561/1a8ee2a1-77d8-4e2d-a184-413fc60d4e6e)

Usage:<br /><br />
  1) Play around with the computer.<br /><br />
        Clone into the repo and open the '.circ' with [logisim-evolution](https://github.com/logisim-evolution/logisim-evolution)<br />
        ```
        git clone https://github.com/Harsha-vardhan-R/SynthBite16
        ```
        There are a couple of examples in the `examples` directory in the cloned repo, mosty it will not be enough so to write your own program: <br />
             &nbsp;&nbsp;&nbsp;&nbsp;* Refer to the `Docs.txt` to see the Instructions and what exactly they do. <br />
             &nbsp;&nbsp;&nbsp;&nbsp;* compile the program with the assembler whose binary is provided with these commands.(if you want to compile it yourself just use g++ on main.cpp)(it is a single file program) <br />
             ```
              ./Assembler <input file> <output file name>
             ```
             &nbsp;&nbsp;&nbsp;&nbsp;* load the RAM in logisim with the output file from the `Assembler`. <br />
             &nbsp;&nbsp;&nbsp;&nbsp;* Reset all the registers in the computer with the `COMPUTER REBOOT` button. <br /><br />
        *** ALWAYS THE SIMULATION SHOULD START FROM LOW CLOCK VALUE!!! <br /><br />
 2) Found Bugs? <br />
    &nbsp;&nbsp;&nbsp;&nbsp;- If you think you found a bug and can fix it, please try to fix it and send a PULL REQUEST. <br />
    &nbsp;&nbsp;&nbsp;&nbsp;- else raise an Issue so that anyone who s intrested can fix it. <br />

Instruction set of the computer : <br />

```
-OPERAND IS REQUIRED : 
    -DIRECT AND INDIRECT ADDRESSING
            => 1 X X X | 9 X X X -> LFA -> LOAD       -> LOAD THE VALUE FROM THE ADDRESS INTO DR.
            => 2 X X X | A X X X -> STR -> STORE      -> STORES THE PRESENT VALUE OF AC IN THE OPERAND ADDRESS.
            => 3 X X X | B X X X -> JMP -> JUMP_COND  -> JUMP CONDITIONALLY TO THE GIVEN ADDRESS.
            => 4 X X X | C X X X -> CAL -> CALL       -> CALL A SUBROUTINE.
            => 5 X X X | D X X X -> RET -> RETURN     -> RETURN FROM A SUBROUTINE.
            => 6 X X X | E X X X -> LDI -> LOAD IMM   -> LOAD IMMEDIATE VALUE INTO THE AC, (THE HIGHEST 4 BITS WILL BE FILLED WITH 0).
            => 7 X X X | F X X X -> SWP -> SWAP       -> SWAPS THE VALUES OF AC AND THE OPERAND.

    -OPERAND IS NOT REQUIRED : INSTRUCTION FORMAT 0-X-X-X OR 8-X-X-X

        -INSTRUCTIONS MANIPULATING DATA :

            -INSTRUCTIONS ON THE STACK :
                => 8 0 X X -> PSH  -> PUSH ONTO THE STACK                     -> (STACK TOP <- DR, SP <- SP + 1)
                => 8 1 X X -> POP  -> POP FROM THE STACK                      -> POP THE VALUE FROM THE STACK TOP AND PLACE IT IN THE OUT BUFFER REGISTER.
                => 8 2 X X -> ADS  -> ADD THE TOP TWO ELEMENTS IN THE STACK   -> (STACK TOP <- STACK TOP + STACK TOP AFTER A POP)
                => 8 3 X X -> SDS  -> SUBTRACT ON THE STACK                   -> (STACK TOP <- STACK TOP - STACK TOP AFTER A POP)
                => 8 4 X X -> MDS  -> MULTIPLY ON THE STACK                   -> (STACK TOP <- STACK TOP * STACK TOP AFTER A POP)
                => 8 5 X X -> DDS  -> DIVIDE ON THE STACK                     -> (STACK TOP <- STACK TOP / STACK TOP AFTER A POP)
                => 8 6 X X -> LFS  -> LOAD FROM STACK TOP                     -> (AC <- STACK TOP)

            -INSTRUCTIONS ON ACCUMULATOR AND DATA REGISTER : 
                => 0 0 X X -> LFD  -> LOAD FROM DR(AC <- DR)                       -> LOAD THE VALUE OF THE DATA REGISTER INTO AC.
                => 0 1 X X -> LDC  -> LOAD DATA COMPLIMENT(AC <- ~DR)              -> LOAD THE COMPLIMENT OF THE VALUE OF DR INTO AC.
                => 0 2 X X -> ADD  -> ADD(DR + AC)                                 -> ADD THE VALUE OF DR AND AC AND STORE IT IN AC.
                => 0 3 X X -> MUL  -> MULTIPLY(DR * AC)                            -> MULTIPLY THE VALUE OF DR WITH AC.
                => 0 4 X X -> SUB  -> SUBTRACT(AC <- DR + (~AC + 1))               -> SUBTRACT AC FROM DR.
                => 0 5 X X -> DIV  -> DIVIDE(AC <- REMAINDER OF AC/DR, QR <- AC/DR)-> DIVIDE AC WITH DR.
                => 0 6 X X -> CMP  -> COMPARE THE VALUES OF DR AND AC              -> WILL SET THREE BITS BASED ON THE COMPARISION(SMALLER, EQUAL, GREATER).
                => 0 7 X X -> AND  -> BITWISE AND(AC <- AC && DR)                  -> BITWISE AND WITH AC.
                => 0 8 X X -> BOR  -> BITWISE OR(AC <- AC || DR)                   -> BITWISE OR WITH AC.
                => 0 9 X X -> XOR  -> BITWISE XOR(AC <- AC ^ DR)                   -> BITWISE XOR WITH AC.
                => 0 A X X -> SHR  -> SHIFT AC TO RIGHT                            -> SHIFT THE AC VALUE TO THE RIGHT.
                => 0 B X X -> SHL  -> SHIFT AC TO THE LEFT                         -> SHIFT THE AC VALUE TO THE LEFT.
                => 0 D X X -> RND  -> RANDOM NUMBER IN AC                          -> LOAD A RANDOM NUMBER IN AC.
                => 0 E X X -> IBO  -> INCREMENT BY ONE                             -> AC <= AC+1

            -INSTRUCTIONS TO SET THE COMPARISION BITS : 
                => 8 7 X X -> CSC ->  COMPARISION STATUS BIT -> COMPARE REGISTER CARRY(SETS THE COMPARE REGISTER THE PRESENT VALUE OF THE CARRY FLIP FLOP)
                => 8 8 X X -> CSO ->  COMPARISION STATUS BIT -> COMPARE REGISTER ONE(SETS THE COMPARE ONE WITHOUT CHECKING ANY OTHER STUFF)
                => 8 9 X X -> CSL ->  COMPARISION STATUS BIT -> COMPARE REGISTER(LESSER)(COMPARE SET LESSER)
                => 8 A X X -> CSE ->  COMPARISION STATUS BIT -> COMPARE REGISTER(EQUAL)
                => 8 B X X -> CSG ->  COMPARISION STATUS BIT -> COMPARE REGISTER(GREATER)

            -EXTRA
                => 0 C X X -> SSI -> SET SERIAL INPUT TO 1        -> SET THE SERIAL INPUTS FOR THE SHIFT OPERATIONS.(IT WILL BE 0 BY DEFAULT EVERY TIME)
                => 0 F X X -> HLT -> PROGRAM HALT                 -> HALT THE EXECUTIION.

        -INSTRUCTIONS RELATED I/O : 
            => 8 C X X -> INP -> LOAD THE VALUE FROM INPUT REGISTER INTO THE DATA REGISTER.
            => 8 D X X -> OUT -> PLACE THE VALUE OF AC IN THE OUPUT REGISTER.
            => 8 E X X -> IEN -> SET THE INTERRUPT ENABLE ON.
            => 8 F X X -> IEF -> SET THE INTERRUPT ENABLE OF.

```
*** ROM IS NOT WRITTEN FOR DIVISION AND DIVISION ON THE STACK.
    INTERRUPT NOT TESTED.
