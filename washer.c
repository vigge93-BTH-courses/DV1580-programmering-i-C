#include <assert.h>
#include <stdio.h>
#include <string.h>

enum wash_program
{
   Delicates,
   Cotton,
   Woolens,
   Sportswear,
   Shirts,
   Eco
};

typedef struct
{
   unsigned char wash_program : 3;
   unsigned char notification : 1;
   unsigned char delayed_start : 8;
} Program;

int main()
{
   // Test #1 - check size
   Program running = {Delicates, 1, 250};

   int check = sizeof(running);

   assert(check < 4);

   if (running.wash_program == Delicates && running.notification == 1)
   {
      printf("Send notification to user that program is done!\n");
   }

   return 0;
}