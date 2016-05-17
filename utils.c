#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/**
 * Convert seconds into hh:mm:ss format
 * Params:
 *  seconds - seconds value
 * Returns: hms - formatted string
 **/
 char* seconds_to_time(float raw_seconds) {
  char *hms;
  int hours, hours_residue, minutes, seconds, milliseconds;
  hms = (char*) malloc(100);

  sprintf(hms, "%f", raw_seconds);

  hours = (int) raw_seconds/3600;
  hours_residue = (int) raw_seconds % 3600;
  minutes = hours_residue/60;
  seconds = hours_residue % 60;
  milliseconds = 0;

  // get the decimal part of raw_seconds to get milliseconds
  char *pos;
  pos = strchr(hms, '.');
  int ipos = (int) (pos - hms);
  char decimalpart[15];
  memset(decimalpart, ' ', sizeof(decimalpart));
  strncpy(decimalpart, &hms[ipos+1], 3);
  milliseconds = atoi(decimalpart); 

  
  sprintf(hms, "%d:%d:%d.%d", hours, minutes, seconds, milliseconds);
  return hms;
}

long little_to_big_4_bytes(unsigned char buffer4[]) {
    return buffer4[0] | (buffer4[1]<<8) | (buffer4[2]<<16) | (buffer4[3]<<24);
}


long little_to_big_2_bytes(unsigned char buffer2[]) {
    return buffer2[0] | (buffer2[1]<<8);
}

char* get_file_path(int argc, char **argv) {

    char *filename = (char*) malloc(sizeof(char) * 1024);

    if (filename == NULL) {
      printf("Error in malloc\n");
      return NULL;
    }

    // get file path
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
      
       strcpy(filename, cwd);

       // get filename from command line
       if (argc < 2) {
         printf("No wave file specified\n");
         return NULL;
       }
       
       strcat(filename, "/");
       strcat(filename, argv[1]);
       printf("Reading %s\n", filename);
       return filename;
    }
    return NULL;
}
