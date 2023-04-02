#include <gtk/gtk.h>

GtkWidget* label;

static void
activate (GtkApplication *app,
          gpointer        user_data)
{
  GtkWidget *window;

  window = gtk_application_window_new (app);
  gtk_window_set_title (GTK_WINDOW (window), "Window");
  gtk_window_set_default_size (GTK_WINDOW (window), 100, 20);

  // create label
  label = gtk_label_new ("aho");
  gtk_container_add (GTK_CONTAINER (window), label);

  gtk_widget_show_all (window);
}

gboolean my_callback(GIOChannel *source, GIOCondition condition, gpointer data){
  gchar *buf=NULL;
  GError *error = NULL;

  if (condition == G_IO_IN ){
    g_io_channel_read_line(source, &buf, NULL, NULL, NULL);
    gtk_label_set_text (GTK_LABEL(label), buf);
    g_print("buf: %s\n",buf);
    g_free(buf);
    GIOChannel *channel;
    channel = g_io_channel_new_file ("info", "r", NULL);
    g_io_add_watch(channel, G_IO_IN | G_IO_HUP,(GIOFunc) my_callback, NULL);
    return FALSE;
/*  } else if (condition == G_IO_HUP){
    GIOChannel *channel;
    channel = g_io_channel_new_file ("info", "r", NULL);
    g_io_add_watch(channel, G_IO_IN | G_IO_HUP,(GIOFunc) my_callback, NULL);
    return FALSE;*/
  }

  // for next watch
/*  {
    GIOChannel *channel;
    channel = g_io_channel_new_file ("info", "r", NULL);
    g_io_add_watch(channel, G_IO_IN | G_IO_HUP,(GIOFunc) my_callback, NULL);
  }*/

  //return FALSE;
  return TRUE;
}

int
main (int    argc,
      char **argv)
{
  GtkApplication *app;
  GIOChannel *channel;
  int status, fd;

  app = gtk_application_new ("org.gtk.example", G_APPLICATION_FLAGS_NONE);  
  g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);

  channel = g_io_channel_new_file ("info", "r", NULL);
  g_io_add_watch(channel, G_IO_IN | G_IO_HUP,(GIOFunc) my_callback, NULL);

  status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);

  return status;
}