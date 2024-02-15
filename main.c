#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct items {
    char item[20];
    float price;
    int qty;
};

struct orders {
    char customer[50];
    char date[50];
    int numOfItems;
    struct items itm[50];
};

void generateBillHeader(char name[50], char date[30]) {
    printf("\n\n");
    printf("\t    ADV. CAFETERIA ZU TRC");
    printf("\n\t   -----------------------");
    printf("\n Date:%s", date);
    printf("\n Invoice To: %s", name);
    printf("\n");
    printf("-------------------------------------\n");
    printf("Items\t\t");
    printf("Qty\t\t");
    printf("Total\t\t");
    printf("\n-------------------------------------");
    printf("\n\n");
}

void generateBillBody(char item[50], int qty, float price) {
    printf("%s\t\t", item);
    printf("%d\t\t", qty);
    printf("%.2f\t\t", qty * price);
    printf("\n");
}

void generateBillFooter(float total) {
    printf("\n");
    float dis = 0.1 * total;
    float netTotal = total - dis;
    float cgst = 0.09 * netTotal, grandTotal = netTotal + 2 * cgst;

    printf("---------------------------------------\n");
    printf("Sub Total\t\t\t%.2f", total);
    printf("\nDiscount @10%s\t\t\t%.2f", "%", dis);
    printf("\n\t\t\t\t-------");
    printf("\nNet Total\t\t\t%.2f", netTotal);
    printf("\nCGST @9%s\t\t\t%.2f", "%", cgst);
    printf("\nSGST @9%s\t\t\t%.2f", "%", cgst);
    printf("\n--------------------------------------");
    printf("\nGrand Total\t\t\t%.2f", grandTotal);
    printf("\n--------------------------------------\n");
}
// Function for writing an order in CSV file
void writeOrderToCSV(struct orders ord) {
    FILE* csvFile = fopen("CafeteriaBill.csv", "a+");
    if (csvFile == NULL) {
        printf("Error opening CSV file.\n");
        return;
    }

    if (ftell(csvFile) == 0) {
        fprintf(csvFile, "Customer, Date, Item, Quantity, Price \n");
    }

    for (int i = 0; i < ord.numOfItems; i++) {
        fprintf(csvFile, "%s,%s,%s,%d,%.2f\n", ord.customer, ord.date, ord.itm[i].item, ord.itm[i].qty, ord.itm[i].price);
    }

    fclose(csvFile);
}

int main() {
    int opt, n;
    struct orders ord;
    struct orders order;
    char saveBill = 'y', contFlag = 'y';
    char name[50];
    FILE* fp;

    while (contFlag == 'y') {
        system("clear");
        float total = 0;
        int invoiceFound = 0;

        printf("\t============ADV. CAFETERIA ZU TRC============");
        printf("\n\nPlease select your preferred operation");
        printf("\n\n1.Generate An Invoice");
        printf("\n2.Show all Invoices");
        printf("\n3.Search An Invoice");
        printf("\n4.Exit The System");
        printf("\n5.Restart The System");

        printf("\n\nEnter Your choice: \t");
        scanf("%d", &opt);
        fgetc(stdin);

        switch (opt) {
            case 1:
                system("clear");
                printf("\nPlease enter the name of the customer:\t");
                fgets(ord.customer, 50, stdin);
                ord.customer[strlen(ord.customer) - 1] = 0;
                strcpy(ord.date, __DATE__);
                printf("\nPlease enter the number of items:\t");
                scanf("%d", &n);
                ord.numOfItems = n;
                int i;
                for (i = 0; i < n; i++) {
                    fgetc(stdin);
                    printf("\n\n");
                    printf("Please enter the item %d:\t", i + 1);
                    fgets(ord.itm[i].item, 20, stdin);
                    ord.itm[i].item[strlen(ord.itm[i].item) - 1] = 0;
                    printf("Please enter the quantity:\t");
                    scanf("%d", &ord.itm[i].qty);
                    printf("Please enter the unit price:\t");
                    scanf("%f", &ord.itm[i].price);
                    total += ord.itm[i].qty * ord.itm[i].price;
                }

                generateBillHeader(ord.customer, ord.date);
                for (int i = 0; i < ord.numOfItems; i++) {
                    generateBillBody(ord.itm[i].item, ord.itm[i].qty, ord.itm[i].price);
                }

                generateBillFooter(total);

                printf("\n Do you want to save the invoice [y/n]:\t");
                scanf("%s", &saveBill);

                if (saveBill == 'y') {
                    writeOrderToCSV(ord);
                    fp = fopen("CafeteriaBill.dat", "a+");
                    fwrite(&ord, sizeof(struct orders), 1, fp);
                    if (fwrite != 0)
                        printf("\nSuccessfully saved");
                    else
                        printf("\nError saving");
                    fclose(fp);
                }
                break;

            case 2:
                system("clear");
                fp = fopen("CafeteriaBill.dat", "r");
                printf("\n  ******Your Previous Invoices******\n");
                while (fread(&order, sizeof(struct orders), 1, fp)) {
                    float tot = 0;
                    writeOrderToCSV(order);
                    generateBillHeader(order.customer, order.date);
                    for (int i = 0; i < order.numOfItems; i++) {
                        generateBillBody(order.itm[i].item, order.itm[i].qty, order.itm[i].price);
                        tot += order.itm[i].qty * order.itm[i].price;
                    }
                    generateBillFooter(tot);
                }
                fclose(fp);
                break;

            case 3:
                printf("Enter the name of the customer:\t");
                fgets(name, 50, stdin);
                name[strlen(name) - 1] = 0;
                system("clear");
                fp = fopen("CafeteriaBill.dat", "r");
                printf("\t******Invoice of %s******", name);
                while (fread(&order, sizeof(struct orders), 1, fp)) {
                    float tot = 0;
                    if (!strcmp(order.customer, name)) {
                        writeOrderToCSV(order);
                        generateBillHeader(order.customer, order.date);
                        for (int i = 0; i < order.numOfItems; i++) {
                            generateBillBody(order.itm[i].item, order.itm[i].qty, order.itm[i].price);
                            tot += order.itm[i].qty * order.itm[i].price;
                        }
                        generateBillFooter(tot);
                        invoiceFound = 1;
                    }
                }
                if (!invoiceFound) {
                    printf("Sorry, the invoice for %s does not exist", name);
                }
                fclose(fp);
                break;

            case 4:
                printf("\n\t\t Bye Bye :)\n\n");
                exit(0);
                break;

            case 5:
                // Restart the system
                printf("Rebooting The Program...\n");
                main();
                break;

            default:
                printf("Sorry invalid option");
                break;
        }

        printf("\nDo you want to perform another operation?[y/n]:\t");
        scanf("%s", &contFlag);
    }

    printf("\n\t\t Sorry To See You Leaving Good Bye :)\n\n");
    printf("\n\n");

    return 0;
}
