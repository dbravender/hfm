#include <gnome.h>
#include <stdio.h>
#include <bonobo.h>
#include <glib.h>
#include <gtk/gtk.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>

#include "hfm.h"

#define DV_DIRECTORY 0
#define DV_FILE 1
#define DV_MIME 2

#define UNIT_COUNT 4
gchar* unit[4] = { "bytes", "k","MB","GB" };

GHashTable *icon_table = NULL;

void hfm_drag_begin(GtkWidget *widget, GdkDragContext *drag_context, gpointer userData)
{
  struct directory_view *dv = NULL;
  struct hfm_window	*window = NULL;
	GList	*selected_files = NULL;
	GList	*file_to_draw = NULL;
	gchar	*focused_filename = NULL;
	guint	string_width = 0;
	/*struct RowStyle	*row_style = NULL;*/
	GdkGC	*pixmap_gc = NULL;
	GdkGC	*mask_gc = NULL;
	GtkStyle *ctree_style = NULL;
	GdkPixmap *drag_pixmap = NULL;
	GdkPixmap *drag_mask = NULL;
	GdkColor mask_color;
	guint width = 0;
	guint height = 0;
	gchar *too_many_filenames = NULL;
	guint counter = 0;
	guint seperator_line_y = 0;

  dv = (struct directory_view *)userData;
  window = dv->parent_window;

	printf("hfm_window_drag_begin\n");

	gtk_signal_emit_stop_by_name(GTK_OBJECT(widget), "drag_begin");
    
	/* get the gtk style of the ctree */
	ctree_style = gtk_widget_get_style(GTK_WIDGET(dv->clist));
    
	/* make a sorted GList of the filenames that should be used to construct the drag icon */
	/*if(!g_list_length(fm_window->pSelection))
	{
        focused_filename = g_strdup(fmWindow_getFilenameToRemember(fm_window, fm_window->pFocusedNode));
		selected_files = g_list_append(selected_files, focused_filename);
		row_style = rowStyleFind(pgPreferences->pRowStyles, "Focus");
        gtk_object_set_data(GTK_OBJECT(widget), "free_selection_data", (gpointer)1);
	}
	else
	{*/
    /*selected_files = g_list_copy(fm_window->pSelection);*/
		selected_files = g_list_append(selected_files, dv->selected);
    selected_files = g_list_sort(selected_files, (GCompareFunc)strcmp);
		/*row_style = rowStyleFind(pgPreferences->pRowStyles, "Selected");*/
	/*}*/

	if(selected_files)
	{
		/* store the drag selection data */
        gtk_object_set_data(GTK_OBJECT(widget), "drag_selection_data", selected_files);
		
		/* calculate the drag icon dimensions */
		file_to_draw = selected_files;
		while(file_to_draw)
		{
			string_width = gdk_string_width(ctree_style->font, (gchar*)file_to_draw->data);
			if(string_width > width)
			{
				width = string_width;
			}
			file_to_draw = file_to_draw->next;
		}
		width = width + 4;
		height = g_list_length(selected_files) * (ctree_style->font->ascent + ctree_style->font->descent + 2);

		if(height > (gdk_screen_height() / 3) * 2)
		{
			height = ctree_style->font->ascent + ctree_style->font->descent + 2;
            too_many_filenames = g_new(gchar, 100);
            sprintf(too_many_filenames, "Big selection, dragging %d files", g_list_length(selected_files));
			width = gdk_string_width(ctree_style->font, too_many_filenames) + 4;
		}

		/* allocate the pixmaps */
		drag_pixmap = gdk_pixmap_new(NULL, width, height, -1);
        drag_mask = gdk_pixmap_new(NULL, width, height, 1);
		
		/* initialize the graphical contexts */
		pixmap_gc = gdk_gc_new(drag_pixmap);
		/*gdk_gc_set_background(pixmap_gc, &(row_style->background));*/
		/*gdk_gc_set_foreground(pixmap_gc, &(row_style->foreground));*/
		gdk_gc_set_function(pixmap_gc, GDK_COPY);
		gdk_gc_set_font(pixmap_gc, ctree_style->font);
		
		mask_gc = gdk_gc_new(drag_mask);
        mask_color.pixel = 1;
		gdk_gc_set_foreground(mask_gc, &mask_color);
		gdk_gc_set_function(mask_gc, GDK_COPY);

		/* set the drag icon backgrounds */
		/*gdk_gc_set_foreground(pixmap_gc, &(row_style->background));*/
		gdk_draw_rectangle(drag_pixmap, pixmap_gc, TRUE, 0, 0, width, height);
		/*gdk_gc_set_foreground(pixmap_gc, &(row_style->foreground));*/

		gdk_draw_rectangle(drag_mask, mask_gc, TRUE, 0, 0, width, height);
        mask_color.pixel = 0;
		gdk_gc_set_foreground(mask_gc, &mask_color);

		if(too_many_filenames)
		{
			gdk_draw_text(drag_pixmap, ctree_style->font, pixmap_gc, 2, ctree_style->font->ascent, too_many_filenames, strlen(too_many_filenames));
			g_free(too_many_filenames);
		}
		else
		{
			/* draw the filenames */
			file_to_draw = selected_files;
			counter = 0;
			while(file_to_draw)
			{
				gdk_draw_text(drag_pixmap, ctree_style->font, pixmap_gc, 2, ctree_style->font->ascent + counter * (ctree_style->font->ascent + ctree_style->font->descent + 2), (gchar*)file_to_draw->data, strlen((gchar*)file_to_draw->data));
				counter++;
				if(file_to_draw->next)
				{
					seperator_line_y = counter * (ctree_style->font->ascent + ctree_style->font->descent + 2);
					gdk_draw_line(drag_mask, mask_gc, 0, seperator_line_y, width, seperator_line_y);
				}
				file_to_draw = file_to_draw->next;
			}
		}
		gtk_drag_set_icon_pixmap(drag_context, gtk_widget_get_colormap(GTK_WIDGET(window->window)), drag_pixmap, drag_mask, -10, -10);
	}
}

