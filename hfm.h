/* hfm.h */

#ifndef _HFM_H_
#define _HFM_H_

struct icon_entry {
  GdkPixmap *pixmap;
  GdkBitmap *mask;
};

int window_count = 0;

struct hfm_window {
  GtkWidget *window;
  BonoboUIContainer *uic;
  GtkWidget *scroll;
  GtkWidget *hbox;
  GtkWidget *vbox;
  GtkWidget *status_label;
  struct directory_view *dv_head;
  struct directory_view *dv_tail;
};

struct directory_view {
  int type;
  struct hfm_window *parent_window;
  GtkWidget *clist;
  GtkWidget *scroll;
  GtkWidget *vbox;
  GtkWidget *bonobo_preview;
  GList *buttons;
  GList *commands;
  gchar *directory;
  gchar *fullpath;
  gchar *selected;
  struct directory_view *next;
};

struct hfm_window *hfm_window_new(char *base_directory);
void hfm_window_destroy(struct hfm_window *window);
void hfm_destroy_view(struct directory_view *dv);
struct directory_view *hfm_add_directory_view(struct hfm_window *window, char *file);
struct directory_view *hfm_add_mime_view(struct hfm_window *window, const char *mime_type);

#endif
