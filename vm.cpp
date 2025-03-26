#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <map>
#define MEM_SIZE 100//Total memory size for virtual machine
using namespace std;

//Register initialization with default values
map<string, int> reg = {{"a", 0}, {"b", 0}, {"c", 0}, {"d", 0}, {"e", 0}, {"f", 0}, //Genral purpose Registers
	                {"sp", 0},       //Stack pointer
			{"acc", 0},      //Accumulator
			{"pc", 0},       //Program counter
			{"ivec", 0},     //Interrupt vector
			{"int", 0},      //Interrupt flag
			{"timer", 0},    //Timer for interrupts
			{"halt", 0}};    //Halt flag


vector<vector<string>> memory(MEM_SIZE);//memory representation-each location is a vector of string


// Initializes all memory locations with "0" to prevent segmentation faults from accessing uninitialized memory.
void initializeMemory() 
{
    for (int i = 0; i < MEM_SIZE; i++) 
    {
        memory[i] = vector<string>(1, "0");// Each memory location contains at least "0"
    }
}



// Instruction Implementations*********============================================================================================================================================
 
/* MOV instruction - moves value from one register to another
 * @param opr Vector containing destination and source registers*/
void mov(vector<string> opr) 
{
    reg[opr[0]] = reg[opr[1]];
    reg["pc"]++;
}

/* MOVV instruction - moves immediate value into a register
 * @param opr Vector containing destination register and immediate value*/
void movv(vector<string> opr) 
{
    reg[opr[0]] = stoi(opr[1]);
    reg["pc"]++;
}

 /* LOAD instruction - loads value from memory into register
 * @param opr Vector containing destination register and memory address*/
void load(vector<string> opr) 
{
    int addr = stoi(opr[1]);
    if (addr < 0 || addr >= MEM_SIZE || memory[addr].empty()) 
    {
        cerr << "Error: Invalid memory access at address " << addr << endl;
        reg["halt"] = 1;
        return;
    }
    reg[opr[0]] = stoi(memory[addr][0]);
    reg["pc"]++;
}

 /* LOADR instruction - loads value from memory (address in register) into register
 * @param opr Vector containing destination register and register holding address*/
void loadr(vector<string> opr) 
{
    int addr = reg[opr[1]];
    if (addr < 0 || addr >= MEM_SIZE || memory[addr].empty()) 
    {
        cerr << "Error: Invalid memory access at address " << addr << endl;
        reg["halt"] = 1;
        return;
    }
    reg[opr[0]] = stoi(memory[addr][0]);
    reg["pc"]++;
}

 /* STORE instruction - stores register value into memory
 * @param opr Vector containing memory address and source register*/
void store(vector<string> opr) 
{
    int addr = stoi(opr[0]);
    if (addr < 0 || addr >= MEM_SIZE) 
    {
        cerr << "Error: Invalid memory access at address " << addr << endl;
        reg["halt"] = 1;
        return;
    }
    memory[addr][0] = to_string(reg[opr[1]]);
    reg["pc"]++;
}

 /* STORER instruction - stores register value into memory (address in register)
 * @param opr Vector containing register holding address and source register*/
void storer(vector<string> opr) 
{
    int addr = reg[opr[0]];
    if (addr < 0 || addr >= MEM_SIZE) 
    {
        cerr << "Error: Invalid memory access at address " << addr << endl;
        reg["halt"] = 1;
        return;
    }
    memory[addr][0] = to_string(reg[opr[1]]);
    reg["pc"]++;
}

 /* ADD instruction - adds two registers and stores result in accumulator
 * @param opr Vector containing two registers to add*/
void add(vector<string> opr) 
{
    reg["acc"] = reg[opr[0]] + reg[opr[1]];
    reg["pc"]++;
}

 /* SUB instruction - subtracts two registers and stores result in accumulator
 * @param opr Vector containing two registers to subtract*/
void sub(vector<string> opr) 
{
    reg["acc"] = reg[opr[0]] - reg[opr[1]];
    reg["pc"]++;
}

 /* MOD instruction - calculates modulus of two registers and stores result in accumulator
 * @param opr Vector containing two registers for modulus operation*/
void mod(vector<string> opr) 
{
    reg["acc"] = reg[opr[0]] % reg[opr[1]];
    reg["pc"]++;
}

 /* CALL instruction - calls a subroutine at specified address
 * @param opr Vector containing target address*/
void call(vector<string> opr) 
{
    reg["sp"]++;
    if (reg["sp"] >= MEM_SIZE) 
    {
        cerr << "Error: Stack overflow at sp = " << reg["sp"] << endl;
        reg["halt"] = 1;
        return;
    }
    memory[reg["sp"]][0] = to_string(reg["pc"] + 1);// Save return address
    reg["pc"] = stoi(opr[0]);//Jump to subroutine
}

 /* RET instruction - returns from subroutine
 * @param opr Unused parameter (maintained for consistency)*/
void ret(vector<string> opr) 
{
    if (reg["sp"] < 0) 
    {
        cerr << "Error: Stack underflow at sp = " << reg["sp"] << endl;
        reg["halt"] = 1;
        return;
    }
    reg["pc"] = stoi(memory[reg["sp"]][0]);
    reg["sp"]--;
}

 /* OUT instruction - outputs register value to console
 * @param opr Vector containing register to output*/
