#include <stdio.h>
#include <ctype.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>


typedef struct Node {
	char *value;
	int charValue;
	int count;
	struct Node * left;
	struct Node * right;
	struct Node * next;
	int type;
}Node;

char* print_bits(int ch) {
	char *result = (char *)malloc(sizeof(char) * (CHAR_BIT + 1));
	int array[CHAR_BIT];
	int bit_index = 0;
	int temp = ch;
	if(ch == ' '){
		printf(" ");
	} else {
		for (int i = CHAR_BIT - 1; i >= 0; i--) {
			array[i] = temp % 2;
			result[i] = temp % 2 + '0';
			temp = temp / 2;
			
		}
		result[CHAR_BIT] = '\0';
	
		/*for (int i = 0; i <= CHAR_BIT - 1; i++) {
			printf("%d", array[i]);
		}
		
		printf("\n%s\n", result);*/
	}
	
	return result;
}

void print_tree(struct Node* cur){
	if(cur == NULL){
		return;
	}
	if(cur->type == 0){
		printf("0");
	} else {
		printf("1%s", cur->value);
	}
	print_tree(cur->left);
	print_tree(cur->right);
}

void print_tree_to_cstring(struct Node* cur, char* result){
	if(cur == NULL){
		return;
	}
	if(cur->type == 0){
		strcat(result, "0");
		//printf("0 ");
	} else {
		strcat(result, "1");
		strcat(result, cur->value);
		//printf("1 %s ", cur->value);
	}
	print_tree_to_cstring(cur->left, result);
	print_tree_to_cstring(cur->right, result);
}

void update_bit_string(struct Node *cur, char *bitStringArray[], char *currentString, int leftOrRight){
	if(cur != NULL && cur->type == 1){
		int temp = cur->charValue;
		if (cur->charValue == -1){
			temp = 255;
		} 
		bitStringArray[temp] = (char *)malloc(sizeof(char) * (strlen(currentString) + 1));
		strcpy(bitStringArray[temp], currentString);
		//printf("%d--> %s\n", temp, bitStringArray[temp]);
		return;
	}
	if(cur->type == 0){
		char* temp = "0";
		strcat(currentString, temp);
		
		update_bit_string(cur->left, bitStringArray, currentString ,0);
		
		currentString[strlen(currentString)-1] = '1';
		
		update_bit_string(cur->right, bitStringArray, currentString, 1);
		
		if(strlen(currentString) == 0){
			return;
		} else {
			currentString[strlen(currentString)-1] = '\0';
		}
		
		/*
		//printf("0 ");
		if(leftOrRight == 0) {
			char* temp = "0";
			strcat(currentString, temp);
			printf("%s", currentString);
			leftOrRight = 1;
		} else {
			char* temp = "1";
			strcat(currentString, temp);
			printf("%s", currentString);
			leftOrRight = 0;
		}*/
		
	} /*else {
		bitStringArray[cur->charValue] = currentString;
		if(leftOrRight == 0) {
			leftOrRight = 1;
		} else {
			leftOrRight = 0;
		}
	}
	strncpy(currentString, currentString, strlen(currentString)-1);
	update_bit_string(cur->left, bitStringArray, currentString ,0);
	update_bit_string(cur->right, bitStringArray, currentString, 1);*/
}

void cleanup(struct Node *cur){
	if(cur == NULL) {
		return;
	}
	
	if(cur->type == 1) {
		free(cur);
		return;
	} else {
		cleanup(cur->left);
		cleanup(cur->right);
		return;
	}
}

