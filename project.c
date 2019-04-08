#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


struct task {
   int activityId;
   char activityName[20];
   int year;
   int month;
   int day;
   int hour;
   int duration;
   int activityType;
}allActivities[8]={
		{0001,"COMP2432A1",2019,04,18,-1,12,2},
		{0002,"COMP2422P1",2019,04,20,-1,26,1},
		{0003,"COMP2000",2019,04,14,19,2,3},
		{0004,"Meeting1",2019,04,18,20,2,4},
		{0005,"Meeting2",2019,04,18,20,6,4},
		{0006,"Meeting3",2019,04,9,21,2,4},
		{0007,"myAssignment2",2019,04,12,-1,4,2},
		{8,"myProject2",2019,04,9,-1,3,1}
};
int totalActivitiesCount=8;
int percentageOfCompletion[8]={0,0,0,0,0,0,0,0};
struct task emptySlot={-1,"Empty",-1,-1,-1,-1,-1,-1};

int dateToArrayIndex(int year, int month, int date,int startDate[]){
	return date-startDate[2];//date of activity - startDate
}
int smallestIndex(int quereIndexArray[],int projectDueDay,int startDate[]){
	int i;
	int minValue=100;
	int minIndex=0;

	for(i=0;i<=projectDueDay-startDate[2];i++){
		if(quereIndexArray[i]<minValue){
			minValue=quereIndexArray[i];
			minIndex=i;
		}
	}
	return minIndex;
}

//this method is luanched when the quere is full, project/assignment can't put in quere. 
//eg. project replace revision/meetings
//eg. assignment replace revision/meetings

