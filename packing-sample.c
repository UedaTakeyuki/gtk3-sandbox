#include <gtk/gtk.h>

int main(int argc, char *argv[]){
  GtkWidget *window, *vbox, *hbox, *button;

  gtk_init (&argc, &argv);

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window), "Packing Sample");
  gtk_container_set_border_width(GTK_CONTAINER(window), 10);
  gtk_widget_set_size_request(window, 400, 150);
  g_signal_connect (G_OBJECT(window), "destroy",
                    G_CALLBACK(gtk_main_quit), NULL);

  vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
  gtk_container_add (GTK_CONTAINER(window), vbox);

  hbox = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
  gtk_box_pack_start (GTK_BOX(vbox), hbox, TRUE, TRUE, 0);
  button = gtk_button_new_with_label("expand:TRUE");
  gtk_box_pack_start (GTK_BOX(hbox), button, TRUE, TRUE, 0);
  button = gtk_button_new_with_label("fill:TRUE");
  gtk_box_pack_start (GTK_BOX(hbox), button, TRUE, TRUE, 0);

  hbox = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
  gtk_box_pack_start (GTK_BOX(vbox), hbox, TRUE, TRUE, 0);
  button = gtk_button_new_with_label("expand:TRUE");
  gtk_box_pack_start (GTK_BOX(hbox), button, TRUE, FALSE, 0);
  button = gtk_button_new_with_label("fill:FALSE");
  gtk_box_pack_start (GTK_BOX(hbox), button, TRUE, FALSE, 0);

  hbox = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
  gtk_box_pack_start (GTK_BOX(vbox), hbox, TRUE, TRUE, 0);
  button = gtk_button_new_with_label("expand:FALSE");
  gtk_box_pack_start (GTK_BOX(hbox), button, FALSE, TRUE, 0);
  button = gtk_button_new_with_label("fill:TRUE");
  gtk_box_pack_start (GTK_BOX(hbox), button, FALSE, TRUE, 0);

  gtk_widget_show_all(window);
  gtk_main();
  return 0;
}