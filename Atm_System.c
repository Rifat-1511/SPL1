#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// XOR Encryption/Decryption
void xor_encrypt(char *data, const char *key)
{
    int i;
    int key_len = strlen(key);
    for (i = 0; i < strlen(data); i++)
    {
        data[i] ^= key[i % key_len];
    }
}

// User structure
typedef struct
{
    char username[50];
    char password[50];
    float balance;
} User;

User users[10]; // Array to store users
int user_count = 0;

// Create account
void create_account()
{
    char username[50], password[50];
    float initial_deposit;
    const char *key = "encryption_key";

    printf("Enter a username: ");
    scanf("%s", username);

    // Check if username already exists
    for (int i = 0; i < user_count; i++)
    {
        if (strcmp(users[i].username, username) == 0)
        {
            printf("Username already exists. Try again.\n");
            return;
        }
    }

    printf("Enter a password: ");
    scanf("%s", password);

    // Encrypt password
    xor_encrypt(password, key);

    printf("Enter initial deposit: ");
    scanf("%f", &initial_deposit);

    // Store user data
    strcpy(users[user_count].username, username);
    strcpy(users[user_count].password, password);
    users[user_count].balance = initial_deposit;
    user_count++;

    printf("Account created successfully!\n");
}

// Login
void login()
{
    char username[50], password[50];
    const char *key = "encryption_key";

    printf("Enter your username: ");
    scanf("%s", username);

    for (int i = 0; i < user_count; i++)
    {
        if (strcmp(users[i].username, username) == 0)
        {
            printf("Enter your password: ");
            scanf("%s", password);

            // Encrypt input password and compare
            xor_encrypt(password, key);
            if (strcmp(users[i].password, password) == 0)
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

// ATM Operations
void atm_operations(int user_index)
{
    int choice;
    float amount;

    while (1)
    {
        printf("\n1. Check Balance\n2. Deposit\n3. Withdraw\n4. Logout\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            printf("Your current balance is: %.2f\n", users[user_index].balance);
            break;
        case 2:
            printf("Enter amount to deposit: ");
            scanf("%f", &amount);
            users[user_index].balance += amount;
            printf("Deposit successful!\n");
            break;
        case 3:
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
            break;
        case 4:
            printf("Logging out...\n");
            return;
        default:
            printf("Invalid choice. Try again.\n");
        }
    }
}

// Main function
int main()
{
    int option;

    while (1)
    {
        printf("\n--- ATM System ---\n");
        printf("1. Create Account\n");
        printf("2. Login\n");
        printf("3. Exit\n");
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
            return 0;
        default:
            printf("Invalid choice. Try again.\n");
        }
    }
    return 0;
}
