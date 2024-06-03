
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <conio.h>

#define num_Item sizeof(item) / sizeof(item[0])
#define MAX_STOCK_LIMIT 10
#define MAX_MONEY_LIMIT 5


struct menu
{
    int itemNo;
    char name[20];
    int *stock;
    int price;
    int selectedQuantity;
};

struct sales_record
{
    int itemNo;
    char name[20];
    int *soldQuantity;
    int price;
};

struct papermoney
{
    int change;
    int *paperMoney;
};

int COLA_STOCK = MAX_STOCK_LIMIT;
int SPRITE_STOCK =  MAX_STOCK_LIMIT;
int MILO_STOCK =  MAX_STOCK_LIMIT;
int ICECOFFEE_STOCK =  MAX_STOCK_LIMIT;
int WATER_STOCK =  MAX_STOCK_LIMIT;

int COLA_SALES = 0;
int SPRITE_SALES =  0;
int MILO_SALES =  0;
int ICECOFFEE_SALES = 0;
int WATER_SALES =  0;

int papermoney100 =  MAX_MONEY_LIMIT;
int papermoney200 =  MAX_MONEY_LIMIT;
int papermoney500 =  MAX_MONEY_LIMIT;
int papermoney1000 =  MAX_MONEY_LIMIT;

struct menu item[] =
{
    {1, "COLA", &COLA_STOCK, 800},
    {2, "SPRITE", &SPRITE_STOCK, 900},
    {3, "MILO", &MILO_STOCK, 1000},
    {4, "ICE COFFEE", &ICECOFFEE_STOCK, 1200},
    {5, "WATER", &WATER_STOCK, 500}
};

struct sales_record sales_history[] =
{
   {1, "COLA",&COLA_SALES, 800},
    {2, "SPRITE",&SPRITE_SALES ,900},
    {3, "MILO",&MILO_SALES , 1000},
    {4, "ICE COFFEE",&ICECOFFEE_SALES , 1200},
    {5, "WATER",&WATER_SALES , 500}
};

struct papermoney dispense[] =
{
    {100, &papermoney100},
    {200, &papermoney200},
    {500, &papermoney500},
    {1000, &papermoney1000}
};

// Function prototypes
void clearScreen();//155
void userMode();//160
void adminMode();//170
void loadStockFromFile();//235
void displayMenu();//245
void selectItem();//261
void displaySelectedItems(struct menu selectedItems[], int count);//431
int  calculateTotalPrice(struct menu selectedItems[], int count);//444
void money_get(struct menu selectedItems[], int count);//455
void loadpapermoneyFromFile();//486
void giveChange(int changeAmount,struct menu selectedItems[], int count,int inputMoney);//508
void dispenseItem(struct menu selectedItems[], int count);//583
void saveStockToFile();//599
void saveSalesAmountToFile();//609
void savepapermoneyToFile();//623
void generateReceipt(struct menu selectedItems[], int count, int receivedAmount);//637
void stockCheck();//647
void saveSalesAmount(struct menu selectedItems[],int count);//735
void salesAmountCheck();//756
void loadSalesAmountFromFile();//779
void remainingMoneyCheck();//795
void MoneyRefill();//817

int main()
{
    loadStockFromFile();
	char enter;

	printf("\n\n\n\n\t\t\t\t\t\t\t\t-----------------------------------------\n");
	printf("\t\t\t\t\t\t\t\t|\t\t\t\t\t|\n");
	printf("\t\t\t\t\t\t\t\t|\tWELCOME TO VENDING MACHINE\t|\n");
	printf("\t\t\t\t\t\t\t\t|\t\t\t\t\t|\n");
	printf("\t\t\t\t\t\t\t\t-----------------------------------------\n");
    printf("\n\t\t\t\t\t\t\t\t\tPRESS ENTER TO CONTINUE_ ");
    scanf("%c", &enter);

    if (enter == '\n')
    {
        clearScreen();

        printf("\n\n\t[1] CUSTOMER\n\n\t[2] ADMIN\n\n\tENTER: ");

        int choice;

        scanf("%d", &choice);

		clearScreen();

		printf("\n\n\n\n");
	    printf("\n\n\n\n\t\t\t\t\t LOADING...");

		for(int i = 0; i < 5; i++)
      	{
	      	sleep(1);
	      	printf(".");
	  	}
	  	clearScreen();

		switch(choice)
		{
			case 1:
				userMode();
				break;

			case 2:
				adminMode();
				break;

			default:
				printf("\n\tINVALID CHOICE!\n");
				break;
		}
    }

    return 0;
}