int priorityReplacing1(struct task quereArray[][totalActivitiesCount],int quereIndexArray[],int remainingHours, struct task activity,int startDate[],int numOfTimeSlot){
	int i,j,k;
	struct task temp;
	printf("who am i %s\n",activity.activityName );
	//find meeting slot
	for(i=0;i<=activity.day-startDate[2];i++){//loop in quereArray 
		for(j=0;j<numOfTimeSlot;j++){
			if(quereArray[i][j].activityType==4){//replace the meetings slot first
				printf("change from %s\n",quereArray[i][j].activityName );
				percentageOfCompletion[quereArray[i][j].activityId-1]=1;//reject
				temp=quereArray[i][j];
				quereArray[i][j]=activity;//replace it with project/
				remainingHours--;

			}
			if (remainingHours==0){
				for(k=j+1;k<numOfTimeSlot;k++){
					if(quereArray[i][k].activityId==temp.activityId){
						quereArray[i][k]=emptySlot;
					}
				}
				return 0;
			}
		}
	}
	//find revision slot
	for(i=0;i<=activity.day-startDate[2];i++){//loop in quereArray 
		for(j=0;j<numOfTimeSlot;j++){
			if(quereArray[i][j].activityType==3){//replace the revision slot 
				printf("change from %s\n",quereArray[i][j].activityName );
				printf("here %d\n", i);
				percentageOfCompletion[quereArray[i][j].activityId-1]=1;//reject
				temp=quereArray[i][j];
				quereArray[i][j]=activity;//replace it with project/assignment
				remainingHours--;

			}
			if (remainingHours==0){
				for(k=j+1;k<numOfTimeSlot;k++){
					if(quereArray[i][k].activityId==temp.activityId){
						quereArray[i][k]=emptySlot;
					}
				}
				return 0;
			}
		}
	}
	if(remainingHours>0){	//if the project.assignment can't put in quere at the end
		percentageOfCompletion[activity.activityId-1]-=remainingHours;	//deduct the %ofCompletion
	}
	return 0;
}
// check if activity is valid or not
// if not valid{
// 		totalActivitiesCount--,
// 		put that activity to the last of the allActivities array
// } 
//not done
int activityValidCheck(int numOfTimeSlot,int startTime,int startDate){//ok for all algorithm
	int i;struct task hold;
	int fakeTimeTable[totalActivitiesCount][numOfTimeSlot];
	for(i=0;i<totalActivitiesCount;i++){
		if(allActivities[i].type==3 || allActivities[i].type==4){
			...
		}

	}

}
int main(int argc, char *argv[]){
	//period
	int startDate[3]={2019,4,8};
	int endDate[3]={2019,4,21};
	int startTime=19;
	int endTime=23;
	int numOfTimeSlot=endTime-startTime;
	int totalDayCount=endDate[2]-startDate[2]+1;
	
	activityValidCheck(numOfTimeSlot,startTime,startDate,totalDayCount);


	//store all type 1 activities
	struct task projects[10];
	int proj_index=0;

	//store all type 2 activities
	struct task assignments[10];
	int assign_index=0;

	//store all type 3 activities
	struct task revisions[10];
	int rev_index=0;

	//store all type 4 activities
	struct task meetings[10];
	int meet_index=0;

	int i;

	//add all projects to projects array
	//add all assignments to assignments array. etc.
	for(i=0;i<totalActivitiesCount;i++){
		if(allActivities[i].activityType==1){
			projects[proj_index]=allActivities[i];
			proj_index++;
		}
		else if(allActivities[i].activityType==2){
			assignments[assign_index]=allActivities[i];
			assign_index++;
		}
		else if(allActivities[i].activityType==3){
			revisions[rev_index]=allActivities[i];
			rev_index++;
		}
		else if(allActivities[i].activityType==4){
			meetings[meet_index]=allActivities[i];
			meet_index++;
		}
	}


	
	struct task quereArray[totalDayCount][numOfTimeSlot];//quere of every day
	int quereIndexArray[totalDayCount];//the current size of each quere


	for(i=0;i<totalDayCount;i++){//assign the size to be 0 for every quere
		quereIndexArray[i]=0;
	}

	int arrayIndex;//temp

	//put revision task to quere, assumed input is correct
	int j;
	for(i=0;i<rev_index;i++){
		arrayIndex=dateToArrayIndex(revisions[i].year,revisions[i].month,revisions[i].day,startDate);
		for(j=revisions[i].duration;j>0;j--){
			quereArray[arrayIndex][quereIndexArray[arrayIndex]]=revisions[i];
			quereIndexArray[arrayIndex]++;
		}
	}
	//put meeting task to quere, assumed input is correct
	for(i=0;i<meet_index;i++){
		arrayIndex=dateToArrayIndex(meetings[i].year,meetings[i].month,meetings[i].day,startDate);
		for(j=meetings[i].duration;j>0;j--){
			quereArray[arrayIndex][quereIndexArray[arrayIndex]]=meetings[i];
			quereIndexArray[arrayIndex]++;
		}
	}

	//print quere/timetable
	
	for(i=0;i<totalDayCount;i++){
		printf("%dth, day: %d ",startDate[2]+i, i);
		for(j=0;j<quereIndexArray[i];j++){
			printf("%s ",quereArray[i][j].activityName);
		}
		printf("\n");
		
	}

	int remainingHours;
	//put project task to quere
	for(i=0;i<proj_index;i++){
		remainingHours=projects[i].duration;
		while(remainingHours!=0){//split each request to hour based
			arrayIndex=smallestIndex(quereIndexArray,projects[i].day,startDate);//find smallest index before due day
			if(quereIndexArray[arrayIndex]<numOfTimeSlot){
				quereArray[arrayIndex][quereIndexArray[arrayIndex]]=projects[i];//assign to quere
				quereIndexArray[arrayIndex]++;
				remainingHours--;
			}else if(quereIndexArray[arrayIndex]>=numOfTimeSlot){//if project can't insert into a slot
				//find a meeting/revision slot
				//replace it
				//%of completion
				//priorityReplacing(quereArray,quereIndexArray,remainingHours,projects[i],startDate,numOfTimeSlot);

				//find a meeting/revision slot
				//delete all meeting slots
				//fill in the slots
				//put null if some slot is not filled
				//if remainingHours != 0, mark %ofCompletion
				priorityReplacing1(quereArray,quereIndexArray,remainingHours,projects[i],startDate,numOfTimeSlot);

				break;
			}

		}
	}

	

	for(i=0;i<assign_index;i++){
		remainingHours=assignments[i].duration;
		while(remainingHours!=0){//split each request to hour based
			arrayIndex=smallestIndex(quereIndexArray,assignments[i].day,startDate);//find smallest index before due day
			if(quereIndexArray[arrayIndex]<numOfTimeSlot){
				quereArray[arrayIndex][quereIndexArray[arrayIndex]]=assignments[i];//assign to quere
				quereIndexArray[arrayIndex]++;
				remainingHours--;
			}else if(quereIndexArray[arrayIndex]>=numOfTimeSlot){//if project can't insert into a slot
				//find a meeting/revision slot
				//replace it
				//%of completion
				priorityReplacing1(quereArray,quereIndexArray,remainingHours,assignments[i],startDate,numOfTimeSlot);
				break;
			}

		}
	}

	//print quere/timetable

	for(i=0;i<totalDayCount;i++){
		printf("%dth, day: %d ",startDate[2]+i, i);
		for(j=0;j<quereIndexArray[i];j++){
			printf("%s ",quereArray[i][j].activityName);
		}
		printf("\n");
		
	}
//calculate %ofCompletion 
	//print quere
	for(i=0;i<totalActivitiesCount;i++){
		printf("%s %d %d \n",allActivities[i].activityName, percentageOfCompletion[i], allActivities[i].duration);
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


}
