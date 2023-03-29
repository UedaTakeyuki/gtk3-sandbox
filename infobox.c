#include <gtk/gtk.h>

GtkWidget* label;

static void
activate (GtkApplication *app,
          gpointer        user_data)
{
  GtkWidget *window;

  window = gtk_application_window_new (app);
  gtk_window_set_title (GTK_WINDOW (window), "Window");
  gtk_window_set_default_size (GTK_WINDOW (window), 600, 80);

  // create label
  label = gtk_label_new ("aho");
//  gtk_widget_set_size_request (label, 600, 80);
  gtk_container_add (GTK_CONTAINER (window), label);
  gtk_label_set_xalign (GTK_LABEL(label), 0.0);

  // set font size of label
  PangoAttrList* al = pango_attr_list_new ();
  PangoAttribute *attr = pango_attr_size_new_absolute(80 * PANGO_SCALE);
  pango_attr_list_insert(al, attr);
  gtk_label_set_attributes (GTK_LABEL(label), al);
  pango_attr_list_unref(al);


  gtk_widget_show_all (window);
}

gboolean my_callback(GIOChannel *source, GIOCondition condition, gpointer data){
  gchar *buf=NULL;
  GError *error = NULL;

  if(g_io_channel_read_line(source, &buf, NULL, NULL, NULL) != G_IO_STATUS_NORMAL)
  {
      fprintf(stderr, "read failed.\n");
      return FALSE;
  }
  g_print(buf);
  gtk_label_set_text (GTK_LABEL(label), buf); 
  g_free(buf);
  g_io_add_watch(source, G_IO_IN,(GIOFunc) my_callback, NULL);  // ここで設定し直さないと一度しか呼んでくれないっぽい。
/*
  switch (condition){
    case G_IO_IN:
      fprintf(stderr, "callback: condition G_IO_IN (%d)\n", condition);
      g_io_channel_read_line(source, &buf, NULL, NULL, &error);
      if (buf){
        g_print(buf);
        gtk_label_set_text (GTK_LABEL(label), buf); 
      }
      if (error){
        g_print("err: %d",error->code);
        g_print(error->message);
      }
  }*/
  return FALSE;
}

int
main (int    argc,
      char **argv)
{
  GtkApplication *app;
  int status;
  GIOChannel *channel;
  int fd;

  app = gtk_application_new ("org.gtk.example", G_APPLICATION_FLAGS_NONE);  
  g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
//  g_timeout_add (1000, change_label_string, NULL);

  // https://gist.github.com/bert/717238/379607561e216a7eca24e4bd7759b9c0dcb94096#file-io-c
//  fd = fileno(stdin); 
//  channel = g_io_channel_unix_new(fd);             // wrap stdout with GIOChannel
  channel = g_io_channel_new_file ("info", "r", NULL);
//  channel = g_io_channel_unix_new(fd);             // wrap stdout with GIOChannel
//  g_io_channel_set_encoding(channel, NULL, NULL);  // accept binary, allow no buffering
//  g_io_channel_set_buffered(channel, FALSE);       // don't buffer
  g_io_add_watch(channel, G_IO_IN,(GIOFunc) my_callback, NULL);

  status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);

  return status;
}

