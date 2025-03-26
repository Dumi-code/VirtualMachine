# Kernel initializes the process and the interrupts
# process 0 is at 80 and its stack starts at 50
0 movv a 80
1 store 50 a
# stack after pushing in dummy values for a, b, c, d, acc is 55
2 movv a 55
# store it at 70
3 store 70 a
# process 1 is at 90 and its stack at 60
4 movv a 90
5 store 60 a
# stack after pushing in dummy values for a, b, c, d, acc is 65
6 movv a 65
# store it at 71
7 store 71 a
# current process 0
8 movv a 0
9 store 72 a

# Start the first process
10 movv sp 55
11 jmp 37
12 halt


#scheduler
20 push a
21 push b
22 push c
23 push d
24 push acc
# current process
25 load a 72
26 movv b 70
27 add a b
28 storer acc sp
# find the next process
29 movv b 1
30 add a b
31 movv b 2
32 mod acc b
# next process
33 store 72 acc
34 movv a 70
35 add a acc
36 loadr sp acc
37 pop acc
38 pop d
39 pop c
40 pop b
41 pop a
42 movv ivec 20
43 movv timer 5
44 movv int 1
45 ret 
46 halt


#process 0
80 movv a 20
81 movv b 1
82 sub a b
83 mov a acc
84 out acc
85 jnz 82 a
86 halt

#process 1
90 movv a 10000
91 movv b 1
92 sub a b
93 mov a acc
94 out acc
95 jnz 92 a
96 halt


