#include <stdio.h>
#include <string.h>

// ======== FUNCTION DECLARATIONS ========
void showMenu(char items[][30], int price[], int available[], int category[], int N);
int assignTable(char name[], char address[]);
void suggestAlternatives(int index, char items[][30], int available[], int category[], int price[], int
 N);

// =======================================
//               MAIN
// =======================================
int main() {
    int N = 18;

    char items[18][30] = {
        "Pizza","Burger","Pasta","Momos","French Fries","Sandwich","Hot Dog","Cold Drink",
        "Coffee","Tea","Lassi","Soda","Veg Thali","Non-Veg Thali","Paneer Thali","Chole Bhature",
        "Rajasthani Thali","Masala Dosa"
    };

    int category[18] = {
        0,0,0,0,0,0,0,2,   // first 8 = fast food (0) except “Cold Drink” labelled 2 for beverage
        2,2,2,2,           // four beverages = 2
        1,1,1,1,           // four thalis = 1
        1,0                // “Rajasthani Thali” = 1, “Masala Dosa” treated as fast food = 0
    };

    int available[18] = {
        1,0,1,1,1,1,0,1,
        1,1,0,1,
        1,1,1,0,
        0,1
    };

    int price[18] = {
        180,90,140,110,70,80,85,40,
        60,30,80,40,150,200,180,120,
        250,90
    };

    int orderedQty[18];
    for(int i = 0; i < N; i++) {
        orderedQty[i] = 0;
    }

    // ===== CUSTOMER DETAILS =====
    char name[50], address[100], contact[20], date[20], timeSlot[20];
    printf("Enter Customer Name: ");
    scanf(" %[^\n]", name);

    printf("Enter Address: ");
    scanf(" %[^\n]", address);

    printf("Enter Contact Number: ");
    scanf("%s", contact);

    // ===== CHOOSE MODE =====
    int mode;
    printf("\nDo you want to (1) BOOK the restaurant for a function OR (2) Just DINE-IN? Enter 1 or 2: ");
    scanf("%d", &mode);

    int people = 0;
    float perPlateRate = 0.0f;
    int tableNumber = 0;
    int addedCustomItem = 0;
    char customItem[30] = "";
    int customPrice = 0;
    int decoration = 0;
    float decorCharge = 0.0f;

    switch(mode) {
        case 1:
            // Booking mode
            printf("Enter Date of function (DD/MM/YYYY): ");
            scanf("%s", date);
            printf("Enter Time of function (HH:MM): ");
            scanf("%s", timeSlot);

            printf("For how many people will attend? ");
            scanf("%d", &people);

            printf("Do you want decoration/themes? (yes=1 / no=0): ");
            scanf("%d", &decoration);
            if(decoration == 1) {
                printf("Decoration charge: Rs 2000 flat + Rs 150 per plate theme extra.\n");
                decorCharge = 2000 + (150 * people);
            }

            printf("Enter Per Plate Rate for the full course (Rs): ");
            scanf("%f", &perPlateRate);

            printf("Do you want to add any special dish for this occasion? (yes=1 / no=0): ");
            scanf("%d", &addedCustomItem);
            if(addedCustomItem == 1) {
                printf("Enter Special Item Name: ");
                scanf(" %[^\n]", customItem);
                printf("Enter Price of this special item: ");
                scanf("%d", &customPrice);
            }

            tableNumber = assignTable(name, address);
            printf("\n>> Booking Confirmed!\n");
            printf("Date: %s   Time: %s\n", date, timeSlot);
            printf("People: %d   Table No: %d\n", people, tableNumber);
            if(decoration == 1) {
                printf("Decoration charge: Rs %.2f\n", decorCharge);
            }
            printf("Per Plate Course Rate: Rs %.2f\n", perPlateRate);
            if(addedCustomItem) {
                printf("Special Dish: %s at Rs %d\n", customItem, customPrice);
            }
            printf("-----------------------------------\n");
            break;

        case 2:
            // Dine-in mode
            printf("Enter Date (DD/MM/YYYY): ");
            scanf("%s", date);
            printf("Enter Time (HH:MM): ");
            scanf("%s", timeSlot);

            tableNumber = assignTable(name, address);
            printf("\n>> Table Booked for Dine-In\n");
            printf("Date: %s   Time: %s\n", date, timeSlot);
            printf("Table No: %d\n", tableNumber);
            printf("-----------------------------------\n");
            break;

        default:
            printf("Invalid mode selected. Exiting.\n");
            return 1;
    }

    // ===== SHOW MENU =====
    showMenu(items, price, available, category, N);
    if(addedCustomItem && mode == 1) {
        printf("\n--- SPECIAL OCCASION ITEM ---\n");
        printf("   19. %-20s Rs %d (Available)\n", customItem, customPrice);
    }

    // ===== ORDERING =====
    int choice, qty;
    float total = 0.0f;
    while(1) {
        printf("\nEnter item number to order (0 to finish): ");
        scanf("%d", &choice);

        if(choice == 0) break;

        if(mode == 1 && addedCustomItem && choice == 19) {
            printf("Enter quantity: ");
            scanf("%d", &qty);
            total += qty * customPrice;
            printf("Added %d x %s (Rs %d)\n", qty, customItem, qty * customPrice);
            continue;
        }

        if(choice < 1 || choice > N) {
            printf("Invalid choice!\n");
            continue;
        }

        int index = choice - 1;
        if(!available[index]) {
            printf("\nSORRY! %s is NOT available.\n", items[index]);
            printf("Here are alternatives:\n");
            suggestAlternatives(index, items, available, category, price, N);
            continue;
        }

        printf("Enter quantity: ");
        scanf("%d", &qty);
        orderedQty[index] += qty;
        total += qty * price[index];
        printf("Added %d x %s\n", qty, items[index]);
    }

    // ===== BILLING =====
    if(mode == 1) {
        float bookingCharge = perPlateRate * people;
        total = bookingCharge;
        if(decoration == 1) total += decorCharge;
        if(addedCustomItem) total += (customPrice);  // assuming one unit
    }

    float cgst = total * 0.09f;
    float sgst = total * 0.09f;
    float finalBill = total + cgst + sgst;

    printf("\n==========================================\n");
    printf("                 BILL RECEIPT              \n");
    printf("==========================================\n");
    printf("Customer: %s\nDate: %s Time: %s\nContact: %s\n", name, date, timeSlot, contact);
    printf("Address : %s\nTable No: %d\n", address, tableNumber);
    if(mode == 1) {
        printf("People  : %d\n", people);
    }
    printf("\n-------------- ORDER SUMMARY --------------\n");
    printf("%-3s %-22s %-6s %-8s\n", "No", "Item", "Qty", "Amount");

    if(mode == 2) {
        for(int i = 0; i < N; i++) {
            if(orderedQty[i] > 0) {
                printf("%-3d %-22s %-6d Rs %-7d\n", i+1, items[i], orderedQty[i], orderedQty[i] * price[i]);
            }
        }
        if(addedCustomItem && mode == 1) {
            printf("%-3d %-22s %-6d Rs %-7d\n", 19, customItem, 1, customPrice);
        }
        printf("-------------------------------------------\n");
        printf("Total (without GST) : Rs %.2f\n", total);
    } else {
        // booking summary
        printf("Full-course for %d people @ Rs %.2f per plate: Rs %.2f\n", people, perPlateRate, perPlateRate * people);
        if(decoration == 1) printf("Decoration Charges      : Rs %.2f\n", decorCharge);
        if(addedCustomItem) printf("Special Dish Charge     : Rs %d\n", customPrice);
        printf("-------------------------------------------\n");
        printf("Total (without GST) : Rs %.2f\n", total);
    }

    printf("CGST (9%%)           : Rs %.2f\n", cgst);
    printf("SGST (9%%)           : Rs %.2f\n", sgst);
    printf("-------------------------------------------\n");
    printf("FINAL BILL          : Rs %.2f\n", finalBill);
    printf("===========================================\n");
    printf("        THANK YOU! PLEASE VISIT AGAIN       \n");
    printf("===========================================\n");

    return 0;
}

