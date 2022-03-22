#include <stdio.h>
#include <math.h>

// reverses a string 'str' of length 'len' 
void reverse(char *str, int len) 
{ 
    int i=0, j=len-1, temp; 
    while (i<j) 
    { 
        temp = str[i]; 
        str[i] = str[j]; 
        str[j] = temp; 
        i++; j--; 
    } 
} 
  
 // Converts a given integer x to string str[].  d is the number 
 // of digits required in output. If d is more than the number 
 // of digits in x, then 0s are added at the beginning. 
int intToStr(int x, char str[], int d) 
{ 
    int i = 0;
    if (x==0) //if x=0, make sure 0 is added to str
    {
        str[i++]='0';
    }
    while (x) 
    { 
        str[i++] = (x%10) + '0'; 
        x = x/10; 
    } 
  
    // If number of digits required is more, then 
    // add 0s at the beginning 
    while (i < d) 
        str[i++] = '0'; 
  
    reverse(str, i); 
    str[i] = '\0'; 
    return i; 
} 
// Converts a floating point number to string with "p" replacing ".". 
void ftoa(float n, char *res, int afterpoint) 
{ 
    // Extract integer part 
    int ipart = (int)n; 
  
    // Extract floating part 
    float fpart = n - (float)ipart; 
  
    // convert integer part to string 
    int i = intToStr(ipart, res, 0); 
  
    // check for display option after point 
    if (afterpoint != 0 && fpart>0) // if n is already int (ie. fpart==0), skip
    { 
        res[i] = 'p';  // add "p" instead of dot 
  
        // Get the value of fraction part upto given no. 
        // of points after dot. The third parameter is needed 
        // to handle cases like 233.007 
        fpart = fpart * pow(10, afterpoint); 
  
        intToStr((int)fpart, res + i + 1, afterpoint); 
    } 
} 

// int main(void) {
//     char str[10];
//     ftoa(3.145,str,2); 
//     printf("str=%s\n",str); //prints 3p14
//     ftoa(0.752,str,2);    
//     printf("str=%s\n",str); //prints 0p75
//     ftoa(3.0000,str,2);    
//     printf("str=%s\n",str); //prints 3
//     ftoa(13,str,2);
//     printf("str=%s\n",str); //prints 13
//     ftoa(3.001,str,2);
//     printf("str=%s\n",str); //prints 3p00
//     return 0;
// }
