#include <bonobo.h>
#include <gtk/gtk.h>
#include "hfm-window.h"
#include "hfm-view-directory.h"

gint create_app(void) {
  GtkWidget *window;
  GtkWidget *view;
  
  window = hfm_window_new();
  
  view = hfm_view_directory_new(HFM_WINDOW(window), "/", "/");
  hfm_window_add_view(HFM_WINDOW(window), view);
  
  gtk_widget_show_all(window);
 
  return FALSE;
}

int main(int argc, char *argv[]) {  
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