void clearScreen()
{
	system("cls");
}

void userMode()
{

    displayMenu();
    selectItem();
    saveStockToFile();

}

void adminMode()
{
    char correctPassword[8] = {'9', '9', '9', '9'};
    int maxAttempts = 5;
    int attempts = 0;

    while (attempts < maxAttempts) {
        char enteredPassword[8];
        char ch;
        int i = 0;

        printf("Enter password: ");

        while (1) {
            ch = _getch();

            if (ch == 13) {
                enteredPassword[i] = '\0';
                break;
            } else if (ch == 8 && i > 0) {
                i--;
                printf("\b \b");
            } else if (i < 8 - 1) {
                enteredPassword[i] = ch;
                i++;
                printf("*");
            }
        }

        if (strcmp(enteredPassword, correctPassword) != 0) {
            attempts++;
            printf("\n\tWRONG PASSWORD! TRY AGAIN! (ATTEMPT %d of %d)\n", attempts, maxAttempts);
        } else {
            clearScreen();

            int check;

            do {
                printf("\n\n\t[1] STOCK CHECK\n\n\t[2] SALES AMOUNT CHECK\n\n\t[3] REMAINING MONEY CHECK\n\n\t[0] TO EXIT\n\n\tENTER: ");
                scanf("%d", &check);
                clearScreen();

                switch (check) {
                    case 1:
                        stockCheck();
                        break;
                    case 2:
                        salesAmountCheck();
                        break;
                    case 3:
                        MoneyRefill();
                        break;
                }
            } while (check != 0);

            break;
        }

        if (attempts == maxAttempts) {
            printf("\n\tMAX PASSWORD ATTEMPTS REACHED. EXITING ADMIN MODE.\n");
            break;
        }
    }
}

void loadStockFromFile()
{
    FILE *file = fopen("stock.txt", "r");
    if (file)
    {
        fscanf(file, "%d %d %d %d %d", &COLA_STOCK, &SPRITE_STOCK, &MILO_STOCK, &ICECOFFEE_STOCK, &WATER_STOCK);
        fclose(file);
    }
}

void displayMenu()
{
    printf("\n\n\t\t\t  AVAILABLE ITEM\n");
	printf("\n\t=====================================================\n");
	printf("\tNo\t\tITEM\t\tSTOCK\t\tPRICE");
	printf("\n\t=====================================================\n");

    for (int i = 0; i < num_Item; i++)
    {

        printf("\n\t%-16d%-15s%5d%17d", item[i].itemNo, item[i].name, *(item[i].stock), item[i].price);
    }

    printf("\n\n\t=====================================================\n");
}

