//meeting is activity,activity is task
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
int calculateTwoDateDistance(int pSYear, int pSMonth, int pSDay, int  pEYear, int  pEMonth, int  pEDay)
{
	double distance_t;

	struct tm startDate = { 0,0,0, pSDay ,pSMonth  - 1,(pSYear - 1900) };

	struct tm endDate = { 0,0,0, pEDay ,pEMonth  - 1 ,(pEYear - 1900) };

	time_t startTime = mktime(&startDate);

	time_t endTime = mktime(&endDate);

	distance_t = difftime(endTime, startTime);

	distance_t = ((distance_t) / 60 / 60 / 24) + 1;

	return distance_t;
}
struct task {
   int activityId;
   char activityName[20];
   int year;
   int month;
   int day;
   int hour;
   int duration;
   int activityType;
}allActivities[4]={//change this line if used other test case
		{0001,"COMP2432A1 ",2019,04,13,-1,12,2},
		{0002,"COMP2422P1 ",2019,04,13,-1,26,1},
		{0003,"COMP2000 ",2019,04,22,19,2,3},
		{0004,"Meeting ",2019,04,22,20,2,4}
};
int totalActivitiesCount=4;//change this line if used other test case
int percentageOfCompletion[4]={0,0,0,0};//change this line if used other test case
struct task emptySlot={-1,"Empty",-1,-1,-1,-1,-1,-1};
int numOfTimeSlot;

//this method is luanched when the slot is full, project/assignment can't put in table. 
//eg. project replace revision/meetings
//eg. assignment replace revision/meetings

int priorityReplacing1(struct task TimeTable[][numOfTimeSlot],int tableSizeArray[],int remainingHours, struct task activity,int totalDayCount,int numOfTimeSlot,int startDate[]){
	int i,j,k;
	struct task temp;
	printf("who am i %s\n",activity.activityName );
	//find meeting slot
	int DueDayDistance=calculateTwoDateDistance(startDate[0], startDate[1], startDate[2], activity.year, activity.month, activity.day);
	for(i=0;i<=DueDayDistance;i++){//loop in timetable //before due day
		for(j=0;j<numOfTimeSlot;j++){
			if(TimeTable[i][j].activityType==4){//replace the meetings slot first
				printf("change from a %d,%d %s\n",i,j,TimeTable[i][j].activityName );

				percentageOfCompletion[TimeTable[i][j].activityId-1]=1;//reject
				temp=TimeTable[i][j];
				TimeTable[i][j]=activity;//replace it with project/
				remainingHours--;

			}
			//delete previous meeting 
			if (remainingHours==0){
				for(k=j+1;k<numOfTimeSlot;k++){
					if(TimeTable[i][k].activityId==temp.activityId){
						TimeTable[i][k]=emptySlot;
						tableSizeArray[i]--;
					}
				}
				return 0;
			}
		}
	}
	//find revision slot
	for(i=0;i<=DueDayDistance;i++){//loop in timetable 
		for(j=0;j<numOfTimeSlot;j++){
			if(TimeTable[i][j].activityType==3){//replace the revision slot 
				printf("change from b %d,%d %s\n",i,j,TimeTable[i][j].activityName );
				percentageOfCompletion[TimeTable[i][j].activityId-1]=1;//reject
				temp=TimeTable[i][j];
				TimeTable[i][j]=activity;//replace it with project/assignment
				remainingHours--;

			}
			if (remainingHours==0){
				for(k=j+1;k<numOfTimeSlot;k++){
					if(TimeTable[i][k].activityId==temp.activityId){
						TimeTable[i][k]=emptySlot;
						tableSizeArray[i]--;
					}
				}
				return 0;
			}
		}
	}
	if(remainingHours>0){	//if the project.assignment can't put in timetable at the end
		percentageOfCompletion[activity.activityId-1]-=remainingHours;	//deduct the %ofCompletion
	}
	return 0;
}


int discardInvalidActivity(struct task inValidActivity){
	int i,j;struct task hold;
	//delete activity from array
	for(i=0;i<totalActivitiesCount;i++){
		if(inValidActivity.activityId==allActivities[i].activityId){
			hold=allActivities[totalActivitiesCount-1];
			allActivities[totalActivitiesCount-1]=inValidActivity;
			allActivities[i]=hold;
			totalActivitiesCount--;
		}
	}
}

