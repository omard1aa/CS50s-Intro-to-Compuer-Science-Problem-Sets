#include <cs50.h>
#include <stdio.h>
void draw(int height);
int main(void)
{
   int height = get_int("Height: "); // Take input from user
    while(height < 1 || height > 8) // Check if the user inputs the right way
        height = get_int("Height: ");
    draw(height); // Call the draw function
}
void draw(int height){
    int height2 = height - 1; // make a new height changeable variable to draw spaces 
    for (int i =0; i < height; i++){
        for(int j = 0; j < height2; j++){
            printf(" ");
        }
        for(int k = height2; k < height ; k++){
            printf("#");
        }
        printf("\n");
        height2--;
    }
}
