#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "btree.h"
#include "dictionary.h"
#include<gtk/gtk.h>

void suggest_use_soundex(gpointer cb_data)
{
  data_give*pointer=(data_give*)cb_data;

  char word_suggest[WORD_LENGTH];
  strcpy(word_suggest, gtk_entry_get_text(GTK_ENTRY(pointer->GW3)));

  chuanHoaXau(word_suggest);

  char *soundex_code=(char*)soundex(word_suggest);
  BTA*btree_soundex=btopn("Dict_soundex.dat",0,0);

  char word[WORD_LENGTH];
  char mean[1];

  int check;
  if(check=btchgr(btree_soundex,soundex_code)!=0) 
    {
      pointer->mess_info->title=NULL;
      char c[]="Suggest";
      pointer->mess_info->GW1=pointer->GW1;
      pointer->mess_info->text1=c;
      char d[]="That is a new word\nmay be you wrong or we did'n know is\ndo you want to add it to your own dictionary?";
      pointer->mess_info->text2=d;
      show_mess(pointer->GW1,pointer->mess_info);
    }
  else
    {
      btpos(btree_soundex,1);//đưa con trỏ về đầu current root
      int rsize;
      while(btseln(btree_soundex,word,mean,0,&rsize)==0)  add_to_list(pointer->store_suggest,word);
    }
  btcls(btree_soundex);
  return;
}