int main(int argc, char*argv[]){
	char c;
	char *binaryCode;
	int counts[256];
	char *chaArray[256];
	char *bitStringArray[256];
	char currentString[256];
	currentString[0] = '\0';
	
	int listSize = 1;
	struct Node *head = (struct Node *)malloc(sizeof(Node));
	head->count = 1;
	head->value = "11111111";
	head->charValue = -1;
	head->type = 1;
	head->left = NULL;
	head->right = NULL;
	head->next = NULL;
	
	int nextIndex = 0; // next node index, -1 for head(EOF) right now.
	
	for(int i = 0; i < 256; i++){
		counts[i] = 0;
		bitStringArray[i] = NULL;
	}
	
	FILE *fp=fopen(argv[1],"r+");
	c=fgetc(fp);
	while(c != EOF) {
		/*if(counts[c] == 0) {
			struct Node * newNode = (struct Node *)malloc(sizeof(Node));
			newNode->count = 1;
			newNode->value = print_bits(c);
			newNode->type = 1;
			newNode->left = NULL;
			newNode->right = NULL;
			newNode->next = NULL;
			
		} else {
			
		}*/
		chaArray[c] = print_bits(c);
		counts[c]++;
		//printf("%s", binaryCode);
		c=fgetc(fp);
	}
	for(int i = 0; i < 256; i++){
		if(counts[i] > 0) {
			//printf("%s: %d\n", chaArray[i], counts[i]);
			Node * newNode = (struct Node *)malloc(sizeof(Node));
			newNode->value = chaArray[i];
			newNode->charValue = i;
			newNode->count = counts[i];
			newNode->type = 1;
				
			if(counts[i] <= head->count) {
				newNode->next = head;
				head = newNode;
				
			} else {
				Node * cur = head;
				Node * pre = NULL;
				while(cur != NULL && counts[i] > cur->count){
					pre = cur;
					cur = cur->next;
				}
				newNode->next = cur;
				pre->next = newNode;
			}
			listSize++;
			
		}
		// set up nodes with non zero characters, and sorting the list with insert sort
		
		
	}
	//printf("%d\n", listSize);
	
	/*Node *cur = head;
	while(cur != NULL){
		printf("%s: %d\n", cur->value, cur->count);
		cur = cur->next;
	}*/
	
	
	// set up tree and get the bit strings
	Node * newNode;
	while(listSize > 1){
		Node *left = head;
		Node *right = head->next;
		newNode = (struct Node *)malloc(sizeof(Node));
		newNode->value = "";
		newNode->count = left->count + right->count;
		newNode->type = 0;
		newNode->left = left;
		newNode->right = right;
		
		head = head->next->next;
		if(head == NULL){
			head = newNode;
		} else {
			Node * cur = head;
			Node * pre = right;
			while(cur != NULL && newNode->count > cur->count){
				pre = cur;
				cur = cur->next;
			}
			newNode->next = cur;
			pre->next = newNode;
		}
		listSize--;
	}
	
	free(newNode);
	
	// preorder print
	//print_tree(head);
	//printf("\n\n");
	
	// get tree into a cstring
	char * resultString = (char*) malloc(sizeof(char) * 1000);
	print_tree_to_cstring(head, resultString);
	//printf("%s\n",resultString);
	
	// update the bitstring array based on the tree
	update_bit_string(head, bitStringArray, currentString, 0);
	
	//get the EOF string
	strcat(resultString, bitStringArray[255]);
	
	/*printf("\n\n");
	
	for(int i = 0; i < 256; i++){
		if(bitStringArray[i] != NULL){
			printf("%s\n", bitStringArray[i]);
		}
	}*/
	
	
	// go through the file again to get the coded strings
	fseek( fp, 0, SEEK_SET );
	c=fgetc(fp);
	while(c != EOF) {
		//printf("%s ", bitStringArray[c]);
		strcat(resultString, bitStringArray[c]);
		c=fgetc(fp);
	}
	//printf("%s", bitStringArray[255]);
	strcat(resultString, bitStringArray[255]);
	
	//printf("\n%s with length %ld\n",resultString, strlen(resultString));
	
	// write byte to binary file
	int numBytes = strlen(resultString) / CHAR_BIT;
	int numFillBits = CHAR_BIT - strlen(resultString) % CHAR_BIT;
	//printf("Will write %d bytes with %d filling zeros\n", numBytes+1, numFillBits);
	for(int i = 0; i < numFillBits;i++){
		strcat(resultString, "0");
	}
	//printf("\n%s with filled zeros\n",resultString);
	
	
	char file_name[] = "outbut.bin";
	FILE * fpb = fopen(file_name, "wb");

	if (fpb == NULL)  /* If an error occurs during the file creation */
	{
		fprintf(stderr, "fopen() failed for '%s'\n", file_name);
	}
	else
	{
		unsigned char tempChar = 0;
		unsigned char *temp = &tempChar;
		for (int i = 0; i < numBytes+1; i++){
			
			for(int j = 0; j < 8; j++){
				*temp = *temp << 1;
				*temp |= (*(resultString + i*8 + j) - '0');
				
				//printf("%d\n", tempChar);
			}
			//printf("%x\n", tempChar);
			/* Writes str (_including_ the NUL-terminator) to the binary file. */
			size_t elements_written = fwrite(temp, 1, 1, fpb); 
			*temp = 0;
		}
		
	}
	free(resultString);
	cleanup(head);
	fclose(fp);
	fclose(fpb);
	return 0;
}