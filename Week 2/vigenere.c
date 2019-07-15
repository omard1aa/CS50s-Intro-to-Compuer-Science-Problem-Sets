#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

string encrypt(string plaintext, char* key);

int main(int argc, string argv[])
{       
    //***********************************Preparing*************************************************
    if(argv[1] == NULL || atoi(argv[1]) != 0){ // User doesn't cooperate .. inputs a digit or does not input a key
        printf("Usage: ./vigenere keyword");
        return 1;
    }
    if(argv[2] != NULL){
        printf("Usage: ./vigenere keyword");
        return 1;
    } // User inputs more than 1 parameter (1 key)
    int size = strlen(argv[1]);
    for(int i = 0; i < size; i++){
        if(isdigit(argv[1][i])){
            printf("Usage: ./vigenere keyword");
            return 1;
        }
        else
           continue;
        
    }
    char* k = (argv[1]);
    char lowercased_key[strlen(k)];
    for(int i = 0; i < strlen(k); i++){ // Converting the Uppercase key to lowercase
        lowercased_key[i] = tolower(k[i]);
    }
    //************************************************************************************
    string plaintext = get_string("plaintext: ");
    string ciphertext = encrypt(plaintext, lowercased_key);
    printf("ciphertext: %s\n", ciphertext);
    
}

string encrypt(string plaintext, char* key){
    string ciphertext = "";
    int key_size = strlen(key);
    int myarr[key_size];
    int arr_size = sizeof(myarr) / sizeof(int);
    int text_size = strlen(plaintext);
     for(int j = 0, x = 0; j <= key_size; j++){ // Filling myarr with digits of the key
        for(char i = 'a'; i <= 'z'; i++){
            if(key[j] == i){
                myarr[j] = x;
            }
            else{
                x++;
            }
        }
        x = 0;
    }
    int my_key_arr[text_size];
    for(int i = 0, counter = 0; i < text_size; i++){ // filling my_key_arr with many of myarrs
        if(counter >= arr_size){
            counter = 0;
        }
        my_key_arr[i] = myarr[counter];
        counter++;
    }
 
    for (int i = 0, counter=0; i < text_size; i++){
        if ((int)plaintext[i] >= 65 && (int)plaintext[i] <= 90){ // if Uppercase
            if((int)plaintext[i] + my_key_arr[counter] > 90){ // if the output will be more than 90 (will be                                                                           lowercase) then back to 'A'
                plaintext[i] = ((plaintext[i] + my_key_arr[counter]) - 26); 
            }
            else
                plaintext[i] += my_key_arr[i];
        }
        else if((int)plaintext[i] >= 97 && (int)plaintext[i] <= 122){ // if lowercase
            if((int)plaintext[i] + my_key_arr[counter] > 122){ // if the output is more than 122 back to 'a'
                plaintext[i] = ((plaintext[i] + my_key_arr[counter]) - 26); 
            }
            else
                plaintext[i] += my_key_arr[counter];
        }
        else{
            continue;
        }
        counter++;
    }
    ciphertext = plaintext;
    return ciphertext;
}

