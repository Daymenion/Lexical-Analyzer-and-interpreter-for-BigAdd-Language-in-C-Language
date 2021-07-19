#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// kullaniciya basilacak tek bilgi out termleri ya da hatalar olacaktir.

char *variableList[100]; int vSize = 0; //degıskenlerımızı tutacagımız array ve kac tane oldugunu tuttuğumuz değişken
int integerList[100] = { 0 }; int iSize = 0; //ıntegerllarımızı tutacagımız array ve kaç tane olduğunu tuttuğumuz değişken
char *stringVariableList[100] = {0}; int sVSize = 0;
char *stringList[100] = {0}; int sSize = 0;
char *KEYWORDS[9] = {"int", "move", "to", "loop", "times", "out", "from", "add", "sub"};
FILE *file;

// verilen stringin dogru bir integer deger olup olmadığını kontrol eder
bool isInteger(char* str) {

	unsigned long length = strlen(str); // stringin boyunu tutacak ve for icerisinde string icinde kontrolerimi yapacagim

	if (length > 100) { //stringin boyu 100'den büyükse otamatik false dönmeli
		return false;
	}

	for (int i = 0; i < length; i++) {
		if (i > 0)
			if (str[i] == '-') //0 dan sonraki indexde eksi isareti varsa hatalidir
				return false;
		if (!(isdigit(str[i]) || str[i] == '-')) //sayisal eleman degilse yada eksi isareti kullaniliyorsa hatalidir
			return false;
	}

	return true; //for dongusu icerisinden hatali donus yapilmamissa sayisal bir degerdir
}

// daha önceden tanımlanmıs bır ınteger variable mi degıl mi onu kontrol ediyoruz.
bool isIntegerVariable(char* str) {

	for (int i = 0; i < vSize; i++)
		if (!strcmp(str, variableList[i])) //stringimiz variablelist içerisinde varsa, true dönüyoruz.
			return true;

	return false; //eğer yukarıda true donmemişse o zman false donebiliriz
}

// daha önceden tanımlanmıs bır string mi degil mi onu kontrol ediyoruz.
bool isStringVariable(char* str) {

    for (int i = 0; i < sVSize; i++)
        if (!strcmp(str, stringVariableList[i])) //stringimiz variablelist içerisinde varsa, true dönüyoruz.
            return true;

    return false; //eğer yukarıda true donmemişse o zman false donebiliriz
}

// geçerli bir variable olup olmadığını kontrol ediyoruz.
bool isVariable(char* str) {
	unsigned long length = strlen(str);

	if (length > 20)  //uzunlugu 20 den buyukse false donuyor
		return false;

	for (int i = 0; i < length; i++) {
		if (str[0] == '_') //alttan cızgı ıle baslayamaz
			return false;

		if (!((str[i] >= 'a' && str[i] <= 'z') || (str[i] >= 'A' && str[i] <= 'Z') || (str[i] == '_')))
			return false; //if içerisindeki elemanlardan birisi değilse false dönüyor (başında değil işareti var)
	}
	return true; // eğer yukarıda false dönmüyorsa tru dönebiliriz
}

// verilen stringin keyword olup olmadıgını kontrol eder.
bool isKeyword(char* str) {
    for (int i = 0; i < 9; i++)
        if (!strcmp(str, KEYWORDS[i]))
            return true;

    return false;
}

//endofline ifadesinin dogru olup olmadigini kontrol eder
void endOfLineControl (int wordCount, int wanted) {
    if (wordCount != wanted) {
        printf("End of line is expected.\n");
        exit(0);
    }
}

