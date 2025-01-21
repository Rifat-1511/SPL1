#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct
{
    char username[50];
    char password[50];
    float balance;
} User;

User users[10];
int user_count = 0;

// Encrypts or decrypts a string using XOR with a key
void xor_encrypt(char *data, const char *key)
{
    int i;
    int key_len = strlen(key);
    for (i = 0; i < strlen(data); i++)
    {
        data[i] ^= key[i % key_len];
    }
}

// Takes input for username
void input_username(char *username)
{
    printf("Enter a username: ");
    scanf("%s", username);
}

// Takes input for password and encrypts it
void input_password(char *password, const char *key)
{
    printf("Enter a password: ");
    scanf("%s", password);
    xor_encrypt(password, key);
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
    const char *key = "encryption_key";

    input_username(username);

    if (is_username_exists(username))
    {
        printf("Username already exists. Try again.\n");
        return;
    }

    input_password(password, key);
    input_initial_deposit(&initial_deposit);

    strcpy(users[user_count].username, username);
    strcpy(users[user_count].password, password);
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

// Takes login details (username and password) and encrypts the password
void input_login_details(char *username, char *password, const char *key)
{
    printf("Enter your username: ");
    scanf("%s", username);
    printf("Enter your password: ");
    scanf("%s", password);
    xor_encrypt(password, key);
}

// Logs in a user by verifying credentials
void login()
{
    char username[50], password[50];
    const char *key = "encryption_key";

    input_login_details(username, password, key);

    for (int i = 0; i < user_count; i++)
    {
        if (strcmp(users[i].username, username) == 0)
        {
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

