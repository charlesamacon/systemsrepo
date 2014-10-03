#include <stdio.h>
#include <string.h>

int main(int argc, char** argv){

	FILE *input;
	FILE *output;
	char c;
	char string[100][100];
	char arr[100][100];
	char placeHolder[3] = " \n";
	int i,k,j,n;
	k=0;
	i=0;
	n=0;
	
	//----------------------------Open File Input-----------------------------------
	
	input = fopen(argv[1],"r");  //Opens the file listed on the command line call for reading
	
	while((c=fgetc(input)) != EOF){  //Copies file to 2d character array string
		if(c!='\n'){
			string[k][i]=c;
			i++;
		}
		else{
			string[k][i]='\n';
			i=0;
			k++;
			if((string[k-1][0]=='f' && string[k-1][1]=='i') ||string[k-1][0]=='}'){
				strcpy(string[k],placeHolder);
				k++;
			}
			/*if(string[k-2][0]=='}'){
				strcpy(string[k],placeHolder);
				k++;
			}
			*/
		}
	}
	string[k][0]= EOF; //Puts EOF at end of the 2d char array
	
	fclose(input);
    
	//-----------------------------Repeat Fix------------------------------------
	
	i = 0;
	k = 0;
	n = 0;
	
	//Really simple way of doing this
	char repeat[100]="repeatIndex1=0\nwhile [ $repeatIndex1 - lt $num ]\n";
	char doString[100]="do\n";
	char doneString[100]="repeatIndex1=$[$repeatIndex1+1]\ndone\n";

	while(string[k][i] != EOF)
	{
		if (string[k][i] == 'r' && string[k][i+1] == 'e' && string[k][i+2] == 'p' && string[k][i+3] == 'e' && string[k][i+4] == 'a' && string[k][i+5] == 't')
		{
			strcpy(string[k],repeat);
			k++;
		}
		else if (string[k][i] == '{')
		{
			strcpy(string[k],doString);
			k++;
		}
		else if (string[k][i] == '}')
		{
			strcpy(string[k],doneString);
			k++;
		}
		else k++;
	}
	//-----------------------------If Fix-----------------------------------------
	
	i = 0;
	k = 0;
	n = 0;
	
	while(string[k][i] != EOF){
		while(string[k][i] != '\n'){
			if (string[k][i] == 't' && string[k][i+1] == 'h' && string[k][i+2] == 'e' && string[k][i+3] == 'n'){
				string[k][i] = '\n';
				string[k][i+1] = ' ';
				string[k][i+2] = ' ';
				string[k][i+3] = ' ';
				string[k][i+4] = ' ';
				string[k][i+5] = ' ';
				string[k][i+6] = 't';
				string[k][i+7] = 'h';
				string[k][i+8] = 'e';
				string[k][i+9] = 'n';
				string[k][i+10] = '\n';
				break;
			}
			i++;
		}
		i=0;
		k++;
	}
	//-----------------------------Declaration Fix--------------------------------
	
	i = 0;
	k = 0;
	n = 0;
	int before,after;
	while(string[k][i] != EOF){
		while(string[k][i] != '\n'){
			if(string[k][i] == '='){
				if(string[k][i-1]==' ' && string[k][i+1]==' '){
					for(j=i-1;string[k][j]!='\n';j++){
						string[k][j]= string[k][j+1];
					}
					for(j=i;string[k][j]!='\n';j++){
						string[k][j]= string[k][j+1];
					}
				}
			}
			i++;
		}
		i=0;
		k++;
	}
	//-----------------------------Write to Output-------------------------------
	
	output = fopen("output","w"); //Creates output file and opens it for writing
	
	while(n<k){ //Temporary code to check file copying and output
		fputs(string[n],output);
		n++;
	}
	
	fclose(output);
	
	return 0;
}
