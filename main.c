/*Standard Float Management Program
 *for simple reading and writing of floating-point values to binary files
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/*
   Todo: Add special routine for serial conversion of a human-readable file format to binary -DONE
         Switch the -w function to be non-interractive OR expand for sequential writing
         Redact the usage form message for clarity; move the filename tags to each option
*/


char* fnam;   //char pointer for passing filename argument to write function
char* fnamin; //char pointer for passing read filename to the file conversion function
char* fnamout;
float tfl;    //temporary floating point for storing user input (or read floats from file)

void floatin (void)
{
    FILE *mwrite;
    mwrite=fopen(fnam, "rb+");
    fseek(mwrite, -1, SEEK_END); //Move cursor to before last EOF char location, overwrite''

    scanf("%f", &tfl);      //simple as float input
    fwrite(& tfl, 1, sizeof(tfl), mwrite);     //writes float values in binary form- convenient af
    fputc(EOF, mwrite);  /// End of file byte to avoid reading 0.000000 ad infinitum

}

void filec (void)      //!File conversion function- takes two file names, iterates through the first for formatted floats, then prints then to the second file
{
    int fsiz;
    FILE *mwritec;
    FILE *mloadc;
    mwritec=fopen(fnamout, "rb+");
    mloadc=fopen(fnamin, "a+");
    fseek(mloadc, 0, SEEK_END);     //Moves pointer to end
    fsiz=ftell(mloadc);             //Records pointer location      --this whole thing simply records the file size
    fseek(mloadc, 0, SEEK_SET);     //Moves pointer back to the start
    printf("%d\n", fsiz);


    while(ftell(mloadc)<fsiz)
    {
    fscanf(mloadc, "%f ", &tfl); //scans for a single floating-point number and stores in tfl; eats newline
    printf("%f\n", tfl);
    fseek(mwritec, -1, SEEK_END); //Move pointer to just before the end of file, overwrite EOF byte
    fwrite(& tfl, 1, sizeof(tfl), mwritec); //writes the binary float to the output file
    fputc(EOF, mwritec); //Add EOF byte to stop endless null reading
    }

}

int main(int argc, char* argv[])
{
    bool sp=0;
    char tch; //stores read bytes from file


    if(argc==1)     //no arguments specified, print usage instructions
    {
        printf("Usage: sfmp [options] [filename] [output filename - only for convert mode]\nOptions: -w: write mode\n         -c: convert mode\n");
    }
    else
    {
        if(argc==2)     //one argument, always interpreted as the target filename
        {
            fnam=argv[1];
            FILE *mload;

            mload=fopen(fnam, "a+");

            while(sp!=1)    //Reads out the target file
            {
                tch=fgetc(mload);
                if (tch==EOF)   //Check for EOF byte to stop perpetual reading of NULL
                {               //Actually this only checks the first byte then every 4 bytes, which should never be EOF normally
                    return 0;
                }
                fseek(mload, -1, SEEK_CUR); //EOF not found, move pointer back and read normally as float format
                fread(&tfl, 1, sizeof(tfl), mload); //stdio is good and blessed

                printf("%f\n", tfl);    //Print the formatted float

            }
        }
        else
        {

            if (argc==3)
            {
                if (strcmp(argv[1], "-w")==0)   //compare the first argument to the -w thing
                {
                    fnam=argv[2];   //copy second argument into filename char pointer
                    floatin();  //the file write function
                    return 0;
                }

            }
            if (argc==4)
            {
                if (strcmp(argv[1], "-c")==0) //compare the first argument to the -c thing
                {
                    fnamin=argv[2]; //copy second argument into the read filename char pointer
                    fnamout=argv[3]; //copy the third argument into the write filename char pointer
                    filec();    //the conversion function
                }
                else    //too many args, print usage form
                {
                    printf("Too many arguments.\nUsage: sfmp [options] [filename]\nOptions: -w: write mode\n         -c: convert mode\n");
                }
            }

        }
    }

    return 0;
}