// =======================================
//             FUNCTIONS
// =======================================

void showMenu(char items[][30], int price[], int available[], int category[], int N) {
    printf("\n================ MENU =================\n");

    printf("\n--- FAST FOOD ---\n");
    for(int i = 0; i < N; i++) {
        if(category[i] == 0) {
            printf("%2d. %-20s Rs %3d %s\n", i+1, items[i], price[i],
                   available[i] ? "(Available)" : "(Not Available)");
        }
    }

    printf("\n--- BEVERAGES ---\n");
    for(int i = 0; i < N; i++) {
        if(category[i] == 2) {
            printf("%2d. %-20s Rs %3d %s\n", i+1, items[i], price[i],
                   available[i] ? "(Available)" : "(Not Available)");
        }
    }

    printf("\n--- THALIS ---\n");
    for(int i = 0; i < N; i++) {
        if(category[i] == 1) {
            printf("%2d. %-20s Rs %3d %s\n", i+1, items[i], price[i],
                   available[i] ? "(Available)" : "(Not Available)");
        }
    }

    printf("=======================================\n");
}

int assignTable(char name[], char address[]) {
    int len1 = strlen(name);
    int len2 = strlen(address);
    return (len1 + len2) % 20 + 1;
}

void suggestAlternatives(int index, char items[][30], int available[], int category[], int price[], int N) {
    int shown = 0;

    // same category first
    for(int i = 0; i < N && shown < 5; i++) {
        if(i != index && category[i] == category[index] && available[i]) {
            printf(" - %s (Rs %d)\n", items[i], price[i]);
            shown++;
        }
    }

    // other categories
    for(int i = 0; i < N && shown < 5; i++) {
        if(i != index && category[i] != category[index] && available[i]) {
            printf(" - %s (Rs %d)\n", items[i], price[i]);
            shown++;
        }
    }

    if(shown == 0) {
        printf("No alternatives available.\n");
    }
}
