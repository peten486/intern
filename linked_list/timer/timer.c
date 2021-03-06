﻿#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

typedef struct _list {
	struct _node *head;
	struct _node *tail;
} linkedList;

typedef struct _node {
	int id;
	time_t cur_time;
	int expire_time;
	struct _node *next;
} node;

void insertNode(linkedList *L, int id, int expire_time){
	node *newNode = (node *)malloc(sizeof(node));
	newNode->next = NULL;
	newNode->id = id;
	newNode->expire_time = expire_time;
	time(&newNode->cur_time);
	if(L->head == NULL && L->tail == NULL){
		L->head = L->tail = newNode;
	} else {
		node *curNode = L->head;
		node *preNode = L->head;
		while(curNode != NULL){
			if(curNode == L->head && curNode->expire_time >= newNode->expire_time){
				newNode->next = curNode;
				L->head = newNode;
				break;
			} else if(curNode->next == NULL && newNode->expire_time > curNode->expire_time){
                                curNode->next = newNode;
                                L->tail = newNode;
                                break;
                        } else if(curNode != L->head && curNode->expire_time >= newNode->expire_time){
				newNode->next = curNode;
				preNode->next = newNode;
				break;
			}
			preNode = curNode;
			curNode = curNode->next;
		}
	}
}


int getListNumber(linkedList *L){
	node *cur = L->head;
	if(L->head == NULL) return 0;
	int cnt = 1;
	while(cur->next != NULL){
		cur = cur->next;
		cnt++;
	}
	return cnt;
}


void deleteLastNode(linkedList *L){
	node *p = L->head;
	node *delNode;
	while(p->next->next != NULL) p = p->next;
	delNode = p->next;
	p->next = p->next->next;
	L->tail = p;
	free(delNode);
}

int deleteNode(linkedList *L, node *delNode){
	int chk = -1;
	if(delNode == L->head){
		L->head = delNode->next;
		free(delNode);
		return 1;
	} else if(delNode->next == NULL){
		deleteLastNode(L);
		return 1;	
	} else {
		node *cur = L->head;
		while(cur->next != delNode) cur = cur->next;
		cur->next = cur->next->next;
		free(delNode);
		return 1;
	}
	return chk;
}

int deleteNodeId(linkedList *L, int id){
	int chk = -1;
	node *cur = L->head;
	while(cur->id != id){
		cur = cur->next;
	}
	if(cur->id == id){	
		chk = deleteNode(L, cur);
	}
	return chk;
}


void printNodes(linkedList *L){
	if(getListNumber(L) == 0){
		return;
	}
	node *cur = L->head;
	time_t now;
	time(&now);

	printf("id texpire_time cur_time(시:분:초)\n");
	while(cur != NULL){
		printf("%d \t %d\t", cur->id, cur->expire_time);
		struct tm *t;
		t = localtime(&cur->cur_time);
        	printf("%02d:%02d:%02d\n", t->tm_hour, t->tm_min, t->tm_sec);
		cur = cur->next;
	}
}

int expire(linkedList *L){
	int chk = 0;
	node *cur = L->head;
	time_t now;
	time(&now);
	
	while(cur != NULL){
		if( (now - cur->cur_time) > cur->expire_time){
			node* delNode = cur;
			cur = cur->next;
			chk = deleteNode(L,delNode);
			continue;
			printfNodes(L);
		}
		cur = cur->next;
	}
	return chk;
}	

void printTime(){
	time_t now;
	struct tm *t;
	now = time(NULL);
	t = localtime(&now);
	printf("%02d:%02d:%02d\n", t->tm_hour, t->tm_min, t->tm_sec);
}

int main(void){
	linkedList *L = (linkedList *)malloc(sizeof(linkedList)); 
	L->head = NULL; 
	L->tail = NULL;

	char temp[255];
	char *tok;
	char *context;

	printf("input id, expire time...(ex) 1 20\n");	
	while(1){
		int new_id = 0;
		int new_expire_time = 0;
		
		fgets(temp,255,stdin);
		tok = strtok_r(temp, " ", &context);
		new_id = atoi(tok);

		if(new_id < 0) break;
			
		tok = strtok_r(NULL, " ", &context);
		new_expire_time = atoi(tok);		

		insertNode(L, new_id, new_expire_time); 
	}	
	printf("현재시간(시:분:초) : ");
	printTime();
	printNodes(L);

	int delId = -1;
	while(1){
		printf("delete id : ");
		scanf("%d",&delId);
		if(getListNumber(L) <= 0){
			break;
		}

		if(delId != -1){
			deleteNodeId(L, delId);
		} else {
			printf("삭제 종료\n");
			break;
		}
		printNodes(L);
	}				
		
	while(1){
		if(getListNumber(L) <= 0) {
			printf("리스트에 데이터가 없습니다.\n");
			break;
		}
		if(expire(L) == 1){
			printNodes(L);
		}
	}

	return 0;
}