void selectItem()
{
    struct menu selectedItem[5];

    int selectedCount = 0;

    while (1)
    {
        int choice, quantity;

        printf("\n\tSELECT ITEM [1 To 5]: ");
        scanf("%d", &choice);

        if (choice < 1 || choice > num_Item)
        {
            printf("\n\tINVALID ITEM NUMBER!\n");
            continue;
        }

        struct menu *selectedItems = &item[choice - 1];

        printf("\n\tENTER QUANTITY: ");
        scanf("%d", &quantity);

        if (quantity <= 0 || quantity > *(selectedItems->stock))
        {
            printf("\n\tINVALID QUANTITY OR INSUFFICIENT STOCK!\n");
            continue;
        }

        *(selectedItems->stock) -= quantity;

        int alreadySelected = -1;
        for (int i = 0; i < selectedCount; i++)
        {
            if (selectedItems->itemNo == selectedItem[i].itemNo)
            {
                alreadySelected = i;
                break;
            }
        }
        if (alreadySelected == -1)
        {
            selectedItem[selectedCount] = *selectedItems;
            selectedItem[selectedCount].selectedQuantity = quantity;
            selectedCount++;
        }
        else
        {
            selectedItem[alreadySelected].selectedQuantity += quantity;
        }

        int num, changeNum, increaseORdecrease, updateQuantity;

        while (1)
        {
            printf("\n\n\t1 ADD MORE ITEM");
            printf("\n\t2 CHANGE QUANTITY");
            printf("\n\t3 CONFIRM");
            printf("\n\t4 CANCEL");
            printf("\n\tENTER: ");
            scanf("%d", &num);

			clearScreen();

            if (num == 1)
            {
                displayMenu();
                break;
            }
            else if (num == 2)
            {
                displaySelectedItems(selectedItem, selectedCount);
                printf("\n\tENTER ITEM NO TO CHANGE QUANTITY [1-%d]: ", selectedCount);
                scanf("%d", &changeNum);

                if (changeNum >= 1 && changeNum <= selectedCount)
                {
                    printf("\n\tCURRENT QUANTITY: %d\n", selectedItem[changeNum - 1].selectedQuantity);
                    printf("\n\t1 INCREASE\n\t2 DECREASE\n\tENTER: ");
                    scanf("%d", &increaseORdecrease);

                    printf("\n\tENTER QUANTITY: ");
                    scanf("%d", &updateQuantity);

					clearScreen();

                    switch (increaseORdecrease)
                    {
                        case 1:

                            if (updateQuantity > 0 && updateQuantity <= *(selectedItem[changeNum - 1].stock))
                            {
                                *(selectedItem[changeNum - 1].stock) -= updateQuantity;
                                selectedItem[changeNum - 1].selectedQuantity += updateQuantity;
								printf("\n\n\tUPDATED TO");
                                printf("\n\n\t%-15s%5d\n\n", selectedItem[changeNum - 1].name, selectedItem[changeNum - 1].selectedQuantity);

                            }
                            else
                            {
                                printf("\n\tINVALID QUANTITY OR INSUFFICIENT STOCK!\n");
                            }
                            break;

                        case 2:

                            if (updateQuantity > 0 && updateQuantity <= selectedItem[changeNum - 1].selectedQuantity)
                            {
                                *(selectedItem[changeNum - 1].stock) += updateQuantity;
                                selectedItem[changeNum - 1].selectedQuantity -= updateQuantity;
								printf("\n\n\tUPDATED TO");
                                printf("\n\n\t%-15s%5d\n\n", selectedItem[changeNum - 1].name, selectedItem[changeNum - 1].selectedQuantity);
                            }

                            else
                            {
                                printf("\n\tINVALID QUANTITY OR QUANTITY IS HIGHER THAN SELECTED QUANTITY!\n");
                            }
                            break;

                        default:
                            printf("\n\tINVALID CHOICE!\n");
                            break;
                    }
                }
                else
                {
                    printf("\n\tINVALID ITEM NUMBER!\n");
                }
            }

            else if (num == 3)
            {
            	clearScreen();
                displaySelectedItems(selectedItem, selectedCount);
                int totalPrice = calculateTotalPrice(selectedItem, selectedCount);

                printf("\n\tTOTAL PRICE: %d\n", totalPrice);

                money_get(selectedItem, selectedCount);
                break;

            }

            else if (num == 4)
            {
                printf("\n\tTRANSACTION CANCEL!\n");
                for(int i = 0; i < selectedCount;i++)
                {
                   *(selectedItem[i].stock) += selectedItem[i].selectedQuantity;
                }
                break;
            }

            else
            {
                printf("\n\tINVALID CHOICE!\n");
            }

        }

        if (num == 3 || num == 4)
        {
            break;
        }

    }
}

void displaySelectedItems(struct menu selectedItems[], int count)
{
    printf("\n\n\tSELECTED ITEMS\n\n");
	printf("\n\t=====================================================\n");
	printf("\tNO\t\tITEM\t\tSTOCK\t\tPRICE");
	printf("\n\t=====================================================\n");
    for (int i = 0; i < count; i++)
    {
        printf("\n\t%-16d%-15s%5d%17d", i + 1, selectedItems[i].name, selectedItems[i].selectedQuantity, selectedItems[i].selectedQuantity * selectedItems[i].price);
    }
	printf("\n\t=====================================================\n");
}

int calculateTotalPrice(struct menu selectedItems[], int count)
{
    int totalPrice = 0;

    for (int i = 0; i < count; i++)
    {
        totalPrice += selectedItems[i].price * selectedItems[i].selectedQuantity;
    }
    return totalPrice;
}

