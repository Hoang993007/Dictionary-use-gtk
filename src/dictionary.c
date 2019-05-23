//DÙNG BOX CHỈNH VỊ TRÍ DỄ DÀNG HƠN NHỀU SO VỚI GRID 
//GRID CHO ĐỂ 2 CÁI LÊN NHAU, TIỆN LÀM NỀN, ĐỂ CHỒNG NHAU,...

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "dictionary.h"
#include "btree.h"
#include<gtk/gtk.h>

void chuanHoaXau(char*tmp)
{
  int tmp_len=strlen(tmp);
  while((tmp[tmp_len-1]==' ')||(tmp[tmp_len-1]=='\n')
        ||(tmp[tmp_len-1]=='\t'))
    {
      tmp[tmp_len-1]='\0';
      tmp_len=tmp_len-1;
    }
  return;
}

//CREATE A PIXBUF BEGIN *************************************
GdkPixbuf*create_pixbuf(const gchar*filename)
{
  GdkPixbuf*pixbuf;
  GError*error=NULL;

  //shows an application icon
  pixbuf=gdk_pixbuf_new_from_file(filename,&error);

  if(!pixbuf)
    {
      fprintf(stderr,"%s\n",error->message);
      g_error_free(error);
    }
  return pixbuf;
}
//CREATE A PIXBUF END **************************************

//CREATE BUTTON INTERFACE BEGIN **************************************
GtkWidget*button_interface(gchar*image_name,const gchar*label_name)//label có thể rỗng, tùy trường hợp 
{
  GtkWidget*tmp=gtk_box_new(GTK_ORIENTATION_VERTICAL,0);
  //GtkWidget*label_tmp;
  //if(label_name[0]!='\0')
  //{
  //label_tmp=gtk_label_new(label_name);
  //gtk_grid_attach(tmp,label_tmp,1,0,1,1);
  //}
  GtkWidget*image_tmp;
  if(image_name[0]!='\0')
    {
      image_tmp=gtk_image_new_from_file(image_name);
      //pmg hay jpg hay bất kì dạng ảnh nào khác đều được
      gtk_box_pack_start(tmp,image_tmp,TRUE,TRUE,0);
    }
  return tmp;
}
//CREATE BUTTON INTERFACE END **************************************

//SHOW MESSAGE BEGIN ***************************************
void show_mess(GtkWidget*widget,gpointer message )
{
  MESSAGE*pointer=(MESSAGE*)message;
  GtkWidget*dialog=gtk_message_dialog_new(GTK_WINDOW(pointer->GW1),
                                          GTK_DIALOG_DESTROY_WITH_PARENT,
                                          pointer->MesType,
                                          pointer->BtnsType,
                                          pointer->text1);
  gtk_message_dialog_format_secondary_text(GTK_MESSAGE_DIALOG(dialog),pointer->text2);
  gtk_window_set_title(GTK_WINDOW(dialog),pointer->title);

  pointer->answer=gtk_dialog_run(GTK_DIALOG(dialog));
  gtk_widget_destroy(dialog);
}
//SHOW MESSAGE END **************************************************|

//SHOW ABOUT BEGIN *********************************************
void show_about(GtkWidget*widget,gpointer data)
{
  GdkPixbuf*pixbuf=gdk_pixbuf_new_from_file("show_about.jpg",NULL);
  GtkWidget*dialog=gtk_about_dialog_new();
  gtk_about_dialog_set_program_name(GTK_ABOUT_DIALOG(dialog),"About us");
  gtk_about_dialog_set_version(GTK_ABOUT_DIALOG(dialog),"Version: 1.0");
  gtk_about_dialog_set_copyright(GTK_ABOUT_DIALOG(dialog),"(c) Hoang - Hieu - Long");
  gtk_about_dialog_set_comments(GTK_ABOUT_DIALOG(dialog),"Hope you enjoy it!");
  gtk_about_dialog_set_logo(GTK_ABOUT_DIALOG(dialog),pixbuf);
  g_object_unref(pixbuf),pixbuf=NULL;
  gtk_dialog_run(GTK_DIALOG(dialog));
  gtk_widget_destroy(dialog);
  return;
}
//SHOW ABOUT END ********************************************

//BEGIN WINDOW BEGIN ***********************************************
int begin_window(GtkWidget*window)
{
  GtkWidget*begin_window;
  GdkPixbuf*window_icon;
  GtkWidget*box;

  GtkWidget*dictionary;
  GtkWidget*hot_picture;

  begin_window=gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(begin_window),"Begin");
  gtk_window_set_default_size(GTK_WINDOW(begin_window),230,250);
  gtk_container_set_border_width(GTK_CONTAINER(begin_window), 15);

  //set button
  box=gtk_box_new(GTK_ORIENTATION_VERTICAL,2);
  gtk_container_add(GTK_CONTAINER(begin_window),box);

  dictionary=gtk_button_new_with_label("Dictionary");
  hot_picture=gtk_button_new_with_label("Hot picture");

  gtk_box_pack_start(GTK_BOX(box),dictionary,TRUE,TRUE,0);
  gtk_box_pack_start(GTK_BOX(box),hot_picture,TRUE,TRUE,0);
  gtk_window_set_position(GTK_WINDOW(begin_window),GTK_WIN_POS_CENTER);

  window_icon=create_pixbuf("web.png");
  gtk_window_set_icon(GTK_WINDOW(begin_window), window_icon);

  gtk_widget_show_all(begin_window);

  //close begin_window
  //g_signal_connect (dictionary, "clicked", G_CALLBACK (gtk_widget_hide), begin_window);

  //open window
  g_signal_connect_swapped(dictionary,"clicked",G_CALLBACK(gtk_widget_show_all),window);

  //destroy windget
  g_signal_connect(begin_window, "destroy",G_CALLBACK(gtk_main_quit), NULL);

  g_object_unref(window_icon);

  return 0;
}
//BEGIN WINDOW END **********************************************

