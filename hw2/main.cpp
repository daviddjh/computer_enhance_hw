#include "stdio.h"
#include <bitset>

#include "d_core.cpp"

using namespace d_std;


d_string decode_effective_address_calculation(Memory_Arena* arena, u8 mod, u8 r_m, u16 displacement){

    d_string reg_string;
    reg_string.string = arena->allocate_array<char>(25);
    reg_string.size = 25;

    if(mod == 0b00){
        switch(r_m){
            case 0b000:
                sprintf(reg_string.string, "[BX + SI]");
                break;
            case 0b001:
                sprintf(reg_string.string, "[BX + DI]");
                break;
            case 0b010:
                sprintf(reg_string.string, "[BP + SI]");
                break;
            case 0b011:
                sprintf(reg_string.string, "[BP + DI]");
                break;
            case 0b100:
                sprintf(reg_string.string, "[SI]");
                break;
            case 0b101:
                sprintf(reg_string.string, "[DI]");
                break;
            case 0b110:
                sprintf(reg_string.string, "%d", r_m);
                break;
            case 0b111:
                sprintf(reg_string.string, "[BX]", r_m);
                break;
        }
    } else if(mod == 0b01 || mod == 0b10) {
        switch(r_m){
            case 0b000:
                sprintf(reg_string.string, "[BX + SI + %d]", displacement);
                break;
            case 0b001:
                sprintf(reg_string.string, "[BX + DI + %d]", displacement);
                break;
            case 0b010:
                sprintf(reg_string.string, "[BP + SI + %d]", displacement);
                break;
            case 0b011:
                sprintf(reg_string.string, "[BP + DI + %d]", displacement);
                break;
            case 0b100:
                sprintf(reg_string.string, "[SI + %d]", displacement);
                break;
            case 0b101:
                sprintf(reg_string.string, "[DI + %d]", displacement);
                break;
            case 0b110:
                sprintf(reg_string.string, "[BP + %d]", displacement);
                break;
            case 0b111:
                sprintf(reg_string.string, "[BX + %d]", displacement);
                break;
        }
    }

    return reg_string;
}

d_string decode_reg(Memory_Arena* arena, u8 reg, u8 w){
    d_string reg_string;
    reg_string.string = arena->allocate_array<char>(2);
    reg_string.size = 2;

    if(reg == 0b000){

        if(w == 0){
            reg_string.string[0] = 'A';
            reg_string.string[1] = 'L';
        } else {
            reg_string.string[0] = 'A';
            reg_string.string[1] = 'X';
        }

    } else if(reg == 0b001){
        if(w == 0){
            reg_string.string[0] = 'C';
            reg_string.string[1] = 'L';
        } else {
            reg_string.string[0] = 'C';
            reg_string.string[1] = 'X';
        }
    } else if(reg == 0b010){
        if(w == 0){
            reg_string.string[0] = 'D';
            reg_string.string[1] = 'L';
        } else {
            reg_string.string[0] = 'D';
            reg_string.string[1] = 'X';
        }
    } else if(reg == 0b011){
        if(w == 0){
            reg_string.string[0] = 'B';
            reg_string.string[1] = 'L';
        } else {
            reg_string.string[0] = 'B';
            reg_string.string[1] = 'X';
        }
    } else if(reg == 0b100){
        if(w == 0){
            reg_string.string[0] = 'A';
            reg_string.string[1] = 'H';
        } else {
            reg_string.string[0] = 'S';
            reg_string.string[1] = 'P';
        }
    } else if(reg == 0b101){
        if(w == 0){
            reg_string.string[0] = 'C';
            reg_string.string[1] = 'H';
        } else {
            reg_string.string[0] = 'B';
            reg_string.string[1] = 'P';
        }
    } else if(reg == 0b110){
        if(w == 0){
            reg_string.string[0] = 'D';
            reg_string.string[1] = 'H';
        } else {
            reg_string.string[0] = 'S';
            reg_string.string[1] = 'I';
        }
    } else if(reg == 0b111){
        if(w == 0){
            reg_string.string[0] = 'B';
            reg_string.string[1] = 'H';
        } else {
            reg_string.string[0] = 'D';
            reg_string.string[1] = 'I';
        }
    }
    return reg_string;
}

