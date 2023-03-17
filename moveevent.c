#include <gtk/gtk.h>

void configure_callback(GtkWindow *window, 
      GdkEvent *event, gpointer data) {
          
   int x, y, width, height;
   GString *buf;
   
   x = event->configure.x;
   y = event->configure.y;
   width = event->configure.width;
   height = event->configure.height;
   
   buf = g_string_new(NULL);   
   g_string_printf(buf, "%d, %d, %d, %d", x, y, width, height);
   
   gtk_window_set_title(window, buf->str);
   g_print ("%s\n", buf->str);

   g_string_free(buf, TRUE);
}

int main(int argc, char *argv[]) {
    
  GtkWidget *window;

  gtk_init(&argc, &argv);

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  gtk_window_set_default_size(GTK_WINDOW(window), 300, 200);
  gtk_widget_add_events(GTK_WIDGET(window), GDK_CONFIGURE);

  g_signal_connect(G_OBJECT(window), "destroy",
        G_CALLBACK(gtk_main_quit), G_OBJECT(window));

  g_signal_connect(G_OBJECT(window), "configure-event",
        G_CALLBACK(configure_callback), NULL);

  gtk_widget_show(window);
  gtk_main();

  return 0;
}
