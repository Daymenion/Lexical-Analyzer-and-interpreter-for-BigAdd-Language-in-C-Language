#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>
#include <wchar.h>

int line_number=1;
static int lexeme_types[6]={0}; //number of identifiers ,paranthesis,keywords,eol characters,errors,
// total lexeme number, lexemes ve keywords ler main içerisinde tanımlandı


void string_writing(FILE *rfptr, FILE *wfptr, unsigned ch) {///tirnak isaretinden sonrasini okuyacak olan fonksiyondur
    int i = 0, max_lenght = 100;
    ch= fgetc(rfptr) ;
    unsigned char* string = malloc(max_lenght); //bellek program cokmemeli

    while (ch != '"' && ch!=EOF) {
        if(ch == '\n')
            line_number++;
        if(i <100) {
            *(string + i) = ch;
            i++;
            ch = fgetc(rfptr);
        }
        else { ///100 u astigindan dolayi gecerli bir string degildir, o yuzden atama islemi devam etmicek
            i++;
            ch = fgetc(rfptr); //dosya icerisinde ilerlemek icin
        }
    } //dosyadan karakterlerin okunup stringe atanmasi ve strigin diğer dosyaya yazilmasi

    *(string+i)='\0';
    if(i>100) {
        lexeme_types[4]++;
        printf("String is exceeding defined memory limit at Line: %d \n",line_number);
    }
    if(i<=100 ) { //100 boyutunu aşmamış ve son karakterimiz EOf olarak yazılmıssa dosyaya yazılması
        if(ch!=EOF) {
            fprintf(wfptr, "String constant (%s)\n", string);
            lexeme_types[5]++;
        }
        else{ //eğer öyle değilse hata olarak verilmektedir
            lexeme_types[4]++;
            printf("unmatched (\") character at Line: %d \n",line_number);
        } //yapisi geregi ayni line da olmasi gerektigini dusundum, satir atlamayi hesaba katmadim bu yuzden
    }

    free(string);
}


void char_detecting(int ch, FILE *rfptr, FILE *wfptr) {/// Gonderilen char karakterinin parantez ya da ayirici olup olmadigini bulan fonksiyondur
    if(!(isalnum(ch)) && ch != '-')
        switch (ch) {

            case '-':case ' ':case'\t': break;

            case '\n': line_number++; break;

            case ',':
                fprintf(wfptr,"Seperator ','\n");
                lexeme_types[5]++;
                break;

            case '"':
                string_writing(rfptr,wfptr,ch);/// hocam vermis oldugunuz tirnak isareti turkce bir karakter olup
                ///C dilinde herhangi bir karsiligi yok bu yuzden international tirnak isaretini kullaniyorum.
                break;

            case '{':
                while ((ch = fgetc(rfptr)) != '}') {
                    if(ch == '\n')
                        line_number++;
                }
                break;

            case '}': //esleniksiz basılan bir kapatma parantezi olursa
                printf("Close Comment paranthesis with no start Line:%d\n",line_number);
                lexeme_types[4]++;
                break;

            case '[':
                fprintf(wfptr,"Open bracket paranthesis\n");
                lexeme_types[1]++;
                break;

            case ']':
                fprintf(wfptr,"Close bracket paranthesis\n");lexeme_types[1]++;
                break;

            case '.':
                fprintf(wfptr,"End of line character\n");
                lexeme_types[3]++;
                break;

            default:
                if(ch=='_') {
                    fprintf(wfptr,"Underscore character\n");
                }
                    //bu kısımlar çok açık olduğu için yorum satırı koymadım
                else {
                    lexeme_types[4]++;
                    wprintf(L"Unrecognized Character (%c) Line : %d\n",ch,line_number);
                }
                ch=fgetc(rfptr);
                if(isalnum(ch)) {
                    printf("Invalid identifier word Line : %d\n",line_number);
                    lexeme_types[4]++;
                    while(isalnum(ch) || ch=='_')
                        ch=fgetc(rfptr);
                }
                if(ch!=EOF)
                    fseek(rfptr,-1,SEEK_CUR);
        }
}

int isAllDigit(char* word,FILE *rfptr,FILE* wfptr) {
    //gönderilen kısmın hepsinin nümeric bir değer olup olmadığını kontrol eder
    int i = 0;
    while(word[i]) {
        if(isdigit(word[i])== 0) { //ısdıgıt fonk. aldığı input numeric değilse 0 değerini verir
            char_detecting(word[i], rfptr, wfptr);
            return 0;
        } // bu if statementine girmişşe gonderilen kısım içerisinde sayısal olmayan değer vardır
        i++;
    }
    //eğer yukarıda 0 döndürülmemişşe hepsi sayı demektir
    return 1;
}