void find_function(GtkWidget*widget, gpointer cb_data);
void delete_function(GtkWidget * widget, gpointer cb_data);
void add_function(GtkWidget * widget, gpointer cb_data);
void edit_function(GtkWidget * widget, gpointer cb_data);


//MAKE TREE VIEW BEGIN ************************************************
enum {LIST_ITEM = 0,N_COLUMNS};
//gán cho LIST_ITEM=0 và N_COLUMNS=1

//thiết lập btree thành kiểu danh sách 1 cột 1 ô
void init_list(GtkWidget *tree_view) 
{
  GtkCellRenderer *renderer = gtk_cell_renderer_text_new ();
  //An object for rendering a single cell - hiển thị một ô duy nhất 

  GtkTreeViewColumn *column = gtk_tree_view_column_new_with_attributes("List Items", renderer, "text", LIST_ITEM, NULL);
  //A visible column in a GtkTreeView widget

  gtk_tree_view_append_column(GTK_TREE_VIEW(tree_view), column);
  //nối cột vào trong danh sách các cột
}

//show word in tree_view
void add_to_list(GtkListStore *store, const gchar *str) 
{
  //GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(tree_view)));
  //câu lệnh trên gây ra lỗi không hiểu được, không dùng

  GtkTreeIter iter;
  //con trỏ vị trí trong tree_view 

  gtk_list_store_append(store, &iter);
  //Prepends a new row to list_store . iter will be changed to point to this new row. The row will be empty after this function is called.
  
  gtk_list_store_set(store, &iter, LIST_ITEM, str, -1);
  //đưa từ str vào trong column 0 tại vị trí con trỏ iter với kiểu G_TYPE_STRING
  //Sets the value of one or more cells in the row referenced by iter . The variable argument list should contain integer column numbers, each column number followed by the value to be set. The list is terminated by a -1. For example, to set column 0 with type G_TYPE_STRING to “Foo”, you would write gtk_list_store_set (store, iter, 0, "Foo", -1).
}

void add_to_list_his(GtkListStore *store, const gchar *str) 
{
  GtkTreeIter iter;
  gtk_list_store_prepend(store, &iter);//lệnh nay cũng đưa iter vào trong list store
  gtk_list_store_set(store, &iter, LIST_ITEM, str, -1);
}

void add_to_list_autocomplete(GtkListStore *store, const gchar *str) 
{
  GtkTreeIter iter;
  gtk_list_store_append(store, &iter);
  gtk_list_store_set(store, &iter, LIST_ITEM, str, -1);
}

//Thiết lập việc 1 từ được chọn nếu tín hiệu trong tree_view có thay đổi (một cell trong tree_view được chọn)
void on_changed(GtkTreeSelection *widget, gpointer cb_data) 
{
  GtkTreeIter iter;
  GtkTreeModel *model;
  gchar *value;

  if (gtk_tree_selection_get_selected(GTK_TREE_SELECTION(widget), &model, &iter)) 
    {
      gtk_tree_model_get(model, &iter, LIST_ITEM, &value,  -1);
      data_give*pointer=(data_give*)cb_data;

      gtk_entry_set_text(GTK_ENTRY(pointer->GW3),value);
      find_function(pointer->GW3,cb_data);

      g_free(value);
    }
}

void on_changed_autocomplete(GtkTreeSelection *widget, gpointer cb_data) 
{
  GtkTreeIter iter;
  GtkTreeModel *model;
  gchar *value;

  if (gtk_tree_selection_get_selected(GTK_TREE_SELECTION(widget), &model, &iter)) 
    {
      gtk_tree_model_get(model, &iter, LIST_ITEM, &value,  -1);
      data_give*pointer=(data_give*)cb_data;

      char mean[MEAN_LENGTH];
      int rsize;//bắt buộc
      int check=btsel(pointer->Btree,value,mean,MEAN_LENGTH,&rsize);
      strcpy(pointer->wordNow, value);
      add_to_list_his(pointer->store_history,value);
      set_textView_text(pointer->GW2,mean);

      g_free(value);
    }
}

//GTK_ENTRY là cách viết khác của GtkEntry*
/*error xuất hiện:
src/dictionary.c:214:6: warning: conflicting types for ‘find_function’
 void find_function(gpointer cb_data);
      ^~~~~~~~~~~~~
src/dictionary.c:159:5: note: previous implicit declaration of ‘find_function’ was here
     find_function(cb_data);
*///lỗi này là do hàm find_function được viết sau hàm on_change trong file code c, nên khi biên dịch biên dịch đến on_change không biết find_function ở chỗ nào

//MAKE TREE VIEW END *******************************************

