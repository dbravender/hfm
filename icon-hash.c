/* icon-hash.c */

#include "icon-hash.h"

#include <gnome.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

GHashTable *icon_table;

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

struct icon_hash *
get_icon_for_file(const gchar *path, const gchar *filename) {
  gchar *image_filename = NULL, *mime_type = NULL, *fullpath = NULL;
  struct icon_hash *entry = NULL;
  
  fullpath = g_strdup_printf("%s/%s", path, filename);
  
  if(is_directory(fullpath)) {
    mime_type = "directory";
    image_filename = g_strdup("directory.xpm");
  } else {  
    mime_type = g_strdup(gnome_mime_type(fullpath));
    if(!mime_type)
      mime_type = g_strdup("na");

    image_filename = g_strdup(gnome_mime_get_value(mime_type, "hfm-icon"));
  }
  
  if(!image_filename)
    image_filename = g_strdup("file.xpm");
  
  entry = g_hash_table_lookup(icon_table, mime_type);
  
  if(!entry) {
    entry = g_malloc(sizeof(struct icon_hash));
    
    entry->pixmap = gdk_pixmap_colormap_create_from_xpm(NULL, gdk_colormap_get_system(), &entry->mask, NULL, image_filename);
    g_hash_table_insert(icon_table, mime_type, entry);
  }
  
  g_free(fullpath);
  /*g_free(mime_type);*/
  g_free(image_filename);
  
  return entry;
}