void putIntConst(char* word,int minus,FILE* rfptr,FILE* wfptr) {
//sayısal kısımlar const olarak resultımıza ekleyeceğimiz kısımdır.isAllDigit dan gelen kısımdaki bütün değerler sayıysa if içine girer
    if(isAllDigit(word,rfptr,wfptr)) {
        if(minus==1) {
            fprintf(wfptr,"int constant (%s)\n",word-1);
            lexeme_types[5]++;
        } //negatif ise sayının önündeki işaretle beraber, pozitifse direkt sayının kendisi dosyaya yazılır.
        else {
            fprintf(wfptr,"int constant (%s)\n",word);
            lexeme_types[5]++;}
    }
    else { //isAllDigit dan gelen değer 0 ise ve üstteki ife girmeşşe gönderilen sayı olduğu varsayılan
        //değerin arasına başka charlarda girmiş demektir, bu durumda da hatamız yazılır.
        if(minus == 1)
            printf("Invalid identifier word (%s) line:%d\n",word-1 ,line_number);
        else
            printf("Invalid word (%s) line:%d\n",word ,line_number);
        lexeme_types[4]++;
    }

}

int word_parsing(FILE * rfptr,char* word,char **keywords,FILE* wfptr) ///verilen kelimenin parcalanmasini ve yanlislarin bulunmasini saglar
{
    if(isdigit(*word) || *word == '-') { //numerik bir değerse yada eksi işareti ise girecektir
        if(*word == '-')
            if(isdigit(*(word+1))) //eksi işareti ise eksiden sonraki değer sayı ise putIntConsta yolluyoruz
                putIntConst(word+1,1,rfptr,wfptr);
            else  {
                int a=0; //eğer eksi ile başlıyor ama ondan sonra sayısal değer gelmiyorsa buraya girer
                while(word[a])  {
                    if(word[a] == '-')  { //eksi ile başlamamsına rağmen tekrar kullanılıyorsa hata verir
                        a++;
                        printf("\n%s is not a valid integer \nInvalid minus sign usage Line : %d\n",word,line_number);
                        lexeme_types[4]++;
                    }
                    else  { //eksi işaretinden başka bir karakterse char_detectinge yollanıp ne olduğu anlaşılmaya çaşlışılır
                        char_detecting(word[a], rfptr, wfptr);
                    }
                    a++;
                }
            }
        else // - işareti ile başlamadığı durumdur oyleyse pozitif bir sayıdır direkt putIntConsta yollayabilirim
            putIntConst(word, 0, rfptr, wfptr);
    }

    else  {
        int iskeyword = 0, i;
        for(i=0; i<10; i++) //keyword olup olmadığı kontrol edilir
        {
            char *key = keywords[i];
            if( strcmp(key,word) == 0)  {
                fprintf(wfptr,"keyword '%s'\n",keywords[i]);
                lexeme_types[2]++;
                iskeyword=1;
                break;
            }
        }
        if(!iskeyword)  { //keyword değilse identifier dır diyoruz ama sayı ya da alt cizgi iceriyorsa hatalı bir identifierdır.
            //Sayı ile baslamasını ele almıyoruz cunku baslasa bile o bir sayısal değer olarak düşünülür ve buraya kadar gelemez.
            if(isdigit(word[0]) || word[0] == '_')  {
                printf("Invalid identifier token (%s) line%d\n",word,line_number);
                lexeme_types[4]++;
                return 0;
            }
            else{ //sayi ile yada '_' ile baslamiyorsa identifierdir
                int isidentifier=1;
                for(i=0;i<strlen(word);i++)  { //stringin icerisinde dolasip onun icinde alphanumeric olmayan bir ve '_' de olmayan bir karakter bulursak
                    //onu char detectinge yollayip Seperator olup olmadigini kontrol ediyoruz ve isidentifier i da 0 yapiyoruz
                    //bu sayede hem yanlis kullanilmis separator varsa onu da yakalamis olacagiz
                    if((!isalnum(word[i]) && word[i] != '_' ))  {
                        isidentifier=0;
                        char_detecting(word[i], rfptr, wfptr);
                    }
                }
                if(isidentifier==1)  { //eger yukaridaki if statementina girmemisse yani o bir identifier ise artik onu artik dosyaya yazabiliriz
                    fprintf(wfptr,"Identifier '%s'\n",word);
                    lexeme_types[0]++;
                }
                else  { //o yanlis girilmis bir identifierdir.
                    printf("Invalid indentifier Token '%s' Line : %d\n",word,line_number);
                    lexeme_types[4]++;
                }
            }
        }
    }
    return 1;//return ifadesinin bir anlami yoktur, identifier da hata alirsa devam etmemesi icin gerekliydi
}