//SET TEXT TEXT VIEW BEGIN ***********************************
void set_textView_text(GtkWidget*text_view, char * text) 
{
  GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
  //buffer là bộ nhớ đệm, lưu trữ tạm thời những gì hiện tại ở trên text view
  if (buffer == NULL)
    {
      printf("Get buffer fail!");
      buffer = gtk_text_buffer_new(NULL);
    }
  gtk_text_buffer_set_text(buffer, text, -1);
  gtk_text_view_set_buffer(GTK_TEXT_VIEW(text_view), buffer);
}
//SET TEXT TEXT VIEW END ***********************************

//PRESS TAB SIGNAL BEGIN ****************************************
gboolean on_key_press(GtkWidget *entry, GdkEvent *event, gpointer cb_data)
{
  //để là event thì sau khi kết thúc hàm này, event mới được xác nhận
  //ví dụ: khi đánh chữ b, đó là 1 event, khi gọi hàm này, vì việc viết b vào chưa được xác nhận, nghĩa là ô giờ vẫn trống,kết thúc hàm này, b mới được viết vào
  data_give*pointer=(data_give*)cb_data;
  GdkEventKey *keyEvent = (GdkEventKey *)event;
  if (keyEvent->keyval == GDK_KEY_Tab)
    {
      g_signal_handler_block(pointer->selection, pointer->id);
      gtk_list_store_clear(pointer->store_suggest);
      g_signal_handler_unblock(pointer->selection, pointer->id);

      suggest_use_soundex(cb_data);
    }
  return FALSE;
}

void autocomplete_function(GtkWidget *entry, gpointer cb_data)
{autocomplete(cb_data);}
//PRESS TAB SIGNAL END ****************************************

