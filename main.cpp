#include <bits/stdc++.h>
using namespace std;


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

const int MEMORY_SIZE = 4096;

struct Chip8 {
    vector<uint8_t> memory;
    vector<uint8_t> reg;
    uint16_t POS; 

    Chip8() : memory(MEMORY_SIZE, 0), reg(16), POS(0){}


    void emulateCycle() {
        uint16_t opcode = (memory[POS] << 8)|memory[POS+1];
        POS += 2; 

        switch(opcode&0xF000){
            case 0x1000: // jump
                POS = opcode & 0xFFF;
                break;
            case 0x3000: // ==
                if(reg[opcode&0x0F00] == opcode&0x00FF) POS+=2;
                break;
            case 0x4000: // !=
                if(reg[opcode&0x0F00] != opcode&0x00FF) POS+=2;
                break;
            case 0x5000: // r==r 
                if(reg[opcode&0x0F00] == reg[opcode&0x00F0]) POS+=2;
                break;
            case 0x6000: // Vx = NN
                reg[(opcode&0x0F00) >> 8] = opcode&0x00FF;
                break;
            case 0x7000: // Vx += NN
                reg[(opcode&0x0F00) >> 8] += opcode&0x00FF;
                break;
            case 0x8000: {
                switch (opcode & 0x000F) {
                    case 0x0: // Vx = Vy
                        reg[(opcode&0x0F00)>>8] = reg[(opcode&0x00F0) >> 4];
                        break;
                    case 0x1: // Vx |= Vy
                        reg[(opcode&0x0F00) >> 8] |= reg[(opcode&0x00F0) >> 4];
                        break;
                    case 0x2: // Vx &= Vy
                        reg[(opcode&0x0F00) >> 8] &= reg[(opcode&0x00F0) >> 4];
                        break;
                    case 0x3: // Vx ^= Vy
                        reg[(opcode&0x0F00) >> 8] ^= reg[(opcode&0x00F0) >> 4];
                        break;
                    case 0x4: // Vx += Vy
                        if (255-reg[(opcode&0x0F00) >> 8] < reg[(opcode&0x00F0) >> 4])reg[15] = 1;
                        else reg[15] = 0;
                        reg[(opcode&0x0F00) >> 8] += reg[(opcode&0x00F0) >> 4];
                        break;
                    case 0x5: // Vx -= Vy
                        if (reg[(opcode&0x0F00) >> 8] < reg[(opcode&0x00F0) >> 4])reg[15] = 0;
                        else reg[15] = 1;
                        reg[(opcode&0x0F00) >> 8] -= reg[(opcode&0x00F0) >> 4];
                        break;
                    case 0x6: // Vx >>= 1
                        reg[15] = reg[(opcode&0x0F00) >> 8]&1;
                        reg[(opcode&0x0F00) >> 8] = reg[(opcode&0x0F00) >> 8] >> 1;
                        break;
                    case 0x7: // Vx = Vy - Vx
                        if (reg[(opcode&0x0F00) >> 8] > reg[(opcode&0x00F0) >> 4])reg[15] = 0;
                        else reg[15] = 1;
                        reg[(opcode&0x0F00) >> 8] = reg[(opcode&0x00F0) >> 4] - reg[(opcode&0x0F00) >> 8];
                        break;
                    case 0xE: // Vx <<= 1
                        reg[(opcode&0x0F00) >> 8] = reg[(opcode&0x0F00) >> 8] << 1;
                        break;
                    default:
                        break;
                }
                break;
            }
            case 0x9000: // Vx != Vy
                if (reg[(opcode&0x0F00) >> 8] != reg[(opcode&0x00F0) >> 4]) POS += 2;
                break;
                
            default:
                break;
        }
    }
};

int main() {
    Chip8 emulator;

    return 0;
}