// bu fonksiyonumuzun amaci kullanicidan alinacak sayisal degerin isleme sokulmasi ve hata kontrolleridir
int integerOrVariable (char *word[100], char *str) {
    //Eğer 1. ındeximiz null ise Integer ya da Variable expected hatası basıp cıkıyoruz
    int Int = 0;
    if (word[1] == NULL) {
        printf("Integer or variable are expected.\n");
        exit(-6);
    }
    else if (isKeyword(word[1])) { //varıable ya da ınteger vermesı gerekıyorken keyword verırse hata basıp cıkıyoruz
        printf("'%s' is a keyword. Integer or variable are expected.\n", word[1]);
        exit(-6);
    }
    else if (isInteger(word[1])) //ınteger verilmiş ise moveınt degıskenimize atıyoruz
        Int = atoi(word[1]);

    else if (isIntegerVariable(word[1])) { // eğer daha onceden tanımlanmıs bır varıable ise

        for (int j = 0; j < vSize; j++) {
            if (!strcmp(variableList[j], word[1])) {
                Int = integerList[j]; //variablin değerini moveinte veriyoruz
                break;
            }
        }
    }
    else { //ınteger ya da variable verilmemişse hata çıktımızı verip çıkıyoruz
        printf("'%s' is not a integer or variable. Integer or variable are expected.\n", word[1]);
        exit(-6);
    }

    //move ve add için to kontrolu
    if(!strcmp(str,"m") || !strcmp(str,"a")) {
        //değerimiz ve ondan sonra gelen to yoksa hatamızı basıp çıkıyoruz
        if (word[2] == NULL) {
            printf("Keyword 'to' is expected.\n");
            exit(-7);
        } else if (strcmp(word[2], "to") != 0) {//todan başka bir şey verilmiş ise hatamızı basıp çıkıyoruz
            printf("Keyword 'to' is expected.\n");
            exit(-7);
        }
    }

    // sub için from kontrolu
    else if(!strcmp(str,"s")) {
        //degerimizden ve ondan sonra gelen from yoksa hatamizi basip cikiyoruz
        if (word[2] == NULL) {
            printf("Keyword 'from' is expected.\n");
            exit(-8);
        } else if (strcmp(word[2], "from") != 0) {//fromdan baska bir sey verilmis ise hatamizi basip cikiyoruz
            printf("Keyword 'from' is expected.\n");
            exit(-8);
        }
    }

    // loop için times kontrolü
    else if(!strcmp(str,"l")) {
        //Times
        if (word[2] == NULL) {
            printf("Keyword 'times' is expected.\n");
            exit(-10);
        }
        else if (!strcmp(word[2], "times"))
            return Int;

        else {
            printf("Keyword 'times' is expected.\n");
            exit(-10);
        }
    }

    if (word[3] == NULL) {
        printf("Variable is expected.\n");
        exit(-5);
    }
    else if (isKeyword(word[3])) {
        printf("'%s' is a keyword. Variable is expected.\n", word[3]);
        exit(-5);
    }
    else if (isIntegerVariable(word[3]))
        return Int;
    else {
        printf("'%s' is not a variable. Variable is expected.\n", word[3]);
        exit(-5);
    }
}