void 
check_exit(GtkObject *window, gpointer data) {
  window_count--;
  
  hfm_window_destroy((struct hfm_window *)data);
  
  if(window_count <=0) {
    g_hash_table_destroy(icon_table);
    gtk_main_quit();
  }
}

inline int 
is_directory(gchar *fullpath) {
  struct stat *stat_entry = NULL;
  int is_dir = 0;
  
  stat_entry = g_malloc(sizeof(struct stat));  
  stat(fullpath, stat_entry);
  
  is_dir = S_ISDIR(stat_entry->st_mode);
  
  g_free(stat_entry);
  
  return is_dir;
}

struct icon_entry *
get_icon_for_file(const gchar *path, const gchar *filename) {
  gchar *image_filename = NULL, *mime_type = NULL, *fullpath = NULL;
  struct icon_entry *entry = NULL;
  
  fullpath = g_strdup_printf("%s/%s", path, filename);
  
  if(is_directory(fullpath)) {
    mime_type = "directory";
    image_filename = g_strdup("directory.xpm");
  } else {  
    mime_type = g_strdup((gchar *)gnome_mime_type(fullpath));
    if(!mime_type)
      mime_type = g_strdup("na");

    image_filename = g_strdup((gchar *)gnome_mime_get_value(mime_type, "hfm-icon"));
  }
  
  if(!image_filename)
    image_filename = g_strdup("file.xpm");
  
  entry = g_hash_table_lookup(icon_table, mime_type);
  
  if(!entry) {
    entry = g_malloc(sizeof(struct icon_entry));
    
    entry->pixmap = gdk_pixmap_colormap_create_from_xpm(NULL, gdk_colormap_get_system(), &entry->mask, NULL, image_filename);
    g_hash_table_insert(icon_table, mime_type, entry);
  }
  
  g_free(fullpath);
  /*g_free(mime_type);*/
  g_free(image_filename);
  
  return entry;
}
  
