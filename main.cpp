#include <bits/stdc++.h>
using namespace std;

#define __LINUX__
//#define _WIN32

#define ll long long
#define INF ((ll)(1e9+7))
#define fo(i, n) for(ll i=0;i<((ll)n);i++)
#define deb(x) cout << #x << " = " << (x) << endl;
#define deb2(x, y) cout << #x << " = " << (x) << ", " << #y << " = " << (y) << endl
#define pb push_back
#define F first
#define S second
#define LSOne(S) ((S) & (-S))
#define all(x) x.begin(), x.end()
#define rall(x) x.rbegin(), x.rend()
typedef pair<ll, ll> pl;
typedef vector<int> vi;
typedef vector<ll> vl;
typedef vector<pl> vpl;
typedef vector<vl> vvl;
typedef vector<vpl> vvpl;

const int INTERVAL = 150;
const int MEMORY_SIZE = 4096;
const int START_ADDRESS = 0x200;

const unsigned int FONTSET_SIZE = 80;
const unsigned int FONTSET_START_ADDRESS = 0x50;

uint8_t fontset[FONTSET_SIZE] =
{
	0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
	0x20, 0x60, 0x20, 0x20, 0x70, // 1
	0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
	0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
	0x90, 0x90, 0xF0, 0x10, 0x10, // 4
	0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
	0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
	0xF0, 0x10, 0x20, 0x40, 0x40, // 7
	0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
	0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
	0xF0, 0x90, 0xF0, 0x90, 0x90, // A
	0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
	0xF0, 0x80, 0x80, 0x80, 0xF0, // C
	0xE0, 0x90, 0x90, 0x90, 0xE0, // D
	0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
	0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};


struct Chip8 {
    vector<uint8_t> memory;
    vector<uint8_t> reg;
    uint16_t POS; 
    uint16_t I;
    uint32_t video[64 * 32];
    string command;

    Chip8() : memory(MEMORY_SIZE, 0), reg(16), POS(START_ADDRESS), video() {
        // Load fontset into memory
        for(int i = 0; i < FONTSET_SIZE; i++){
            memory[FONTSET_START_ADDRESS + i] = fontset[i];
        }
    }

    // Load ROM instructions into memory, return length of program
    int load_instructions(string path){
        ifstream file(path);
        
        if(file.is_open()){
            int size = 0;
            uint16_t r;

            do {
                file >> hex >> r;

                memory[START_ADDRESS + size * 2] = (r >> 8) & 0xFF;
                memory[START_ADDRESS + size * 2 + 1] = r & 0xFF;

                size++;
            } while(r != 0);


            return (int)size;
        }

        return 0;
    }

