#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

typedef struct node {
    char statement[50];
    struct node* link;
} node;

void pinGeneration(void);
int checkLogin(void);
void showBalance(int *);
void depositMoney(node **, int *);
void withdrawMoney(node **, int *);
void saveHistory(node **, char *);
void removeHistory(node **);
void showHistory(node **);

int main(void) {
    int choice1, choice2;
    int loginValid = 0, balance = 0;

    node *head = NULL;

    while (1) {
        printf("\n\nATM System\n======================\n");
        printf("1. Generate User ID and PIN\n2. Use ATM\n3. Exit\n");
        printf("\nYour choice: ");
        scanf("%d", &choice1);

        switch (choice1) {
            case 1: pinGeneration();
                    exit(EXIT_SUCCESS);

            case 2: loginValid = checkLogin();

                    if (loginValid) {
                        printf("\nLogin successful\n");
                    } else {
                        printf("\nInvalid user ID or PIN. Please generate a user ID and PIN if you don't have one.\n");
                        exit(EXIT_FAILURE);
                    }

                    while(loginValid) {
                        printf("\nATM System Menu\n===============\n\n");
                        printf("1. Check Balance\n2. Deposit\n3. Withdraw\n4. View transaction history\n5. Quit\n\n");

                        printf("Enter choice: ");
                        scanf("%d", &choice2);

                        switch(choice2) {
                            case 1: showBalance(&balance);
                                    break;
                            case 2: depositMoney(&head, &balance);
                                    break;
                            case 3: withdrawMoney(&head, &balance);
                                    break;
                            case 4: showHistory(&head);
                                    break;
                            case 5: printf("\nThank you for using the ATM\n");
                                    exit(EXIT_SUCCESS);
                            default: printf("\nInvalid option entered!\n");
                                     break;
                        }
                    }

            case 3: exit(EXIT_SUCCESS);

            default: printf("\nInvalid choice...Try again...\n");
                     break;
        }
    }
    return 0;
}

int checkLogin(void) {
    FILE *fp;
    char userID[20];
    char pin[8];
    char keyUserID[20];
    char keyPin[8];
    int loginValid = 0;

    printf("\n\nEnter the User ID: \n");
    scanf("%s", keyUserID);
    printf("\nEnter the PIN: \n");
    scanf("%s", keyPin);

    fp = fopen("pin.txt", "r");
    if (NULL == fp) {
        printf("\nFile cannot be opened\n");
        exit(EXIT_FAILURE);
    }

    while (fscanf(fp, "%s %s", userID, pin) != EOF) {
        if (strcmp(userID, keyUserID) == 0 && strcmp(pin, keyPin) == 0) {
            loginValid = 1;
            break;
        }
    }
    fclose(fp);

    return loginValid;
}

void pinGeneration(void) {
    FILE *fp;

    srand(time(NULL));
    int generatedPin = 1000 + rand() % 9000;
    char generatedUserID[20];
    printf("\nUser ID and PIN generated successfully\n");

    printf("\nYour generated User ID: ");
    scanf("%s", generatedUserID);
    printf("\nYour generated PIN: %d\n", generatedPin);
    printf("\nRe-run the program and use ATM with this User ID and PIN\n\n");

    fp = fopen("pin.txt", "a");
    if (NULL == fp) {
        printf("\nCannot open file!");
        exit(EXIT_FAILURE);
    }

    fprintf(fp, "%s %d\n", generatedUserID, generatedPin);
    fclose(fp);
}

// Other functions remain unchanged

/*
  This function will display the current balance amount
*/
void showBalance(int *balance) {
    printf("\nYour current balance is BDT.%d\n", *balance);
}

/*
  This function will add the money deposited to the balance
*/
void depositMoney(node **head, int *balance) {
    int depositAmount;
    /*
      buffer to store
    */
    char depositStmt[50];

    printf("\nEnter amount to deposit: ");
    scanf("%d", &depositAmount);

    if (depositAmount > 0) {
        *balance += depositAmount;
        printf("\nBDT.%d deposited\n", depositAmount);

		/*
		  saving formatted string in depositStmt character array
		*/
        snprintf(depositStmt, sizeof(depositStmt), "BDT.%d deposited\n", depositAmount);
        saveHistory(head, depositStmt);
    } else {
        printf("\nInvalid amount entered\n.");
    }
}

/*
  This function will deduct the money withdrawn from the balance
*/
void withdrawMoney(node **head, int *balance) {
    int withdrawAmount;
    char withdrawStmt[50];

    printf("\nEnter amount to withdraw: ");
    scanf("%d", &withdrawAmount);

    if (withdrawAmount > 0) {
        if (withdrawAmount > *balance) {
            printf("\nCannot withdraw. Balance BDT.%d\n", *balance);
        } else {
            *balance -= withdrawAmount;
            printf("\nBDT.%d withdrawn\n", withdrawAmount);

            /*
			  saving formatted string in withdrawStmt character array
			*/
            snprintf(withdrawStmt, sizeof(withdrawStmt), "BDT.%d withdrawn\n", withdrawAmount);
            saveHistory(head, withdrawStmt);
        }
    } else {
        printf("\nInvalid amount entered\n.");
    }
}

/*
  This function will save a transaction statement
*/
void saveHistory(node **head, char *str) {
    static int count = 0;
    node *temp;
    temp = (node *)malloc(sizeof(node));

    strcpy(temp->statement, str);
    temp->link = NULL;

    if (NULL == *head) {
        *head = temp;
        count++;
    } else {
        if (10 == count) {
            removeHistory(head);
            count--;
        }
        node *p;
        p = *head;
        while (NULL != p->link) {
            p = p->link;
        }
        p->link = temp;
        count++;
    }
}

/*
  This function is used to remove the oldest transaction when
  10 transactions are made
*/
void removeHistory(node **head) {
    node *temp;
    temp = *head;
    *head = (*head)->link;
    temp->link = NULL;
    free(temp);
}

/*
  This function will display the transaction history
*/
void showHistory(node **head) {
    node *temp;
    temp = *head;

    if (NULL == temp) {
        printf("\nNo transaction history...\n");
    } else {
        printf("\nTransaction History\n-------------------\n\n");
        while (NULL != temp) {
            printf("%s\n", temp->statement);
            temp = temp->link;
        }
    }
}
