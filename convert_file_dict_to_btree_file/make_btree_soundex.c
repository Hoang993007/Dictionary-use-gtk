//mỗi khi thêm từ lại complie hàm này lại một lần
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include"btree.h"
#include "dictionary.h"

void make_btree_soundex(BTA*btree)
{//phân phối lại các từ vào các nhóm-các gốc theo soundex_code
  btpos(btree,1);//đưa con trỏ btree về vị trí before the first key in the index of file btree input
  BTA*btree_soundex=btopn("./media/hoang/TONG HOP/C-Advance/PROJECT/Team/Main file_not_use_GTK/Dict_soundex.dat",0,0);
  if(btree_soundex==NULL) btree_soundex=btcrt("./media/hoang/TONG HOP/C-Advance/PROJECT/Team/Main file_not_use_GTK/Dict_soundex.dat",0,0);

  char word[WORD_LENGTH];
  char mean[1];//ko cần dùng đến mean nên khai báo nó nên cắt bớt để tiết kiệm bộ nhớ

  int rsize;
  //int num=0;
  while(btseln(btree, word,mean, 0, &rsize)==0)
    {
      //num=num+1;
      char*soundex_code=(char*)soundex(word);

      //printf("%s\t%s\n",soundex_code,word);
      //chỗ này chỉ đưa từ vào thôi, đưa nghĩa vào load rất nặng nên thôi
      if(btchgr(btree_soundex,soundex_code)==0)
        btins(btree_soundex,word,"",0);
      else
        {
          btcrtr(btree_soundex,soundex_code);
          btins(btree_soundex,word,"",0);
        }
    }

  //printf("had add %d word to btree soundex\n",num);
  btcls(btree_soundex);
  return;
}