    void emulateCycle() {
        uint16_t opcode = (memory[POS] << 8)|memory[POS+1];

        POS += 2; 
        
        //cout << hex << opcode << "\n";

        switch(opcode&0xF000){
            case 0x0000: {
                if (opcode&0x00E0){// clear screen
                    memset(video, 0, sizeof(video));
                    command = "disp_clear()";
                }
                break;
            }
            case 0x1000: // jump (relative to START_ADDRESS!!!)
                POS = START_ADDRESS + opcode & 0xFFF;
                command = "jump to " + to_string((int)POS);
                break;
            case 0x3000: // Vx == NN
                if((int)reg[(opcode&0x0F00) >> 8] == (int)(opcode&0x00FF)) POS+=2;
                command = "if(Vx == NN)";
                break;
            case 0x4000: // Vx != NN
                if((int)reg[(opcode&0x0F00) >> 8] != (int)opcode&0x00FF) POS+=2;
                command = "if(Vx != NN)";
                break;
            case 0x5000: // Vx==Vy 
                if((int)reg[(opcode&0x0F00) >> 8] == (int)reg[(opcode&0x00F0) >> 4]) POS+=2;
                command = "if(Vx==Vy)";
                break;
            case 0x6000: // Vx = NN
                reg[(opcode&0x0F00) >> 8] = opcode&0x00FF;
                command = "Vx = NN";
                break;
            case 0x7000: // Vx += NN
                reg[(opcode&0x0F00) >> 8] += opcode&0x00FF;
                command = "Vx += NN";
                break;
            case 0x8000: {
                switch (opcode & 0x000F) {
                    case 0x0: // Vx = Vy
                        reg[(opcode&0x0F00)>>8] = reg[(opcode&0x00F0) >> 4];
                        command = "Vx = Vy";
                        break;
                    case 0x1: // Vx |= Vy
                        reg[(opcode&0x0F00) >> 8] |= reg[(opcode&0x00F0) >> 4];
                        command = "Vx |= Vy";
                        break;
                    case 0x2: // Vx &= Vy
                        reg[(opcode&0x0F00) >> 8] &= reg[(opcode&0x00F0) >> 4];
                        command = "Vx &= Vy";
                        break;
                    case 0x3: // Vx ^= Vy
                        reg[(opcode&0x0F00) >> 8] ^= reg[(opcode&0x00F0) >> 4];
                        command = "Vx ^= Vy";
                        break;
                    case 0x4: // Vx += Vy
                        if (255-reg[(opcode&0x0F00) >> 8] < reg[(opcode&0x00F0) >> 4])reg[15] = 1;
                        else reg[15] = 0;
                        reg[(opcode&0x0F00) >> 8] += reg[(opcode&0x00F0) >> 4];
                        command = "Vx += Vy";
                        break;
                    case 0x5: // Vx -= Vy
                        if (reg[(opcode&0x0F00) >> 8] < reg[(opcode&0x00F0) >> 4])reg[15] = 0;
                        else reg[15] = 1;
                        reg[(opcode&0x0F00) >> 8] -= reg[(opcode&0x00F0) >> 4];
                        command = "Vx -= Vy";
                        break;
                    case 0x6: // Vx >>= 1
                        reg[15] = reg[(opcode&0x0F00) >> 8]&1;
                        reg[(opcode&0x0F00) >> 8] = reg[(opcode&0x0F00) >> 8] >> 1;
                        command = "Vx >>= Vy";
                        break;
                    case 0x7: // Vx = Vy - Vx
                        if (reg[(opcode&0x0F00) >> 8] > reg[(opcode&0x00F0) >> 4])reg[15] = 0;
                        else reg[15] = 1;
                        reg[(opcode&0x0F00) >> 8] = reg[(opcode&0x00F0) >> 4] - reg[(opcode&0x0F00) >> 8];
                        command = "Vx = Vy - Vx";
                        break;
                    case 0xE: // Vx <<= 1
                        reg[(opcode&0x0F00) >> 8] = reg[(opcode&0x0F00) >> 8] << 1;
                        command = "Vx <<= 1";
                        break;
                    default:
                        break;
                }
                break;
            }
            case 0x9000: // Vx != Vy
                if (reg[(opcode&0x0F00) >> 8] != reg[(opcode&0x00F0) >> 4]) POS += 2;
                command = "if(Vx!=Vy)";
                break;
            case 0xA000:
                I = opcode&0x0FFF;
                command = "I = NNN";
                break;
            case 0xB000:
                POS = reg[0] + opcode&0x0FFF;
                break;
            case 0xC000:
                reg[(opcode&0x0F00) >> 8] = rand()&(opcode&0x00FF);
                break;
            case 0xD000: {// DXYN, Display N rows at I to video at (x,y)
                uint16_t vx = reg[(opcode&0x0F00) >> 8] & 63;
                uint16_t vy = reg[(opcode&0x00F0) >> 4] & 31;

                for(int i = 0; i < (opcode&0x000F); i++){
                    if(vy + i > 31)break;
                    for(int j = 0; j < 8; j++){
                        if(vx + j > 63)break;
                        // Should set VF to 1 if both bits are 1 and xor'd to 0
                        video[(vy+i) * 64 + (vx+j)] ^= (int)memory[I + i] & (1<<(7-j));

                    }
                }
                command = "DXYN";
                break;
            }
            case 0xF000: {
                switch (opcode & 0x00FF) {
                    case 0x29: { // Set I to Font Character VX
                        I = FONTSET_START_ADDRESS + (reg[(opcode&0x0F00) >> 8]&15)*5;
                        command = "I = sprite_addr(VX)";
                        break;
                    }
                    case 0x30: { // Set I to Font Character VX % 10
                        I = FONTSET_START_ADDRESS + ((reg[(opcode&0x0F00) >> 8]%10)&15)*5;
                        command = "I = sprite_addr_x(VX)";
                        break;
                    }
                    case 0x31: { // Set I to Font Character (VX/10) % 10
                        I = FONTSET_START_ADDRESS + (((reg[(opcode&0x0F00) >> 8]/10)%10)&15)*5;
                        command = "I = sprite_addr_y(VX)";
                        break;
                    }
                    default:
                        break;
                }
                break;
            }
            default:
                break;
        }
    }

    void print_display(){
        #if defined __LINUX__
            system("clear");
        #elif defined _WIN32
            system("cls");
        #endif

        string s = "";
        for(int j = 0; j < 64; j++)s += "_";
        s += "\n";
        
        for(int i = 0; i < 31; i++){
            s += "|";
            for(int j = 0; j < 64; j++){
                if(video[i * 64 + j])s += "\u25A3";
                else s += " ";
            }
            s += "|\n";
        }
        s+="|";
        for(int j = 0; j < 62; j++)s += "_";
        cout << s << "|\n\n";

        cout << command << "\n";
    }
};

int main() {
    Chip8 emulator;

    int length = emulator.load_instructions("./print.asm");

    while(emulator.POS < START_ADDRESS + 2 * length){
        std::this_thread::sleep_for(std::chrono::milliseconds(INTERVAL));

        emulator.emulateCycle();

        emulator.print_display();
    }

    return 0;
}