int main(int argc, char *argv[])
{
  BTA*btree;
  btree=btopn("DictData.dat",0,0);
  if(btree==NULL) printf("Main_Error: can't open data file\n");
  init();
  char wordNow[WORD_LENGTH]="empty#";//Từ hiện tại đang được hiển thị bởi textview 

  init();

  //initializes GTK+ and parses some standard command line options
  gtk_init(&argc,&argv);//khởi động gtk, chỉ có thể thực hiện những lệnh của gtk mà thôi 


  //SET WINDOW DEFAULT ********************************************************
  GtkWidget*window=gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window),"Dictionary");
  gtk_window_set_default_size(GTK_WINDOW(window),500,450);
  gtk_container_set_border_width(GTK_CONTAINER(window), 15);//sets some border space around the edges of the window
  gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);

  //set window icon
  GdkPixbuf*window_icon=create_pixbuf("web.png");
  gtk_window_set_icon(GTK_WINDOW(window), window_icon);
  //gtk_widget_show_all(window);//show all feature on the window//put it in line after complete all feature want to show

  //A GTK+ user interface is constructed by nesting widgets inside widgets. Container widgets are the inner nodes in the resulting tree of widgets: they contain other widgets. So, for example, you might have a GtkWindow containing a GtkFrame containing a GtkLabel. If you wanted an image instead of a textual label inside the frame, you might replace the GtkLabel widget with a GtkImage widget.

  //CREATE GRID WIDGET *********************************************************************
  GtkWidget*grid=gtk_grid_new();
  //gtk_grid_set_row_spacing(GTK_grid(grid),0,10);
  gtk_grid_set_row_spacing(GTK_GRID(grid),10);
  gtk_grid_set_column_spacing(GTK_GRID(grid),10);
  gtk_container_add(GTK_CONTAINER(window),grid);

  //TEXT VIEW CREATE BEGIN ******************************************************
  GtkWidget*view=gtk_text_view_new();
  gtk_container_set_border_width(GTK_CONTAINER(view), 8); 
  gtk_text_view_set_editable(GTK_TEXT_VIEW(view),FALSE);//non-editable
  gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(view),FALSE);//giấu con chạy đi
  gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(view), GTK_WRAP_WORD_CHAR);//chống tràn bề ngang

  //scrolled for text view
  GtkWidget*view_scrolled=gtk_scrolled_window_new(NULL,NULL);
  gtk_container_add(GTK_CONTAINER(view_scrolled),view);
  gtk_widget_set_size_request(view_scrolled,300,350);

  //set  frame
  GtkWidget* frame_view = gtk_frame_new(NULL);
  gtk_frame_set_shadow_type(GTK_FRAME(frame_view), GTK_SHADOW_OUT);
  gtk_container_add(GTK_CONTAINER(frame_view),view_scrolled);
  gtk_grid_attach(GTK_GRID(grid),frame_view,1,1,2,5);
  GtkTextIter start, end;

  GtkTextBuffer*buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(view));
  //truy cập đến bộ nhớ đệm lưu trữ tạm thời những gì hiển thị trên text view qua biến được khai báo mới là buffer 

  //tạo phông cho text view 
  /*
    gtk_text_buffer_create_tag(buffer, "gap","pixels_above_lines", 30, NULL);
    gtk_text_buffer_create_tag(buffer, "lmarg","left_margin", 5, NULL);
    gtk_text_buffer_create_tag(buffer, "blue_fg","foreground", "blue", NULL);
    gtk_text_buffer_create_tag(buffer, "red_fg","foreground", "red", NULL);
    gtk_text_buffer_create_tag(buffer, "gray_bg","background", "gray", NULL);
    gtk_text_buffer_create_tag(buffer, "italic","style", PANGO_STYLE_ITALIC, NULL);
    gtk_text_buffer_create_tag(buffer, "bold","weight", PANGO_WEIGHT_BOLD, NULL);

    GtkTextIter iter;
    gtk_text_buffer_get_iter_at_offset(buffer, &iter, 0);
    gtk_text_buffer_insert(buffer, &iter, "Plain text\n", -1);
    gtk_text_buffer_insert_with_tags_by_name(buffer, &iter,"Colored Text\n", -1, "blue_fg", "lmarg",  NULL);
    gtk_text_buffer_insert_with_tags_by_name (buffer, &iter,"Text with colored background\n", -1, "lmarg", "gray_bg", NULL);
    gtk_text_buffer_insert_with_tags_by_name (buffer, &iter,"Text in italics\n", -1, "italic", "lmarg",  NULL);
    gtk_text_buffer_insert_with_tags_by_name (buffer, &iter,"Bold text\n", -1, "bold", "lmarg",  NULL);
  */

  //TEXT VIEW CREATE END ******************************************************

  //CREATE SUGGEST TREE BEGIN ****************************************
  GtkWidget*tree_view=gtk_tree_view_new();
  gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(tree_view), FALSE);//che giấu header
  gtk_widget_set_size_request(tree_view,120,350);
  gtk_container_set_border_width(GTK_CONTAINER(tree_view), 3);

  init_list(tree_view);//thiết lập mode các kiểu của tree_view
  GtkTreeSelection *selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(tree_view));
  //thiết lập khả năng lựa chọn cho tree_view 

  //scrolled for tree view
  GtkWidget*tree_view_scrolled=gtk_scrolled_window_new(NULL,NULL);
  gtk_container_add(GTK_CONTAINER(tree_view_scrolled),tree_view);
  gtk_widget_set_size_request(tree_view_scrolled,120,350);

  //set  frame
  GtkWidget* frame_tree_view = gtk_frame_new(NULL);
  gtk_frame_set_shadow_type(GTK_FRAME(frame_tree_view), GTK_SHADOW_OUT);
  gtk_container_add(GTK_CONTAINER(frame_tree_view),tree_view_scrolled);
  GtkWidget*suggest_expander=gtk_expander_new ("Suggest");
  gtk_container_add(GTK_CONTAINER(suggest_expander),frame_tree_view);
  gtk_grid_attach(GTK_GRID(grid),suggest_expander,0,1,1,5);
  gtk_expander_set_expanded (GTK_EXPANDER(suggest_expander),TRUE);
  GtkListStore *store_suggest= gtk_list_store_new(N_COLUMNS, G_TYPE_STRING);
  //A list-like data structure that can be used with the GtkTreeView

  gtk_tree_view_set_model(GTK_TREE_VIEW(tree_view), GTK_TREE_MODEL(store_suggest));
  //GtkTreeModel is required by GtkTreeSortable.
  //set model "store" for tree_view
  //CREATE SUGGEST TREE END ****************************************

  //CREATE HISTORY BEGIN****************************************
  GtkWidget*history=gtk_tree_view_new();
  gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(history), FALSE);
  gtk_widget_set_size_request(history,120,350);
  gtk_container_set_border_width(GTK_CONTAINER(history), 3);

  init_list(history);
  GtkTreeSelection *selection_his = gtk_tree_view_get_selection(GTK_TREE_VIEW(history));

  //scrolled for tree view
  GtkWidget*history_scrolled=gtk_scrolled_window_new(NULL,NULL);
  gtk_container_add(GTK_CONTAINER(history_scrolled),history);
  gtk_widget_set_size_request(history_scrolled,120,350);

  //set  frame
  GtkWidget* frame_history = gtk_frame_new(NULL);
  gtk_frame_set_shadow_type(GTK_FRAME(frame_history), GTK_SHADOW_OUT);
  gtk_container_add(GTK_CONTAINER(frame_history),history_scrolled);
  GtkWidget*history_expander=gtk_expander_new ("History");
  gtk_container_add(GTK_CONTAINER(history_expander),frame_history);
  gtk_grid_attach(GTK_GRID(grid),history_expander,5,1,1,5);
  gtk_expander_set_expanded (GTK_EXPANDER(history_expander),TRUE);
  GtkListStore *store_history= gtk_list_store_new(N_COLUMNS, G_TYPE_STRING);

  gtk_tree_view_set_model(GTK_TREE_VIEW(history), GTK_TREE_MODEL(store_history));
  //CREATE HISTORY END ****************************************

  //CREATE SEARCH ENTRY BEGIN **************************************************
  GtkWidget*search_entry=gtk_search_entry_new();
  gtk_widget_set_size_request(search_entry,100,50);
  gtk_entry_set_placeholder_text ((GtkEntry*)search_entry,"Let's find something!");
  gtk_grid_attach(GTK_GRID(grid),search_entry,1,0,3,1);
  //CREATE SEARCH ENTRY END **************************************************

  //CREATE TREE AUTOCOMPLETE BEGIN ********************************************

  GtkWidget*autocomplete_tree=gtk_tree_view_new();
  gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(autocomplete_tree), FALSE);
  gtk_widget_set_size_request(autocomplete_tree,120,350);
  gtk_container_set_border_width(GTK_CONTAINER(autocomplete_tree), 3);

  init_list(autocomplete_tree);
  GtkTreeSelection *selection_auto = gtk_tree_view_get_selection(GTK_TREE_VIEW(autocomplete_tree));

  //scrolled for tree view
  GtkWidget*autocomplete_tree_scrolled=gtk_scrolled_window_new(NULL,NULL);
  gtk_container_add(GTK_CONTAINER(autocomplete_tree_scrolled),autocomplete_tree);
  gtk_widget_set_size_request(autocomplete_tree_scrolled,120,350);

  //set  frame
  GtkWidget* frame_autocomplete = gtk_frame_new(NULL);
  gtk_frame_set_shadow_type(GTK_FRAME(frame_autocomplete), GTK_SHADOW_OUT);
  gtk_container_add(GTK_CONTAINER(frame_autocomplete),autocomplete_tree_scrolled);
  GtkWidget*autocomplete_tree_expander=gtk_expander_new ("Autocomplete");
  gtk_container_add(GTK_CONTAINER(autocomplete_tree_expander),frame_autocomplete);
  gtk_grid_attach(GTK_GRID(grid),autocomplete_tree_expander,4,1,1,5);
  gtk_expander_set_expanded (autocomplete_tree_expander,TRUE);
  
  GtkListStore *autocomplete_list= gtk_list_store_new(N_COLUMNS, G_TYPE_STRING);
  gtk_tree_view_set_model(GTK_TREE_VIEW(autocomplete_tree), GTK_TREE_MODEL(autocomplete_list));  
  /*GtkEntryCompletion *comple = gtk_entry_completion_new();
    gtk_entry_completion_set_text_column(comple, 0);
    gtk_entry_completion_set_model(comple, GTK_TREE_MODEL(autocomplete_list));
    gtk_entry_set_completion(GTK_ENTRY(search_entry), comple);*/
  //CREATE TREE AUTOCOMPLETE END ********************************************

  //MAKE WINDOW BUTTON BEGIN ***************************************************
  char none[1];none[0]='\0';

  //make add button
  GtkWidget*add_button=gtk_button_new();
  gtk_widget_set_size_request(add_button, 30, 30);
  gtk_container_add(GTK_CONTAINER(add_button),button_interface("add_image.png",(const gchar*)none));
  gtk_widget_set_tooltip_text(add_button,"Add new word");


  //make fix button
  GtkWidget*fix_button=gtk_button_new();
  gtk_container_add(GTK_CONTAINER(fix_button),button_interface("edit_image.png",(const gchar*)none));
  gtk_widget_set_tooltip_text(fix_button,"Edit word");

  //make delete button
  GtkWidget*del_button=gtk_button_new();
  gtk_container_add(GTK_CONTAINER(del_button), button_interface("delete_image.png",(const gchar*)none));
  gtk_widget_set_tooltip_text(del_button,"Delete word");
  
  GtkWidget*box=gtk_box_new(GTK_ORIENTATION_VERTICAL,5);
  gtk_grid_attach(GTK_GRID(grid),box,3,1,1,1);

  gtk_container_add(GTK_CONTAINER(box),add_button);
  gtk_container_add(GTK_CONTAINER(box),fix_button);
  gtk_container_add(GTK_CONTAINER(box),del_button);

  //make info button
  GtkWidget*info_button=gtk_button_new();
  gtk_widget_set_tooltip_text(info_button,"About us");
  gtk_widget_set_size_request(info_button,30,30);
  gtk_grid_attach(GTK_GRID(grid),info_button,3,2,1,1);
  gtk_container_add(GTK_CONTAINER(info_button), button_interface("info_image.png",(const gchar*)none));

  //make search button
  GtkWidget*search_button=gtk_button_new();
  gtk_widget_set_size_request(search_button,70,50);
  gtk_grid_attach(GTK_GRID(grid),search_button,0,0,1,1);
  gtk_container_add(GTK_CONTAINER(search_button), button_interface("search_image.png",(const gchar*)none));


  //GtkWidget*back_ground=gtk_image_new_from_file("star.jpg");
  //gtk_grid_attach(GTK_GRID(grid),back_ground,0,0,4,4);
  //hay đất nhưng vẫn chưa biết cách vận dụng

  //MAKE WINDOW BUTTON END ***************************************************
  

  MESSAGE *show_warning= g_new( MESSAGE, sizeof( MESSAGE*));
  show_warning-> MesType=GTK_MESSAGE_WARNING;
  show_warning-> BtnsType= GTK_BUTTONS_YES_NO;

  MESSAGE *show_info= g_new( MESSAGE, sizeof( MESSAGE*));
  show_info-> MesType=GTK_MESSAGE_INFO;
  show_info-> BtnsType=GTK_BUTTONS_OK;

  MESSAGE*show_error= g_new( MESSAGE, sizeof( MESSAGE*));
  show_error-> MesType=GTK_MESSAGE_ERROR;
  show_error-> BtnsType= GTK_BUTTONS_OK;

  MESSAGE *show_question= g_new( MESSAGE, sizeof( MESSAGE*));
  show_question-> MesType=GTK_MESSAGE_QUESTION;
  show_question-> BtnsType= GTK_BUTTONS_YES_NO;

  //CREATE DIFFERENT DATA BEGIN ************************************************************
  //PASS MUTIL PARAMETER TO A FUNCTION BY USE STRUCT AND G_NEW FUNCTION

  data_give *data_find_function= g_new(data_give, sizeof(data_give*));
  //gần giống hàm malloc, chưc năng tương tự
  data_find_function->Btree=btree;
  data_find_function->GW1=window;
  data_find_function->GW2=view;
  data_find_function->GW3=search_entry;
  data_find_function->wordNow=wordNow;
  data_find_function->store_history=store_history;

  data_give *data_search_entry= g_new(data_give, sizeof(data_give*));
  data_search_entry->Btree=btree;
  data_search_entry->GW1=window;
  data_search_entry->GW2=view;
  data_search_entry->GW3=search_entry;
  data_search_entry->selection=selection_auto;
  data_search_entry->wordNow=wordNow;
  data_search_entry->tree_view1=tree_view;
  data_search_entry->store_suggest=store_suggest;
  data_search_entry->store_autocomplete=autocomplete_list;
  data_search_entry->mess_info=show_info;
  data_search_entry->store_history=store_history;

  data_give *data_search_entry_tab= g_new(data_give, sizeof(data_give*));
  data_search_entry_tab->Btree=btree;
  data_search_entry_tab->GW1=window;
  data_search_entry_tab->GW2=view;
  data_search_entry_tab->GW3=search_entry;
  data_search_entry_tab->selection=selection;
  data_search_entry_tab->wordNow=wordNow;
  data_search_entry_tab->tree_view1=tree_view;
  data_search_entry_tab->store_suggest=store_suggest;
  data_search_entry_tab->store_autocomplete=autocomplete_list;
  data_search_entry_tab->mess_info=show_info;
  data_search_entry_tab->store_history=store_history;

  data_give *tab_suggest= g_new(data_give, sizeof(data_give*));
  tab_suggest->Btree=btree;
  tab_suggest->GW1=window;
  tab_suggest->GW2=view;
  tab_suggest->GW3=search_entry;
  tab_suggest->tree_view1=tree_view;
  tab_suggest->store_suggest=store_suggest;
  tab_suggest->wordNow=wordNow;
  tab_suggest->mess_info=show_info;
  tab_suggest->store_history=store_history;

  data_give *data_history= g_new(data_give, sizeof(data_give*));
  data_history->Btree=btree;
  data_history->GW1=window;
  data_history->GW2=view;
  data_history->GW3=search_entry;
  data_history->history=history;
  data_history->store_history=store_history;
  data_history->wordNow=wordNow;
  data_history->mess_info=show_info;

  data_give *data_auto= g_new(data_give, sizeof(data_give*));
  data_auto->GW1=window;
  data_auto->Btree=btree;
  data_auto->GW2=view;
  data_auto->store_history=store_history;
  data_auto->wordNow=wordNow;

  data_give *data_delete_function= g_new(data_give, sizeof(data_give*));
  data_delete_function->Btree=btree;
  data_delete_function->GW1=window;
  data_delete_function->GW2=view;
  data_delete_function->wordNow=wordNow;
  data_delete_function->mess_info=show_info;
  data_delete_function->mess_warning=show_warning;
  data_delete_function->mess_error=show_error;

  data_give *data_add_function= g_new(data_give, sizeof(data_give*));
  data_add_function->Btree=btree;
  data_add_function->GW3=search_entry;
  data_add_function->mess_info=show_info;
  data_add_function->mess_warning=show_warning;
  data_add_function->mess_error=show_error;

  data_give *data_edit_function= g_new(data_give, sizeof(data_give*));
  data_edit_function->Btree=btree;
  data_edit_function->GW1=window;
  data_edit_function->GW2=view;
  data_edit_function->GW3=search_entry;
  data_edit_function->wordNow=wordNow;
  data_edit_function->mess_info=show_info;
  data_edit_function->mess_warning=show_warning;
  data_edit_function->mess_error=show_error;
  data_edit_function->store_history=store_history;
  //CREATE DIFFERENT DATA END************************************************************


 //CONNECT SIGNAL BEGIN ******************************************************************
  //show about
  gtk_widget_add_events(window,GDK_BUTTON_PRESS_MASK); 
  g_signal_connect(info_button,"button-press-event",G_CALLBACK(show_about),(gpointer)window);

  g_signal_connect(search_entry,"activate",G_CALLBACK(find_function),data_find_function);
  g_signal_connect(search_button,"clicked",G_CALLBACK(find_function),data_find_function);

  data_search_entry_tab->id=g_signal_connect(selection, "changed", G_CALLBACK(on_changed),tab_suggest);

  g_signal_connect(selection_his, "changed", G_CALLBACK(on_changed), data_history);

  data_search_entry->id=g_signal_connect(selection_auto, "changed", G_CALLBACK(on_changed_autocomplete),data_auto);

  g_signal_connect(search_entry,"search-changed", G_CALLBACK(autocomplete_function),data_search_entry);

  g_signal_connect(search_entry, "key-press-event", G_CALLBACK(on_key_press),data_search_entry_tab);

  //những trường hợp tín hiệu bị trục trặc tắt từng cái đi rồi khởi động lại từng cái một
  g_signal_connect(del_button,"clicked",G_CALLBACK(delete_function),data_delete_function);

  g_signal_connect(add_button,"clicked",G_CALLBACK(add_function),data_add_function);
  
  g_signal_connect(fix_button,"clicked",G_CALLBACK(edit_function),data_edit_function);

  g_signal_connect(G_OBJECT(window), "destroy",G_CALLBACK(gtk_main_quit), NULL);
  //CONNECT SIGNAL END ******************************************************************

  begin_window(window);

  //The g_object_unref() decreases the reference count of the pixbuf object.

  //enters the GTK+ main loop. From this point, the application sits and waits for events to happen
  gtk_main();

  g_object_unref(window_icon);
  g_object_unref(store_suggest);
  g_object_unref(autocomplete_list);
  g_free (data_auto);
  g_free (data_find_function);
  g_free (data_search_entry);
  g_free (tab_suggest);
  g_free ( data_delete_function);
  g_free (data_add_function);
  g_free (data_edit_function);
