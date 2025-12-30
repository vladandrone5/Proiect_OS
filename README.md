# Proiect_OS
Proiect ITSC

## Scrie Boot banner, da un "clear screen" si scrie ceva la terminal
## Momentan scrierea intr un registru csr face ca programul sa intre intr un loop infinit ciudat
## morti lui de opensbi ca ma baga in supervisor mode inloc sa ma lase in mmode; am pierdut ZILE IN PULA MEA INCERCAND SA IMID AU SEAMA
## merge printf si sa lucram cu csr doar sa fie scsr(supervisor control and status registers)
## working timer interrupts and print stuff
## wrote func prototypes for plic 
## TO DO: create remove proces func: add moving buffer for processes so that they ocupy the first available idxs everytime: if there are no more processes set current_process to 8
## TO DO: create logic to call process done from inside schedule? think of how to find out that the last ret instrunction has been called
## TO DO: add error handling to exec for program names: add logic with dictionary for program names with addresses
## TO DO: add string function to get buffer until first wanted character occurs, stops at \0, has return code for when it finds that character
## TO DO: implement yield function, finish process preemtion
## TO DO: add syscalls
## TO DO: create hex printf: modify ps to look better: add shell 
## TO DO: add file system
