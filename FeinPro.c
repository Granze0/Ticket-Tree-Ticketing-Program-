z#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <conio.h>
#include <stdbool.h>

#define BG_COLOR_ROOT "\x1b[48;5;34m"  // Background color for root node (green)
#define BG_COLOR_NODE "\x1b[48;5;255m" // Background color for other nodes (white)
#define RESET_COLOR "\x1b[0m"

FILE *file;

typedef struct
{
    char title[50];
    char genre[50];
    char rating[5];
    char director[35];
    char kelas[10];
    char id[50];
    int idnum;
    int stock;
    char key[10];
} kuda;

kuda temp;

typedef struct avl
{
    kuda data;
    struct avl *left;
    struct avl *right;
    int height;
} avl;

avl *oyot = NULL;
int lemper = 1;
int somehowunsigneddeclaration;

int height(avl *N)
{
    if (N == NULL)
    {
        return 0;
    }
    return N->height;
}

int max(int a, int b)
{
    if (a > b)
    {
        return a;
    }
    return b;
}

int getBalance(avl *N)
{
    if (N == NULL)
    {
        return 0;
    }
    return height(N->left) - height(N->right);
}

avl *rightRotate(avl *y)
{
    avl *x = y->left;
    avl *z = x->right;

    x->right = y;
    y->left = z;

    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    return x;
}

avl *newNode(kuda a)
{
    avl *node = (avl *)malloc(sizeof(avl));
    node->data = a;
    node->left = node->right = NULL;
    node->height = 1;
    return (node);
}

avl *leftRotate(avl *y)
{
    avl *x = y->right;
    avl *z = x->left;

    x->left = y;
    y->right = z;

    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    return x;
}