//ko dc free truoc gtk_main

  return 0;
}


void find_function(GtkWidget*widget, gpointer cb_data) 
{
  data_give*pointer=(data_give*)cb_data;

  char mean[MEAN_LENGTH];
  int check=search_word(mean,pointer);
  
  if ((check!=0)||(check==100))  set_textView_text(pointer->GW2,"Rất tiếc, từ này hiện không có trong từ điển.\n\nGợi ý:\n\t\t- Nhấn nút \"ADD\", để bổ sung vào từ điển.");
  else if(check==0)	
    {
      set_textView_text(pointer->GW2,mean);
      //add_to_list(pointer->tree_view1,pointer->wordNow);
    }
  else if(check==100) set_textView_text(pointer->GW2,"");
  return;
}

void delete_function(GtkWidget * widget, gpointer cb_data)
{
  data_give*pointer=(data_give*)cb_data;

  if(strcmp(pointer->wordNow,"empty#")==0)
    {
      pointer->mess_info->title=NULL;
      char c[]="Delete";
      pointer->mess_info->GW1=pointer->GW1;
      pointer->mess_info->text1=c;
      char d[]="There are nothing to delete!";
      pointer->mess_info->text2=d;
      show_mess(widget,pointer->mess_info);
      return;
    }

  pointer->mess_warning->title=NULL;
  char a[]="Delete";
  pointer->mess_warning->GW1=pointer->GW1;
  pointer->mess_warning->text1=a;
  char b[]="Are you sure to delete this word ?";
  pointer->mess_warning->text2=b;

  show_mess(widget,pointer->mess_warning);//phải có widget ở đây
  if(pointer->mess_warning->answer==GTK_RESPONSE_YES)
    {
      int check=delete_word(pointer);
      if(check==0)
        {
          BTA*btree_soundex=btopn("Dict_soundex.dat",0,0);
          char*soundex_code=(char*)soundex(pointer->wordNow);
          btchgr(btree_soundex,soundex_code);
          btdel(btree_soundex,pointer->wordNow);
          btcls(btree_soundex);

          set_textView_text(pointer->GW2,"successfully deleted\n");
          pointer->mess_info->title=NULL;
          char c[]="Delete";
          pointer->mess_info->GW1=pointer->GW1;
          pointer->mess_info->text1=c;
          char d[]="Successfully deleted!";
          pointer->mess_info->text2=d;
          show_mess(widget,pointer->mess_info);
          
          strcpy(pointer->wordNow,"empty#");
        }
      else
        {
          set_textView_text(pointer->GW2,"faild to deleted\n");
          pointer->mess_error->title=NULL;
          char c[]="Delete";
          pointer->mess_error->GW1=pointer->GW1;
          pointer->mess_error->text1=c;
          char d[]="Faild to deleted!";
          pointer->mess_error->text2=d;
          show_mess(widget,pointer->mess_error);
        }
    }
  return;
}

