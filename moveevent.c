// https://zetcode.com/gui/gtk2/gtkevents/
#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>

struct pointsize {
  int x;
  int y;
  int width;
  int height;
};

int readCurrentPositionSize(struct pointsize *currentPositionSize){
  FILE* fp;

  fp = fopen("moveevent.dat", "rb");
  if (fp == NULL) {
    return 0;
  }
  // read currentPositionSiz from file
  int flag = 0;
  flag = fread(currentPositionSize, sizeof(struct pointsize), 1, fp);
  if (flag) {
    printf("Contents of the structure written "
           "successfully");
  }
  // close file
  fclose(fp);

  return 1;

}

void saveCurrentPositionSize(struct pointsize *currentPositionSize){
  FILE* fp;

  fp = fopen("moveevent.dat", "wb");
  if (fp == NULL) {
      fprintf(stderr, "\nError opened file\n");
      exit(1);
  }
  // write currentPositionSiz to file
  int flag = 0;
  flag = fwrite(currentPositionSize, sizeof(struct pointsize), 1, fp);
  if (flag) {
    printf("Contents of the structure written "
           "successfully");
  }
  else
    printf("Error Writing to File!");

  // close file
  fclose(fp);
}

void configure_callback(GtkWindow *window, 
  GdkEvent *event, gpointer data) {
          
  int x, y, width, height;
  GString *buf;


  x = event->configure.x;
  y = event->configure.y;
  width = event->configure.width;
  height = event->configure.height;

  // save
  struct pointsize currentPointSize = { x, y, width, height };
  saveCurrentPositionSize(&currentPointSize);

  // show on the toolbar
  buf = g_string_new(NULL);   
  g_string_printf(buf, "%d, %d, %d, %d", x, y, width, height);

  // print on the console
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

  // move and set to previous position and size
  struct pointsize previsousPositionSize;
  if (readCurrentPositionSize(&previsousPositionSize)){
    gtk_window_move(GTK_WINDOW(window), previsousPositionSize.x, previsousPositionSize.y);
    gtk_window_resize(GTK_WINDOW(window), previsousPositionSize.width, previsousPositionSize.height);
  }
  
  gtk_main();

  return 0;
}
