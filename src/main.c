#include <stdio.h>
#include <stdlib.h>
#include "../header-files/functions.h"
#include <unistd.h>
#include <string.h>

int main(int argc,char** argv)
{

	uint32_t size_A=150;
	int32_t A[size_A];
	uint32_t size_B=50;
	int32_t B[size_B];

	writeFile(size_A,size_B);

	readFile(A,&size_A,B,&size_B);

	resultList* resList;
	oneColumnRelation *S,*R;

	createRelations(A,size_A,B,size_B,&S,&R);
	resList = RadixHashJoin(R,S,size_A,size_B);
	printResults(resList);

	deleteResultList(resList);


	//ta kainourgia -----------------------------------------------------------------------------------------------


	printf("\n\n/////////////////////////////////////\n\n\n");
	char *filename,*paths,*onepath;
	FILE *fp;
	int fsize,i,j,k,pointer,number_of_files;
	multiColumnRelation *relationArray;

	if (argc!=2){
		printf("\n\nWRONG NUMBER OF ARGS!!!\n\n\n");
		//return 1;
	}
	filename=malloc(strlen("input-files/")+strlen(argv[1])+1);//dineis mono to arxeio , to path to ftiaxnw edw
	strcpy(filename,"input-files/");
	strcat(filename,argv[1]);

	printf("filename=%s\n",filename);

	fp=fopen(filename,"rb");

	fseek(fp,0,SEEK_END);
	fsize = ftell(fp);
	fseek(fp,0,SEEK_SET);
	paths=malloc(fsize+1);
	fread(paths,fsize,1,fp);
	fclose(fp);
	paths[fsize-1]='\0';
	
	printf("paths:\n%s\n",paths);

	//koitazw posa arxeia exw
	if(fsize==0){//na to dw pali
		printf("\n\nKENO ARXEIO !!!\n\n");
		return 1;
	}
	number_of_files=1;
	for(i=0;i<fsize;i++){
		if(paths[i]=='\n'){
			number_of_files++;
		}
	}
	printf("number_of_files=%d\n",number_of_files);

	//ftiaxnw ton pinaka relationArray kai pairnw ta data apo ka8e arxeio
	relationArray=malloc(number_of_files*sizeof(multiColumnRelation));

	onepath=malloc(strlen("input-files/")+fsize+1);
	pointer=0;
	for(i=0;i<number_of_files;i++){
		memset(onepath,'\0',fsize);
		strcpy(onepath,"input-files/");
		j=strlen("input-files/");
		while( paths[pointer]!='\n' && paths[pointer]!='\0' ){
			onepath[j]=paths[pointer];
			j++;
			pointer++;
		}
		pointer++;
		printf("onepath :%s...\n",onepath);

		fp=fopen(onepath,"rb");

		uint64_t temp;
		
		fread(&temp,sizeof(uint64_t),1,fp);
		relationArray[i].rowCount=temp;
		fread(&temp,sizeof(uint64_t),1,fp);
		relationArray[i].colCount=temp;

		relationArray[i].table=malloc(relationArray[i].colCount*sizeof(uint64_t *));

		for(j=0;j<relationArray[i].colCount;j++){
			relationArray[i].table[j]=malloc(relationArray[i].rowCount*sizeof(uint64_t));//mporei na prepei na bgoun eksw
			for(k=0;k<relationArray[i].rowCount;k++){
				fread(&temp,sizeof(uint64_t),1,fp);
				//for(int k=0;k<sizeof(uint64_t);k++){
					//printf("%ld ",temp);
				//}
				relationArray[i].table[j][k]=temp;
			}
			//printf("\n");
		}

		fclose(fp);
	}

	/*for(i=0;i<number_of_files;i++){
		for(j=0;j<relationArray[i].colCount;j++){
			for(k=0;k<relationArray[i].rowCount;k++){
				printf("%ld ",relationArray[i].table[j][k]);
			}
			printf("\n\nend of %d\n\n",i);
		}
		sleep(20);
	}*/

	free(filename);
	free(onepath);
	free(paths);

	for(i=0;i<number_of_files;i++){
		for(j=0;j<relationArray[i].colCount;j++){
			free(relationArray[i].table[j]);
		}
		free(relationArray[i].table);
	}
	free(relationArray);

	readWorkFile(argv[2]);
	//telos ta kainourgia ------------------------------------------------------------------------------------------

	//diabazoume ta r0
	/*printf("\n\n/////////////////////////////////////\n\n");
	FILE *fp;
	fp=fopen("input-files/r0","rb");
	unsigned char buffer[8];
	uint64_t temp,tuples,cols;
	
	fread(&temp,sizeof(uint64_t),1,fp);
	tuples=temp;
	fread(&temp,sizeof(uint64_t),1,fp);
	cols=temp;
	for(int j=0;j<cols;j++){
		for(int i=0;i<tuples;i++){
			fread(&temp,sizeof(uint64_t),1,fp);
			//for(int i=0;i<sizeof(uint64_t);i++){
				printf("%d ",temp);
			//}
			
		}
		printf("\n");
	}
	fclose(fp);*/
	//

}

