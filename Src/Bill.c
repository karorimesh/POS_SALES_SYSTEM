///		1.	Select bill payment
///		2.	Enter bill number (bill number should be numeric, its length should between 5-8 characters). Perform validation of input and display appropriate error message where necessary.
///		3.	Enter bill amount
///		4.	Print receipt showing bill number entered, amount and date/time
#include <stdio.h>
#include <sdk_tplus.h>
#include "Menu.h"
#include "Goal.h"

void bill(){
	// Declarations
	int billMaskSize = 8;
	int i = 0;
	char confirmMessage[10000];
	char billAmount[15];
	char billAmountMask[] = "/d/d/d,/d/d/d,/d/d/D";
	char currency[10] = "Ksh";
	ulong amountResult = 0;
	char title[] = "Bill Number";
	char text[] = "Enter Bill Number";
	char billMask[15]; //TODO: 5-8 characters of numbers;
	char billNumber [10];
	char printData [10000];
	ulong message = 0;
	ulong billResult = 0;



	// Memory reset
	memset(billNumber, 0, sizeof billNumber);
	memset(billAmount, 0, sizeof billAmount);
	memset(printData, 0, sizeof printData);
	memset(confirmMessage, 0, sizeof confirmMessage);
	memset(billMask, 0, sizeof billMask);
	// Functions for Bill number input
	//	Cast Mask
	for (i ; i <= billMaskSize ; ++i) {
		strcat(billMask,"/d");
	}

	labelBill:
	billResult = GL_Dialog_Text(gGoalGraphicLibInstance, title, text, billMask, billNumber, 8, GL_TIME_MINUTE);
	if(billResult == GL_KEY_CANCEL){
		goto labelEnd;
	}

	// Validation method + Validation error response
	if (strlen(billNumber) < 5) {
		message = GL_Dialog_Message(gGoalGraphicLibInstance, NULL, "Please Enter a valid\nBill number", GL_ICON_ERROR, GL_BUTTON_ALL, 5*1000);
		if(message == GL_KEY_CANCEL){
			goto labelEnd;
		}
		goto labelBill;
	}

	// Input amount of sale
	amountResult = GL_Dialog_Amount(gGoalGraphicLibInstance, "Bill", "Enter Amount", billAmountMask, billAmount, sizeof(billAmount),currency, GL_ALIGN_BOTTOM_RIGHT, 60 * 1000);

	if(amountResult == GL_KEY_CANCEL){
		goto labelEnd;
	}

	// Confirmation

	strcat(confirmMessage, "Confirm Amount:\nKsh. ");
	strcat(confirmMessage, billAmount);
	strcat(confirmMessage, "\nTo Bill: ");
	strcat(confirmMessage, billNumber);
	message = GL_Dialog_Message(gGoalGraphicLibInstance, "Confirm Transaction", confirmMessage , GL_ICON_QUESTION, GL_BUTTON_VALID_CANCEL, GL_TIME_INFINITE );
	if(message != GL_KEY_VALID){
		goto labelEnd;
	}

	// Print data format
	strcat(printData, "Bill Payment\n=============================\n");
	strcat(printData, "Bill Number:         ");
	strcat(printData, billNumber);
	strcat(printData, "\nBill Amount:       Ksh.");
	strcat(printData, billAmount);

//	TODO Print receipt
	makeReceipt(printData);
	goto labelFinal;
	// End
	labelEnd:
		GL_Dialog_Message(gGoalGraphicLibInstance, NULL, "Transaction Cancelled", GL_ICON_ERROR, GL_BUTTON_ALL, 5*1000);

	labelFinal:
	GL_Dialog_Message(gGoalGraphicLibInstance, NULL, "End of transaction", GL_ICON_INFORMATION, GL_BUTTON_ALL, 5*1000);
}

void bill2(){
	GL_Dialog_Message(gGoalGraphicLibInstance, "SALE",
	    	          "This is the\nPayment and Cashback Logic", GL_ICON_INFORMATION, GL_BUTTON_VALID,
	    	          3 * GL_TIME_SECOND);
}