// code'umuzun temel okuma islemlerinin yapildigi fonksiyon
void wordParsing(char* code) {
    int lineCount = 0;
	char *line[1000]; //lineları tutacağımız array
	char *splitCode = strtok(code, "."); // end of line karakterlerimize göre stringi ayırıyoruz
	while (splitCode != NULL) {
		line[lineCount++] = splitCode;
		splitCode = strtok(NULL, "."); //splitcode içerisinde ilerleyerek line arrayine atıyoruz
	}

	for (int lineIndex = 0; lineIndex < lineCount; lineIndex++) {
		int wordCount = 0;
		char *word[1000];
        memset(word, 0, sizeof word);
		char *splitLine = strtok(line[lineIndex], " \n\r[]\"");
		while (splitLine != NULL) {
			word[wordCount++] = splitLine; //ayrılan kelimeleri de word arrayi içerisine atıyoruz
			splitLine = strtok(NULL, " \n\r"); //line içerisinde girilen delimeterlara göre ilerliyoruz
		} //bunlarda boşluk, satır atlama, return key gibi ya da parantez açma kapama tırnak işaretine göre kelime kelime ayırmaya çalışıyoruz

		if (wordCount == 0) {
            printf("No word to read ");
		    exit(-4);
		} //herhangi bir kelime yoksa 0 dönebiliriz

		//Int tanımlanıyorsa
		if (!strcmp(word[0], "int")) {

			//Int'dan sonra Variable gelmeli, gelmezse hatalı
			if (word[1] == NULL) {
				printf("Variable is expected.\n");
                exit(-5);
			}// int'dan sonra keyword gelirse hatalı
			else if (isKeyword(word[1])) {
				printf("'%s' is a keyword. Variable is expected.\n", word[1]);
                exit(-5);
			} // variable gelirse'de kullanıcıya basılması  ve arraylerimize atanması
			else if (isVariable(word[1])) {
                if (!isIntegerVariable(word[1])) {
                    variableList[vSize++] = word[1];
                    integerList[iSize++] = 0;
                }
                else{
                    printf("%s is defined before!",word[1]);
                    exit(-12);
                }
			}
			else { // eğer isvariable değilse hatamızı basıp çıkıyoruz
				printf("'%s' is not a variable. Variable is expected\n", word[1]);
                exit(-5);
			}
            //dogru tanimlamada 2. indeximiz endofline olmali
            endOfLineControl(wordCount, 2);
		}

		//Move
		else if (!strcmp(word[0], "move")) {

            int moveInt = integerOrVariable(word,"m");

            for (int j = 0; j < vSize; j++) { //variable listesini dolaşıyoruz, bize verilen variablemizi bulup
                if (!strcmp(variableList[j], word[3])) { //o variablemizi move ınt değişkeninde tuttuğumuz değeri atıyoruz
                    integerList[j] = moveInt;
                    break; //bulduktan sonra dolaşmamıza devam yok
                }
            }
            // dogru tanimlamada 4. indeximiz end of line
            endOfLineControl(wordCount, 4);
		}

		//Add
		else if (!strcmp(word[0], "add")) {
			int addInt = integerOrVariable(word,"a");
			for (int j = 0; j < vSize; j++) {
			    if (!strcmp(variableList[j], word[3])) {
			        integerList[j] += addInt;
			        break;
			    }
			}
            endOfLineControl(wordCount, 4);
		}

		//Sub
		else if (!strcmp(word[0], "sub")) {
			int subInt = integerOrVariable(word,"s");
            for (int j = 0; j < vSize; j++) {
                if (!strcmp(variableList[j], word[3])) {
                    integerList[j] -= subInt;
                    break;
                }
            }
			//End Of Line
            endOfLineControl(wordCount, 4);
		}

		//Out
		else if (!strcmp(word[0], "out")) {
			//Integer, Variable Or String
			if (word[1] == NULL) {
				printf("Integer, variable or string are expected.\n");
                exit(-9);
			}
            else if (isStringVariable(word[1])) {
                for (int j = 0; j < sSize; j++) {
                    if (!strcmp(stringVariableList[j], word[1])) {
                        printf("%s", stringList[j]);
                        break;
                    }
                }
                endOfLineControl(wordCount, 2);
            }
			else if (isInteger(word[1])) {
                printf("%s", word[1]);
            }
			else if (isIntegerVariable(word[1])) {
				for (int j = 0; j < vSize; j++) {
					if (!strcmp(variableList[j], word[1])) {
						printf("%d", integerList[j]);
						break;
					}
				}
                if(word[2] == NULL){
                    endOfLineControl(wordCount,2);
                }
                else if(!strcmp(word[2],",")){
                    if(!strcmp(word[3],"newline")){
                        printf("\n");
                        endOfLineControl(wordCount, 4);
                    }
                    else {
                        printf("newline expected\n");
                        exit(-13);
                    }
                }
                else {
                    printf("Seperator expected");
                    exit(-11);
                }
			}
			else if (!strcmp(word[1], "newline")) {
                if (word[2] == NULL) {
                    printf("\n");
                } else if (!strcmp(word[2], ",")) {
                    if (isStringVariable(word[3])) {
                        for (int j = 0; j < sSize; j++) {
                            if (!strcmp(stringVariableList[j], word[3])) {
                                printf("%s", stringList[j]);
                                break;
                            }
                        }
                    }
                    if (word[4] == NULL);
                    else if (!strcmp(word[4], ",")) {
                        if (isIntegerVariable(word[5])) {
                            for (int j = 0; j < vSize; j++) {
                                if (!strcmp(variableList[j], word[5])) {
                                    printf("%d\n", integerList[j]);
                                    break;
                                }
                            }
                            endOfLineControl(wordCount, 6);
                        }
                        else {
                            printf("'%s' is not a variable. Variable are expected.\n", word[5]);
                            exit(-5);
                        }
                    }
                    else {
                        printf("\nSeperator is expected!\n");
                        exit(-11);
                    }
                }
                else{
                    printf("\nSeperator is expected!\n");
                    exit(-11);
                }
            }
                //Dilde tanımlanmamış bir girdi ise
            else {
                printf("\n%s\n",word[1]);
                printf("%s is Undefined word for after out !!!\nInteger, variable or string are expected.\n", word[1]);
                exit(-9);
            }
		}

		//Loop
		else if (!strcmp(word[0], "loop")) {

			//Integer Or Variable
			int loopInt = integerOrVariable(word,"l");

			//looop içerisinde yer alan Code'u birlestiriyoruz ve
			// looop miktarinca onu wordparsinge gonderip kodun gorevini yaptiriyoruz
            int atNow = lineIndex;
            char newCodes[100] = "";
            int length=0;
            for (int j = 3; j < wordCount; j++) {
                strcat(newCodes, " ");
                strcat(newCodes, word[j]);
            }
            strcat(newCodes, ".");
            if(strcmp(word[3],"[") != 0) {
                for (int k = 0; k < loopInt; k++) {
                    char tempcodes[100];
                    strcpy(tempcodes, newCodes);
                    wordParsing(tempcodes);
                }
            }
            else {
                for (int a = atNow + 1; a < lineCount; a++) {
                    int wordCounts = 0;
                    char *words[1000];
                    memset(words, 0, sizeof words);

                    if (line[a][1] == ']')
                        break;
                    else {
                        strcat(newCodes, "\n");
                        length++;
                    }
                    char *splitLines = strtok(line[a], " [");
                    while (splitLines != NULL) {
                        words[wordCounts++] = splitLines; //ayrılan kelimeleri de word arrayi içerisine atıyoruz
                        splitLines = strtok(NULL, " ["); //line içerisinde girilen delimeterlara göre ilerliyoruz
                    } //bunlarda boşluk, satır atlama, return key gibi ya da parantez açma kapama tırnak işaretine göre kelime kelime ayırmaya çalışıyoruz
                    for (int j = 0; j < wordCounts; j++) {
                        strcat(newCodes, " ");
                        strcat(newCodes, words[j]);

                        if (j == wordCounts - 1) {
                            strcat(newCodes, ".");
                        }
                    }
                }
                lineIndex += length;
                //line arrayini atip belki onun icerisinde wordparsingte yaptigim islemi yaparak
                // loop icerisinde birden fazla satiri almayi basaririm
                for (int k = 0; k < loopInt; k++) {
                    char tempcodes[100];
                    strcpy(tempcodes, newCodes);
                    wordParsing(tempcodes);

                }
            }


            //End Of Line
			if (wordCount <= 4) {
                printf("End of line is expected.\n");
                exit(0);
			}
		}

		//Dilde tanımlanmamış bir girdi ise
		else {
			printf("%s , Undefined word for this Language!!!", word[0]);
            exit(404);
		}
	}
    //process complete
}

