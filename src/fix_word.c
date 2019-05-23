#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "btree.h"
#include "dictionary.h"

void fix_word(GtkWidget * widget, gpointer cb_data)
{
  data_give*pointer=(data_give*)cb_data;

  GtkTextIter edit_view_start,edit_view_end;
 gtk_text_buffer_get_start_iter(gtk_text_view_get_buffer(GTK_TEXT_VIEW(pointer->GW6)), &edit_view_start);
  //Initialized iter with the first position in the text buffer. This is the same as using gtk_text_buffer_get_iter_at_offset() to get the iter at character offset 0.
  gtk_text_buffer_get_end_iter(gtk_text_view_get_buffer(GTK_TEXT_VIEW(pointer->GW6)), &edit_view_end);

  char new_mean[MEAN_LENGTH];
  strcpy(new_mean, gtk_text_buffer_get_text(gtk_text_view_get_buffer(GTK_TEXT_VIEW(pointer->GW6)), &edit_view_start, &edit_view_end, FALSE));

  int check=btupd(pointer->Btree,pointer->wordNow, new_mean, MEAN_LENGTH);

  if(check==0)
        {
          pointer->mess_info->title=NULL;
          char c[]="Add";
          pointer->mess_info->GW1=pointer->GW5;
          pointer->mess_info->text1=c;
          char d[]="Successfully fixed word";
          pointer->mess_info->text2=d;
          show_mess(widget,pointer->mess_info);
          pointer->flag=0;
          gtk_entry_set_text(GTK_ENTRY(pointer->GW3),pointer->wordNow); 
          find_function(pointer->GW3,cb_data);

        }
      else
        {
          pointer->mess_error->title=NULL;
          char c[]="Add";
          pointer->mess_error->GW1=pointer->GW5;
          pointer->mess_error->text1=c;
          char d[]="Faild to fixed word";
          pointer->mess_error->text2=d;
          show_mess(widget,pointer->mess_error);
          pointer->flag=1;
        }
return;
}