int main(){

    // Init
    Memory_Arena* base_arena = make_arena();
    File test_file = open_file("..\\..\\computer_enhance\\perfaware\\part1\\listing_0039_more_movs", "r"); 
    d_string test_file_contents = read_file(base_arena, test_file, test_file.size);

    // Decode
    char* it = test_file_contents.string;
    for(int i = 0; i < test_file_contents.size; i++, it++){
        // MOV
        // Register - To - Register
        if((*it & 0b11111000) == 0b10001000){
            u8 d = (*it & 0b00000010) >> 1;
            u8 w = *it & 0b00000001;
            it++, i++;
            u8 mod = (*it & 0b11000000) >> 6; 
            u8 reg  = (*it & 0b00111000) >> 3; 
            u8 r_m  = *it & 0b00000111; 
            printf("mov ");
            d_string dest_string; 
            d_string source_string; 
            if(mod == 0b00){  // Memory, no displacement

                if(d == 0){
                    source_string = decode_reg(base_arena, reg, w);
                    dest_string   = decode_effective_address_calculation(base_arena, mod, r_m, 0);

                } else if (d == 1){
                    dest_string   = decode_reg(base_arena, reg, w);
                    source_string = decode_effective_address_calculation(base_arena, mod, r_m, 0);
                }

            } else if(mod == 0b01){  // Memory, 8bit displacement

                u16 displacement = 0;

                it++, i++;
                displacement = *it;

                if(d == 0){
                    source_string = decode_reg(base_arena, reg, w);
                    dest_string = decode_effective_address_calculation(base_arena, mod, r_m, displacement);

                } else if (d == 1){
                    dest_string   = decode_reg(base_arena, reg, w);
                    source_string = decode_effective_address_calculation(base_arena, mod, r_m, displacement);
                    
                }

            } else if(mod == 0b10){  // Memory, 16bit displacement

                u16 displacement = 0;
                u8 disp_low  = 0;
                u8 disp_high = 0;

                it++, i++;
                disp_low = *it;

                it++, i++;
                disp_high = *it;

                displacement = ((u16)disp_high << 8) | disp_low;

                if(d == 0){
                    source_string = decode_reg(base_arena, reg, w);
                    dest_string   = decode_effective_address_calculation(base_arena, mod, r_m, displacement);

                } else if (d == 1){
                    dest_string   = decode_reg(base_arena, reg, w);
                    source_string = decode_effective_address_calculation(base_arena, mod, r_m, displacement);
                }

            } else if(mod == 0b11){  // Reg to reg
                if(d == 0){
                    dest_string   = decode_reg(base_arena, r_m, w);
                    source_string = decode_reg(base_arena, reg, w);
                } else if (d == 1){
                    dest_string   = decode_reg(base_arena, reg, w);
                    source_string = decode_reg(base_arena, r_m, w);
                }
            }
            printf("%.*s, %.*s\n", dest_string.size, dest_string.string, source_string.size, source_string.string);
        }
        else if((*it & 0b11110000) == 0b10110000){
            u8 w =     (*it & 0b00001000) >> 3;
            u8 reg  =  *it & 0b00000111; 
            it++, i++;
            printf("mov ");
            d_string dest_reg; 
            dest_reg   = decode_reg(base_arena, reg, w);
            if(w == 1){
                u8 data_low  = *it;
                it++, i++;
                u8 data_high = *it;
                u16 data = (((u16)data_high) << 8) | data_low; 
                printf("%.*s, %d\n", dest_reg.size, dest_reg.string, data);
            } else if (w == 0) {
                u16 data = *it;
                printf("%.*s, %d\n", dest_reg.size, dest_reg.string, data);
            }
        } else {
            std::bitset<8>binary_op_code(*it);
            printf("Unrecognized Op Code! : %s\n", binary_op_code.to_string().c_str());
        }
    }

    // Destroy
    bool success   = close_file(test_file);
    base_arena->release();
}