void money_get(struct menu selectedItems[], int count)
{
    int grandTotal;
    int received_amount_input;
    int cashOutMoney=0;
    int changeMoney = 0;
    int countMoney = 0;

    printf("\n\tINSERT YOUR MONEY_ ");
    scanf("%d", &received_amount_input);
    printf("\n");
	for(int i = 0; i < 3; i++)
    {
	      	sleep(1);
	      	printf("\t-");
	}
    grandTotal = calculateTotalPrice(selectedItems, count);

	if (received_amount_input >= grandTotal)

    {
        cashOutMoney= received_amount_input - grandTotal;

            giveChange(cashOutMoney,selectedItems, count,received_amount_input);

    }

    else
    {
        printf("\n\tNOT ENOUGH MONEY!\n");
        printf("\n\tREFUND _  %d\n\n\n ",received_amount_input);

        for(int i = 0; i < count;i++)
        {
           *(selectedItems[i].stock) += selectedItems[i].selectedQuantity;
        }
    }
}

void loadpapermoneyFromFile()
{
    FILE *savepapermoney = fopen("changemoney.txt", "r");
    if (savepapermoney)
    {
        fscanf(savepapermoney, "%d %d %d %d", &papermoney100, &papermoney200, &papermoney500, &papermoney1000);
        fclose(savepapermoney);
    }
    else
    {
       printf("\n\tERROR OPENING FILE FOR READING.\n");
    }
}

void giveChange(int changeAmount,struct menu selectedItems[], int count,int inputMoney)
{
    int remainingChange = changeAmount;
    int remainderOfChange = 0;
    int tempremainderChange = 0;
    int tempPaperMoney[4];
    int receipt_OutorNot;

	loadpapermoneyFromFile();

    for(int i = 3; i >= 0; i--)
    {

            tempPaperMoney[i] = *(dispense[i].paperMoney);

            if(remainingChange >= dispense[i].change && *(dispense[i].paperMoney) > 0 )
            {
              remainderOfChange = remainingChange/dispense[i].change;

              //printf("Remainder:%d",remainderOfChange);
                if(remainderOfChange >= tempPaperMoney[i] && remainderOfChange > 0)
                {
                    tempremainderChange = remainderOfChange - tempPaperMoney[i];

                     tempPaperMoney[i] = 0;
                     remainingChange =  (remainingChange%dispense[i].change)+ (tempremainderChange * dispense[i].change);

                }

               else if(remainderOfChange > 0 && remainderOfChange < tempPaperMoney[i])
                {
                      tempPaperMoney[i] -= remainderOfChange;
                      remainingChange =  remainingChange%dispense[i].change;

                }
          }

    }

    if(remainingChange == 0 )
        {

             for (int i = 3; i >= 0; i--)
        {
               *(dispense[i].paperMoney) = tempPaperMoney[i];

        }
         savepapermoneyToFile();

	        printf("\n\n\tYOUR CHANGE: %d",changeAmount);
	        printf("\n\n\tDO YOU WANT RECEIPT?\n\t[1] YES\n\t[2] NO\n\tENTER: ");
	        scanf("%d", &receipt_OutorNot);

            saveSalesAmount(selectedItems,count);

			clearScreen();

		    if (receipt_OutorNot == 1)
		        {
		            generateReceipt(selectedItems, count,inputMoney);

		        }

		        dispenseItem(selectedItems, count);
        }
    else
    {
        printf("\n\n\tNOT ENOUGH TO CHANGE!");
        printf("\n\n REFUND _  %d\n\n\n ",inputMoney);

        for(int i = 0; i < count;i++)
        {
           *(selectedItems[i].stock) += selectedItems[i].selectedQuantity;
        }

   }
}

void dispenseItem(struct menu selectedItems[], int count)
{
	printf("\n\n\n\tDISPENSING ITEMS");
    for (int i = 0; i < count; i++)
    {
        for (int j = 0; j < selectedItems[i].selectedQuantity; j++)
        {
        	printf("\n\t-------------xx-------------\n");
            printf("\n\t      DISPENSING: %s\n", selectedItems[i].name);
            sleep(1);
        }
    }

    printf("\n\n\tTHANKS FOR USING THIS SERVICE!\n");
}

void saveStockToFile()
{
    FILE *file = fopen("stock.txt", "w");
    if (file)

    {
        fprintf(file, "%d %d %d %d %d", COLA_STOCK, SPRITE_STOCK, MILO_STOCK, ICECOFFEE_STOCK,WATER_STOCK);
        fclose(file);
    }
}

void saveSalesAmountToFile()
{

    FILE *saveSalesAmount = fopen("salesAmount.txt", "w");

    if (saveSalesAmount)
    {
        	fprintf(saveSalesAmount, "%d %d %d %d %d", COLA_SALES, SPRITE_SALES, MILO_SALES, ICECOFFEE_SALES,WATER_SALES);

            fclose(saveSalesAmount);

    }
}

