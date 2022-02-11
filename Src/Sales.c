///	1.	Select sale
///	2.	Enter amount (application should only accept amount > 1 and < 5,000).
///	3.	Prompt user to insert/swipe card
///	4.	Prompt user to enter card's PIN
///	5.	Print receipt showing the card number, amount entered and date/time
#include <stdio.h>
#include <sdk_tplus.h>
#include "Menu.h"
#include "Goal.h"

// Function to Enter Amount
void sale(){
	//Declarations
	char saleAmount[15];
	char confirmSale[100];
	char saleMasc[] = "/d,/d/d/D";
	char pinMask[] = "/d/d/d/d";
	char currency[10] = "Ksh";
	char pinValue[4 + 1];
	ulong amountResult = 0;
	ulong message = 0;
	char pinEntry = 0;
	int amount = 0;
	char printData[1000];
	char cardData[128];



	//Clean memory before use
	memset(saleAmount, 0, sizeof saleAmount);
	memset(confirmSale, 0, sizeof confirmSale);
	memset(pinValue, 0, sizeof pinValue);
	memset(printData, 0, sizeof printData);
	memset(cardData, 0, sizeof cardData);


	//Dialog for input Amount
	labelAmount:
		memset(saleAmount, 0, sizeof saleAmount);
		amount = 0;
		amountResult = GL_Dialog_Amount(gGoalGraphicLibInstance, "Sale", "Enter Amount", saleMasc, saleAmount, sizeof(saleAmount),currency, GL_ALIGN_BOTTOM_RIGHT, 60 * 1000);
		if(amountResult == GL_KEY_CANCEL){
			goto labelEnd;
		}

//		Type cast amount to enable validation
	sscanf(saleAmount, "%d", &amount);

	if( amount < 1 || amount >= 5000){
		message = GL_Dialog_Message(gGoalGraphicLibInstance, NULL, "Please enter amount less than 5000 or more than 1", GL_ICON_NONE, GL_BUTTON_ALL, 5*1000);

		if(message == GL_KEY_CANCEL){
			goto labelEnd;
		}
		goto labelAmount;
	}


//	Card Reader or Swipe

	cardOps(cardData);


	//PIN Input
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
	strcat(confirmSale, "Sale amount: Ksh. ");
	memset(saleAmount, 0, sizeof saleAmount);
	sprintf(saleAmount, "%d", amount);
	strcat(confirmSale, saleAmount);
	strcat(confirmSale, "\nBy account: ");
	strcat(confirmSale, cardData);
	message = GL_Dialog_Message(gGoalGraphicLibInstance, "Confirm Transaction", confirmSale , GL_ICON_QUESTION, GL_BUTTON_VALID_CANCEL, GL_TIME_INFINITE);
	if(message == GL_KEY_CANCEL){
		goto labelEnd;
	}

	//TODO Print receipt
	strcat(printData, "Sale Payment\n=============================\n");
	strcat(printData, "Card No:         ");
	strcat(printData, cardData);
	strcat(printData, "\nSale Amount:       Ksh.");
	strcat(printData, saleAmount);
	makeReceipt(printData);
	goto labelFinal;
	//End

	labelEnd:
		GL_Dialog_Message(gGoalGraphicLibInstance, NULL, "Transaction Cancelled", GL_ICON_ERROR, GL_BUTTON_ALL, 5*1000);

	labelFinal:
	GL_Dialog_Message(gGoalGraphicLibInstance, NULL, "End of transaction", GL_ICON_INFORMATION, GL_BUTTON_ALL, 5*1000);
}



void makeReceipt(char *printData){
	//Printer declarations
		T_GL_HWIDGET printDocument = NULL;
		T_GL_HWIDGET printLayout;
		T_GL_HWIDGET print;
		byte printLine = 0;
		Telium_Date_t currentDate;
		char date[50];

	//	Date function
		Telium_Read_date(&currentDate);
		Telium_Sprintf(date,"Date:          %2.2s/%2.2s/20%2.2s %2.2s:%2.2s:%2.2s",currentDate.day, currentDate.month, currentDate.year, currentDate.hour, currentDate.minute, currentDate.second);

		strcat(printData,"\n");
		strcat(printData, date);
		strcat(printData,"\n=============================\n\n\n\n");
	//Do printing
		printDocument = GoalCreateDocument(gGoalGraphicLibInstance, GL_ENCODING_UTF8);
		printLayout = GL_Layout_Create(printDocument);
		print = GL_Print_Create    (printLayout);
		GL_Widget_SetText      (print, printData);
		GL_Widget_SetItem      (print, 0, printLine++);
		GL_Widget_SetFontScale (print, GL_SCALE_LARGE);
		GL_Widget_SetBackAlign (print, GL_ALIGN_CENTER);
		GL_Widget_SetWrap      (print, TRUE);
		GL_Widget_SetFontStyle (print, GL_FONT_STYLE_BOLD);
		GoalPrnDocument(printDocument);
}