avl *insert_data(avl *node, kuda jawa)
{
    if (node == NULL)
    {
        return (newNode(jawa));
    }

    if (strcmp(node->data.key, jawa.key) > 0)
    {
        node->left = insert_data(node->left, jawa);
    }
    else if (strcmp(node->data.key, jawa.key) < 0)
    {
        node->right = insert_data(node->right, jawa);
    }
    else
    {
        return node;
    }

    node->height = 1 + max(height(node->left), height(node->right));
    int balance = getBalance(node);

    if (balance > 1 && strcmp(node->data.key, jawa.key) > 0)
        return rightRotate(node);

    if (balance < -1 && strcmp(node->data.key, jawa.key) < 0)
        return leftRotate(node);

    if (balance > 1 && strcmp(node->data.key, jawa.key) < 0)
    {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }
    if (balance < -1 && strcmp(node->data.key, jawa.key) > 0)
    {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

avl *minValueNode(avl *node)
{
    avl *current = node;
    while (current && current->left != NULL)
        current = current->left;
    return current;
}

void masukinkedatabes(avl *oyot, FILE *file) // Memasuukan data ke file
{
    if (oyot != NULL)
    {
        masukinkedatabes(oyot->left, file);
        fprintf(file, "%s#%s#%s#%s#%s#%s#%d#%d\n", oyot->data.title, oyot->data.genre, oyot->data.rating, oyot->data.director, oyot->data.kelas, oyot->data.id, oyot->data.idnum, oyot->data.stock);
        masukinkedatabes(oyot->right, file);
    }
}

avl *deleteNode(avl *root, char *key) // Menghapus data dari root
{
    if (root == NULL)
    {
        printf("Data Tidak Ditemukan\n");
        return root;
    }

    if (strcmp(key, root->data.key) < 0)
        root->left = deleteNode(root->left, key);

    else if (strcmp(key, root->data.key) > 0)
        root->right = deleteNode(root->right, key);

    else
    {
        if ((root->left == NULL) || (root->right == NULL))
        {
            avl *temp = root->left ? root->left : root->right;

            if (temp == NULL)
            {
                temp = root;
                root = NULL;
            }
            else
            {
                *root = *temp;
            }
            printf("\n Data Berhasil Dihapus\n");
            free(temp);
        }
        else
        {
            avl *temp = minValueNode(root->right);
            root->data = temp->data;
            root->right = deleteNode(root->right, temp->data.key);
        }
    }

    if (root == NULL)
        return root;

    root->height = 1 + max(height(root->left), height(root->right));
    int balance = getBalance(root);

    if (balance > 1 && getBalance(root->left) >= 0)
        return rightRotate(root);

    if (balance > 1 && getBalance(root->left) < 0)
    {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    if (balance < -1 && getBalance(root->right) <= 0)
        return leftRotate(root);

    if (balance < -1 && getBalance(root->right) > 0)
    {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}

avl *checkid(avl *root, char *kode) // Mengecek ID
{
    if (root == NULL)
    {
        return root;
    }
    if (strcmp(root->data.key, kode) == 0)
    {
        return root;
    }
    else if (strcmp(root->data.key, kode) > 0)
    {
        if (root->left == NULL)
        {
            return root;
        }
        else
        {
            return checkid(root->left, kode);
        }
    }
    else
    {
        if (root->right == NULL)
        {
            return root;
        }
        else
        {
            return checkid(root->right, kode);
        }
    }
}

void save_to_databes(avl *root) // Menyimpan data ke file
{
    file = fopen("databes.txt", "w");
    if (file == NULL)
    {
        printf("Error opening file!\n");
        return;
    }
    masukinkedatabes(root, file);
    fclose(file);
}

void update_stock(avl *root, char *kode, int newstok) // Mengupdate stock dari root
{
    if (root == NULL)
    {
        printf("Movie with ID '%s' not found.\n", kode);
        return;
    }

    if (strcmp(kode, root->data.key) < 0)
    {
        update_stock(root->left, kode, newstok);
    }
    else if (strcmp(kode, root->data.key) > 0)
    {
        update_stock(root->right, kode, newstok);
    }
    else
    {
        root->data.stock += newstok;
        printf("\nStock for movie '%s' updated successfully.\n", kode);
        printf("  Title     : %s\n", root->data.title);
        printf("  New Stock : %d\n", root->data.stock);
    }
}

void generate_key(char *inisial, int idnum) // Menggenerate key
{
    sprintf(temp.key, "%s-%05d", inisial, idnum);
}

// Fungsi untuk memperbarui stok berdasarkan ID
void Update()
{
    char title[10]; // Array untuk menyimpan ID yang diinput pengguna
    int stok;       // Variabel untuk menyimpan jumlah stok yang akan ditambahkan

    // Meminta pengguna untuk memasukkan ID data yang ingin diperbarui
    printf("Update\n ID          : ");
    scanf(" %[^\n]", title);

    // Meminta pengguna untuk memasukkan jumlah stok yang ingin ditambahkan
    printf(" Add Stock   : ");
    scanf(" %d", &stok);

    // Memperbarui stok berdasarkan ID
    update_stock(oyot, title, stok);

    // Menyimpan data terbaru ke dalam database
    save_to_databes(oyot);
}

// Fungsi untuk menghapus data berdasarkan ID
void Delete()
{
    avl *find;      // Pointer untuk menyimpan node yang ditemukan
    char ingfo[50]; // Array untuk menyimpan ID yang diinput pengguna

    // Meminta pengguna untuk memasukkan ID data yang ingin dihapus
    printf("Delete\n ID    : ");
    scanf(" %[^\n]", ingfo);

    // Mencari data berdasarkan ID yang diberikan
    find = checkid(oyot, ingfo);

    // Jika data tidak ditemukan atau ID tidak cocok
    if (find == NULL || strcmp(find->data.key, ingfo) != 0)
    {
        printf("Data dengan ID %s tidak ditemukan\n", ingfo);
        return; // Keluar dari fungsi
    }
    else
    {
        // Menampilkan data yang ditemukan
        printf("\nTitle    : %s\n", find->data.title);
        printf("Genre    : %s\n", find->data.genre);
        printf("ID       : %s\n", find->data.key);
        printf("Rating   : %s\n", find->data.rating);
        printf("Director : %s\n", find->data.director);
        printf("Class    : %s\n", find->data.kelas);
        printf("Stock    : %d\n", find->data.stock);
    }

    // Menghapus node dari AVL tree
    oyot = deleteNode(oyot, ingfo);

    // Menyimpan data terbaru ke dalam database
    save_to_databes(oyot);
}

void view_data(avl *root, int is_root)
{
    if (root != NULL)
    {
        view_data(root->left, 0); // Not root
        if (is_root)
        {
            printf(BG_COLOR_ROOT "|%-50s|%-30s|%-10s|%-7s|%-32s|%-7d|%-10s|" RESET_COLOR "\n", root->data.title, root->data.genre, root->data.key, root->data.rating, root->data.director, root->data.stock, root->data.kelas);
            printf("----------------------------------------------------------------------------------------------------------------------------------------------------------\n");
        }
        else
        {
            printf("|%-50s|%-30s|%-10s|%-7s|%-32s|%-7d|%-10s|\n", root->data.title, root->data.genre, root->data.key, root->data.rating, root->data.director, root->data.stock, root->data.kelas);
            printf("----------------------------------------------------------------------------------------------------------------------------------------------------------\n");
        }
        view_data(root->right, 0); // Not root
    }
}

void View()
{
    printf("View\n");
    printf("----------------------------------------------------------------------------------------------------------------------------------------------------------\n");
    printf(BG_COLOR_NODE "|%-50s|%-30s|%-10s|%-7s|%-32s|%-7s|%-10s|" RESET_COLOR "\n", "Title", "Genre", "ID", "Rating", "Director", "Stock", "Class");
    printf("----------------------------------------------------------------------------------------------------------------------------------------------------------\n");
    view_data(oyot, 1);
}

void capitalizeWords(char *str)
{
    int capitalizeNext = 1; // Penanda apakah karakter berikutnya harus diubah menjadi huruf kapital

    for (int i = 0; str[i] != '\0'; i++)
    {
        if (capitalizeNext && isalpha(str[i]))
        {
            str[i] = toupper(str[i]); // Ubah karakter menjadi huruf kapital
            capitalizeNext = 0;       // Setel penanda menjadi 0 karena sudah mengubah karakter pertama dalam kata
        }
        else
        {
            str[i] = tolower(str[i]); // Ubah karakter menjadi huruf kecil
        }

        if (str[i] == ' ')
        {
            str[i] = ',';       // Ganti spasi menjadi koma
            capitalizeNext = 1; // Setel penanda menjadi 1 untuk mengubah karakter berikutnya menjadi huruf kapital
        }
        else if (str[i] == ',')
        {
            capitalizeNext = 1; // Setel penanda menjadi 1 karena koma memisahkan kata
        }
    }
}

void capitalizeAfterSpace(char *str)
{
    int i = 0;
    str[0] = toupper(str[0]); // Ubah huruf pertama menjadi huruf kapital

    while (str[i] != '\0')
    {
        if (str[i] == ' ' && str[i + 1] != '\0')
        {
            str[i + 1] = toupper(str[i + 1]); // Ubah huruf setelah spasi menjadi huruf kapital
        }
        i++;
    }
}

// Fungsi untuk mendapatkan inisial dari nama dan menyimpannya dalam string 'initials'
void getInitials(char *name, char *initials)
{
    int j = 0, i;    // Inisialisasi variabel penghitung
    int isSpace = 1; // Variabel flag untuk menentukan apakah karakter sebelumnya adalah spasi

    // Loop melalui setiap karakter dalam string 'name'
    somehowunsigneddeclaration = strlen(name);
    for (i = 0; i < somehowunsigneddeclaration; i++)
    {
        // Jika karakter saat ini bukan spasi dan karakter sebelumnya adalah spasi
        if (!isspace(name[i]) && isSpace)
        {
            // Tambahkan karakter saat ini (dalam bentuk huruf besar) ke dalam string 'initials'
            initials[j++] = toupper(name[i]);
            isSpace = 0; // Set flag isSpace menjadi 0 karena karakter saat ini bukan spasi
        }
        else if (isspace(name[i]))
        {
            isSpace = 1; // Set flag isSpace menjadi 1 jika karakter saat ini adalah spasi
        }
    }
    initials[j] = '\0'; // Tambahkan null-terminator di akhir string 'initials'
}

void reduce_stock(avl *root, char *kode, int newstok)
{
    if (strcmp(kode, root->data.key) < 0)
    {
        reduce_stock(root->left, kode, newstok);
    }
    else if (strcmp(kode, root->data.key) > 0)
    {
        reduce_stock(root->right, kode, newstok);
    }
    else
    {
        root->data.stock -= newstok;
    }
    save_to_databes(oyot);
}

// Fungsi untuk membuat kunci berdasarkan inisial dan nomor ID
int create_key(char *inisial, int idnum)
{
    int sum = 0;
    somehowunsigneddeclaration = strlen(inisial);
    // Menghitung jumlah nilai ASCII dari setiap karakter dalam inisial
    for (int i = 0; i < somehowunsigneddeclaration; i++)
    {
        sum += inisial[i];
    }

    // Jika jumlah tersebut genap
    if (sum % 2 == 0)
    {
        // Kalikan jumlah dengan 3
        sum = sum * 3;
        // Kembalikan hasil pengurangan antara jumlah yang telah dikalikan dan nomor ID
        return sum - idnum;
    }
    // Jika jumlah tersebut ganjil
    else
    {
        // Kalikan jumlah dengan 2
        sum = sum * 2;
        // Kembalikan hasil penjumlahan antara jumlah yang telah dikalikan dan nomor ID
        return sum + idnum;
    }
}

int Insert()
{
    int temu, hasil_generate_key;
    char initials[10];

    // Input dan validasi judul
    do
    {
        printf("Insert\n Title    : ");
        scanf(" %[^\n]", temp.title);
    } while (strlen(temp.title) > 50 || strlen(temp.title) < 1);

    // Input dan validasi genre
    do
    {
        printf(" Genre    : ");
        scanf(" %[^\n]", temp.genre);

        // Mengganti spasi pertama dengan koma
        char *spacePos = strchr(temp.genre, ' ');
        if (spacePos != NULL)
        {
            *spacePos = ',';
        }
        // Mengkapitalisasi kata-kata di genre
        capitalizeWords(temp.genre);
    } while (strlen(temp.genre) > 50 || strlen(temp.genre) < 1);

    // Input dan validasi rating
    do
    {
        printf(" Rating   : ");
        scanf(" %[^\n]", temp.rating);
        temu = 0;
        somehowunsigneddeclaration = strlen(temp.rating);
        bool hasDecimal = false; // Flag untuk memeriksa jika ditemukan titik desimal
        for (int i = 0; i < somehowunsigneddeclaration; i++)
        {
            // Memeriksa apakah karakter bukan digit atau titik desimal
            if ((temp.rating[i] < '0' || temp.rating[i] > '9') && temp.rating[i] != '.')
            {
                temu = 1;
                break; // Keluar dari loop jika ditemukan karakter yang bukan digit (kecuali '.')
            }
            if (temp.rating[i] == '.')
            {
                hasDecimal = true; // Menandai jika ditemukan titik desimal
            }
        }
        // Jika tidak ditemukan titik desimal, set flag untuk input tidak valid
        if (!hasDecimal)
        {
            temu = 1;
        }
        if (temu == 1)
        {
            printf("Rating harus berupa angka\n");
        }
    } while (temu == 1);

    // Input dan validasi director
    do
    {
        printf(" Director : ");
        scanf(" %[^\n]", temp.director);
        // Mengkapitalisasi kata setelah spasi di direktur
        capitalizeAfterSpace(temp.director);
    } while (strlen(temp.director) > 35 || strlen(temp.director) < 1);

    // Mendapatkan inisial dari direktur
    getInitials(temp.director, initials);
    strcpy(temp.id, initials);

    // Input dan validasi kelas
    do
    {
        printf(" \n  G - Untuk semua penonton\n  PG - Dibawah 13 tahun tidak diperkenankan\n  R - Terbatas, dibawah 17 tahun tidak diperkenankan\n  A - Hanya untuk dewasa\n\n");
        printf(" Class    : ");
        scanf(" %[^\n]", temp.kelas);
        somehowunsigneddeclaration = strlen(temp.kelas);
        // Mengkapitalisasi semua karakter di kelas
        for (int i = 0; i < somehowunsigneddeclaration; i++)
        {
            temp.kelas[i] = toupper(temp.kelas[i]);
        }
    } while (strlen(temp.kelas) > 10 || strlen(temp.kelas) < 1 || (strcasecmp(temp.kelas, "G") != 0 && strcasecmp(temp.kelas, "PG") != 0 && strcasecmp(temp.kelas, "R") != 0 && strcasecmp(temp.kelas, "A") != 0));

    // Input dan validasi stok
    do
    {
        printf(" Stock    : ");
        temu = 0;
        temu = scanf("%d", &temp.stock);
        fflush(stdin);
    } while (temp.stock <= 0 || temu == 0);

    // Menghasilkan kunci unik untuk data baru
    hasil_generate_key = create_key(temp.id, lemper);
    generate_key(temp.id, hasil_generate_key);
    // Menyisipkan data ke pohon AVL
    oyot = insert_data(oyot, temp);
    temp.idnum = hasil_generate_key;
    lemper++;

    // Membuka file untuk menulis data baru
    file = fopen("databes.txt", "a");
    // Menulis data ke file dalam format tertentu
    fprintf(file, "%s#%s#%s#%s#%s#%s#%d#%d\n", temp.title, temp.genre, temp.rating, temp.director, temp.kelas, temp.id, temp.idnum, temp.stock);
    // Menutup file setelah selesai menulis
    fclose(file);
    printf("\nData Berhasil Dimasukkan\n");
}

// Fungsi untuk memeriksa apakah usia pengguna sesuai dengan batasan kelas film
int age_checker(avl *root, int age)
{
    // Memeriksa apakah kelas film adalah "G" (General audience)
    if (strcmp(root->data.kelas, "G") == 0)
    {
        // Jika usia kurang dari 0, kembalikan 0 (tidak valid)
        if (age < 0)
        {
            return 0;
        }
    }
    // Memeriksa apakah kelas film adalah "PG" (Parental Guidance)
    else if (strcmp(root->data.kelas, "PG") == 0)
    {
        // Jika usia kurang dari 13 tahun, tampilkan pesan dan kembalikan 0 (tidak diizinkan)
        if (age < 13)
        {
            printf("You are not allowed to watch this movie\n");
            return 0;
        }
    }
    // Memeriksa apakah kelas film adalah "R" (Restricted)
    else if (strcmp(root->data.kelas, "R") == 0)
    {
        // Jika usia kurang dari 17 tahun, tampilkan pesan dan kembalikan 0 (tidak diizinkan)
        if (age < 17)
        {
            printf("You are not allowed to watch this movie\n");
            return 0;
        }
    }
    // Memeriksa apakah kelas film adalah "A" (Adults only)
    else if (strcmp(root->data.kelas, "A") == 0)
    {
        // Jika usia kurang dari 18 tahun, tampilkan pesan dan kembalikan 0 (tidak diizinkan)
        if (age < 18)
        {
            printf("You are not allowed to watch this movie\n");
            return 0;
        }
    }
    else
    {
        // Jika kelas film tidak sesuai dengan salah satu di atas, kembalikan 0 (tidak valid)
        return 0;
    }
    // Jika semua pemeriksaan lulus, kembalikan 1 (diizinkan)
    return 1;
}

// Fungsi untuk memeriksa apakah sebuah string hanya berisi karakter numerik
bool is_numeric(const char *str)
{
    // Loop melalui setiap karakter dalam string
    while (*str)
    {
        // Memeriksa apakah karakter saat ini bukan digit
        if (!isdigit(*str))
        {
            return false; // Jika ditemukan karakter non-digit, kembalikan false
        }
        str++; // Pindah ke karakter berikutnya
    }
    return true; // Jika semua karakter adalah digit, kembalikan true
}

void user_beli()
{
    char kode[10], name_user[20], str_age[4];
    int jumlah, age, temu;

    // Input nama pengguna
    printf(" Name   : ");
    scanf(" %[^\n]", name_user);

    avl *temp1;

    // Input dan validasi usia
    do
    {
        printf(" Age    : ");
        scanf(" %9s", str_age);

        // Memeriksa apakah usia berupa angka
        if (!is_numeric(str_age))
        {
            printf("Invalid age. Please enter a numeric value.\n");
            continue;
        }

        // Mengonversi usia dari string ke integer
        age = atoi(str_age);
        break;
    } while (1 || age < 0);

    // Input dan validasi ID
    do
    {
        printf(" ID     : ");
        scanf(" %[^\n]", kode);

        // Memeriksa apakah ID valid
        temp1 = checkid(oyot, kode);
        if (temp1 == NULL || strcmp(temp1->data.key, kode) != 0)
        {
            printf("Invalid ID. Please try again.\n");
            continue;
        }

        // Memeriksa apakah usia memenuhi syarat untuk ID tersebut
        if (!age_checker(temp1, age))
        {
            continue;
        }

        break;
    } while (1);

    // Input dan validasi jumlah
    do
    {
        temu = 0;
        printf(" Jumlah : ");
        temu = scanf(" %d", &jumlah);
        fflush(stdin);
    } while (jumlah < 0 || temu == 0);

    // Mengurangi stok berdasarkan ID dan jumlah
    reduce_stock(oyot, kode, jumlah);
}

void print_table_header()
{
    printf("----------------------------------------------------------------------------------------------------------------------------------------------------------\n");
    printf("|%-50s|%-30s|%-10s|%-7s|%-32s|%-7s|%-10s|\n", "Title", "Genre", "ID", "Rating", "Director", "Stock", "Class");
    printf("----------------------------------------------------------------------------------------------------------------------------------------------------------\n");
}

bool search_by_title(avl *root, char *title)
{
    bool found = false;
    if (root != NULL)
    {
        search_by_title(root->left, title);
        if (strstr(root->data.title, title) != NULL)
        { // Fixed condition to strstr returning a non-null pointer
            printf("|%-50s|%-30s|%-10s|%-7s|%-32s|%-7d|%-10s|\n", root->data.title, root->data.genre, root->data.key, root->data.rating, root->data.director, root->data.stock, root->data.kelas);
            printf("----------------------------------------------------------------------------------------------------------------------------------------------------------\n");
        }
        search_by_title(root->right, title);
    }
    return found;
}

// Helper function to check if the target genre is in the genre list
bool is_genre_in_list(char *genre_list, char *target_genre)
{
    char *token;
    char genre_copy[100]; // Copy of genre_list to avoid modifying the original string

    // Copy the genre list to a temporary buffer
    strncpy(genre_copy, genre_list, sizeof(genre_copy));
    genre_copy[sizeof(genre_copy) - 1] = '\0'; // Ensure null-termination

    // Tokenize the string by commas
    token = strtok(genre_copy, ",");
    while (token != NULL)
    {
        // Trim whitespace from token
        while (*token == ' ')
            token++;
        char *end = token + strlen(token) - 1;
        while (end > token && *end == ' ')
            end--;
        *(end + 1) = '\0';

        // Compare the tokenized genre with the target genre
        if (strcasecmp(token, target_genre) == 0)
        {
            return true;
        }
        token = strtok(NULL, ",");
    }
    return false;
}

bool search_by_genre(avl *root, char *genre)
{
    bool found = false;

    if (root != NULL)
    {
        found |= search_by_genre(root->left, genre);

        if (is_genre_in_list(root->data.genre, genre))
        {
            printf("|%-50s|%-30s|%-10s|%-7s|%-32s|%-7d|%-10s|\n", root->data.title, root->data.genre, root->data.key, root->data.rating, root->data.director, root->data.stock, root->data.kelas);
            printf("----------------------------------------------------------------------------------------------------------------------------------------------------------\n");
            found = true;
        }

        found |= search_by_genre(root->right, genre);
    }

    return found;
}

bool search_by_director(avl *root, char *director)
{
    bool found = false;
    if (root != NULL)
    {
        search_by_director(root->left, director);
        if (strcasecmp(root->data.director, director) == 0)
        {
            printf("|%-50s|%-30s|%-10s|%-7s|%-32s|%-7d|%-10s|\n", root->data.title, root->data.genre, root->data.key, root->data.rating, root->data.director, root->data.stock, root->data.kelas);
            printf("----------------------------------------------------------------------------------------------------------------------------------------------------------\n");
        }
        search_by_director(root->right, director);
    }
    return found;
}

bool search_by_class(avl *root, char *kelas)
{
    bool found = false;
    if (root != NULL)
    {
        search_by_class(root->left, kelas);
        if (strcasecmp(root->data.kelas, kelas) == 0)
        {
            printf("|%-50s|%-30s|%-10s|%-7s|%-32s|%-7d|%-10s|\n", root->data.title, root->data.genre, root->data.key, root->data.rating, root->data.director, root->data.stock, root->data.kelas);
            printf("----------------------------------------------------------------------------------------------------------------------------------------------------------\n");
        }
        search_by_class(root->right, kelas);
    }
    return found;
}

// Fungsi untuk menghitung jumlah node dalam pohon AVL
int countNodes(avl *root)
{
    if (!root) // Jika root kosong (NULL), mengembalikan 0
        return 0;

    // Mengembalikan jumlah node, yaitu 1 (untuk node saat ini) ditambah jumlah node pada subtree kiri dan kanan
    return 1 + countNodes(root->left) + countNodes(root->right);
}

// Fungsi untuk mengumpulkan node-node dalam sebuah array selama traversal in-order
void collectNodes(avl *root, kuda **nodes, int *index)
{
    if (root)
    {
        collectNodes(root->left, nodes, index);
        nodes[(*index)++] = &root->data;
        collectNodes(root->right, nodes, index);
    }
}

// Fungsi untuk membandingkan rating untuk pengurutan qsort
int compareRatings(const void *a, const void *b)
{
    kuda *dataA = *(kuda **)a;                   // Cast a ke pointer kuda
    kuda *dataB = *(kuda **)b;                   // Cast b ke pointer kuda
    return strcmp(dataB->rating, dataA->rating); // Membandingkan rating secara menurun
}

// Fungsi untuk mencetak node berdasarkan rating secara menurun
void printNodesByRating(avl *root)
{
    if (!root)
        return;

    int totalNodes = countNodes(root);                           // Menghitung total node dalam pohon AVL
    kuda **nodes = (kuda **)malloc(totalNodes * sizeof(kuda *)); // Mengalokasikan memori untuk array nodes
    int index = 0;

    collectNodes(root, nodes, &index); // Mengumpulkan node dalam array nodes

    qsort(nodes, totalNodes, sizeof(kuda *), compareRatings); // Mengurutkan nodes berdasarkan rating menggunakan qsort

    printf("----------------------------------------------------------------------------------------------------------------------------------------------------------\n");
    for (int i = 0; i < totalNodes; i++)
    {
        // Mencetak data setiap node dalam baris tabel yang diformat
        printf("|%-50s|%-30s|%-10s|%-7s|%-32s|%-7d|%-10s|\n", nodes[i]->title, nodes[i]->genre, nodes[i]->key, nodes[i]->rating, nodes[i]->director, nodes[i]->stock, nodes[i]->kelas);
        printf("----------------------------------------------------------------------------------------------------------------------------------------------------------\n");
    }
    free(nodes); // Membebaskan memori yang dialokasikan secara dinamis
}

void search_by()
{
    char choise[20];
    char cari[20];
    do
    {
        system("cls");
        printf("===================\n");
        printf("==== Search by ====\n");
        printf("===================\n");
        printf("1. Title\n");
        printf("2. Genre\n");
        printf("3. Rating\n");
        printf("4. Director\n");
        printf("5. Class\n");
        printf("6. Back\n");
        printf(">>> ");
        scanf(" %[^\n]", choise);
        if (strcasecmp(choise, "1") == 0 || strcasecmp(choise, "Title") == 0)
        {
            printf("Title : ");
            scanf(" %[^\n]", cari);
            print_table_header();
            if (!search_by_title(oyot, cari))
            {
                printf("Movie with title '%s' not found.\n", cari);
            }
        }
        else if (strcasecmp(choise, "2") == 0 || strcasecmp(choise, "Genre") == 0)
        {
            printf("Genre : ");
            scanf(" %[^\n]", cari);
            print_table_header();
            if (!search_by_genre(oyot, cari))
            {
                printf("Movie with genre '%s' not found.\n", cari);
            }
        }
        else if (strcasecmp(choise, "3") == 0 || strcasecmp(choise, "Rating") == 0)
        {
            printf("Sort by Rating\n");
            print_table_header();
            printNodesByRating(oyot);
        }
        else if (strcasecmp(choise, "4") == 0 || strcasecmp(choise, "Director") == 0)
        {
            printf("Director : ");
            scanf(" %[^\n]", cari);
            print_table_header();
            if (!search_by_director(oyot, cari))
            {
                printf("Movie with director '%s' not found.\n", cari);
            }
        }
        else if (strcasecmp(choise, "5") == 0 || strcasecmp(choise, "Class") == 0)
        {
            printf("G - For all audiences\n");
            printf("PG - Under 13 not permitted\n");
            printf("R - Restricted, under 17 not permitted\n");
            printf("A - Adults only\n");
            printf("Class : ");
            scanf(" %[^\n]", cari);
            print_table_header();
            if (!search_by_class(oyot, cari))
            {
                printf("Movie with class '%s' not found.\n", cari);
            }
        }
        else if (strcasecmp(choise, "6") == 0 || strcasecmp(choise, "Back") == 0)
        {
            printf("\n");
        }
        else
        {
            printf("Menu Tidak Ada\n");
        }
        return;
    } while (strcasecmp(choise, "6") != 0 && strcasecmp(choise, "Back") != 0);
}

void clear_Tree(avl *root)
{
    if (root != NULL)
    {
        clear_Tree(root->left);
        clear_Tree(root->right);
        free(root);
    }
}

void ngadmin()
{
    char choise[30];

    do
    {
        system("cls");
        printf("====================\n");
        printf("==== Admin Menu ====\n");
        printf("====================\n");
        printf("1. Insert\n");
        printf("2. View\n");
        printf("3. Update\n");
        printf("4. Delete\n");
        printf("5. Search\n");
        printf("6. Back\n");
        printf("7. Exit\n");
        printf(">>> ");
        scanf(" %[^\n]", choise);

        if (strcasecmp(choise, "1") == 0 || strcasecmp(choise, "Insert") == 0)
        {
            Insert();
            printf("\nPress Enter Key to Continue . . .");
            getchar();
        }
        else if (strcasecmp(choise, "2") == 0 || strcasecmp(choise, "View") == 0)
        {
            View();
            printf("\nPress Enter Key to Continue . . .");
            getchar();
            while (getchar() != '\n')
                ;
        }
        else if (strcasecmp(choise, "3") == 0 || strcasecmp(choise, "Update") == 0)
        {
            Update();
            printf("\nPress Enter Key to Continue . . .");
            getchar();
            while (getchar() != '\n')
                ;
        }
        else if (strcasecmp(choise, "4") == 0 || strcasecmp(choise, "Delete") == 0)
        {
            Delete();
            printf("\nPress Enter Key to Continue . . .");
            getchar();
            while (getchar() != '\n')
                ;
        }
        else if (strcasecmp(choise, "5") == 0 || strcasecmp(choise, "search") == 0)
        {
            search_by();
            printf("\nPress Enter Key to Continue . . .");
            getchar();
            while (getchar() != '\n')
                ;
        }
        else if (strcasecmp(choise, "6") == 0 || strcasecmp(choise, "Back") == 0)
        {
            printf("\nBack to Main Menu\n");
        }
        else if (strcasecmp(choise, "7") == 0 || strcasecmp(choise, "Exit") == 0)
        {
            exit(0);
        }

        else
        {
            printf("Menu Tidak Ada\n");
        }
    } while (strcasecmp(choise, "6") != 0 && strcasecmp(choise, "back") != 0);

    clear_Tree(oyot);
    oyot = NULL;
}

void nguser()
{
    char choise[20];
    do
    {
        system("cls");
        printf("=====================\n");
        printf("===== User Menu =====\n");
        printf("=====================\n");
        printf("1. Beli\n");
        printf("2. View\n");
        printf("3. Search\n");
        printf("4. Back\n");
        printf("5. Exit\n");
        printf(">>> ");
        scanf(" %[^\n]", choise);
        if (strcasecmp(choise, "1") == 0 || strcasecmp(choise, "Beli") == 0)
        {
            View();
            user_beli();
            printf("\nPress Enter Key to Continue . . .");
            getchar();
            while (getchar() != '\n')
                ;
        }
        else if (strcasecmp(choise, "2") == 0 || strcasecmp(choise, "View") == 0)
        {
            View();
            printf("\nPress Enter Key to Continue . . .");
            getchar();
            while (getchar() != '\n')
                ;
        }
        else if (strcasecmp(choise, "3") == 0 || strcasecmp(choise, "search") == 0)
        {
            search_by();
            printf("\nPress Enter Key to Continue . . .");
            getchar();
            while (getchar() != '\n')
                ;
        }
        else if (strcasecmp(choise, "4") == 0 || strcasecmp(choise, "Back") == 0)
        {
            printf(" Terimakasih telah menggunakan layanan kami\n");
        }
        else if (strcasecmp(choise, "5") == 0 || strcasecmp(choise, "Exit") == 0)
        {
            exit(0);
            printf("Terimakasih telah menggunakan program kami\n");
        }
        else
        {
            printf("Menu Tidak Ada\n");
        }
    } while (strcasecmp(choise, "4") != 0 && strcasecmp(choise, "back") != 0);

    clear_Tree(oyot);
    oyot = NULL;
}

int main()
{
    char choise[20];
    char username[20], password[20];
    file = fopen("databes.txt", "r");
    if (file == NULL)
    {
        printf("File Tidak Ditemukan\n");
    }
    else
    {
        while (fscanf(file, " %[^#]#%[^#]#%[^#]#%[^#]#%[^#]#%[^#]#%d#%d\n", temp.title, temp.genre, temp.rating, temp.director, temp.kelas, temp.id, &temp.idnum, &temp.stock) != EOF)
        {
            generate_key(temp.id, temp.idnum);
            oyot = insert_data(oyot, temp);
            lemper++;
        }
        fclose(file);
    }
    do
    {
        system("cls");
        printf("======================\n");
        printf("=== Selamat Datang ===\n");
        printf("======================\n");
        printf("1. Admin\n");
        printf("2. User\n");
        printf("3. Exit\n");
        printf(">>> ");
        scanf(" %[^\n]", choise);
        if (strcasecmp(choise, "1") == 0 || strcasecmp(choise, "Admin") == 0)
        {
            printf("Username : ");
            scanf(" %[^\n]", username);
            printf("Password : ");
            scanf(" %[^\n]", password);
            if (strcasecmp(username, "ngadmin") == 0 && strcasecmp(password, "ngadmin") == 0)
            {
                printf("\nLogin Berhasil\n");
                ngadmin();
            }
            else
            {
                printf("\nLogin Gagal\n");
            }
            printf("\nPress Enter Key to Continue . . .");
            getchar();
            while (getchar() != '\n')
                ;
        }
        else if (strcasecmp(choise, "2") == 0 || strcasecmp(choise, "User") == 0)
        {
            nguser();
            printf("\nPress Enter Key to Continue . . .");
            getchar();
            while (getchar() != '\n')
                ;
        }
        else if (strcasecmp(choise, "3") == 0 || strcasecmp(choise, "Exit") == 0)
        {
            printf("\nTerimakasih telah menggunakan program kami\n\n");
        }
        else
        {
            printf("Menu Tidak Ada\n");
        }
    } while (strcasecmp(choise, "3") != 0 && strcasecmp(choise, "Exit") != 0);

    clear_Tree(oyot);
    oyot = NULL;
}