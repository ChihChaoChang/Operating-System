/* fscanf example */
#include <stdio.h>

FILE * pFile2;
int main ()
{
    char f[]="FCFS";
	char s[]="SRTF";
	char r[]="RR";
	char str[80];	
	int at[10],bt[10],n=0,timeslice;
	
    FILE * pFile = fopen("test\\testdata6.txt","r");
    pFile2 =fopen ("0346008.txt","w");
    fscanf(pFile, "%s%s%s", str, str, str);	 
    int test1 = strcmp(str,f);
    int test2 = strcmp(str,s);
	int test3 = strcmp(str,r);
	if(test3 == 0)
	{
		fscanf(pFile, "%d", &timeslice);
	}	
    while(!feof(pFile))
    {
        fscanf(pFile, "%s%s%s%d%s%s%d\n", str, str, str, &at[n], str, str, &bt[n]);
			n=n+1;
	}   
      if (test1 == 0) 
	 {
       FCFS(at,bt,n);
     }
     else if (test2 == 0) 
	 {
       SRTF(at,bt,n);
     }
	 else if (test3 == 0) 
	 {
    	RR(at,bt,n,timeslice);
     }
	else
	printf("wrong data!");      
    return 0;
}

void FCFS(int at[],int bt[] ,int n)
{	
	int terminate[10],waiting[10],takeover[10],starting[10],i=0,m;
	float att=0,awt=0;
    	for(i=0;i<10;i++)
        	{
            waiting[i]=0; takeover[i]=0,starting[i]=0;
            }
    takeover[0]=0;
        for(i=0;i<10;i++)
        {   
    		if (takeover[i]>at[i])  //takeover time's exception 
        		takeover[i+1]=takeover[i]+bt[i];
			else	
				takeover[i+1]=at[i]+bt[i];	
		}	
		for(i=0;i<n;i++)
            {     
                terminate[i]=takeover[i+1];
                starting[i+1]=terminate[i];
                if(starting[i]< at[i])
    				starting[i]= at[i];
    			waiting[i]=starting[i]-at[i];
                awt=awt+waiting[i];
                att=att+terminate[i]-at[i]; 
            }
     		awt =awt/n;
            att=att/n;
		for(i=0;i<n;i++)
            {
            fprintf(pFile2,"P%d\t%d-%d\n",i+1,starting[i],terminate[i]);
		    }
	fprintf(pFile2,"Average Waiting Time: %.2f\n",awt);
	fprintf(pFile2,"Average Turnaround Time: %.2f\n",att);
}

void RR(int at[],int bt[],int n,int timeslice)
{
	int i,j=0,k=1,time,remain,flag=0,tmp;
	int sum_wait=0,sum_turnaround=0,remainingtime[10],endtime[10];//
	int ganttP[50],ganttStartTime[50];
	int line[100];
    int x = 0;
	remain=n;//process amount
		for(i=0;i<n;i++)
		{
    		remainingtime[i]=bt[i];
  		}	
	line[x] = 0;
		for(time=0,i=0;remain!=0;i++)//if not yet finish then do in loop
		{ 
    		if(remainingtime[line[i]]<=timeslice && remainingtime[line[i]]>0)
    		{	
      		ganttP[j]=line[i]+1;//process record 
      		ganttStartTime[j++]=time;
      		time=time+remainingtime[line[i]];
      		remainingtime[line[i]]=0;
      		flag=1;
    		}
    		else if(remainingtime[line[i]]>0)
    		{	
      		ganttP[j]=line[i]+1;
      		ganttStartTime[j++]=time;
      		remainingtime[line[i]]=remainingtime[line[i]]-timeslice;
      		time=time+timeslice;
    		}
    		if(remainingtime[line[i]]==0 && flag==1)
    		{	
      		remain--;
        	sum_wait=sum_wait+time-at[line[i]]-bt[line[i]];
        	sum_turnaround=sum_turnaround+time-at[line[i]];
      		flag=0;
    		}
    	 	for(tmp = k;tmp < n;tmp++)
        	{
            	if(at[tmp]<=time)
            	{	
                	line[++x] = tmp;
                	k++;
            	}
        	}
        	line[++x] = line[i];
		}
	endtime[line[i]]=0;
		for(i=0;i<j-1;i++)
		{	  	
			endtime[i]=ganttStartTime[i+1];
      		fprintf(pFile2,"P%d\t%d-%d\n",ganttP[i],ganttStartTime[i],endtime[i]);    
  		}
   		for(i=j-1;i<j;i++)
   		{
   			fprintf(pFile2,"P%d\t%d-%d\n",ganttP[i],ganttStartTime[i],time);
   		}
  		fprintf(pFile2,"Average waiting time = %.2f\n",sum_wait*1.0/n);
  		fprintf(pFile2,"Average Turnaround time = %.2f",sum_turnaround*1.0/n);
}

void SRTF(int at[],int bt[] ,int n)
{
	int rt[10],endTime,i,smallest,processGantt[100];
    int remain=0,time,sum_wait=0,sum_turnaround=0;
	int tmp;;
    	for(i=0;i<n;i++)
    	{		
        	rt[i]=bt[i];
    	}	
    rt[9]=9999;
    	for(time=0;remain!=n;time++)
    	{
        	smallest=9;
        	for(i=0;i<n;i++)
        	{
            	if(at[i]<=time && rt[i]<rt[smallest] && rt[i]>0)
            	{
                	processGantt[time]=i;
                	smallest=i;
            	}
        	}
        rt[smallest]--;
        	if(rt[smallest]==0)
        	{
            	remain++;
            	endTime=time+1;
            	sum_wait+=endTime-bt[smallest]-at[smallest];
            	sum_turnaround+=endTime-at[smallest];
        	}
    	}
    	for(i=0;i<time;i++)
    	{
        	if(processGantt[i]+1 == tmp)
        	{
            	if(i == time-1)
                fprintf(pFile2,"%d\n",time);
            	continue;
        	}
        	else
        	{
            	if(i != 0)
                fprintf(pFile2,"%d\n",i);
            	fprintf(pFile2,"P%d\t%d-",processGantt[i]+1,i);
        	}
        	tmp = processGantt[i]+1;
   		}	
    	fprintf(pFile2,"Average Waiting time = %.2f\n",sum_wait*1.0/n);
    	fprintf(pFile2,"Average Turnaround time = %.2f",sum_turnaround*1.0/n);
}


