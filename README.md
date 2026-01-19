# Proiect_OS
Proiect ITSC

## Scrie Boot banner, da un "clear screen" si scrie ceva la terminal
## Momentan scrierea intr un registru csr face ca programul sa intre intr un loop infinit ciudat
## morti lui de opensbi ca ma baga in supervisor mode inloc sa ma lase in mmode; am pierdut ZILE IN PULA MEA INCERCAND SA IMID AU SEAMA
## merge printf si sa lucram cu csr doar sa fie scsr(supervisor control and status registers)
## working timer interrupts and print stuff
## wrote func prototypes for plic 
## TO DO: add finite program trmpolin in start process and modify schedule so it works and removes programs that have finished
## TO DO: create logic to call process done from inside schedule? think of how to find out that the last ret instrunction has been called
## TO DO: add file system
