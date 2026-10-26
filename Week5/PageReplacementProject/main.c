#include <gtk/gtk.h>
#include "page_replacement.h"

int main(int argc, char *argv[])
{
    gtk_init(&argc, &argv);

    start_gui();

    gtk_main();

    return 0;
}
