// http://iim.cs.tut.ac.jp/member/sugaya/GTK+/files/gtkbook-20210127.pdf
#include <gtk/gtk.h>
#include <stdlib.h>

// callback for menu item "open"
static void cb_open (GSimpleAction *action,
                     GVariant  *parameter,
                     gpointer  user_data){
  GtkApplication *app;
  GtkWindow *window;
  GtkWidget *dialog;
  gint response;

  // get the Application window
  app = GTK_APPLICATION (user_data);
  window = gtk_application_get_active_window(app);

  // create file chooser dialog
  dialog = gtk_file_chooser_dialog_new ("Open an image",
                                        GTK_WINDOW(window),
                                        GTK_FILE_CHOOSER_ACTION_OPEN,
                                        "_Cancel",
                                        GTK_RESPONSE_CANCEL,
                                        "_OPEN",
                                        GTK_RESPONSE_ACCEPT,
                                        NULL);

  // show dialog
  gtk_widget_show_all (dialog);

  // set selected file as image file
  response = gtk_dialog_run (GTK_DIALOG(dialog));
  if (response == GTK_RESPONSE_ACCEPT){
    gchar *filename;
    GtkWidget *image;

    // get file name
    filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));

    // read image from file and set image widget
    image = GTK_WIDGET(g_object_get_data(G_OBJECT(window), "image"));
    gtk_image_set_from_file(GTK_IMAGE(image), filename);

    // free filename gchar
    g_free(filename);
  }

  // destroy dialog
  gtk_widget_destroy(dialog);
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
  "          <attribute name='accel'>&lt;Primary&gt;o</attribute>"
  "        </item>"
  "      </section>"
  "      <section>"
  "        <item>"
  "          <attribute name='label'>Quit</attribute>"
  "          <attribute name='action'>app.quit</attribute>"
  "          <attribute name='accel'>&lt;Primary&gt;q</attribute>"
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
  builder = gtk_builder_new();
  gtk_builder_add_from_string (builder, 
                               menu_info,
                               -1, NULL);
  GMenuModel *menubar;
  menubar = (GMenuModel *) gtk_builder_get_object(builder, "appmenu");
//  gtk_application_set_app_menu(GTK_APPLICATION(app), menubar);
  gtk_application_set_menubar (GTK_APPLICATION(app), menubar);

  {
    GtkWidget *image;
    GtkWidget *button;
    GtkWidget *scroll_window;

    // create scrolled widow
    scroll_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER(window), scroll_window);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW(scroll_window), 
                                    GTK_POLICY_AUTOMATIC,
                                    GTK_POLICY_AUTOMATIC);

    // create image
    image = gtk_image_new();
    g_object_set_data(G_OBJECT(window), "image", (gpointer) image);
    gtk_container_add (GTK_CONTAINER(scroll_window), image);
  }

  gtk_widget_show_all(window);
}

int main(int argc, char *argv[]){
//  GtkWidget *window;
  GtkApplication *app;

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
