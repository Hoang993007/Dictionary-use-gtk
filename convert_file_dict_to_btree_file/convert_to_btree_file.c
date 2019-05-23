#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"btree.h"

#define WORD_LENGTH 100
#define MEAN_LENGTH 100000

int main(int argc, char *argv[])
{
  if(argc!=3) printf("not enough argumen that function require\n");

  BTA*btree;
  btree=btcrt(argv[2],0,0);
  //argv[2] là tên của file muốn tạo

  int num=0;
  FILE*converted;
  //argv[1] là file gốc cần convert
  if((converted=fopen(argv[1],"r"))==NULL)
    {
      printf("file input can't not open\n");
      exit(0);
    }

  fseek(converted,0,SEEK_SET);
  while(!feof(converted))
    {
      char  word[WORD_LENGTH];
      char mean[MEAN_LENGTH];

      fscanf(converted,"%[^\t]s",word);//load word from dictionary
      fgetc(converted);//đọc kí tự \t còn sót lại

      char t;
      int i=-1;
      while((t=fgetc(converted))&&(t!='\n'))
        {
          i++;
          if(t=='\\')
            {
         fseek(converted,1,SEEK_CUR);
         mean[i]='\n';
            }
          else
            mean[i]=t;
        }
      i=i+1;
      mean[i]='\0';
      //load mean from dictionary
      do
        {
          t=fgetc(converted);
          if((t!='\n')&&(t!=EOF)) fseek(converted,-1,SEEK_CUR);
             }while(t=='\n');//kiểm tra hết file hay chưa

          int check=btins(btree,word,mean,strlen(mean)+1);

          if(check==0)
            {
              num=num+1;
              printf("successfully loaded word '%s'\n",word);
            }
          else
            printf("fail to loaded word '%s'\n",word);
        }

  printf("Program had successfully loaded %d word from file dictionary\n",num);
  fclose(converted);
  btcls(btree);
  return 0;
}
