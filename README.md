# fork-pipe-demo
In this repository I made a demo program to learn how to use pipe() and fork() from Linux C.

<h2>ex2.c</h2> <br>
In ex2.c i create 4 processes: A parent a child 1, child 1.1 and 1.2.<br>
The parent will read from a file and pass the words to child 1.<br>
Child 1 will send: LowerCaseLetters to child 1.1<br> 
                   UpperCaseLetters to child 1.2<br>
Child 1.1 and child 1.2 will print the results.<br>
<br>
<br>
<h2>demo.c</h2>  <br>  
This file will contain only the processes with a print statement.<br>
I used this as a skeleton for making ex2.c <br>
