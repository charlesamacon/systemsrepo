#include <stdio.h>

int main(int argc, char** argv){

	FILE *input;
	FILE *output;
	char c;
	char string[100][100];
	int i,k,j,n;
	k=0;
	i=0;
	n=0;
	
	input = fopen(argv[1],"r");  //Opens the file listed on the command line call for reading
	
	while((c=fgetc(input)) != EOF){  //Copies file to 2d character array string
		if(c!='\n'){
			string[k][i]=c;
			i++;
		}
		else{
			i=0;
			string[k][i]='\n';
			k++;
		}
	}
	string[k][0]= EOF; //Puts EOF at end of the 2d char array
	
	fclose(input);
	
	output = fopen("output","w"); //Creates output file and opens it for writing
	
	while(n<k){ //Temporary code to check file copying and output
		fputs(string[n],output);
		n++;
	}
	
	fclose(output);
	
	return 0;
}

Sanders(char string[][]){  //Recursively searches input 2d character array for assignment objectives and passes them to their respective functions. Recursion is to catch nested statements.
	int i = 0;
	int k = 0;
	int n=0;
	char arr[100][100];
	
	while(string[k][i] != EOF){ 
		if(string[k][i] == 'i' && string[k][i+1] == 'f'){  //Searches for if statements
			while(string[k][i] != 'f' && string[k][i+1] != 'i'){
				arr[n]=string[k];
				k++;
				n++;
			}
			arr = Sanders(arr); //Array is not complete FIX THIS
			arr[n]="fi";
			arr=Wescott(arr);
			return arr;
		}
		
		if(string[k][i] == 'r' && string[k][i+1] == 'e' && string[k][i+2] == 'p' && string[k][i+3] == 'a' && string[k][i+4] == 'e' && string[k][i+5] == 't'){  //Searches for repeat statements
			arr = Sanders(arr);
			while(string[k][i] != '}'){
				arr[n]=string[k];
				k++;
				n++;
			}
			arr[n] = '}';
			arr=Macon(arr);
			return arr;
		}
	}
}


	
Macon(char ** string)
{
	int i = 0;
	int k = 0;
	int n = 0;
	
	char arr[100][100];
	
	//Really simple way of doing this
	char repeat[100]="repeatIndex1=0\nwhile [ $repeatIndex1 - lt $num ]\n";
	char doString[100]="do\n";
	char doneString[100]="repeatIndex1=$[$repeatIndex1+1]\ndone\n";

	while(string[k][i] != EOF)
	{
		if (string[k][i] == 'r' && string[k][i+1] == 'e' && string[k][i+2] == 'p' && string[k][i+3] == 'e' && string[k][i+4] == 'a' && string[k][i+5] == 't')
		{
			strcpy(arr[k],repeat);
			k++;
		}
		else if (string[k][i] == '{')
		{
			strcpy(arr[k],doString);
			k++;
		}
		else if (string[k][i] == '}')
		{
			strcpy(arr[k],doneString);
			k++;
		}
		else
		{
			while (string[k][i+1] != NULL)
			{
				arr[k][i] = string[k][i];
				i++;
			}
			arr[k][i+1] = '\';
			arr[k][i+2] = 'n';	// To make sure there is a newline at the end of each line.
			k++;
		}
	}
	
	// Let me know how this works, it works in my test cases.
	return arr;
	
}