// http://iim.cs.tut.ac.jp/member/sugaya/GTK+/files/gtkbook-20210127.pdf
#include <gtk/gtk.h>
#include <stdlib.h>

// callback for menu item "quit"
static void cb_quit (GSimpleAction *action,
                     GVariant  *parameter,
                     gpointer  user_data){
  // quit main loop
  GApplication *app = G_APPLICATION(user_data);
  g_application_quit(app);
}

//
// menu structure
//
static const gchar menu_info[] =
  "<interface>"
"  <menu id='appmenu'>"
"    <section>"
"      <item>"
"        <attribute name='label' translatable='yes'>_Quit</attribute>"
"        <attribute name='action'>app.quit</attribute>"
"        <attribute name='accel'>&lt;Primary&gt;q</attribute>"
"      </item>"
"    </section>"
"  </menu>"
  "</interface>";

// detail of menu items
static GActionEntry entries[] = {
  {"quit", cb_quit, NULL, NULL, NULL},
};

// callback for event "activate"
static void cb_activate (GApplication *app, gpointer user_data){
  GtkWidget *window;

  // create window
  window = gtk_application_window_new(GTK_APPLICATION(app));

  // set window size
  gtk_widget_set_size_request(window, 300, 200);

  // create menu
  GtkBuilder *builder;
//  gtk_builder_add_from_string (builder, menu_info, -1, NULL);
  builder = gtk_builder_new();
  gtk_builder_add_from_string (builder, 
                               menu_info,
                               -1, NULL);
  GMenuModel *menubar;
  menubar = (GMenuModel *) gtk_builder_get_object(builder, "appmenu");
  gtk_application_set_app_menu(GTK_APPLICATION(app), menubar);
//  gtk_application_set_menubar (GTK_APPLICATION(app), menubar);
  
  gtk_widget_show_all(window);
}

int main(int argc, char *argv[]){
//  GtkWidget *window;
  GtkApplication *app;

  // create app
  app = gtk_application_new("test.gtk.menu", 0);

  // register action
  g_action_map_add_action_entries (G_ACTION_MAP(app),
                                   entries, G_N_ELEMENTS(entries),
                                   app);


  // register signal handler
  g_signal_connect(app, "activate", G_CALLBACK(cb_activate), argv[1]);

  // main loop
  g_application_run(G_APPLICATION(app), 0, NULL);

  return 0;
}