//source stringimizi alt bir stringe ayiriyor CC StackOverFlow
int substring(const char *source, int from, unsigned long n, char *target) {
    int length, i;
    for (length = 0; source[length] != '\0'; length++); // source stringimizin lengthi
    if (from > length) {
        printf("Starting index is invalid.\n");
        return 1;
    }
    if ((from + n) > length) {
        n = (length - from);
    }
    for (i = 0; i < n; i++) {
        target[i] = source[from + i];
    }
    target[i] = '\0';
    return 0;
}

char commentLine[100]; char a[1000]; char b[1000]; //alttaki fonksiyonumuz da ise yarayacaklar
char stringLine[100]; char a2[1000]; char b2[1000];

//stringimizi ve commentimizi bulup codeumuzu ona gore ayirmamizi saglayan sey
void isCommentOrString(char* str) {
	bool trueComment = false, trueString = false;
	int j;
	for (int i = 0; i < strlen(str); i++) { //kodumuzun icerisinde ilerliyoruz
		if (str[i] == '{') { // acma ayracimizi bulduktan sonra kodda ilerleyip kapama ayracimizi bulmaya calisiyoruz
            for (j = i; j < strlen(str); j++) {
				if (str[j] == '}') { // kapama ayracimizi da bulunca var olan kodumuzu alt stringlere ayirip komut satirindan ayrsitiracagiz
					substring(str, i + 1, j - i - 1, commentLine); //parantezler arasinda kalan yeri commentline a atiyoruz

					substring(str, 0, i, a);
					substring(str, j + 1, strlen(str), b);
					strcat(a, b);
					strcpy(str, a);

					trueComment = true;
                    isCommentOrString(str);
				}
			}
			if (!trueComment) {
				printf("Comment Line is not completed.\n");
				exit(-3); //comment line hatasi -3 hata kodu ile cikar
			}
		}
		else if (str[i] == '"') { //comment icin yaptigimizin aynisini yapiyoruz
            for (j = i+1; j < strlen(str); j++) {
                if (str[j] == '"') {
                    substring(str, i + 1, j - i - 1, stringLine);
                    char iNum[10] = {0};
                    char temp[100] = {0};
                    substring(str, 0, i, a2);
                    substring(str, j + 1, strlen(str), b2);

                    itoa(sSize, iNum, 10);
                    strcat(iNum, "str");

                    stringVariableList[sVSize++] = iNum;
                    strcpy(temp,stringLine);
                    stringList[sSize++] = temp;


                    strcat(a2, iNum );
                    strcat(a2, b2);
                    strcpy(str, a2);

                    trueString = true;
                    isCommentOrString(str);
                }
            }
            if (!trueString) {
                printf("String line is not completed.\n");
                exit(-3); //string line hatasi comment line gibi -3 hata kodu ile cikar
            }
        }
	}
}

