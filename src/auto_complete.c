#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "btree.h"
#include "dictionary.h"
#include<gtk/gtk.h>

int compare_prefix_word(char*or_word,char*word)
{
  if(strlen(or_word)>strlen(word)) return 1;
  char tmp_word[WORD_LENGTH];
  strcpy(tmp_word,word);

  tmp_word[strlen(or_word)]='\0';//luôn nhớ xâu kí tự bắt đầu từ 0
  return strcmp(or_word,tmp_word);
}

void autocomplete(gpointer cb_data)
{
  data_give*pointer=(data_give*)cb_data;

  char or_word[WORD_LENGTH];
  strcpy(or_word, gtk_entry_get_text(GTK_ENTRY(pointer->GW3)));

  char word[WORD_LENGTH];
  char mean[1];

  int rsize;
  int check=1;

  g_signal_handler_block(pointer->selection, pointer->id);
  gtk_list_store_clear(pointer->store_autocomplete); 

  //khi clear list của treeView thì phải duyệt đến từng hộp-chưa các từ trong store để xóa => phải đọc từng hộp một => có tín hiệu "change" - hàm đọc change sẽ tự khởi động => vậy khi clear dưới sự hoạt động của hàm đọc change đã thiết lập, ta phải tra lại toàn bộ trong list => khắc phục bằng cách ngăt tín hiệu hàm đọc change 
  g_signal_handler_unblock(pointer->selection, pointer->id);

  btpos(pointer->Btree,1);
  while((btseln(pointer->Btree,word,mean,0,&rsize)==0)&&(check=compare_prefix_word(or_word,word))&&(check!=0));

  int num=0;
  if(check==0)
    do
      {
        num=num+1;if(num>1000) return;
        add_to_list_autocomplete(pointer->store_autocomplete,word);
      }while((btseln(pointer->Btree,word,mean,0,&rsize)==0)&&(compare_prefix_word(or_word,word)==0));

  return;
}
