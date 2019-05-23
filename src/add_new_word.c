#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "btree.h"
#include "dictionary.h"
#include<gtk/gtk.h>

void add_new_word(GtkWidget*widget,gpointer cb_data)
{
  data_give*pointer=(data_give*)cb_data;

  //những gì hiển thị trên text view được lưu trữ tạm thời trên một bộ nhớ đệm

  //khởi tạo một số con trỏ chạy trên dữ liệu text được lưu trong buffer
  GtkTextIter word_add_view_start,word_add_view_end;

  gtk_text_buffer_get_start_iter(gtk_text_view_get_buffer(GTK_TEXT_VIEW(pointer->GW2)), &word_add_view_start);
  //Initialized iter with the first position in the text buffer. This is the same as using gtk_text_buffer_get_iter_at_offset() to get the iter at character offset 0.
  gtk_text_buffer_get_end_iter(gtk_text_view_get_buffer(GTK_TEXT_VIEW(pointer->GW2)), &word_add_view_end);

  char word_new[WORD_LENGTH];
  strcpy(word_new, gtk_text_buffer_get_text(gtk_text_view_get_buffer(GTK_TEXT_VIEW(pointer->GW2)), &word_add_view_start, &word_add_view_end, FALSE));

  //Làm việc với BUFFER

  GtkTextIter mean_add_view_start,mean_add_view_end;

  gtk_text_buffer_get_start_iter(gtk_text_view_get_buffer(GTK_TEXT_VIEW(pointer->GW3)), &mean_add_view_start);
  gtk_text_buffer_get_end_iter(gtk_text_view_get_buffer(GTK_TEXT_VIEW(pointer->GW3)), &mean_add_view_end);

  char mean[MEAN_LENGTH];
  strcpy(mean,gtk_text_buffer_get_text(gtk_text_view_get_buffer(GTK_TEXT_VIEW(pointer->GW3)), &mean_add_view_start, &mean_add_view_end, FALSE));

  if((word_new[0]=='\0')&&(mean[0]=='\0')) {pointer->flag=1;return;}

  if(word_new[0]=='\0')
    {
      pointer->mess_info->title=NULL;
      char c[]="Add";
      pointer->mess_info->GW1=pointer->GW1;
      pointer->mess_info->text1=c;
      char d[]="You haven't input word!";
      pointer->mess_info->text2=d;
      show_mess(widget,pointer->mess_info);
      pointer->flag=1;return;
    }

  if(mean[0]=='\0')
    {
      pointer->mess_info->title=NULL;
      char c[]="Add";
      pointer->mess_info->GW1=pointer->GW1;
      pointer->mess_info->text1=c;
      char d[]="You haven't input mean!";
      pointer->mess_info->text2=d;
      show_mess(widget,pointer->mess_info);
      pointer->flag=1;return;
    }

  chuanHoaXau(word_new);
  chuanHoaXau(mean);

  char mean_finded[1];
  int rsize;//bắt buộc
  int check_exist=btsel(pointer->Btree,word_new,mean_finded,0,&rsize);
  if(check_exist==0)
    {
      pointer->mess_error->title=NULL;
      char c[]="Add";
      pointer->mess_error->GW1=pointer->GW1;
      pointer->mess_error->text1=c;
      char d[]="This word have already exist!";
      pointer->mess_error->text2=d;
      show_mess(widget,pointer->mess_error);
      pointer->flag=1;return;
    }
  else
    {
      int check=btins(pointer->Btree,word_new,mean,strlen(mean)+1);

      if(check==0)
        {
          pointer->mess_info->title=NULL;
          char c[]="Add";
          pointer->mess_info->GW1=pointer->GW1;
          pointer->mess_info->text1=c;
          char d[]="Successfully added new word";
          pointer->mess_info->text2=d;
          show_mess(widget,pointer->mess_info);
        
          BTA*btree_soundex=btopn("Dict_soundex.dat",0,0);
          char*soundex_code=(char*)soundex(word_new);
          btchgr(btree_soundex,soundex_code);
          btins(btree_soundex,word_new,"",0);
          btcls(btree_soundex);

          pointer->flag=0;return;
        }
      else
        {
          pointer->mess_error->title=NULL;
          char c[]="Add";
          pointer->mess_error->GW1=pointer->GW1;
          pointer->mess_error->text1=c;
          char d[]="Faild to added new word";
          pointer->mess_error->text2=d;
          show_mess(widget,pointer->mess_error);
          pointer->flag=1;return;
        }
    }
}

/*RETURN VALUE 
  1 : UNSUCESSFULL
  0 : SUCCESSFULL*/