void savepapermoneyToFile()
{
    FILE *savepapermoney = fopen("changemoney.txt", "w");
    if (savepapermoney)
    {
        fprintf(savepapermoney, "%d %d %d %d", papermoney100, papermoney200, papermoney500, papermoney1000);
        fclose(savepapermoney);
    }
    else
    {
        printf("\n\tERROR OPENING FILE FOR WRITING.\n");
    }
}

void generateReceipt(struct menu selectedItems[], int count, int receivedAmount)
{
    printf("\n\n\tRECEIPT\n");
    printf("\n\t-------------------------------\n");

    for (int i = 0; i < count; i++)
    {
        if( selectedItems[i].selectedQuantity > 0 )
        {
          printf("\n\t%-15s %d x %d = %d", selectedItems[i].name, selectedItems[i].selectedQuantity, selectedItems[i].price,selectedItems[i].selectedQuantity * selectedItems[i].price);
        }

    }

    printf("\n\n\t--------------------------------\n\n");
    printf("\tTOTAL PRICE: %d\n", calculateTotalPrice(selectedItems, count));
    printf("\tRECEIVED AMOUNT: %d\n", receivedAmount);
    printf("\tCHANGE: %d\n\n", receivedAmount - calculateTotalPrice(selectedItems, count));
    printf("\t----------------------------------\n\n");
}

void stockCheck()
{
		int refillNo, refillQuantity;
    	int count = 0;


    printf("\n\n\tDISPLAYING ITEMS WHICH STOCKS ARE UNDER 10\n");
    printf("\n\t=====================================\n");
	printf("\tNO\t\tITEM\t\tSTOCK");
	printf("\n\t=====================================\n");

    for (int i = 0; i < num_Item; i++)
    {
        if (*(item[i].stock) <= 5)
        {
            printf("\n\t%-16d%-15s %5d", item[i].itemNo, item[i].name, *(item[i].stock));
            count++;
        }
        else if (*(item[i].stock) > 5 && *(item[i].stock) < MAX_STOCK_LIMIT)
        {
            printf("\n\t%-16d%-15s %5d", item[i].itemNo, item[i].name, *(item[i].stock));
            count++;
        }
    }

    printf("\n\t=====================================\n");

    // Refill items until all reach maximum stock (10)
    while (*(item[0].stock) < MAX_STOCK_LIMIT || *(item[1].stock) < MAX_STOCK_LIMIT || *(item[2].stock) < MAX_STOCK_LIMIT ||
           *(item[3].stock) < MAX_STOCK_LIMIT || *(item[4].stock) < MAX_STOCK_LIMIT)
    {
        printf("\n\n\tENTER ITEM NO [1-5] TO REFILL OR [0] TO EXIT: ");
        scanf("%d", &refillNo);

        if (refillNo == 0)
        {
            break;
        }

        if (refillNo < 1 || refillNo > num_Item)
        {
            printf("\n\tINVALID ITEM NUMBER!\n");
            continue;
        }

        printf("\n\tENTER REFILL QUANTITY: ");
        scanf("%d", &refillQuantity);

        if ((*(item[refillNo - 1].stock) + refillQuantity) <= MAX_STOCK_LIMIT)
        {
            *(item[refillNo - 1].stock) += refillQuantity;
            printf("\n\n\t-------------------------------------");
            printf("\n\tNO\t\tITEM\t\tSTOCK");
            printf("\n\t--------------------------------------");
            printf("\n\t%-16d%-15s%5d", item[refillNo - 1].itemNo, item[refillNo - 1].name, *(item[refillNo - 1].stock));
            printf("\n\t-------------------------------------");
        }
        else
        {
            printf("\n\tLIMITED STOCK IS OVER!");
        }
    }

    saveStockToFile();
    clearScreen();

    printf("\n\n\tSUCCESSFULLY UPDATED!\n");
    printf("\n\t=====================================\n");
	printf("\tNO\t\tITEM\t\tSTOCK");
	printf("\n\t=====================================");

    for (int i = 0; i < num_Item; i++)
    {
        printf("\n\t%-16d%-15s%5d", item[i].itemNo, item[i].name, *(item[i].stock));
    }

    printf("\n\t=====================================\n\n\n");
}