struct hfm_window *
hfm_window_new(gchar *base_directory) {
  struct hfm_window *window;
  
  if(!icon_table)
    icon_table = g_hash_table_new(g_str_hash, g_str_equal);
  
  window = g_malloc(sizeof(struct hfm_window));
  
  if(!window) {
    printf("Unable to allocate memory for hfm_window!\n");
    exit(1);
  }
  
  window_count++;

  window->dv_tail = NULL;
  window->dv_head = NULL;
  
  window->window = bonobo_window_new("hfm", "hfm");
  
  window->uic = bonobo_ui_container_new();
  bonobo_ui_container_set_win (window->uic, BONOBO_WINDOW(window->window));
  
  gtk_widget_set_usize(GTK_WIDGET(window->window), 780, 300);
  
  
  gtk_signal_connect(GTK_OBJECT(window->window), "destroy", GTK_SIGNAL_FUNC(check_exit), (gpointer)window);
  
  window->scroll = gtk_scrolled_window_new(NULL, NULL);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW (window->scroll),
                                    GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
                                    
  window->hbox = gtk_hbox_new(TRUE, 0);
  gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(window->scroll), window->hbox);
  
  window->status_label = gtk_label_new("/");
  
  window->vbox = gtk_vbox_new(FALSE, 0);
  /*gtk_container_add(GTK_CONTAINER(window->vbox), window->scroll);*/
  /*gtk_container_add(GTK_CONTAINER(window->vbox), window->status_label);*/
  
  bonobo_window_set_contents(BONOBO_WINDOW(window->window), window->scroll);
  
  gtk_widget_show_all(GTK_WIDGET(window->window));
  
  hfm_add_directory_view(window, base_directory);
  
  return window;
}

void 
hfm_window_destroy(struct hfm_window *window) {
  struct directory_view *p;
  
  if(!window) {
    printf("Attempting to destroy NULL hfm window!\n");
    return;
  }
  
  for(p = window->dv_head; p != NULL; p = p->next) {
    hfm_destroy_view(p);
  }  
  
  if(window->uic) {
    bonobo_object_unref(BONOBO_OBJECT(window->uic));
  } 
  
  gtk_widget_destroy(GTK_WIDGET(window->scroll));
  gtk_widget_destroy(GTK_WIDGET(window->hbox));
  gtk_widget_destroy(GTK_WIDGET(window->vbox));
  gtk_widget_destroy(GTK_WIDGET(window->window));
  
  g_free(window);
}

void 
run(gchar *path, gchar *file) {
  gchar *mime_type = NULL, *command = NULL, *command_location = NULL, *run_command = NULL;
  gchar *fullpath = NULL;
  
  fullpath = g_strdup_printf("%s/%s", path, file);
  
  mime_type = g_strdup(gnome_mime_type_of_file(fullpath));
  command = g_strdup(gnome_mime_get_value(mime_type, "open"));
  
  if(!command) {
    g_free(fullpath);
    g_free(mime_type);
    
    return;
  }
  
  command_location = index(command, '%');
  if(command_location != NULL)
    command_location[0] = '\0';
  
  run_command = g_strdup_printf("%s \"%s\" &", command, fullpath);
  
  printf("Executing command %s\n", run_command);
  
  system(run_command);
  
  g_free(mime_type);
  g_free(fullpath);
  g_free(command);
  g_free(run_command);
}

void 
file_clicked(GtkWidget *src, GdkEventButton *event, gpointer user_data) {
  struct directory_view *dv;
  struct hfm_window *window;

  dv = (struct directory_view *)user_data;
  window = (struct hfm_window *)dv->parent_window;
  
  if(event->type == GDK_2BUTTON_PRESS && dv->selected)
    run(dv->fullpath, dv->selected);
}

void 
action_selected(GtkWidget *src, gpointer user_data) {
  printf("Executing %s\n", (char *)user_data);
  
  system((gchar *)user_data);
}
  
