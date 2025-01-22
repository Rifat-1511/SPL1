#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct
{
    char username[50];
    char password_hash[64];
    float balance;
} User;

User users[10];
int user_count = 0;

void simple_hash(const char *str, char *output)
{
    unsigned long hash = 5381;
    int c;
    while ((c = *str++))
    {
        hash = ((hash << 5) + hash) + c;
    }
    sprintf(output, "%lx", hash);
}

void input_username(char *username)
{
    printf("Enter a username: ");
    scanf("%s", username);
}

void input_password(char *password)
{
    printf("Enter a password: ");
    scanf("%s", password);
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
    char hashed_password[64];
    simple_hash(password, hashed_password);

    input_initial_deposit(&initial_deposit);

    strcpy(users[user_count].username, username);
    strcpy(users[user_count].password_hash, hashed_password);

    users[user_count].balance = initial_deposit;
    user_count++;

    printf("Account created successfully!\n");
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
    scanf("%s", username);
    printf("Enter your password: ");
    scanf("%s", password);
}

void login()
{
    char username[50], password[50];

    input_login_details(username, password);

    char hashed_password[64];
    simple_hash(password, hashed_password);

    for (int i = 0; i < user_count; i++)
    {
        if (strcmp(users[i].username, username) == 0)
        {
            if (strcmp(users[i].password_hash, hashed_password) == 0)
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

void display_main_menu()
{
    printf("\n--- ATM System ---\n");
    printf("1. Create Account\n");
    printf("2. Login\n");
    printf("3. Exit\n");
}

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

int main()
{
    main_menu();
    return 0;
}
