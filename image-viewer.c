// http://iim.cs.tut.ac.jp/member/sugaya/GTK+/files/gtkbook-20210127.pdf
#include <gtk/gtk.h>
#include <stdlib.h>

// callback for menu item "open"
static void cb_open (GSimpleAction *action,
                     GVariant  *parameter,
                     gpointer  user_data){
  g_print ("This function is not implemented yet .\n");
}

// callback for menu item "quit"
static void cb_quit (GSimpleAction *action,
                     GVariant  *parameter,
                     gpointer  user_data){
  // quit main loop
  GApplication *app = G_APPLICATION(user_data);
  g_application_quit(app);
}

// callback for button click
static void cb_button_clicked (GtkWidget *button, 
                               GVariant *parameter,
                               gpointer user_data){
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
  "    <submenu>"
  "      <attribute name='label'>File</attribute>"
  "      <section>"
  "        <item>"
  "          <attribute name='label'>Open</attribute>"
  "          <attribute name='action'>app.open</attribute>"
  "          <attribute name='accel'><Primary>o</attribute>"
  "        </item>"
  "      </section>"
  "      <section>"
  "        <item>"
  "          <attribute name='label'>Quit</attribute>"
  "          <attribute name='action'>app.quit</attribute>"
  "          <attribute name='accel'><Primary>q</attribute>"
  "        </item>"
  "      </section>"
  "    </submenu>"
  "  </menu>"
  "</interface>";

// detail of menu items
static GActionEntry entries[] = {
  {"open", cb_open, NULL, NULL, NULL},
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
//  gtk_application_set_app_menu(GTK_APPLICATION(app), menubar);
  gtk_application_set_menubar (GTK_APPLICATION(app), menubar);
  {
//    GtkWidget *vbox;
    GtkWidget *image;
    GtkWidget *button;
    GtkWidget *scroll_window;

    // create box
/*    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);
    gtk_container_add(GTK_CONTAINER(window), vbox);*/

    // create scrolled widow
    scroll_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER(window), scroll_window);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW(scroll_window), 
                                    GTK_POLICY_AUTOMATIC,
                                    GTK_POLICY_AUTOMATIC);

    // create image
    image = gtk_image_new_from_file((char *) user_data);
    gtk_container_add (GTK_CONTAINER(scroll_window), image);

    // create button
/*    button = gtk_button_new_with_label("Quit");
    gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, FALSE, 0);
    g_signal_connect(button, "clicked", 
                    G_CALLBACK (cb_button_clicked), NULL);*/
  }

  gtk_widget_show_all(window);
}

int main(int argc, char *argv[]){
//  GtkWidget *window;
  GtkApplication *app;

  // check commandline params
  if (argc != 2){
    g_print("Usage: %s image-file\n", argv[0]);
    exit(1);
  }

  // create app
  app = gtk_application_new("test.gtk.imageviewer", 0);

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
