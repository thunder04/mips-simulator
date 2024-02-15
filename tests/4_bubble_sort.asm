.data   

numbers:    .word   4390, 494, -43, 0, 1, 10

.text   
main:       
    la      $s0,        numbers
    ori     $t0,        $0,         1               # $t0 = 1

    ori     $s1,        $0,         6               # $s1 = n = 6
outer_loop: 
    ori     $s2,        $0,         1               # $s2 = i = 1
    ori     $s3,        $s1,        0               # $s3 = new_n = n
    ori     $t1,        $s0,        0               # $t1 = tmp_addr: address of A
inner_loop: 
    bne     $s1,        $s2,        end_of_inner    # if (n != i), goto end_of_inner
    lw      $t2,        -4($t1)                     # $t2 = A[i - 1]
    lw      $t3,        0($t1)                      # $t3 = A[i]
    # Pretend you are the assembler implementing the bgt instruction
    sub     $t4,        $t2,        $t3             # $t4 = A[i - 1] - A[i]
    srl     $at,        $t4,        31              # Save the sign bit (SB) to $at
    beq     $t2,        $t3,        end_if          # If $t2 == $t3, goto end_if
    # There can be two possibilities, $t2 < $t3 <=> $t4 < 0 (SB == 1) or $t2 > $t3 <=> $t4 > 0 (SB == 0).
    # Check the sign bit to see which is true. Here we want $t4 > 0 to go into the if body.
    beq     $at,        $t0,        end_if
    # Swap
    sw      $t3,        -4($t1)
    sw      $t2,        0($t1)
    ori     $s3,        $s2,        0               # new_n = i
end_if:     
    addi    $s2,        $s2,        1               # i += 1
    addi    $t1,        $t1,        4               # tmp_addr += 4
    j       inner_loop
end_of_inner:
    ori     $s1,        $s3,        0               # n = new_n
    bne     $s1,        $t0,        outer_loop      # if (n != 1), goto outer_loop
    break   
