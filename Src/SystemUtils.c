char intToChar( int val){
    return val;
}

int checkChar( char chartr ){
    int hex ;
    switch (chartr) {
        case '0':
            hex = 0;
            break;
        case '1':
            hex = 1;
            break;
        case '2':
            hex = 2;
            break;
        case '3':
            hex = 3;
            break;
        case '4':
            hex = 4;
            break;
        case '5':
            hex = 5;
            break;
        case '6':
            hex = 6;
            break;
        case '7':
            hex = 7;
            break;
        case '8':
            hex = 8;
            break;
        case '9':
            hex = 9;
            break;
        case 'A':
            hex = 10;
            break;
        case 'a':
            hex = 10;
            break;
        case 'B':
            hex = 11;
            break;
        case 'b':
            hex = 11;
            break;
        case 'C':
            hex = 12;
            break;
        case 'c':
            hex = 12;
            break;
        case 'D':
            hex = 13;
            break;
        case 'd':
            hex = 13;
            break;
        case 'E':
            hex = 14;
            break;
        case 'e':
            hex = 14;
            break;
        case 'F':
            hex = 15;
            break;
        case 'f':
            hex = 15;
            break;
        default:
            hex =15;
    }
    return hex;
}

char charToHex(char *hex){
	char charVal;
	int val = 0;
	int hex1, hex2;
	hex1 = checkChar(hex[0]);
	hex2 = checkChar(hex[1]);
	val = (hex1 * 16) + hex2;
	charVal = intToChar(val);
	return charVal;
}
