rm *.elf
rm *.objdump.txt

riscv32-unknown-elf-gcc     ./03_c_program_to_add_two_integers.c     -O1 -o 03_c_program_to_add_two_integers.elf
riscv32-unknown-elf-objdump ./03_c_program_to_add_two_integers.elf   -d   > 03_c_program_to_add_two_integers.objdump.txt

riscv32-unknown-elf-gcc     ./04_c_program_to_multiply_two_floating_point_numbers.c     -O1 -o 04_c_program_to_multiply_two_floating_point_numbers.elf
riscv32-unknown-elf-objdump ./04_c_program_to_multiply_two_floating_point_numbers.elf   -d   > 04_c_program_to_multiply_two_floating_point_numbers.objdump.txt

riscv32-unknown-elf-gcc     ./05_c_program_to_find_ascii_value_of_a_character.c     -O1 -o 05_c_program_to_find_ascii_value_of_a_character.elf
riscv32-unknown-elf-objdump ./05_c_program_to_find_ascii_value_of_a_character.elf   -d   > 05_c_program_to_find_ascii_value_of_a_character.objdump.txt

riscv32-unknown-elf-gcc    ./06_c_program_to_compute_quotient_and_remainder.c     -O1 -o 06_c_program_to_compute_quotient_and_remainder.elf
riscv32-unknown-elf-objdump ./06_c_program_to_compute_quotient_and_remainder.elf  -d   > 06_c_program_to_compute_quotient_and_remainder.objdump.txt

riscv32-unknown-elf-gcc     ./07_c_program_to_find_the_size_of_int_float_double_and_char.c     -O1 -o 07_c_program_to_find_the_size_of_int_float_double_and_char.elf
riscv32-unknown-elf-objdump ./07_c_program_to_find_the_size_of_int_float_double_and_char.elf   -d   > 07_c_program_to_find_the_size_of_int_float_double_and_char.objdump.txt

riscv32-unknown-elf-gcc     ./08_c_program_to_demonstrate_the_working_of_keyword_long.c     -O1 -o 08_c_program_to_demonstrate_the_working_of_keyword_long.elf
riscv32-unknown-elf-objdump ./08_c_program_to_demonstrate_the_working_of_keyword_long.elf   -d   > 08_c_program_to_demonstrate_the_working_of_keyword_long.objdump.txt

riscv32-unknown-elf-gcc     ./09_c_program_to_swap_two_numbers.c     -O1 -o 09_c_program_to_swap_two_numbers.elf
riscv32-unknown-elf-objdump ./09_c_program_to_swap_two_numbers.elf   -d   > 09_c_program_to_swap_two_numbers.objdump.txt

riscv32-unknown-elf-gcc     ./10_c_program_to_multiply_two_numbers.c     -O1 -o 10_c_program_to_multiply_two_numbers.elf
riscv32-unknown-elf-objdump ./10_c_program_to_multiply_two_numbers.elf   -d   > 10_c_program_to_multiply_two_numbers.objdump.txt

riscv32-unknown-elf-gcc     ./11_c_program_to_check_whether_a_character_is_a_vowel_or_consonant.c     -O1 -o 11_c_program_to_check_whether_a_character_is_a_vowel_or_consonant.elf
riscv32-unknown-elf-objdump ./11_c_program_to_check_whether_a_character_is_a_vowel_or_consonant.elf   -d   > 11_c_program_to_check_whether_a_character_is_a_vowel_or_consonant.objdump.txt

riscv32-unknown-elf-gcc     ./12_c_program_to_find_the_largest_number_among_three_numbers.c     -O1 -o 12_c_program_to_find_the_largest_number_among_three_numbers.elf
riscv32-unknown-elf-objdump ./12_c_program_to_find_the_largest_number_among_three_numbers.elf   -d   > 12_c_program_to_find_the_largest_number_among_three_numbers.objdump.txt

#riscv32-unknown-elf-gcc     ./13_c_program_to_find_the_roots_of_a_quadratic_equation.c     -O1 -o 13_c_program_to_find_the_roots_of_a_quadratic_equation.elf
#riscv32-unknown-elf-objdump ./13_c_program_to_find_the_roots_of_a_quadratic_equation.elf   -d   > 13_c_program_to_find_the_roots_of_a_quadratic_equation.objdump.txt

riscv32-unknown-elf-gcc     ./14_c_program_to_check_leap_year.c     -O1 -o 14_c_program_to_check_leap_year.elf
riscv32-unknown-elf-objdump ./14_c_program_to_check_leap_year.elf   -d   > 14_c_program_to_check_leap_year.objdump.txt

riscv32-unknown-elf-gcc     ./15_c_program_to_check_whether_a_number_is_positive_or_negative.c     -O1 -o 15_c_program_to_check_whether_a_number_is_positive_or_negative.elf
riscv32-unknown-elf-objdump ./15_c_program_to_check_whether_a_number_is_positive_or_negative.elf   -d   > 15_c_program_to_check_whether_a_number_is_positive_or_negative.objdump.txt

riscv32-unknown-elf-gcc     ./16_c_program_to_check_whether_a_character_is_an_alphabet_or_not.c     -O1 -o 16_c_program_to_check_whether_a_character_is_an_alphabet_or_not.elf
riscv32-unknown-elf-objdump ./16_c_program_to_check_whether_a_character_is_an_alphabet_or_not.elf   -d   > 16_c_program_to_check_whether_a_character_is_an_alphabet_or_not.objdump.txt

riscv32-unknown-elf-gcc     ./17_c_program_to_calculate_the_sum_of_natural_numbers.c     -O1 -o 17_c_program_to_calculate_the_sum_of_natural_numbers.elf
riscv32-unknown-elf-objdump ./17_c_program_to_calculate_the_sum_of_natural_numbers.elf   -d   > 17_c_program_to_calculate_the_sum_of_natural_numbers.objdump.txt

riscv32-unknown-elf-gcc     ./18_c_program_to_find_factorial_of_a_number.c     -O1 -o 18_c_program_to_find_factorial_of_a_number.elf
riscv32-unknown-elf-objdump ./18_c_program_to_find_factorial_of_a_number.elf   -d   > 18_c_program_to_find_factorial_of_a_number.objdump.txt

riscv32-unknown-elf-gcc     ./19_c_program_to_generate_multiplication_table.c     -O1 -o 19_c_program_to_generate_multiplication_table.elf
riscv32-unknown-elf-objdump ./19_c_program_to_generate_multiplication_table.elf   -d   > 19_c_program_to_generate_multiplication_table.objdump.txt

riscv32-unknown-elf-gcc     ./20_c_program_to_display_fibonacci_sequence.c     -O1 -o 20_c_program_to_display_fibonacci_sequence.elf
riscv32-unknown-elf-objdump ./20_c_program_to_display_fibonacci_sequence.elf   -d   > 20_c_program_to_display_fibonacci_sequence.objdump.txt