void 
file_selected(GtkWidget *src, gint row, gint column, GdkEventButton *event, gpointer user_data) {
  struct directory_view *dv, *p;
  struct hfm_window *window;
  gchar *file;
  
  dv = (struct directory_view *)user_data;
  window = (struct hfm_window *)dv->parent_window;
  
  if(dv->type == DV_DIRECTORY) {
    gtk_clist_get_text(GTK_CLIST(dv->clist), row, 1, &file);  
  
    if(dv->selected)
      g_free(dv->selected);
     
    dv->selected = g_strdup(file);
    
    for(p = dv->next; p != NULL; p = p->next) {
      hfm_destroy_view(p);
    }

    dv->next = NULL;
    window->dv_tail = dv;
 
    hfm_add_directory_view(window, file);
  } else if(dv->type == DV_FILE) {
    gtk_clist_get_text(GTK_CLIST(dv->clist), row, column, &file);
    
    if(strcmp("mime-type", file) == 0) {
      for(p = dv->next; p != NULL; p = p->next) {
        hfm_destroy_view(p);
      }

      dv->next = NULL;
      window->dv_tail = dv;
 
      gtk_clist_get_text(GTK_CLIST(dv->clist), 2, 1, &file);
      hfm_add_mime_view(window, file); 
    }
  }
  gtk_label_set_text(GTK_LABEL(window->status_label), window->dv_tail->fullpath);
}

void 
hfm_destroy_view(struct directory_view *dv) {  
  GList *p;
  
  if(dv->bonobo_preview)
    gtk_widget_destroy(GTK_WIDGET(dv->bonobo_preview));
  gtk_widget_destroy(GTK_WIDGET(dv->clist));
  if(dv->vbox)
    gtk_widget_destroy(GTK_WIDGET(dv->vbox));
  gtk_widget_destroy(GTK_WIDGET(dv->scroll));

  for(p = dv->buttons; p != NULL; p = p->next) {
    gtk_widget_destroy(GTK_WIDGET(p->data));
  }
  
  for(p = dv->commands; p != NULL; p = p->next) {
    g_free(p->data);
  }

  g_list_free(dv->buttons);
  g_list_free(dv->commands);

  g_free(dv->selected);
  g_free(dv->directory);
  g_free(dv->fullpath);
  
  g_free(dv);
}

void 
add_action_buttons(struct directory_view *dv) {
  GtkWidget *button;
  gchar *mime_type, *actions;
  gchar **key_pair, **action_pair;
  char *run_command = NULL, *command_location = NULL;
  int i = 0;
  
  mime_type = g_strdup(gnome_mime_type_of_file(dv->fullpath));
  actions = g_strdup(gnome_mime_get_value(mime_type, "hfm-action"));
  
  if(actions) {
    key_pair = g_strsplit(actions, ",", 0);
    
    for(i = 0; key_pair[i] != NULL; i++) {
      action_pair = g_strsplit(key_pair[i], "=", 0);
      if(action_pair[0] && action_pair[1]) {
        button = gtk_button_new_with_label(g_strdup_printf("%s (%s)", g_strstrip(action_pair[0]), g_strstrip(action_pair[1])));
        
        command_location = index(action_pair[1], '%');
        if(command_location != NULL)
          command_location[0] = '\0';
  
        run_command = g_strdup_printf("%s \"%s\" &", g_strstrip(action_pair[1]), dv->fullpath);
  
        gtk_signal_connect(GTK_OBJECT(button), "clicked", GTK_SIGNAL_FUNC(action_selected), (char *)run_command);
        gtk_widget_set_usize(GTK_WIDGET(button), 100, 25);
        dv->buttons = g_list_append(dv->buttons, (gpointer)button);
        dv->commands = g_list_append(dv->commands, (gpointer)run_command);
        gtk_box_pack_start(GTK_BOX(dv->vbox), button, FALSE, FALSE, 0);
        gtk_widget_show(button);
      }
      g_strfreev(action_pair);
    }
    
    g_strfreev(key_pair);
    g_free(actions);
  }
  
  g_free(mime_type);
}

