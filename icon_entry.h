#ifndef _ICON_ENTRY_H_
#define _ICON_ENTRY_H_

GHashTable *icon_table = NULL;

struct icon_entry {
  GdkPixmap *pixmap;
  GdkBitmap *mask;
};

struct icon_entry *get_icon_for_file(gchar *path, gchar *filename);
void icon_entry_table_destroy(GHashTable *icon_table);

#endif
