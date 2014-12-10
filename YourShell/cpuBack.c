#include <stdio.h>

void cpuUsage(){ //Call this in the background
	//printf("cpUsage\n");
    float first[4], final[4], loadavg, time, total;
	time = 0.0f;
    FILE *fp;
	
	

    while(1){
		//printf("OPENING FILE\n");
        fp = fopen("/proc/stat","r");
        fscanf(fp,"%*s %f %f %f %f",&first[0],&first[1],&first[2],&first[3]);
        fclose(fp);
        sleep(60);
		time += 1;

        fp = fopen("/proc/stat","r");
        fscanf(fp,"%*s %f %f %f %f",&final[0],&final[1],&final[2],&final[3]);
        fclose(fp);

        loadavg = ((final[0]+final[1]+final[2]) - (first[0]+first[1]+first[2])) / ((final[0]+final[1]+final[2]+final[3]) - (first[0]+first[1]+first[2]+first[3]));

        total += loadavg;
		loadavg = total/time;
		fp = fopen("cpuStats","w");
		fprintf(fp,"%f %f",loadavg,time);
		fclose(fp);
    }
	return;
}

int main(){
	cpuUsage();
	return 0;
}