struct directory_view *
hfm_add_mime_view(struct hfm_window *window, const gchar *mime_type) {
  struct directory_view *dv = NULL;
  GList *mime_keys, *p;
  gchar *insert[] = {NULL, NULL, NULL};
  
  dv = g_malloc(sizeof(struct directory_view));
  
  if(!dv) {
    printf("Unable to allocate memory for directory_view structure\n");
    exit(1);
  }
  
  dv->type = DV_MIME;
  
  dv->next = NULL;
  dv->selected = NULL;
  
  dv->bonobo_preview = NULL;
  dv->vbox = NULL;
  
  dv->parent_window = window;
  
  dv->directory = g_strdup(mime_type);
   
  if(window->dv_tail) {
    dv->fullpath = g_strdup_printf("%s/%s", window->dv_tail->fullpath, mime_type);
  } else {
    dv->fullpath = g_strdup("/");
  }
  
  insert[1] = dv->directory;

  dv->clist = gtk_clist_new_with_titles(2, insert);
  
  gtk_clist_set_column_title(GTK_CLIST(dv->clist), 0, dv->directory);
  gtk_clist_set_column_width(GTK_CLIST(dv->clist), 0, 60);
  gtk_clist_set_column_width(GTK_CLIST(dv->clist), 1, 160);
  
  dv->scroll = gtk_scrolled_window_new(NULL, NULL);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW (dv->scroll), GTK_POLICY_NEVER, GTK_POLICY_ALWAYS);
  
  gtk_signal_connect(GTK_OBJECT(dv->clist), "select_row", GTK_SIGNAL_FUNC(file_selected), (gpointer)dv);
  gtk_signal_connect(GTK_OBJECT(dv->clist), "button_press_event", GTK_SIGNAL_FUNC(file_clicked), (gpointer)dv);
  
  gtk_container_add(GTK_CONTAINER(dv->scroll), dv->clist);
  gtk_container_add(GTK_CONTAINER(window->hbox), dv->scroll);
  
  if(!window->dv_head)
    window->dv_head = dv;
  
  if(window->dv_tail)
    window->dv_tail->next = dv;
  
  window->dv_tail = dv;
  
  mime_keys = gnome_mime_get_keys(mime_type);
  
  p = mime_keys;
  
  for(p = mime_keys; p != NULL; p = g_list_next(p)) {
    insert[0] = g_strdup((gchar *)p->data);
    insert[1] = g_strdup(gnome_mime_get_value(mime_type, insert[0]));
    gtk_clist_append(GTK_CLIST(dv->clist), insert); 
  
    g_free(insert[0]); insert[0] = NULL;
    g_free(insert[1]); insert[1] = NULL;
  }
    
  gtk_widget_show(dv->scroll);
  gtk_widget_show(dv->clist);
  
  return dv;
}