void out(vector<string> opr) 
{
    cout << reg[opr[0]] << endl;
    reg["pc"]++;
}

 /* PUSH instruction - pushes register value onto stack
 * @param opr Vector containing register to push*/
void push(vector<string> opr) 
{
    if (reg["sp"] + 1 >= MEM_SIZE) 
    {
        cerr << "Error: Stack overflow at sp = " << reg["sp"] << endl;
        reg["halt"] = 1;
        return;
    }
    reg["sp"]++;
    memory[reg["sp"]][0] = to_string(reg[opr[0]]);
    reg["pc"]++;
}

 /* POP instruction - pops value from stack into register
 * @param opr Vector containing destination register*/
void pop(vector<string> opr) 
{
    if (reg["sp"] < 0) 
    {
        cerr << "Error: Stack underflow at sp = " << reg["sp"] << endl;
        reg["halt"] = 1;
        return;
    }
    reg[opr[0]] = stoi(memory[reg["sp"]][0]);
    reg["sp"]--;
    reg["pc"]++;
}

 /* JMP instruction - unconditional jump to address
 * @param opr Vector containing target address*/
void jmp(vector<string> opr) 
{
    reg["pc"] = stoi(opr[0]);
}

 /* JNZ instruction - jump to address if register is not zero
 * @param opr Vector containing target address and register to test*/
void jnz(vector<string> opr) 
{
    if (reg[opr[1]] != 0) 
    {
        reg["pc"] = stoi(opr[0]);
    }
    else 
    {
        reg["pc"]++;
    }
}

 /* HALT instruction - stops program execution
 * @param opr Unused parameter (maintained for consistency)*/
void halt(vector<string> opr) 
{
    reg["halt"] = 1;
    reg["pc"]++;
}

// Main execution loop - fetches and executes instructions until halt 
void runm() 
{
    while (reg["halt"] == 0) 
    {
        int i = reg["pc"];
        if (i < 0 || i >= MEM_SIZE || memory[i].empty()) // Check for invalid program counter
	{
            cerr << "Error: Invalid memory access at pc = " << i << endl;
            reg["halt"] = 1;
            break;
        }

        string op = memory[i][0];// Get opcode
        vector<string> opr(memory[i].begin() + 1, memory[i].end());// Get operands

	// Instruction dispatch
        if (op == "mov") mov(opr);
        else if (op == "movv") movv(opr);
        else if (op == "load") load(opr);
        else if (op == "loadr") loadr(opr);
        else if (op == "store") store(opr);
        else if (op == "storer") storer(opr);
        else if (op == "add") add(opr);
        else if (op == "sub") sub(opr);
        else if (op == "mod") mod(opr);
        else if (op == "call") call(opr);
        else if (op == "ret") ret(opr);
        else if (op == "out") out(opr);
        else if (op == "push") push(opr);
        else if (op == "pop") pop(opr);
        else if (op == "jmp") jmp(opr);
        else if (op == "jnz") jnz(opr);
        else if (op == "halt") halt(opr);
        else 
	{
            cerr << "Error: Invalid instruction '" << op << "' at pc = " << i << endl;
            reg["halt"] = 1;
        }

	// Timer and interrupt handling
        reg["timer"]--;
        if (reg["int"] == 1 && reg["timer"] == 0) 
	{
            reg["sp"]++;
            if (reg["sp"] >= MEM_SIZE) 
	    {
                cerr << "Error: Stack overflow during interrupt at sp = " << reg["sp"] << endl;
                reg["halt"] = 1;
                continue;
            }
            memory[reg["sp"]][0] = to_string(reg["pc"]);// Save current PC
            reg["pc"] = reg["ivec"];// Jump to interrupt vector
            reg["int"] = 0;// Clear interrupt flag
        }
    }
}

int main() 
{
    initializeMemory();// Initialize all memory locations
    
    ifstream file("prog.asm");// Load program from file
    if (!file.is_open()) 
    {
        cerr << "Error: Could not open file prog.asm" << endl;
        return 1;
    }

    // Parse program file
    string line;
    while (getline(file, line)) 
    {
        if (line.empty() || line[0] == '#') continue;// Skip comments and empty lines

	// Tokenize line
        stringstream ss(line);
        string word;
        vector<string> tokens;

        while (ss >> word) 
	{
            tokens.push_back(word);
        }

	// Store instruction in memory
        if (!tokens.empty()) 
	{
            int addr = stoi(tokens[0]);
            if (addr < 0 || addr >= MEM_SIZE) 
	    {
                cerr << "Error: Invalid memory address " << addr << " in program" << endl;
                continue;
            }
            memory[addr] = vector<string>(tokens.begin() + 1, tokens.end());
        }
    }

    // Execute program
    runm();

    // Print final register states
    cout << "\nRegister states:\n";
    for (const auto& [key, value] : reg) 
    {
        cout << key << ": " << value << endl;
    }

    // Print non-empty memory locations
    cout << "\nMemory contents:\n";
    for (int i = 0; i < MEM_SIZE; i++) 
    {
        if (!memory[i].empty()) 
	{
            cout << i << ": ";
            for (const auto& val : memory[i]) 
	    {
                cout << val << " ";
            }
            cout << endl;
        }
    }

    return 0;
}