void edit_function(GtkWidget * widget, gpointer cb_data)
{
  data_give*pointer=(data_give*)cb_data;

  if(strcmp(pointer->wordNow,"empty#")==0)
    {
      pointer->mess_info->title=NULL;
      char c[]="Edit";
      pointer->mess_info->GW1=pointer->GW1;
      pointer->mess_info->text1=c;
      char d[]="There are nothing to edit!";
      pointer->mess_info->text2=d;
      show_mess(widget,pointer->mess_info);
      return;
    }

  GtkWidget*edit_window;
  GdkPixbuf*window_icon;

  edit_window=gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(edit_window),"Edit");
  gtk_window_set_default_size(GTK_WINDOW(edit_window),300,300);
  gtk_container_set_border_width(GTK_CONTAINER(edit_window), 15);
  gtk_window_set_position(GTK_WINDOW(edit_window),GTK_WIN_POS_CENTER);

  window_icon=create_pixbuf("web.png");
  gtk_window_set_icon(GTK_WINDOW(edit_window), window_icon);

  GtkWidget*grid = gtk_grid_new();
  gtk_container_add(GTK_CONTAINER(edit_window), grid);
  gtk_grid_set_column_spacing(GTK_GRID(grid),10);

//text view edit
  GtkWidget*edit_view=gtk_text_view_new();
  gtk_container_set_border_width(GTK_CONTAINER(edit_view), 8); 
  gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(edit_view), GTK_WRAP_WORD_CHAR);

  GtkWidget*edit_view_scrolled=gtk_scrolled_window_new(NULL,NULL);
  gtk_container_add(GTK_CONTAINER(edit_view_scrolled),edit_view);

  gtk_widget_set_size_request(edit_view_scrolled,300,350);

  GtkWidget* frame_edit_view = gtk_frame_new("Edit");
  gtk_frame_set_shadow_type(GTK_FRAME(frame_edit_view), GTK_SHADOW_OUT);
  gtk_container_add(GTK_CONTAINER(frame_edit_view),edit_view_scrolled);
  gtk_grid_attach(GTK_GRID(grid),frame_edit_view,0,0,1,1);

  //set mean
  char mean_set[MEAN_LENGTH];
  int rsize;
  btsel(pointer->Btree,pointer->wordNow,mean_set,MEAN_LENGTH,&rsize);
  set_textView_text(edit_view,mean_set);

  //make OK button
  GtkWidget*ok_button=gtk_button_new();
  gtk_widget_set_size_request(ok_button,60,70);
  gtk_grid_attach(GTK_GRID(grid),ok_button,1,0,1,1);
  
  char none[1];none[0]='\0';
  gtk_container_add(GTK_CONTAINER(ok_button),button_interface("OK.png",(const gchar*)none));

  pointer->GW5=edit_window;
  pointer->GW6=edit_view;

  g_signal_connect(ok_button,"clicked",G_CALLBACK(fix_word), cb_data);

  //if(pointer->flag==0) 

  gtk_widget_show_all(edit_window);

  g_object_unref(window_icon);          
  return;
}