char *trimwhitespace(char *str)
{
    char *end;

    // Trim leading space
    while(isspace((unsigned char)*str)) str++;

    if(*str == 0)  // All spaces?
        return str;

    // Trim trailing space
    end = str + strlen(str) - 1;
    while(end > str && isspace((unsigned char)*end)) end--;

    // Write new null terminator character
    end[1] = '\0';

    return str;
}

int main() {

    FILE* file;
    FILE* writen;

    char* lexemes[]={"Identifiers","Paranthesis","Keywords","EOL Characters","Errors","Lexemes"};
    char* keywords[]={"move","add","sub","out","loop","to","from","times","int","newline"};

    setlocale(LC_ALL,"Turkish");
    //keywordslerimizi ve lexemelarimi tanimliyoruz ve dosyaya yazmada hata cikmamasi icin local dili turkceyi tanimliyoruz.

    char* filename = malloc(100);
    l:
    printf("Please Enter Source File With it's Extention (exmp: input.ba) : ");
    fgets(filename, 100 , stdin);

    filename = trimwhitespace(filename);
    file = fopen(filename,"r");
    //ayrac kullanilarak kullanicidan dosya adi isteniyor eger istenmeyen bir sekilde ya da yanlis bir isimle girerse
    //kullanicidan tekrar girilmesi isteniyor, kullanici dogru girene kadar goto ile bu dongu devam ediyor
    if(!strstr(filename, ".ba")) {
        printf("\nThis is Not a Big Add language file \nPlease make sure it is Big Add language file\n\n");
        goto l;
    }
    if(file == NULL)  {
        printf("\nSorry! File couldn't open \nPlease make sure the file name is correct\n\n");
        goto l;
    }
    else
        printf("\nFile Opened For Reading...\n\n");


    char ch;
    int chrForward; //Dosyadan veri okunurken bunlar stringlere atanacak okunan verinin ust uste yazilmasinin onune gecmek
    //ve yazilan stringde ilerlemek icin tanimladim, ch'i ise karakterleri tutmak icin

    writen = fopen("AnalysisResult.lx", "w");

    while((ch=fgetc(file))!=EOF)  { //aldigimiz chari char detectinge yolluyoruz ve separator ise ona gore dosyaya yazacagiz
        char_detecting(ch, file, writen);

        if(isalnum(ch) || ch == '-')  { //alpha numarik karakterse ya da eksi isareti ile basliyorsa buraya giriyor, stringimize yer ayiriyoruz
            //ve string icerisinde ilerlememizi saglayacak chrForwardi sifirliyoruz.
            char* ptrword = malloc(100);
            chrForward=0;
            while(ch != ' ' && ch != '\n'&& ch != '\t'&& ch != ',' && ch != '.')  {
                //bosluk satir arasi gibi kelimeyi bolen seylerde while dongusunden cikiyoryz
                *(ptrword+chrForward) = ch;
                chrForward++; //karakteri stringe atayip chrForwardi arttiriyoruz ve dosyadan yeni char aliyoruz
                ch = fgetc(file);
                if(ch == EOF)  { //dosyanin sonuysa break le cikiyoruz
                    break;
                }
            }
            fseek(file,-1,SEEK_CUR); //dosyanin aktif konumunu bir byte geri aliyoruz ki deger atlamayalim
            //ustteki while dongusunde karsimiza virgul gibi bir deger cikarsa bunu char detectinge yollamadan kaybetmis oluruz
            //cunku ilk whilemizde yeni bir karakter aliyoruz her seferinde

            *(ptrword+chrForward) = '\0'; //olusturdugumuz stringi tamamliyoruz ve word_parsinge yolluyoruz
            word_parsing(file,ptrword,keywords,writen);
            free(ptrword); //deallocate ediyoruz ki yeni bir sey tanimlamadan ayni seyi kullanalim

        }
    }


    fclose(file); //filelarimizi kapatiyoruz ve kullanciya bittigine dair bilgi veriyoruz
    fclose(writen);
    for(int i=0; i<4; i++)
        lexeme_types[5] += lexeme_types[i]; //toplam kac lexeme oldugunu hesapliyoruz
    printf("\n");
    for(int i=0; i<6; i++)
        printf(" %s ",lexemes[i]); //lexeme tiplerini ve onun altina da kac tane oldugunu yazdiriyoruz

    printf("\n");

    for(int i=0; i<6; i++)
        printf("% -12d ",lexeme_types[i]);

    printf("\n\nProcess Completed... See You\n\n"); //Proje bitti//

    return 0;
}

