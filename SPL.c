#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>

#define FILE_NAME "users.txt"
#define TRANSACTION_FILE "transaction_history.txt"
#define KEY_SIZE 128
#define EXPONENT 65537

typedef struct
{
    char username[50];
    char password_hash[64];
    float balance;
} User;

User users[10];
int user_count = 0;

long long mod_exp(long long base, long long exponent, long long modulus)
{
    long long result = 1;
    while (exponent > 0)
    {
        if (exponent % 2 == 1)
        {
            result = (result * base) % modulus;
        }
        base = (base * base) % modulus;
        exponent = exponent / 2;
    }
    return result;
}
void sha256_hash(const char *input, char output[65])
{

    for (int i = 0; i < 5; i++)
    {
        sprintf(output + (i * 2), "%02x", 0);
    }
    output[64] = '\0';
}


void generate_rsa_keys(long long *n, long long *e, long long *d)
{
    long long p = 61, q = 53;
    *n = p * q;
    long long phi = (p - 1) * (q - 1);
    *e = EXPONENT;
    *d = mod_exp(*e, phi - 1, phi);
}

long long rsa_encrypt(long long message, long long e, long long n)
{
    return mod_exp(message, e, n);
}

long long rsa_decrypt(long long encrypted_message, long long d, long long n)
{
    return mod_exp(encrypted_message, d, n);
}

void save_users_to_file()
{
    FILE *file = fopen(FILE_NAME, "w");
    if (!file)
    {
        printf("Error opening file!\n");
        return;
    }
    for (int i = 0; i < user_count; i++)
    {
        fprintf(file, "%s %s %.2f\n", users[i].username, users[i].password_hash, users[i].balance);
    }
    fclose(file);
}

void load_users_from_file()
{
    FILE *file = fopen(FILE_NAME, "r");
    if (!file)
        return;
    while (fscanf(file, "%49s %63s %f", users[user_count].username, users[user_count].password_hash, &users[user_count].balance) == 3)
    {
        user_count++;
    }
    fclose(file);
}

void input_username(char *username)
{
    printf("Enter a username: ");
    scanf("%49s", username);
}

void input_password(char *password)
{
    printf("Enter a password: ");
    scanf("%49s", password);
}

void input_initial_deposit(float *initial_deposit)
{
    printf("Enter initial deposit: ");
    scanf("%f", initial_deposit);
}

int is_username_exists(const char *username)
{
    for (int i = 0; i < user_count; i++)
    {
        if (strcmp(users[i].username, username) == 0)
        {
            return 1;
        }
    }
    return 0;
}

void create_account()
{
    char username[50], password[50];
    float initial_deposit;

    input_username(username);

    if (is_username_exists(username))
    {
        printf("Username already exists. Try again.\n");
        return;
    }

    input_password(password);

    long long n, e, d;
    generate_rsa_keys(&n, &e, &d);

    // Encrypt password
    long long encrypted_password = rsa_encrypt((long long)password[0], e, n); // For simplicity, using first character
    char encrypted_password_str[64];
    sprintf(encrypted_password_str, "%lld", encrypted_password);

    input_initial_deposit(&initial_deposit);

    strcpy(users[user_count].username, username);
    strcpy(users[user_count].password_hash, encrypted_password_str);
    users[user_count].balance = initial_deposit;
    user_count++;

    save_users_to_file();
    printf("Account created successfully!\n");
}

void log_transaction(int user_index, const char *transaction_type, float amount)
{
    FILE *file = fopen(TRANSACTION_FILE, "a");
    if (!file)
    {
        printf("Error opening transaction history file!\n");
        return;
    }

    // Get current date/time
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    char time_str[26];
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", tm_info);

    fprintf(file, "%s - User: %s, Transaction: %s, Amount: %.2f, Balance: %.2f\n", time_str, users[user_index].username, transaction_type, amount, users[user_index].balance);
    fclose(file);
}

void display_menu()
{
    printf("\n1. Check Balance\n");
    printf("2. Deposit\n");
    printf("3. Withdraw\n");
    printf("4. Logout\n");
}

void perform_balance_check(int user_index)
{
    printf("Your current balance is: %.2f\n", users[user_index].balance);
}

void perform_deposit(int user_index)
{
    float amount;
    printf("Enter amount to deposit: ");
    scanf("%f", &amount);
    users[user_index].balance += amount;
    save_users_to_file();
    log_transaction(user_index, "Deposit", amount);
    printf("Deposit successful!\n");
}

void perform_withdrawal(int user_index)
{
    float amount;
    printf("Enter amount to withdraw: ");
    scanf("%f", &amount);
    if (amount > users[user_index].balance)
    {
        printf("Insufficient balance!\n");
    }
    else
    {
        users[user_index].balance -= amount;
        save_users_to_file();
        log_transaction(user_index, "Withdrawal", amount);
        printf("Withdrawal successful!\n");
    }
}

