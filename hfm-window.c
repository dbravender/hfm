/* Generated by GOB (v1.0.9) on Tue Jun 19 23:46:03 2001
   (do not edit directly) */

/* End world hunger, donate to the World Food Programme, http://www.wfp.org */


#line 1 "hfm-window.gob"

#include <bonobo.h>
#include <gnome.h>
#include "icon-hash.h"

int window_count;

#line 16 "hfm-window.c"
#define GOB_VERSION_MAJOR 1
#define GOB_VERSION_MINOR 0
#define GOB_VERSION_PATCHLEVEL 9

#include <string.h> /* memset() */

#include "hfm-window.h"

#include "hfm-window-private.h"

/* self casting macros */
#define SELF(x) HFM_WINDOW(x)
#define SELF_CONST(x) HFM_WINDOW_CONST(x)
#define IS_SELF(x) HFM_IS_WINDOW(x)
#define SELF_CLASS(x) HFM_WINDOW_CLASS(x)

#define SELF_GET_CLASS(x) HFM_WINDOW_GET_CLASS(x)

/* self typedefs */
typedef HfmWindow Self;
typedef HfmWindowClass SelfClass;

/* GTK_CLASS_TYPE for 1.2<->1.3/2.0 GTK+ compatibility */
#ifndef GTK_CLASS_TYPE
#define GTK_CLASS_TYPE(x) (GTK_OBJECT_CLASS(x)->type)
#endif /* GTK_CLASS_TYPE */

/* here are local prototypes */
static void hfm_window_init (HfmWindow * o) G_GNUC_UNUSED;
static void hfm_window_class_init (HfmWindowClass * c) G_GNUC_UNUSED;
static void ___1_hfm_window_destroy (GtkObject * self) G_GNUC_UNUSED;

/* pointer to the class of our parent */
static BonoboWindowClass *parent_class = NULL;

GtkType
hfm_window_get_type (void)
{
	static GtkType type = 0;

	if (type == 0) {
		static const GtkTypeInfo info = {
			"HfmWindow",
			sizeof (HfmWindow),
			sizeof (HfmWindowClass),
			(GtkClassInitFunc) hfm_window_class_init,
			(GtkObjectInitFunc) hfm_window_init,
			/* reserved_1 */ NULL,
			/* reserved_2 */ NULL,
			(GtkClassInitFunc) NULL
		};

		type = gtk_type_unique (bonobo_window_get_type(), &info);
	}

	return type;
}

/* Short form macros */
#if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#define add_view(args...) hfm_window_add_view(args)
#define add_view_after(args...) hfm_window_add_view_after(args)
#define new() hfm_window_new()
#endif /* __GNUC__ && !__STRICT_ANSI__ */

/* Short form pointers */
static void (* const add_view) (HfmWindow * self, GtkWidget * widget) = hfm_window_add_view;
static void (* const add_view_after) (HfmWindow * self, GtkWidget * current, GtkWidget * new_widget) = hfm_window_add_view_after;
static GtkWidget * (* const new) (void) = hfm_window_new;

/* a macro for creating a new object of our type */
#define GET_NEW ((HfmWindow *)gtk_type_new(hfm_window_get_type()))


static void
___destroy(GtkObject *obj_self)
{
#define __GOB_FUNCTION__ "Hfm:Window::destroy"
	HfmWindow *self = HFM_WINDOW (obj_self);
#line 21 "hfm-window.gob"
	___1_hfm_window_destroy(obj_self);
#line 98 "hfm-window.c"
#line 10 "hfm-window.gob"
	if(self->uic) { ((*(void (*)(void *))bonobo_object_unref)) (self->uic); self->uic = NULL; }
#line 101 "hfm-window.c"
#line 14 "hfm-window.gob"
	if(self->views) { ((*(void (*)(void *))g_list_free)) (self->views); self->views = NULL; }
#line 104 "hfm-window.c"
	return;
	self = NULL;
}
#undef __GOB_FUNCTION__

static void 
hfm_window_init (HfmWindow * o)
{
#define __GOB_FUNCTION__ "Hfm:Window::init"
	return;
	o = NULL;
}
#undef __GOB_FUNCTION__
static void 
hfm_window_class_init (HfmWindowClass * c)
{
#define __GOB_FUNCTION__ "Hfm:Window::class_init"
	GtkObjectClass *gtk_object_class = (GtkObjectClass*) c;
#ifdef G_OBJECT_CLASS
	GObjectClass *g_object_class = (GObjectClass*) c;
#endif /* G_OBJECT_CLASS */

	parent_class = gtk_type_class (bonobo_window_get_type ());

#line 21 "hfm-window.gob"
	gtk_object_class->destroy = ___destroy;
#line 131 "hfm-window.c"
	return;
	c = NULL;
	gtk_object_class = NULL;
#ifdef G_OBJECT_CLASS
	g_object_class = NULL;
#endif /* G_OBJECT_CLASS */
}
#undef __GOB_FUNCTION__



