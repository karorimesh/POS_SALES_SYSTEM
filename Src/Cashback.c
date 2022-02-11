///	1.	Select sale with cashback
///	2.	Enter sale amount
///	3.	Enter cashback amount
///	4.	Prompt user to insert/swipe card
///	5.	Prompt user to enter card's pin
///	6.	Print receipt showing the card number, sale amount, cashback amount and total amount entered and date/time. Note that total amount = sale amount + cashback amount.

#include <stdio.h>
#include <sdk_tplus.h>
#include "Menu.h"


void cashback(){
	//Declarations
	char saleAmount[100];
	char cashbackAmount[100];
	char confirmMessage[1000];
	char saleAmountMask[] = "/d/d/d,/d/d/d,/d/d/D";
	char pinMask[] = "/d/d/d/d";
	char currency[] = "Ksh";
	ulong amountResult = 0;
	ulong cashbackResult = 0;
	ulong pinEntry = 0;
	ulong message = 0;
	char pinValue[4 + 1];
	char printData[1000];
	int sale = 0;
	int cashback = 0;
	int total = 0;
	char totalAmount[20];
	char cardData[128];

//	memory clearing operations

	memset(saleAmount, 0, sizeof saleAmount);
	memset(cashbackAmount, 0, sizeof cashback);
	memset(pinValue, 0, sizeof pinValue);
	memset(printData, 0, sizeof printData);
	memset(confirmMessage, 0, sizeof confirmMessage);
	memset(cardData, 0, sizeof cardData);
	//Enter sale Amount
	amountResult = GL_Dialog_Amount(gGoalGraphicLibInstance, "Sale", "Enter Amount", saleAmountMask, saleAmount, sizeof(saleAmount),currency, GL_ALIGN_BOTTOM_RIGHT, 60 * 1000);
	if (amountResult == GL_KEY_CANCEL) {
		goto labelEnd;
	}
	sscanf(saleAmount, "%d", &sale);
	// Cashback Amount
	cashbackResult = GL_Dialog_Amount(gGoalGraphicLibInstance, "Cashback", "Enter Amount", saleAmountMask, cashbackAmount, sizeof(cashbackAmount),currency, GL_ALIGN_BOTTOM_RIGHT, 60 * 1000);
	if (amountResult == GL_KEY_CANCEL) {
		goto labelEnd;
	}
	sscanf(cashbackAmount, "%d", &cashback);

	total = sale + cashback;
	sprintf(totalAmount, "%d.00", total);

	// Prompt to enter Swipe card
	cardOps(cardData);

	//Pin Entry
	labelPin:
	pinEntry = GL_Dialog_Password(gGoalGraphicLibInstance, NULL, "Enter pin", pinMask, pinValue, sizeof pinValue, GL_TIME_INFINITE);
	if(message == GL_KEY_CANCEL ){
		goto labelEnd;
	}
	if (strlen(pinValue) != 4) {
		GL_Dialog_Message(gGoalGraphicLibInstance, "ERROR", "Enter the correct PIN" , GL_ICON_ERROR, GL_BUTTON_VALID_CANCEL, 3 * 1000);
		goto labelPin;
	}

	// TODO: Confirm transaction
	strcat(confirmMessage, "Sale Amount: Ksh.");
	strcat(confirmMessage, saleAmount);
	strcat(confirmMessage, "\nCashback Amount: Ksh.");
	strcat(confirmMessage, cashbackAmount);
	strcat(confirmMessage, "\nTotal Amount: Ksh.");
	strcat(confirmMessage, totalAmount);
	GL_Dialog_Message(gGoalGraphicLibInstance, "Confirm Transaction", confirmMessage , GL_ICON_QUESTION, GL_BUTTON_VALID_CANCEL, GL_TIME_INFINITE);
	if (message == GL_KEY_CANCEL ){
		goto labelEnd;
	}

	//TODO Print receipt
	strcat(printData, "Sale With Cashback Payment\n=============================\n");
	strcat(printData, "Card No:         ");
	strcat(printData, cardData);
	strcat(printData, "\nSale Amount:       Ksh.");
	strcat(printData, saleAmount);
	strcat(printData, "\nCashback Amount:       Ksh.");
	strcat(printData, cashbackAmount);
	strcat(printData, "\nTotal Amount:       Ksh.");
	strcat(printData, totalAmount);
	makeReceipt(printData);
	goto labelFinal;
	//End
	labelEnd:
		GL_Dialog_Message(gGoalGraphicLibInstance, NULL, "Transaction Cancelled", GL_ICON_ERROR, GL_BUTTON_ALL, 5*1000);

	labelFinal:
	GL_Dialog_Message(gGoalGraphicLibInstance, NULL, "End of transaction", GL_ICON_INFORMATION, GL_BUTTON_ALL, 5*1000);
}
