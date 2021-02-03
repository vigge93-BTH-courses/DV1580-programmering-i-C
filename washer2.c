#include <assert.h>
#include <stdio.h>
#include <string.h>

typedef struct
{
   char wash_program[256];
   int notification;
   unsigned int delayed_start;
} Program;

int main()
{
   // Test #1 - check size
   Program running = {"Delicate", 1, 250};

   int check = sizeof(running);
   printf("%d", check);
   assert(check < 4);

   if (running.notification == 1)
   {
      printf("Send notification to user that program is done!\n");
   }

   return 0;
}