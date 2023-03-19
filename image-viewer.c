// http://iim.cs.tut.ac.jp/member/sugaya/GTK+/files/gtkbook-20210127.pdf
#include <gtk/gtk.h>
#include <stdlib.h>
static void cb_button_clicked (GtkWidget *button, gpointer user_data){
//  gtk_main_quit();
  GApplication *app = G_APPLICATION(user_data);
  g_application_quit(app);
}

static void cb_activate (GApplication *app, gpointer user_data){
  GtkWidget *window;

  window = gtk_application_window_new(GTK_APPLICATION(app));
  gtk_widget_set_size_request(window, 300, 200);
  {
    GtkWidget *vbox;
    GtkWidget *image;
    GtkWidget *button;
    GtkWidget *scroll_window;

    // create box
    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    // create scrolled widow
    scroll_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_box_pack_start(GTK_BOX(vbox), scroll_window, TRUE, TRUE, 0);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW(scroll_window), 
                                    GTK_POLICY_AUTOMATIC,
                                    GTK_POLICY_AUTOMATIC);

    // create image
    image = gtk_image_new_from_file((char *) user_data);
    gtk_container_add (GTK_CONTAINER(scroll_window), image);

    // create button
    button = gtk_button_new_with_label("Quit");
    gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, FALSE, 0);
    g_signal_connect(button, "clicked", 
                    G_CALLBACK (cb_button_clicked), NULL);
  }
  gtk_widget_show_all(window);
}

int main(int argc, char *argv[]){
//  GtkWidget *window;
  GtkApplication *app;

  if (argc != 2){
    g_print("Usage: %s image-file\n", argv[0]);
    exit(1);
  }

/*
  gtk_init(&argc, &argv);
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_widget_set_size_request(window, 300, 200);
  {
    GtkWidget *vbox;
    GtkWidget *image;
    GtkWidget *button;
    GtkWidget *scroll_window;

    // create box
    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    // create scrolled widow
    scroll_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_box_pack_start(GTK_BOX(vbox), scroll_window, TRUE, TRUE, 0);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW(scroll_window), 
                                    GTK_POLICY_AUTOMATIC,
                                    GTK_POLICY_AUTOMATIC);

    // create image
    image = gtk_image_new_from_file(argv[1]);
    gtk_container_add (GTK_CONTAINER(scroll_window), image);

    // create button
    button = gtk_button_new_with_label("Quit");
    gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, FALSE, 0);
    g_signal_connect(button, "clicked", 
                    G_CALLBACK (cb_button_clicked), NULL);
  }
  gtk_widget_show_all(window);
  gtk_main();
*/
  app = gtk_application_new("gtk.imageviewer", 0);
  g_signal_connect(app, "activate", G_CALLBACK(cb_activate), argv[1]);
  g_application_run(G_APPLICATION(app), 0, NULL);

  return 0;
}
