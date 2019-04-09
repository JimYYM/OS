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
//check if the activity is valid or not
//if not, delete it from allActivities table.
int activityValidCheck(int numOfTimeSlot,int startTime,int endTime,int startDate[],int totalDayCount){//usable for all algorithm
	int i,j,x;struct task hold;
	struct task fakeTimeTable[totalDayCount][numOfTimeSlot];//fakeTable means it's only for checking, not the real timetable
	//init all slots in fakeTable by inserting emptySlot
	for(i=0;i<totalDayCount;i++){
		for(j=0;j<numOfTimeSlot;j++){
			fakeTimeTable[i][j]=emptySlot;
		}
	
	}
	//start checking
	int y,index;
	for(i=0;i<totalActivitiesCount;i++){
		if(allActivities[i].activityType==3 || allActivities[i].activityType==4){//only check meetings/revision
			x=calculateTwoDateDistance(startDate[0],startDate[1],startDate[2],allActivities[i].year,allActivities[i].month,allActivities[i].day);
			for(j=0;j<allActivities[i].duration;j++){
				y=allActivities[i].hour+j-startTime;
				if(fakeTimeTable[x-1][y].activityId!=-1){	//time crash
					printf("report %s invalid: time crash\n",allActivities[i].activityName );
					//delete added invalid activity
					index=1;
					while(fakeTimeTable[x-1][y-index].activityId==allActivities[i].activityId){
						fakeTimeTable[x-1][y-index]=emptySlot;
						index++;
					}
					
					discardInvalidActivity(allActivities[i]);//delete activity
					i--;//becaue the allActivities is changed 
					break;
				}
				//add to faketable if activity has no problem,then continue checking
				fakeTimeTable[x-1][y]=allActivities[i];
			}
			//duration check
			if(allActivities[i].hour+allActivities[i].duration>endTime){
				printf("report %s invalid: exceed time limit\n",allActivities[i].activityName );
				discardInvalidActivity(allActivities[i]);
				i--;//becaue the allActivities is changed 

			}
		}
	}
	

	for(i=0;i<totalDayCount;i++){//print check table
		printf("%dth, day: %d ",startDate[2]+i, i);
		for(j=0;j<numOfTimeSlot;j++){
			printf("%s ",fakeTimeTable[i][j].activityName);
		}
		printf("\n");
		
	}
	return 0;

}