void cardOps(char *cardData){
	//Magstripe definitions

	int magCardPresent = FALSE;
	unsigned int swipeStatus = 0;
	char *cardStream = 0;
	Telium_File_t *hMag31=NULL, *hMag2=NULL, *hMag3=NULL;
	char dataRead[128];
	byte decodeCount = 0;
	int statusIso = ISO_OK - 1;
	ulong message = 0;
	char trackNo[129];
	char *source = 0;
	char *dest = 0;


	//Card inserted variables
	int i;
	Telium_File_t* portCam;
	unsigned char ucStatus;
	portCam = NULL;
	T_APDU apduCmd;
	T_APDU apduRpns;
	HISTORIC histData;
	byte cmdSqn[256];
	byte rspnSqn[256];

	//Prompt for Card insert or swipe

	memset(dataRead, 0, sizeof dataRead);
	memset(trackNo, 0, sizeof trackNo);
	memset(cmdSqn,0 ,sizeof cmdSqn);
	memset(rspnSqn, 0, sizeof rspnSqn);
	memcpy (cmdSqn,"80C002A108", 20);// Read Data from chip
	apduCmd.length = 20;

	while(!magCardPresent){
		message = GL_Dialog_Message(gGoalGraphicLibInstance, NULL, "Please Swipe or Insert card", GL_ICON_INFORMATION, GL_BUTTON_ALL, 2*1000);

		if (IsISO1() == 1) {
			hMag31 = Telium_Fopen("SWIPE31", "r*");
		}
		if (IsISO2() == 1) {
			hMag2 = Telium_Fopen("SWIPE2", "r*");
		}
		if (IsISO3() == 1) {
			hMag3 = Telium_Fopen("SWIPE3", "r*");
		}


		//		Process card entry
		portCam = Telium_Fopen("CAM0", "rw");

		swipeStatus = Telium_Ttestall( CAM0 | SWIPE31 | SWIPE2 | SWIPE3 | KEYBOARD, 0);

		if(swipeStatus & CAM0){
			statusIso = Telium_EMV_power_on(portCam, &histData);
			if (statusIso == ISO_OK) {
				apduCmd.data = cmdSqn;
				apduRpns.data = rspnSqn;
				Telium_EMV_apdu(portCam, &apduCmd, &apduRpns);
				for ( i = 0; i<apduCmd.length; i++){
						Telium_Sprintf(&cardData[3*i], "%02X ", rspnSqn[i]);
				}
			}
		}

//		if (portCam != NULL) {
//			Telium_Status(hCam, &ucStatus);
//			if ((ucStatus & CAM_PRESENT) != 0) {
//				magCardPresent = TRUE;
//			}
//		}

		if(swipeStatus & SWIPE31){
			memset(dataRead, 0, sizeof dataRead);
			statusIso = Telium_Is_iso1(hMag31, &decodeCount, (byte*)dataRead);
		}

		if(swipeStatus & SWIPE2)
		{
			memset(dataRead, 0, sizeof dataRead);
			statusIso = Telium_Is_iso2(hMag2, &decodeCount, (unsigned char*)dataRead);
			if(statusIso < DEF_SEP) {
				source = dataRead;
				dest = trackNo;
				while(*dataRead) {
					if(*source++ == 'B')
						break;
				}
				while(*source) {
					if(*source == 'F')
						break;
					if(*source == 'D')
						break;
					if(*source == '?')
						break;
					*dest++ = *source++;
				}
			}
			strncat(cardData, trackNo, 128);

		}

		if(swipeStatus & SWIPE3)
		{
			memset(dataRead, 0, sizeof dataRead);
			statusIso = Telium_Is_iso3(hMag3, &decodeCount, (byte*)dataRead);
		}

		if(statusIso == ISO_OK){
			magCardPresent = TRUE;
		}else {
			GL_Dialog_Message(gGoalGraphicLibInstance, NULL, "Card not Swiped/ Inserted properly" , GL_ICON_ERROR, GL_BUTTON_VALID_CANCEL, 5*1000);
			magCardPresent = FALSE;
		}

		Telium_Fclose(hMag31);
		Telium_Fclose(hMag3);
		Telium_Fclose(hMag2);
		Telium_Fclose(portCam);

	}
}

//Retrieve card information
int retrieveData(char *trackNo, char *source, char *dest ){
	int trcksNo = 0;
	byte len = 0;
	char temp[128];
	Telium_File_t *hMag31=NULL, *hMag2=NULL, *hMag3=NULL;

	memset(temp, 0, sizeof temp);
	trcksNo = Telium_Is_iso2(hMag2, &len, (byte*)temp);
//	if(trcksNo < DEF_SEP) {
//		source = temp;
//		dest = trackNo;
//		while(*source) {       //find start sentinel
//			if(*source++ == '%')
//				break;
//		}
//		while(*source) {       //copy all data between start and end sentinels
//			if(*source == '?')
//				break;
//			*dest++ = *source++;
//		}
//	}
	return trcksNo;
}