void saveSalesAmount(struct menu selectedItems[],int count)
{

    loadSalesAmountFromFile();

	        for(int i = 0; i< count ;i++)
	       {
	            for(int j = 0; j< num_Item ;j++)
	            {
	                    if(strcmp(sales_history[j].name,selectedItems[i].name)==0)
	                    {

	                        *(sales_history[j].soldQuantity) += selectedItems[i].selectedQuantity;
	                         sales_history[j].price =   *(sales_history[j].soldQuantity)*sales_history[j].price;

	                         saveSalesAmountToFile();
						}
	            }
			}
}

void salesAmountCheck()
{
        int totalSalesAmount = 0;
        loadSalesAmountFromFile();

        printf("\n\t===================================================================");
        printf("\n\tNO\t\tITEM\t\tSOLD QUANTITY\t\tSALE AMOUNT ");
        printf("\n\t===================================================================");

        for(int i = 0; i < num_Item ; i++)
	    {
	        printf("\n\t%-16d%-15s%8d%28d", i + 1, sales_history[i].name, *(sales_history[i].soldQuantity),*(sales_history[i].soldQuantity) * sales_history[i].price);

			totalSalesAmount += *(sales_history[i].soldQuantity)  * sales_history[i].price;

		}

		printf("\n\t===================================================================");
		printf("\n\tTOTAL SALE AMOUNT\t\t\t%27d",totalSalesAmount);
		printf("\n\t===================================================================\n\n");

}

void loadSalesAmountFromFile()
{

    FILE *saveSalesAmount = fopen("salesAmount.txt", "r");

    if (saveSalesAmount)

    {
        fscanf(saveSalesAmount, "%d %d %d %d %d", &COLA_SALES, &SPRITE_SALES, &MILO_SALES, &ICECOFFEE_SALES, &WATER_SALES);

		fclose(saveSalesAmount);

    }

}

void remainingMoneyCheck()
{
       loadpapermoneyFromFile();


    printf("\n\n\tDISPLAYING MONEY WITH AMOUNT\n");
    printf("\n\t======================================");
    printf("\n\tNO\t\tMONEY\t\tAMOUNT");
    printf("\n\t======================================");

    for (int i = 0; i < sizeof(dispense)/sizeof(dispense[0]); i++)
    {
        if (*(dispense[0].paperMoney) <= 5)
        {
            printf("\n\t%d%19d%18d",i+1, dispense[i].change, *(dispense[i].paperMoney));

        }
    }

    printf("\n\t======================================");
}

void MoneyRefill()
{
		int refillNo, refillAmount;

        remainingMoneyCheck();
    // Refill items until all reach maximum stock (10)
    while (*(dispense[0].paperMoney) < MAX_MONEY_LIMIT ||*(dispense[1].paperMoney) < MAX_MONEY_LIMIT || *(dispense[2].paperMoney) < MAX_MONEY_LIMIT ||
           *(dispense[3].paperMoney) < MAX_MONEY_LIMIT)
    {
        printf("\n\n\tENTER ITEM NO [1-4] TO REFILL OR [0] TO EXIT: ");
        scanf("%d", &refillNo);

        if (refillNo == 0)
        {
            break;
        }

        if (refillNo < 1 || refillNo > sizeof(dispense)/sizeof(dispense[0]))
        {
            printf("\n\tINVALID ITEM NO!\n");
            continue;
        }

        printf("\n\tENTER REFILL AMOUNT: ");
        scanf("%d", &refillAmount);

        if ((*(dispense[refillNo - 1].paperMoney) + refillAmount) <= MAX_MONEY_LIMIT)
        {
            *(dispense[refillNo - 1].paperMoney) += refillAmount;
            printf("\n\n\t--------------------------------------");
            printf("\n\tNO\t\tMONEY\t\tAMOUNT");
            printf("\n\t--------------------------------------");
            printf("\n\t%d%17d%17d",refillNo, dispense[refillNo - 1].change, *(dispense[refillNo - 1].paperMoney));
            printf("\n\t--------------------------------------");
        }
        else
        {
            printf("\n\tLIMITED AMOUNT IS OVER!");
        }
    }

   savepapermoneyToFile();

    clearScreen();

    printf("\n\n\tSUCCESSFULLY UPDATED!\n");
    printf("\n\t======================================");
    printf("\n\tNO\t\tMONEY\t\tAMOUNT");
    printf("\n\t======================================");

    for (int i = 0; i < sizeof(dispense)/sizeof(dispense[0]); i++)
    {
         printf("\n\t%2d%17d%17d",i+1, dispense[i].change, *(dispense[i].paperMoney));
    }

    printf("\n\t======================================\n");
}

