struct directory_view *
hfm_add_directory_view(struct hfm_window *window, gchar *file) {
  struct directory_view *dv = NULL;
  DIR *cur_dir = NULL;
  struct dirent *dir_entry = NULL;
  struct stat *stat_entry = NULL;
  gfloat size = 0;
  int count = 0;
  gchar *insert[] = {NULL, NULL, NULL};
  GList *directory_list = NULL, *p = NULL;
  struct icon_entry *icon = NULL;
  int row = 0;
  
  dv = g_malloc(sizeof(struct directory_view));
  
  if(!dv) {
    printf("Unable to allocate memory for directory_view structure\n");
    exit(1);
  }
  
  dv->commands = NULL;
  dv->buttons = NULL;
  
  dv->next = NULL;
  dv->selected = NULL;
  
  dv->parent_window = window;
  
  dv->directory = g_strdup(file);
  
  if(window->dv_tail) {
    dv->fullpath = g_strdup_printf("%s/%s", window->dv_tail->fullpath, file);
  } else {
    dv->fullpath = g_strdup("/");
  }
  
  insert[1] = dv->directory;
  
  dv->clist = gtk_clist_new_with_titles(2, insert);
  
  gtk_clist_set_column_title(GTK_CLIST(dv->clist), 1, dv->directory);
  gtk_clist_set_column_width(GTK_CLIST(dv->clist), 0, 15);
  gtk_clist_set_column_width(GTK_CLIST(dv->clist), 1, 205);
  
  dv->scroll = gtk_scrolled_window_new(NULL, NULL);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW (dv->scroll), GTK_POLICY_NEVER, GTK_POLICY_ALWAYS);
  
  gtk_signal_connect(GTK_OBJECT(dv->clist), "drag_begin", GTK_SIGNAL_FUNC(hfm_drag_begin), (gpointer)dv);
  gtk_signal_connect(GTK_OBJECT(dv->clist), "select_row", GTK_SIGNAL_FUNC(file_selected), (gpointer)dv);
  gtk_signal_connect(GTK_OBJECT(dv->clist), "button_press_event", GTK_SIGNAL_FUNC(file_clicked), (gpointer)dv);
  
  dv->bonobo_preview = bonobo_widget_new_control(g_strdup_printf("file:%s",dv->fullpath), BONOBO_OBJREF(window->uic));
  if(!dv->bonobo_preview)
    dv->bonobo_preview = gtk_label_new("No preview available.");
  
  dv->vbox = gtk_vbox_new(FALSE, 0);
  
  gtk_container_add(GTK_CONTAINER(dv->vbox), dv->clist);
  
  add_action_buttons(dv);
  
  gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(dv->scroll), dv->vbox);
  gtk_container_add(GTK_CONTAINER(window->hbox), dv->scroll);
 
  if(!window->dv_head)
    window->dv_head = dv;
  
  if(window->dv_tail)
    window->dv_tail->next = dv;
  
  window->dv_tail = dv;
  
  cur_dir = opendir(dv->fullpath);
  
  if(cur_dir) {
    /* Directory */
    dv->type = DV_DIRECTORY;
    
    while((dir_entry = readdir(cur_dir))) {
      if(dir_entry->d_name[0] == '.') 
        continue;
      directory_list = g_list_insert_sorted(directory_list, g_strdup(dir_entry->d_name), (GCompareFunc)g_strncasecmp);
    }
    
    for(p = directory_list; p != NULL; p = g_list_next(p)) {
      insert[1] = (gchar *)p->data;
      gtk_clist_append(GTK_CLIST(dv->clist), insert);
      icon = get_icon_for_file(dv->fullpath, p->data);
      gtk_clist_set_pixmap(GTK_CLIST(dv->clist), row, 0, icon->pixmap, icon->mask);
      g_free(p->data);
      row++;
    }
    
    g_list_free(directory_list);
    
    closedir(cur_dir);
  } else {
    /* File */
    dv->type = DV_FILE;
    
    stat_entry = g_malloc(sizeof(struct stat));
    
    stat(dv->fullpath, stat_entry);

    size = (gfloat)stat_entry->st_size;
    
    while ( size >= 1024 && count < UNIT_COUNT ) {
      size /= 1024;
      count++;
    }

    gtk_clist_set_column_title(GTK_CLIST(dv->clist), 0, "info");
    gtk_clist_set_column_title(GTK_CLIST(dv->clist), 1, "");
    
    insert[0] = "filename";
    insert[1] = dv->directory;
    gtk_clist_append(GTK_CLIST(dv->clist), insert);

    insert[0] = "size";
    insert[1] = g_strdup_printf("%.01f %s", size, unit[count]);
    gtk_clist_append(GTK_CLIST(dv->clist), insert);

    g_free(insert[1]);
    
    insert[0] = "mime-type";
    insert[1] = g_strdup(gnome_mime_type_of_file(dv->fullpath));
    gtk_clist_append(GTK_CLIST(dv->clist), insert);

    g_free(insert[1]);
  
    gtk_clist_set_column_width(GTK_CLIST(dv->clist), 0, 60);
    gtk_clist_set_column_width(GTK_CLIST(dv->clist), 1, 160);
    gtk_container_add(GTK_CONTAINER(dv->vbox), dv->bonobo_preview);
    gtk_widget_show(dv->bonobo_preview);
  }
  
  gtk_widget_show(dv->scroll);
  gtk_widget_show(dv->vbox);
  gtk_widget_show(dv->clist);
 
  return dv;
}

gint 
create_app(void) {
  struct hfm_window *hfm_main;
  
  hfm_main = (struct hfm_window *)hfm_window_new("");
 
  return FALSE;
}

int 
main(int argc, char *argv[]) {  
  CORBA_ORB orb;
  
  gnome_init_with_popt_table ("hfm", "0.0", argc, argv, oaf_popt_options, 0, NULL);

  orb = oaf_init (argc,argv);
  if(!orb)
    g_error ("initializing orb failed");
  if(!bonobo_init (orb, NULL, NULL))
    g_error ("could not initialize Bonobo");

  gtk_idle_add ((GtkFunction) create_app, NULL);
  bonobo_main ();
  
  return(0);
}
