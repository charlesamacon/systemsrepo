#include <stdio.h>

int main(int argc, char** argv){

	FILE *input;
	FILE *output;
	char c;
	char string[100][100];
	int i,k,j,n;
	k=0;
	i=0;
	j=0;
	n=0;
	
	input = fopen(argv[1],"r");
	
	while((c=fgetc(input)) != EOF){
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
	string[k][0]= EOF;
	
	fclose(input);
	
	output = fopen("output","w");
	
	while(n<k){
		fputs(string[n],output);
		n++;
	}
	
	fclose(output);
	
	return 0;
}
/*
Sanders(char** string){
	int i = 0;
	int k = 0;
	int n=0;
	char arr[100][100];
	
	while(string[k][i] != EOF){
		if(string[k][i] == 'i' && string[k][i+1] == 'f'){
			arr = Sanders(arr);
			while(string[k][i] != 'f' && string[k][i+1] != 'i'){
				arr[n]=string[k];
				k++;
				n++;
			}
			arr[n]="fi";
			arr=Wescott(arr);
			return arr;
		}
		
		if(string[k][i] == 'r' && string[k][i+1] == 'e' && string[k][i] == 'p' && string[k][i+1] == 'a' && string[k][i] == 'e' && string[k][i+1] == 't'){
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
			
			
	*/