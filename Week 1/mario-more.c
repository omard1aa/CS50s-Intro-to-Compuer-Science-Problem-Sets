#include <cs50.h>
#include <stdio.h>

int get_input(void);
void drawHash(int x);
int main(void)
{
    int height = get_input();
    for(int i = 1; i <= height; i++){
        for(int j = i; j < height; j++){
            printf(" ");
        }
      drawHash(i);
      printf("  ");
      drawHash(i);
      printf("\n");
    }
}

int get_input(void){
    int height; 
    do{
        height = get_int("Height: ");
      }while(height < 1 || height > 8);
    return height;
}

void drawHash(int x){
  for(int i = 0; i< x; i++){
            printf("#");
        }
}