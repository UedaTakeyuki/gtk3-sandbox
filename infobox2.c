#include <stdio.h>
#include <gtk/gtk.h>


GtkBox* makeWindow()
{
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(window), box);


    gtk_widget_show_all(window);


    return GTK_BOX(box);
}


void addContent(GtkBox *parent, const char* string)
{
    GtkWidget *label = gtk_label_new(string);
    gtk_box_pack_start(parent, label, FALSE, FALSE, 0);
    gtk_widget_show(label);
}


gboolean onReadable(GIOChannel *source, GIOCondition condition, gpointer data)
{
    gchar *buf;


    if(g_io_channel_read_line(source, &buf, NULL, NULL, NULL) != G_IO_STATUS_NORMAL)
    {
        fprintf(stderr, "read failed.\n");
        return FALSE;
    }


    addContent((GtkBox *)data, buf);


    g_free(buf);


    g_io_add_watch(source, G_IO_IN, onReadable, data);  // ここで設定し直さないと一度しか呼んでくれないっぽい。


    return FALSE;
}


void main()
{
    gtk_init(NULL, NULL);


    GtkBox *box = makeWindow();


    GIOChannel *channel = g_io_channel_unix_new(0);  // fdが0。つまりstdin。
    g_io_add_watch(channel, G_IO_IN, onReadable, box);


    gtk_main();
}