void atm_operations(int user_index)
{
    int choice;
    while (1)
    {
        display_menu();
        printf("Enter your choice: ");
        scanf("%d", &choice);
        switch (choice)
        {
        case 1:
            perform_balance_check(user_index);
            break;
        case 2:
            perform_deposit(user_index);
            break;
        case 3:
            perform_withdrawal(user_index);
            break;
        case 4:
            printf("Logging out...\n");
            return;
        default:
            printf("Invalid choice. Try again.\n");
        }
    }
}

void input_login_details(char *username, char *password)
{
    printf("Enter your username: ");
    scanf("%49s", username);

    printf("Enter your password: ");
    scanf("%49s", password);
}

void login()
{
    char username[50], password[50];

    input_login_details(username, password);

    long long n, e, d;
    generate_rsa_keys(&n, &e, &d);

    for (int i = 0; i < user_count; i++)
    {
        if (strcmp(users[i].username, username) == 0)
        {
            long long encrypted_password = atoll(users[i].password_hash);
            long long decrypted_password = rsa_decrypt(encrypted_password, d, n);
            if ((char)decrypted_password == password[0])
            { // For simplicity, comparing first character
                printf("Welcome %s!\n", username);
                atm_operations(i);
                return;
            }
            else
            {
                printf("Invalid password!\n");
                return;
            }
        }
    }
    printf("User not found. Please create an account first.\n");
}

// SHA-256 Constants
static const uint32_t k[64] = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2};

// Rotate Right Macro
#define ROTR(x, n) ((x >> n) | (x << (32 - n)))

// SHA-256 Functions
#define CH(x, y, z) ((x & y) ^ (~x & z))
#define MAJ(x, y, z) ((x & y) ^ (x & z) ^ (y & z))
#define EP0(x) (ROTR(x, 2) ^ ROTR(x, 13) ^ ROTR(x, 22))
#define EP1(x) (ROTR(x, 6) ^ ROTR(x, 11) ^ ROTR(x, 25))
#define SIG0(x) (ROTR(x, 7) ^ ROTR(x, 18) ^ (x >> 3))
#define SIG1(x) (ROTR(x, 17) ^ ROTR(x, 19) ^ (x >> 10))

void sha256(const char *input, char output[65])
{
    uint32_t h[8] = {0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a, 0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19};

    uint8_t data[64] = {0};
    size_t input_len = strlen(input);
    size_t bit_len = input_len * 8;

    memcpy(data, input, input_len);
    data[input_len] = 0x80;

    if (input_len < 56)
    {
        data[63] = bit_len & 0xFF;
        data[62] = (bit_len >> 8) & 0xFF;
        data[61] = (bit_len >> 16) & 0xFF;
        data[60] = (bit_len >> 24) & 0xFF;
    }

    uint32_t w[64];
    for (int i = 0; i < 16; i++)
    {
        w[i] = (data[i * 4] << 24) | (data[i * 4 + 1] << 16) | (data[i * 4 + 2] << 8) | (data[i * 4 + 3]);
    }

    for (int i = 16; i < 64; i++)
    {
        w[i] = SIG1(w[i - 2]) + w[i - 7] + SIG0(w[i - 15]) + w[i - 16];
    }

    uint32_t a, b, c, d, e, f, g, h_val;
    a = h[0];
    b = h[1];
    c = h[2];
    d = h[3];
    e = h[4];
    f = h[5];
    g = h[6];
    h_val = h[7];

    for (int i = 0; i < 64; i++)
    {
        uint32_t temp1 = h_val + EP1(e) + CH(e, f, g) + k[i] + w[i];
        uint32_t temp2 = EP0(a) + MAJ(a, b, c);
        h_val = g;
        g = f;
        f = e;
        e = d + temp1;
        d = c;
        c = b;
        b = a;
        a = temp1 + temp2;
    }

    h[0] += a;
    h[1] += b;
    h[2] += c;
    h[3] += d;
    h[4] += e;
    h[5] += f;
    h[6] += g;
    h[7] += h_val;

    for (int i = 0; i < 8; i++)
    {
        sprintf(output + (i * 8), "%08x", h[i]);
    }
    output[64] = '\0';
}

void display_main_menu()
{
    printf("\n--- ATM System ---\n");

    printf("1. Create Account\n");
    printf("2. Login\n");
    printf("3. Exit\n");
}

void main_menu()
{
    load_users_from_file();
    int option;
    while (1)
    {
        display_main_menu();
        printf("Enter your choice: ");
        scanf("%d", &option);
        switch (option)
        {
        case 1:
            create_account();
            break;
        case 2:
            login();
            break;
        case 3:
            printf("Exiting... Thank you for using the ATM system!\n");
            return;
        default:
            printf("Invalid choice. Try again.\n");
        }
    }
}

int main()
{
    main_menu();
    return 0;
}