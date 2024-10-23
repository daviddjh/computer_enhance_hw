#include "stdio.h"

#include "d_core.cpp"

using namespace d_std;

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
    File test_file = open_file("..\\..\\computer_enhance\\perfaware\\part1\\listing_0038_many_register_mov", "r"); 
    d_string test_file_contents = read_file(base_arena, test_file, test_file.size);

    // Decode
    char* it = test_file_contents.string;
    for(int i = 0; i < test_file_contents.size; i++, it++){
        if(*it & 0b10001000){
            u8 d = (*it & 0b00000010) >> 1;
            u8 w = *it & 0b00000001;
            it++, i++;
            u8 mode = (*it & 0b11000000) >> 6; 
            u8 reg  = (*it & 0b00111000) >> 3; 
            u8 r_m  = *it & 0b00000111; 
            printf("mov ");
            d_string dest_reg; 
            d_string source_reg; 
            if(d == 0){
                dest_reg   = decode_reg(base_arena, r_m, w);
                source_reg = decode_reg(base_arena, reg, w);
            } else if (d == 1){
                dest_reg   = decode_reg(base_arena, reg, w);
                source_reg = decode_reg(base_arena, r_m, w);
            }
            printf("%.*s, %.*s\n", dest_reg.size, dest_reg.string, source_reg.size, source_reg.string);
        }
    }

    // Destroy
    bool success   = close_file(test_file);
    base_arena->release();
}