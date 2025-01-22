#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct
{
    char username[50];
    char password[64]; // 256-bit hash
    float balance;
} User;

User users[10];
int user_count = 0;

// Simple implementation of SHA-256 hash function (not using external libraries)
void sha256(const char *str, char *output)
{
    unsigned int h0 = 0x6a09e667;
    unsigned int h1 = 0xbb67ae85;
    unsigned int h2 = 0x3c6ef372;
    unsigned int h3 = 0xa54ff53a;
    unsigned int h4 = 0x510e527f;
    unsigned int h5 = 0x9b05688c;
    unsigned int h6 = 0x1f83d9ab;
    unsigned int h7 = 0x5be0cd19;

    unsigned int K[] = {
        0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
        0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
        0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
        0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
        0x8f1bbcdc, 0x5be0cd19, 0x4274d4f5, 0x67b4386b,
        0x98ddc3e1, 0xe13fa5ea, 0xe1db6cb7, 0x42b58f5e,
        0x95b82f47, 0x127a67e2, 0x1b2c7e77, 0x6f29f9e5};
    int i;
    int len = strlen(str);
    unsigned char *data = (unsigned char *)malloc(len + 64);
    memcpy(data, str, len);

    data[len] = 0x80;
    for (i = len + 1; i < len + 64; i++)
        data[i] = 0;

    unsigned long long bit_len = 8 * len;
    memcpy(&data[len + 56], &bit_len, sizeof(bit_len));

    for (i = 0; i < len + 64; i += 64)
    {
        unsigned int w[64];
        int j;

        for (j = 0; j < 16; j++)
            w[j] = (data[i + j * 4] << 24) | (data[i + j * 4 + 1] << 16) | (data[i + j * 4 + 2] << 8) | data[i + j * 4 + 3];

        for (j = 16; j < 64; j++)
        {
            unsigned int s0 = (w[j - 15] >> 7) | (w[j - 15] << (32 - 7));
            unsigned int s1 = (w[j - 2] >> 17) | (w[j - 2] << (32 - 17));
            w[j] = w[j - 16] + s0 + w[j - 7] + s1;
        }

        unsigned int a = h0, b = h1, c = h2, d = h3, e = h4, f = h5, g = h6, h = h7;

        for (j = 0; j < 64; j++)
        {
            unsigned int s1 = (e >> 6) | (e << (32 - 6));
            unsigned int ch = (e & f) ^ ((~e) & g);
            unsigned int temp1 = h + s1 + ch + K[j] + w[j];
            unsigned int s0 = (a >> 2) | (a << (32 - 2));
            unsigned int maj = (a & b) ^ (a & c) ^ (b & c);
            unsigned int temp2 = s0 + maj;

            h = g;
            g = f;
            f = e;
            e = d + temp1;
            d = c;
            c = b;
            b = a;
            a = temp1 + temp2;
        }

        h0 += a;
        h1 += b;
        h2 += c;
        h3 += d;
        h4 += e;
        h5 += f;
        h6 += g;
        h7 += h;
    }

    sprintf(output, "%08x%08x%08x%08x%08x%08x%08x%08x", h0, h1, h2, h3, h4, h5, h6, h7);

    free(data);
}

// Takes input for username
void input_username(char *username)
{
    printf("Enter a username: ");
    scanf("%s", username);
}

// Takes input for password and hashes it
void input_password(char *password)
{
    printf("Enter a password: ");
    scanf("%s", password);
}

// Takes input for initial deposit amount
void input_initial_deposit(float *initial_deposit)
{
    printf("Enter initial deposit: ");
    scanf("%f", initial_deposit);
}

// Checks if a username already exists
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

// Creates a new user account
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
    char hashed_password[64];
    sha256(password, hashed_password);

    input_initial_deposit(&initial_deposit);

    strcpy(users[user_count].username, username);
    strcpy(users[user_count].password, hashed_password);
    users[user_count].balance = initial_deposit;
    user_count++;

    printf("Account created successfully!\n");
}

// Displays the ATM menu options
void display_menu()
{
    printf("\n1. Check Balance\n");
    printf("2. Deposit\n");
    printf("3. Withdraw\n");
    printf("4. Logout\n");
}

// Checks and displays the balance of the current user
void perform_balance_check(int user_index)
{
    printf("Your current balance is: %.2f\n", users[user_index].balance);
}

// Deposits a specified amount to the current user's account
void perform_deposit(int user_index)
{
    float amount;
    printf("Enter amount to deposit: ");
    scanf("%f", &amount);
    users[user_index].balance += amount;
    printf("Deposit successful!\n");
}

// Withdraws a specified amount from the current user's account
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
        printf("Withdrawal successful!\n");
    }
}

// Handles ATM operations for a logged-in user
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

// Takes login details (username and password) and hashes the password
void input_login_details(char *username, char *password)
{
    printf("Enter your username: ");
    scanf("%s", username);
    printf("Enter your password: ");
    scanf("%s", password);
}

// Logs in a user by verifying credentials
void login()
{
    char username[50], password[50];

    input_login_details(username, password);

    char hashed_password[64];
    sha256(password, hashed_password);

    for (int i = 0; i < user_count; i++)
    {
        if (strcmp(users[i].username, username) == 0)
        {
            if (strcmp(users[i].password, hashed_password) == 0)
            {
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

// Displays the main menu of the ATM system
void display_main_menu()
{
    printf("\n--- ATM System ---\n");
    printf("1. Create Account\n");
    printf("2. Login\n");
    printf("3. Exit\n");
}

// Handles the main menu interactions
void main_menu()
{
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

// Main Function
int main()
{
    main_menu();
    return 0;
}
