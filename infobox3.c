#include <gtk/gtk.h>
#include <stdio.h>     // for fprintf()
#include <unistd.h>    // for close()
#include <sys/epoll.h> // for epoll_create1()
#include <string.h>    // for strncmp
#include <stdlib.h>
#include <fcntl.h>
#define MAX_EVENTS 5
#define READ_SIZE 10

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

void* thread_function(void *data){
  int epoll_fd = epoll_create1(0);
  if (epoll_fd == -1) {
		fprintf(stderr, "Failed to create epoll file descriptor\n");
	}

	size_t bytes_read;
	char read_buffer[READ_SIZE + 1];
  int running = 1, event_count, i;

  int fd = open("info", O_RDONLY);
  struct epoll_event event, events[MAX_EVENTS];;
  event.events = EPOLLIN;
	event.data.fd = fd;

  if(epoll_ctl(epoll_fd, EPOLL_CTL_ADD, 0, &event)){
		fprintf(stderr, "Failed to add file descriptor to epoll\n");
		close(epoll_fd);
	}

  while(running){
    printf("\nPolling for input...\n");
		event_count = epoll_wait(epoll_fd, events, MAX_EVENTS, 30000);
		printf("%d ready events\n", event_count);
		for (int i = 0; i < event_count; i++) {
			printf("Reading file descriptor '%d' -- ", events[i].data.fd);
			bytes_read = read(events[i].data.fd, read_buffer, READ_SIZE);
			printf("%zd bytes read.\n", bytes_read);
			read_buffer[bytes_read] = '\0';
			printf("Read '%s'\n", read_buffer);
		
			if(!strncmp(read_buffer, "stop\n", 5))
			running = 0;
		}
  }
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

//  channel = g_io_channel_new_file ("info", "r", NULL);
//  g_io_add_watch(channel, G_IO_IN | G_IO_HUP,(GIOFunc) my_callback, NULL);
  GThread *thread = g_thread_new(NULL, thread_function, NULL);
  
  status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);

  return status;
}