#line 21 "hfm-window.gob"
static void 
___1_hfm_window_destroy (GtkObject * self)
#line 146 "hfm-window.c"
#define PARENT_HANDLER(___self) \
	{ if(GTK_OBJECT_CLASS(parent_class)->destroy) \
		(* GTK_OBJECT_CLASS(parent_class)->destroy)(___self); }
{
#define __GOB_FUNCTION__ "Hfm:Window::destroy"
#line 21 "hfm-window.gob"
	g_return_if_fail (self != NULL);
#line 21 "hfm-window.gob"
	g_return_if_fail (GTK_IS_OBJECT (self));
#line 156 "hfm-window.c"
{
#line 21 "hfm-window.gob"
	
    window_count--;
  
    if(window_count <= 0) {
      g_hash_table_destroy(icon_table);
      gtk_main_quit();
    }
    
    PARENT_HANDLER(self);
  }}
#line 169 "hfm-window.c"
#undef __GOB_FUNCTION__
#undef PARENT_HANDLER

#line 32 "hfm-window.gob"
void 
hfm_window_add_view (HfmWindow * self, GtkWidget * widget)
#line 176 "hfm-window.c"
{
#define __GOB_FUNCTION__ "Hfm:Window::add_view"
#line 32 "hfm-window.gob"
	g_return_if_fail (self != NULL);
#line 32 "hfm-window.gob"
	g_return_if_fail (HFM_IS_WINDOW (self));
#line 183 "hfm-window.c"
{
#line 32 "hfm-window.gob"
	
    self->views = g_list_append(self->views, widget);
    
    gtk_container_add(GTK_CONTAINER(self->hbox), widget);
    gtk_widget_show_all(widget);
  }}
#line 192 "hfm-window.c"
#undef __GOB_FUNCTION__

#line 39 "hfm-window.gob"
void 
hfm_window_add_view_after (HfmWindow * self, GtkWidget * current, GtkWidget * new_widget)
#line 198 "hfm-window.c"
{
#define __GOB_FUNCTION__ "Hfm:Window::add_view_after"
#line 39 "hfm-window.gob"
	g_return_if_fail (self != NULL);
#line 39 "hfm-window.gob"
	g_return_if_fail (HFM_IS_WINDOW (self));
#line 205 "hfm-window.c"
{
#line 39 "hfm-window.gob"
	
    GList *next, *p;

    next = g_list_find(self->views, current);

    if(next) {
      next = next->next; /*We want to delete _after_ current*/
      while(next != NULL) {
        p = next;
        next = next->next;
        
        gtk_widget_destroy(GTK_WIDGET(p->data));
        self->views = g_list_remove(self->views, p->data);
      } 
    }
    
    add_view(self, new_widget);
  }}
#line 226 "hfm-window.c"
#undef __GOB_FUNCTION__

#line 58 "hfm-window.gob"
GtkWidget * 
hfm_window_new (void)
#line 232 "hfm-window.c"
{
#define __GOB_FUNCTION__ "Hfm:Window::new"
{
#line 58 "hfm-window.gob"
	
    HfmWindow *self = GET_NEW;
    
    if(!icon_table)
      icon_table = g_hash_table_new(g_str_hash, g_str_equal);
    
    window_count++;
    
    gtk_widget_set_usize(GTK_WIDGET(self), 780, 300);
    
    self->uic = bonobo_ui_container_new();
    bonobo_ui_container_set_win(self->uic, BONOBO_WINDOW(self));
    
    bonobo_window_set_name(BONOBO_WINDOW(self), "hfm: /");
     
    self->hbox = gtk_hbox_new(TRUE, 0);
    self->scrollwindow = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW (self->scrollwindow), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    
    gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(self->scrollwindow), self->hbox);
    bonobo_window_set_contents(BONOBO_WINDOW(self), self->scrollwindow);

    gtk_widget_show_all((GtkWidget *)self);

    return (GtkWidget *)self;
  }}
#line 263 "hfm-window.c"
#undef __GOB_FUNCTION__


#if (!defined __GNUC__) || (defined __GNUC__ && defined __STRICT_ANSI__)
/*REALLY BAD HACK
  This is to avoid unused warnings if you don't call
  some method.  I need to find a better way to do
  this, not needed in GCC since we use some gcc
  extentions to make saner, faster code */
static void
___hfm_window_really_bad_hack_to_avoid_warnings(void)
{
	((void (*)(void))add_view)();
	((void (*)(void))add_view_after)();
	((void (*)(void))new)();
	___hfm_window_really_bad_hack_to_avoid_warnings();
}
#endif /* !__GNUC__ || (__GNUC__ && __STRICT_ANSI__) */

