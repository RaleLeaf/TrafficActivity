#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define MAX 15

typedef enum {
    Main, Div, Leftmain, Rightmain, Leftdiv, Rightdiv, PedestrianMain, PedestrianDiv
} movementType;

typedef struct {
    movementType priority;
    char movement[25];
    int time;
} Traffic;

typedef struct {
    Traffic t[MAX];
    int size;
    int front;
    int rear;
} TrafficQueue;

Traffic t;

Traffic createTraffic(movementType priority, char* movement, int time);
void display(Traffic t);
void priorityQueue(TrafficQueue arr);
void insertQueue(TrafficQueue *arr, Traffic t);
int processTraffic(TrafficQueue *arr);
Traffic dequeue(TrafficQueue *arr);
void readTrafficResult();

int main() {
	// Traffic list[5];
	// list[0]=createTraffic(2,"Leftmain",25);
	// list[1]=createTraffic(1,"Div",30);
	// list[2]=createTraffic(5,"Rightdiv",15);
	// list[3]=createTraffic(0,"Main",10);
	// list[4]=createTraffic(6,"PedestrianMain",5);
	
	// FILE *fp;
	// fp=fopen("traffic.dat","wb");
	
	// if(fp!=NULL){
	// 	fwrite(&list,sizeof(Traffic),5,fp);
	// }
	// fclose(fp);

    FILE *fp;
    TrafficQueue arr;
    arr.size = 0;
    arr.front = 0;
    arr.rear = 0;

    fp = fopen("traffic.dat", "rb");

    if(fp!=NULL){
        printf("\n");
        while (fread(&t, sizeof(Traffic), 1, fp)) {
            insertQueue(&arr, t);
        }
        fclose(fp);
    }else{
        printf("Failed to open traffic.dat.\n");
        return 1;
    }

	TrafficQueue saved=arr;
    int time = processTraffic(&arr);
	int x;
    printf("Queue:\n");
    for (x=saved.front; x<saved.front+saved.size;x++) {
        display(saved.t[x % MAX]);
        printf("\n");
    }
    printf("\nTotal Time Before Main Pedestrian: %ds\n\n", time);

    readTrafficResult();
    return 0;
}

Traffic createTraffic(movementType priority, char* movement, int time) {
    Traffic t;
    t.priority=priority;
    strcpy(t.movement, movement);
    t.time = time;
    return t;
}

void display(Traffic t){
    printf("%5d", t.priority);
    printf("\t %15s", t.movement);
    printf("\t%3d", t.time);
}

void insertQueue(TrafficQueue *arr, Traffic t) {
    int i=(arr->rear-1 + MAX)%MAX; 
    while(i >= arr->front && arr->t[i].priority > t.priority){
        arr->t[(i + 1) % MAX]=arr->t[i];
        i--;
    }

    arr->t[(i + 1)%MAX]= t;
    arr->rear=(arr->rear + 1)%MAX;
    arr->size++;
}

Traffic dequeue(TrafficQueue *arr) {
    Traffic t = arr->t[arr->front];

    FILE *dequeueFile=fopen("dequeued.dat", "ab");
    if(dequeueFile != NULL){
        fwrite(&t, sizeof(Traffic), 1, dequeueFile);
        fclose(dequeueFile);
    }

    arr->front=(arr->front + 1)%MAX;
    arr->size--;
    
    return t;
}

int processTraffic(TrafficQueue *arr) {
    int time = 0;
    Traffic t;
    while(arr->size>0){
        t = arr->t[arr->front];
        if(t.priority==PedestrianMain){
            break;
        }
        t=dequeue(arr);
        time+=t.time;
    }
    FILE *resultFile=fopen("traffic_result.dat", "ab");
    if(resultFile!=NULL){
        fwrite(&time, sizeof(int), 1, resultFile);
        fclose(resultFile);
    }
    return time;
}


void readTrafficResult() {
    FILE *resultFile=fopen("traffic_result.dat", "rb");
    if (resultFile!=NULL) {
        int time;
        printf("\nfrom traffic_result.dat:\n");
        while(fread(&time, sizeof(int),1,resultFile)==1){
            printf("%d seconds\n",time);
        }
        fclose(resultFile);
    }
}