//baskadaki ve sondaki bosluklari siler
char *trimwhitespace(char *str) {
    char *end;
    while(isspace((unsigned char)*str)) str++;
    if(*str == 0)
        return str;
    end = str + strlen(str) - 1;
    while(end > str && isspace((unsigned char)*end)) end--;
    end[1] = '\0';
    return str;
}

int main() {

	long lSize;
	char *code;

    char* filename = malloc(100);
    l:
    printf("Please Enter Source File With it's Extention (exmp: input.ba) :");
    fgets(filename, 100 , stdin);

    filename = trimwhitespace(filename);
    file = fopen(filename,"rb+");
    //ayrac kullanilarak kullanicidan dosya adi isteniyor eger istenmeyen bir sekilde ya da yanlis bir isimle girerse
    //kullanicidan tekrar girilmesi isteniyor, kullanici dogru girene kadar goto ile bu dongu devam ediyor
    if(file == NULL)  {
        printf("\nSorry! File couldn't open \nPlease make sure the file name is correct\n\n");
        goto l;
    }
    else
        printf("\nFile Opened For Reading...\n\n");

	fseek(file, 0L, SEEK_END);
	lSize = ftell(file);
	rewind(file);

	code = calloc(1, lSize + 1);
	if (!code) {
	    fclose(file); fputs("memory alloc fails", stderr); exit(-1);
	}

	if (1 != fread(code, lSize, 1, file)) {
		fclose(file); free(code); fputs("entire read fails", stderr); exit(-2);
	}

    isCommentOrString(code);
	wordParsing(code);
    printf("\nProcess complete!");
    exit(1);
}
/*       Çıkış kodlarının çizelgesi
 * 1) Doğru çalışma çıkış kodudur.
 * 0)End of line expected hatası çıkış kodudur
 * -1)Dosyanın memory allocete hatasının çıkış kodudur
 * -2)Dosyanın Okunamama hatasının çıkış kodudur
 * -3)Yorum satırınının ya da stringin tamamlanmaması hatasının çıkış kodudur.
 * -4)Dosya içerisi okunabiliyor ama kelime bulunmuyorsa bu çıkış kodu verilir
 * -5)Variable is expected hatasının çıkış kodudur.
 * -6)Integer or Variable is expected hatasının çıkış kodudur
 * -7)Keyword "to" is expected hatasının çıkış kodudur
 * -8)keyword "from" is expected hatasının çıkış kodudur
 * -9)Out'da yazılacak verinin olmaması hatasıdır
 * -10)keyword "times" is expected hatasının çıkış kodudur
 * -11)Seperator is expected in out method
 * -12) daha onceden tanimlanmis bir variableyi tekrar tanimlanmaya calisilirsa
 * -13) newline expected
 * 404)Dilde Tanımlanmamış bir girdinin verilmesi
 */