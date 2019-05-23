#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "btree.h"
#include "dictionary.h"
#include<gtk/gtk.h>

int delete_word(data_give*pointer)
{return btdel(pointer->Btree,pointer->wordNow);}
