/* Generated by GOB (v1.0.9)   (do not edit directly) */



#include <gnome.h>
#include <time.h>
#include <sys/stat.h>
#include "hfm-window.h"
#include "hfm-view-mime.h"

#include <gtk/gtk.h>

#ifndef __HFM_VIEW_FILE_H__
#define __HFM_VIEW_FILE_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*
 * Type checking and casting macros
 */
#define HFM_TYPE_VIEW_FILE	(hfm_view_file_get_type())
#define HFM_VIEW_FILE(obj)	GTK_CHECK_CAST((obj), hfm_view_file_get_type(), HfmViewFile)
#define HFM_VIEW_FILE_CONST(obj)	GTK_CHECK_CAST((obj), hfm_view_file_get_type(), HfmViewFile const)
#define HFM_VIEW_FILE_CLASS(klass)	GTK_CHECK_CLASS_CAST((klass), hfm_view_file_get_type(), HfmViewFileClass)
#define HFM_IS_VIEW_FILE(obj)	GTK_CHECK_TYPE((obj), hfm_view_file_get_type ())

#ifdef GTK_CHECK_GET_CLASS
#define HFM_VIEW_FILE_GET_CLASS(obj)	GTK_CHECK_GET_CLASS((obj), hfm_view_file_get_type(), HfmViewFileClass)
#else /* !GTK_CHECK_GET_CLASS */
#define HFM_VIEW_FILE_GET_CLASS(obj)	((HfmViewFileClass *)GTK_OBJECT(obj)->klass)
#endif /* GTK_CHECK_GET_CLASS */

/*
 * Main object structure
 */
#ifndef __TYPEDEF_HFM_VIEW_FILE__
#define __TYPEDEF_HFM_VIEW_FILE__
typedef struct _HfmViewFile HfmViewFile;
#endif
struct _HfmViewFile {
	GtkHBox __parent__;
	/*< public >*/
	gchar * file;
	gchar * fullpath;
	GtkWidget * file_button;
	GtkWidget * clist;
	GtkWidget * scrollwindow;
	GtkWidget * vbox;
	GtkWidget * bonobo_preview;
	GList * buttons;
	GList * commands;
	HfmWindow * parent;
};

/*
 * Class definition
 */
typedef struct _HfmViewFileClass HfmViewFileClass;
struct _HfmViewFileClass {
	GtkHBoxClass __parent__;
};


/*
 * Public methods
 */
GtkType	hfm_view_file_get_type	(void) G_GNUC_CONST;
GtkWidget * 	hfm_view_file_new	(HfmWindow * parent,
					gchar * fullpath,
					gchar * file);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