//find the most free day of the timetable
//the main algorithm will insert activities into that day
int smallestIndex(int tableSizeArray[],int DueYear,int DueMonth,int DueDay,int startDate[]){
	int i;
	int minValue=100;
	int minIndex=0;
	int DueDateDistance;
	DueDateDistance=calculateTwoDateDistance(startDate[0],startDate[1],startDate[2],DueYear,DueMonth,DueDay);

	for(i=0;i<DueDateDistance;i++){
		if(tableSizeArray[i]<minValue){
			minValue=tableSizeArray[i];
			minIndex=i;
		}
	}
	if (minValue>=numOfTimeSlot){//if all slots are filled
		return -1;
	}

	
	return minIndex;
}
//algorithm
int main(int argc, char *argv[]){
	//period
	int startDate[3]={2019,4,8};//change this line if used other test case
	int endDate[3]={2019,4,21};//change this line if used other test case
	int startTime=19;//change this line if used other test case
	int endTime=23;//change this line if used other test case
	numOfTimeSlot=endTime-startTime;
	int totalDayCount=calculateTwoDateDistance(startDate[0],startDate[1],startDate[2],endDate[0],endDate[1],endDate[2]);
	int tableSizeArray[totalDayCount];//tableSizeArray is the  no. of task inside timetable

	//activity valid check
	int i,j,x;struct task hold;
	struct task TimeTable[totalDayCount][numOfTimeSlot];//Table means it's only for checking, not the real timetable
	//init all slots in Table by inserting emptySlot
	for(i=0;i<totalDayCount;i++){
		for(j=0;j<numOfTimeSlot;j++){
			TimeTable[i][j]=emptySlot;
		}
	
	}
	//check invalid meetings
	int y,index;
	for(i=0;i<totalActivitiesCount;i++){
		if(allActivities[i].activityType==3 || allActivities[i].activityType==4){//only check meetings/revision
			x=calculateTwoDateDistance(startDate[0],startDate[1],startDate[2],allActivities[i].year,allActivities[i].month,allActivities[i].day);
			for(j=0;j<allActivities[i].duration;j++){
				y=allActivities[i].hour+j-startTime;
				if(TimeTable[x-1][y].activityId!=-1){	//time crash
					printf("report %s invalid: time crash\n",allActivities[i].activityName );
					//delete added invalid activity
					index=1;
					while(TimeTable[x-1][y-index].activityId==allActivities[i].activityId){
						printf("%d %d \n",x-1,y-index );
						TimeTable[x-1][y-index]=emptySlot;
						index++;
					}
					
					discardInvalidActivity(allActivities[i]);//delete activity
					i--;//becaue the allActivities is changed 
					break;
				}
				//duration check
				if(allActivities[i].hour+allActivities[i].duration>endTime){
					printf("report %s invalid: exceed time limit\n",allActivities[i].activityName );
					index=1;
					while(TimeTable[x-1][y-index].activityId==allActivities[i].activityId){
						printf("%d %d \n",x-1,y-index );
						TimeTable[x-1][y-index]=emptySlot;
						index++;
					}
					discardInvalidActivity(allActivities[i]);
					i--;//becaue the allActivities is changed 
					break;

				}
				//add to table if activity has no problem,then continue checking
				TimeTable[x-1][y]=allActivities[i];
			}

		}
	}
	


	//store all type 1 activities
	struct task projects[totalActivitiesCount];
	int proj_index=0;

	//store all type 2 activities
	struct task assignments[totalActivitiesCount];
	int assign_index=0;



	//add all projects to projects array
	//add all assignments to assignments array.
	for(i=0;i<totalActivitiesCount;i++){
		if(allActivities[i].activityType==1){
			projects[proj_index]=allActivities[i];
			proj_index++;
		}
		else if(allActivities[i].activityType==2){
			assignments[assign_index]=allActivities[i];
			assign_index++;
		}

	}

	//init tableSizeArray
	for(i=0;i<totalDayCount;i++){//print check table
		tableSizeArray[i]=0;
		for(j=0;j<numOfTimeSlot;j++){
			if(TimeTable[i][j].activityId!=-1){
				tableSizeArray[i]++;
			}
		}	
	}


	int arrayIndex;
	int remainingHours;
	//init slotIndexArray
	//slotIndexArray is the next index of timetable available to put task
	int slotIndexArray[totalDayCount];
	for(i=0;i<totalDayCount;i++){
		slotIndexArray[i]=0;
	}

		//input project
	for(i=0;i<proj_index;i++){
		remainingHours=projects[i].duration;
		while(remainingHours!=0){//split each request to hour based
			arrayIndex=smallestIndex(tableSizeArray,projects[i].year,projects[i].month,projects[i].day,startDate);//find smallest index before due day
			if(arrayIndex==-1){
				printf("%s: All slots are filled\n",projects[i].activityName);
				percentageOfCompletion[projects[i].activityId-1]-=remainingHours;
				break;
			}
			//when slotArrayIndex need to increase by meetings/revisions
			else if(TimeTable[arrayIndex][slotIndexArray[arrayIndex]].activityId!=-1){
				slotIndexArray[arrayIndex]++;
				continue;
			}
			if(slotIndexArray[arrayIndex]<numOfTimeSlot){
				TimeTable[arrayIndex][slotIndexArray[arrayIndex]]=projects[i];//assign to table
				slotIndexArray[arrayIndex]++;
				tableSizeArray[arrayIndex]++;
				remainingHours--;
			}else if(slotIndexArray[arrayIndex]>=numOfTimeSlot){//if project can't insert into a slot
				printf("project exceed slot limit\n");
				//find a meeting/revision slot
				//delete all meeting slots
				//fill in the slots
				//put null if some slot is not filled
				//if remainingHours != 0, mark %ofCompletion
				priorityReplacing1(TimeTable,tableSizeArray,remainingHours,projects[i],totalDayCount,numOfTimeSlot,startDate);
				break;
			}
		}
	}



	//reinitialize slotIndexArray
	for(i=0;i<totalDayCount;i++){
		slotIndexArray[i]=0;
	}

	//input assignment
	for(i=0;i<assign_index;i++){
		remainingHours=assignments[i].duration;
		while(remainingHours!=0){//split each request to hour based
			arrayIndex=smallestIndex(tableSizeArray,assignments[i].year,assignments[i].month,assignments[i].day,startDate);//find smallest index before due day
			//when slotArrayIndex need to increase by meetings/revisions
			if(arrayIndex==-1){
				printf("%s: All slots are filled\n",assignments[i].activityName);
				percentageOfCompletion[assignments[i].activityId-1]-=remainingHours;
				break;
			}
			else if(TimeTable[arrayIndex][slotIndexArray[arrayIndex]].activityId!=-1){
				slotIndexArray[arrayIndex]++;
				continue;
			}
			if(slotIndexArray[arrayIndex]<numOfTimeSlot){
				TimeTable[arrayIndex][slotIndexArray[arrayIndex]]=assignments[i];//assign to timetable
				slotIndexArray[arrayIndex]++;
				tableSizeArray[arrayIndex]++;
				remainingHours--;
			}else if(slotIndexArray[arrayIndex]>=numOfTimeSlot){//if assignments can't insert into a slot
				printf("exceed slot limit\n");
				//find a meeting/revision slot
				//delete all meeting slots
				//fill in the slots
				//put null if some slot is not filled
				//if remainingHours != 0, mark %ofCompletion
				priorityReplacing1(TimeTable,tableSizeArray,remainingHours,assignments[i],totalDayCount,numOfTimeSlot,startDate);
				break;
			}
		}
	}





	//print time table
	for(i=0;i<totalDayCount;i++){
		printf(" day: %d ", i);
		for(j=0;j<numOfTimeSlot;j++){
			printf("%10.10s ",TimeTable[i][j].activityName);
		}
		printf("\n");
		
	}

	//calculate %ofCompletion 
	for(i=0;i<totalActivitiesCount;i++){
		//printf("%s %d %d \n",allActivities[i].activityName, percentageOfCompletion[i], allActivities[i].duration);	
		if(percentageOfCompletion[i]!=1){
			percentageOfCompletion[i]=100*(percentageOfCompletion[i]+allActivities[i].duration)/allActivities[i].duration;

		}else if(percentageOfCompletion[i]==1){
			percentageOfCompletion[i]=0;
		}
	}

	printf("percentageOfCompletion: \n");
	for(i=0;i<totalActivitiesCount;i++){
		printf("%13.13s %5d percent ",allActivities[i].activityName, percentageOfCompletion[i] );
		if(percentageOfCompletion[i]==0){
			printf(" Reject ");
		}else{
			printf(" Accept ");
		}
		printf("\n");
	}

	// print the rejected invalid activities.
	for(i=totalActivitiesCount;i<sizeof(allActivities)/sizeof(allActivities[0]);i++){
		printf("%s is rejected for invalid\n",allActivities[i].activityName );
	}

	

}