void add_function(GtkWidget * widget, gpointer cb_data)
{
  data_give*pointer=(data_give*)cb_data;

  GtkWidget*add_window;
  GdkPixbuf*window_icon;

  add_window=gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(add_window),"Add");
  gtk_window_set_default_size(GTK_WINDOW(add_window),300,300);
  gtk_container_set_border_width(GTK_CONTAINER(add_window), 15);
  gtk_window_set_position(GTK_WINDOW(add_window),GTK_WIN_POS_CENTER);

  window_icon=create_pixbuf("web.png");
  gtk_window_set_icon(GTK_WINDOW(add_window), window_icon);

  GtkWidget*grid = gtk_grid_new();
  gtk_container_add(GTK_CONTAINER(add_window), grid);

//text view word_add
  GtkWidget*word_add_view=gtk_text_view_new();
  gtk_container_set_border_width(GTK_CONTAINER(word_add_view), 8); 
  gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(word_add_view), GTK_WRAP_WORD_CHAR);

  GtkWidget*word_add_view_scrolled=gtk_scrolled_window_new(NULL,NULL);
  gtk_container_add(GTK_CONTAINER(word_add_view_scrolled),word_add_view);

  gtk_widget_set_size_request(word_add_view_scrolled,300,350);

  GtkWidget* frame_word_add_view = gtk_frame_new("Word");
  gtk_frame_set_shadow_type(GTK_FRAME(frame_word_add_view), GTK_SHADOW_OUT);
  gtk_container_add(GTK_CONTAINER(frame_word_add_view),word_add_view_scrolled);
  gtk_grid_attach(GTK_GRID(grid),frame_word_add_view,0,0,1,1);

  //text view mean_add
  GtkWidget*mean_add_view=gtk_text_view_new();
  gtk_container_set_border_width(GTK_CONTAINER(mean_add_view), 8); 
  gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(mean_add_view), GTK_WRAP_WORD_CHAR);

  GtkWidget*mean_add_view_scrolled=gtk_scrolled_window_new(NULL,NULL);
  gtk_container_add(GTK_CONTAINER(mean_add_view_scrolled),mean_add_view);
  
  gtk_widget_set_size_request(mean_add_view_scrolled,300,350);

  GtkWidget* frame_mean_add_view = gtk_frame_new("Meaning");
  gtk_frame_set_shadow_type(GTK_FRAME(frame_mean_add_view), GTK_SHADOW_OUT);
  gtk_container_add(GTK_CONTAINER(frame_mean_add_view),mean_add_view_scrolled);
  gtk_grid_attach(GTK_GRID(grid),frame_mean_add_view,1,0,1,1);

  gtk_grid_set_column_spacing(GTK_GRID(grid),10);

  //make OK button
  GtkWidget*ok_button=gtk_button_new();
  gtk_widget_set_size_request(ok_button,60,70);
  gtk_grid_attach(GTK_GRID(grid),ok_button,3,0,1,1);

  char none[1];none[0]='\0';
  gtk_container_add(GTK_CONTAINER(ok_button),button_interface("OK.png",(const gchar*)none));

  pointer->GW1=add_window;
  pointer->GW2=word_add_view;
  pointer->GW3=mean_add_view;

  g_signal_connect(ok_button,"clicked",G_CALLBACK(add_new_word), cb_data);

  //if(pointer->flag==1) 

  gtk_widget_show_all(add_window);

  //destroy windget
  g_signal_connect_swapped (add_window, "destroy", G_CALLBACK (gtk_widget_destroy), add_window);

  g_object_unref(window_icon);

  return ;
}

// empty# : ko có gì đã được đọc

