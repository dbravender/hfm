/* icon-entry.h */

#ifndef _ICON_ENTRY_H_
#define _ICON_ENTRY_H_

#include <gnome.h>

extern GHashTable *icon_table;

struct icon_hash {
  GdkPixmap *pixmap;
  GdkBitmap *mask;
};

inline int is_directory(gchar *fullpath);
struct icon_hash *get_icon_for_file(const gchar *path, const gchar *filename);
void icon_hash_destroy(GHashTable *icon_table);

#endif
