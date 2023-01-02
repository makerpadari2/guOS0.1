#include "print.h"
#include "screen.h"
#include "keyboard.h"
#include "string.h"
#include "isr.h"
#include "idt.h"

void kernel_main() {
    isr_install();
    clearScreen();

    print("Welcome!\n");



       

       print("Command> ");
       while (1)
       {
                string ch = readStr();
                print("\n");
                print(ch);
               /* if(strEql(ch,"cmd"))
                {
                        print("\nYou are allready in cmd\n");
                }
                else if(strEql(ch,"clear"))
                {
                        clearScreen();
                        print("NIDOS> ");
                }
                
                else
                {
                        print("Bad command!");
                } */       
       }
        

}