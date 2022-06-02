#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <time.h>
#include<locale.h>
/*Structer definitions  */
struct File{
    char filePath[256];
    char fileName[50];
    char fileExt[10];
};

/*Global variables*/
int TOTALSIZE,SHOWALLCOUNTER=0; //Memory Allocation islemleri icin
char * DESKTOPPATH;
char TIMENOW[50];

/*Function prototypes.*/
int OzelKarakterKontrol(char*array); // Ozel karakter kontrolü
void OzelKarakterDegistir(int length,struct File* files); // Ozel karakterlerin degisimi
void UzantiyaGoreOzelKarakterDegistir(int length,struct File* files,char* extension); // Uzantiya gore Ozel karakterler degisimi
int TurkceKarakterKontrol(char*array); // Turkce karakter kontrolü
void TurkceKarakterDegistir(int length,struct File* files); // Turkce karakterlerin degisimi
void UzantiyaGoreTurkceKarakterDegistir(int length,struct File* files,char* extension); // Uzantiya gore türkçe karakter degisimi
void UzantiyaGoreIsimleriDegistir(int length,struct File*files); // Uzantiya göre isimleri degistirme
void IsmiDegistir(int length,struct File* files); //Direkt belirli dosyanın ismini degistirme
void Sirala(int length,struct File* files); //Sıralama işlemi
int GetLengthJustFolders(const char* filePath); // Sadece dosyaları sayma
int GetLength(const char* fileName); // Alt klasore girmeden sayma
int GetAllSize(const char* filePath); //Alt klasor girerek sayma
void SearchFilesInFile(const char* filePath); //Dosyalar arası gecis
void ShowCurrentFile(const char* filePath); // Dosyanın icindekileri tarama
void ShowAllFiles(const char* filePath); // Ana ShowAll metodu --- Alt dosyalar dahil tarama
void ShowAll(const char* fileName,struct File*files); // Alt klasörlere girme islemi
void ChangeNames(int length,struct File* files); // İsim degistirme islemleri icin


int main(int argc, char *argv[]) {
    int secim;
    //Local time ve masaüstü uzantısını alma işlemleri
    time_t rawtime=time(NULL);
    struct tm* timenow=localtime(&rawtime);
    strftime(TIMENOW,sizeof(TIMENOW),"%x - %I:%M%p",timenow);
    DESKTOPPATH=getenv("USERPROFILE");
    strcat(DESKTOPPATH,"\\Desktop");

    /*struct File file[3];
    strcpy(file[0].fileName,"Argö.jpg");
    strcpy(file[0].fileExt,".jpg");
    strcpy(file[0].filePath,"C:\\Users\\taner\\Desktop\\Odev\\CarImages");
    strcpy(file[1].fileName,"Audi_TT.jpg");
    strcpy(file[1].fileExt,".jpg");
    strcpy(file[1].filePath,"C:\\Users\\taner\\Desktop\\Odev\\CarImages");
    strcpy(file[2].fileName,"Bayon.jpg");
    strcpy(file[2].fileExt,".jpg");
    strcpy(file[2].filePath,"C:\\Users\\taner\\Desktop\\Odev\\CarImages");
    ChangeNames(3,file);*/

    while(1){
        printf("\n\n1-Masaustunu tara\n\n2-Masaustundeki alt klasore girmek icin\n\n3-Dosyanin yoluyla icindekileri tara ");
        printf("\n\n4-Dosyanin yoluyla icindekilerin hepsini tara\n\n5-Cikis yap\n\nSeciminiz: ");
	    scanf("%d",&secim);
	    switch(secim){
            case 1:{
                ShowCurrentFile(DESKTOPPATH);
                system("cls");
                break;
            }
            case 2:{
                SearchFilesInFile(DESKTOPPATH);
                system("cls");
                break;
            }
            case 3:{
                char path[100];
                printf("Ornek path-> C\\:Users\\userName\\Desktop\n\n");
                printf("Bir path giriniz: ");
                scanf("%s",path);
                ShowCurrentFile(path);
                system("cls");
                break;
            }
            case 4:{
                char path[100];
                printf("Ornek path-> C\\:Users\\userName\\Desktop\n\n");
                printf("Bir path giriniz: ");
                scanf("%s",path);
                ShowAllFiles(path);
                system("cls");
                break;
            }
            case 5:{
                exit(0);
            }
        }
    }
	return 0;
}
int TurkceKarakterKontrol(char*array){
    printf("BEFORE->%s\n",array);
    int control=0;
    int size = strlen(array);
    for(int i=0;i<size;i++){
        if(array[i]!="\0"){
            int a = array[i];
            switch(a){
                case -89:{ //ğ
                    array[i]=103;
                    control=1;
                    break;
                }
                case -90:{ //Ğ
                    array[i]=71;
                    control=1;
                    break;
                }
                case -97:{ //ş
                    array[i]=115;
                    control=1;
                    break;
                }
                case -98:{ //Ş
                    array[i]=83;
                    control=1;
                    break;
                }
                case -102:{ //Ü
                    array[i]=85;
                    control=1;
                    break;
                }
                case -103:{ //Ö
                    array[i]=79;
                    control=1;
                    break;
                }
                case -104:{ //İ
                    array[i]=73;
                    control=1;
                    break;
                }
                case -108:{ //ö
                    array[i]=111;
                    control=1;
                    break;
                }
                case -115:{ //ı
                    array[i]=105;
                    control=1;
                    break;
                }
                case -121:{ //ç
                    array[i]=99;
                    control=1;
                    break;
                }
                case -127:{ //ü
                    array[i]=117;
                    control=1;
                    break;
                }
                case -128:{ //Ç
                    array[i]=67;
                    control=1;
                    break;
                }
            }
        }
    }
    printf("AFTER->%s\n",array);
    return control;
}
void TurkceKarakterDegistir(int length,struct File* files){
    FILE * filetxt =fopen("hru_ceng_scanning.txt","a");
    FILE * filedat=fopen("hru_ceng_scanning.dat","a");
    char oldFullName[256];
    char newFullName[256];
    char tempFileName[256];

    for(int i =0;i<length;i++){
        strcpy(oldFullName,(files+i)->filePath);
        strcat(oldFullName,"\\");
        strcat(oldFullName,(files+i)->fileName);
        strcpy(tempFileName,(files+i)->fileName);
        printf("oldFullName-> %s\n",oldFullName);
        if(TurkceKarakterKontrol((files+i)->fileName)!=1){
            continue;
        }
        strcpy(newFullName,(files+i)->filePath);
        strcat(newFullName,"\\");
        strcat(newFullName,(files+i)->fileName);
        printf("newFullName-> %s\n",newFullName);
        if(rename(oldFullName,newFullName)!=0){
            printf("\n\n\t\t\t %s Isimli dosyanin ismi degistirilemedi!\n",tempFileName);
            continue;
            }
            printf("%s isimli dosya %s olarak yeniden adlandirildi.\n",tempFileName,(files+i)->fileName);
        fprintf(filetxt,"%s isimli dosya %s olarak yeniden adlandırıldı\n",tempFileName,(files+i)->fileName);
        fprintf(filedat,"%s isimli dosya %s olarak yeniden adlandırıldı\n",tempFileName,(files+i)->fileName);
    }
    fclose(filetxt);
    fclose(filedat);
    return;
}
void UzantiyaGoreTurkceKarakterDegistir(int length,struct File* files,char * extension){
    FILE * filetxt =fopen("hru_ceng_scanning.txt","a");
    FILE * filedat=fopen("hru_ceng_scanning.dat","a");
    char oldFullName[256];
    char newFullName[256];
    char tempFileName[256];
    for(int i =0;i<length;i++){
        if(strcmp(extension,(files+i)->fileExt)==0){
            strcpy(oldFullName,(files+i)->filePath);
            strcat(oldFullName,"\\");
            strcat(oldFullName,(files+i)->fileName);
            strcpy(tempFileName,(files+i)->fileName);
            printf("oldFullName-> %s\n",oldFullName);
            if(TurkceKarakterKontrol((files+i)->fileName)!=1){
                continue;
            }
            printf("\n\n\n");
            strcpy(newFullName,(files+i)->filePath);
            strcat(newFullName,"\\");
            strcat(newFullName,(files+i)->fileName);
            printf("newFullName-> %s\n",newFullName);
            if(rename(oldFullName,newFullName)!=0){
            printf("\n\n\t\t\t %s Isimli dosyanin ismi degistirilemedi!\n",tempFileName);
            continue;
            }
            printf("%s isimli dosya %s olarak yeniden adlandirildi.\n",tempFileName,(files+i)->fileName);
            fprintf(filetxt,"%s isimli dosya %s olarak yeniden adlandırıldı\n",tempFileName,(files+i)->fileName);
            fprintf(filedat,"%s isimli dosya %s olarak yeniden adlandırıldı\n",tempFileName,(files+i)->fileName);
        }
    }
    fclose(filetxt);
    fclose(filedat);
    return;
}
int OzelKarakterKontrol(char* array){
    int conrtol=0;
    int size = strlen(array);
    for(int i=0;i<size;i++){
        if((array+i)!="\0"){
            int a = array[i];
            switch(a){
                case 32:{
                    array[i]=95;
                    conrtol=1;
                    break;
                }
                case 33:{
                    array[i]=95;
                    conrtol=1;
                    break;
                }
                case 34:{
                    array[i]=95;
                    conrtol=1;
                    break;
                }
                case 35:{
                    array[i]=95;
                    conrtol=1;
                    break;
                }
                case 36:{
                    array[i]=95;
                    conrtol=1;
                    break;
                }
                case 37:{
                    array[i]=95;
                    conrtol=1;
                    break;
                }
                case 38:{
                    array[i]=95;
                    conrtol=1;
                    break;
                }
                case 39:{
                    array[i]=95;
                    conrtol=1;
                    break;
                }
                case 40:{
                    array[i]=95;
                    conrtol=1;
                    break;
                }
                case 41:{
                    array[i]=95;
                    conrtol=1;
                    break;
                }
                case 42:{
                    array[i]=95;
                    conrtol=1;
                    break;
                }
                case 43:{
                    array[i]=95;
                    conrtol=1;
                    break;
                }
                case 44:{
                    array[i]=95;
                    conrtol=1;
                    break;
                }
                case 45:{
                    array[i]=95;
                    conrtol=1;
                    break;
                }
                case 47:{
                    array[i]=95;
                    conrtol=1;
                    break;
                }
                case 58:{
                    array[i]=95;
                    conrtol=1;
                    break;
                }
                case 59:{
                    array[i]=95;
                    conrtol=1;
                    break;
                }
                case 60:{
                    array[i]=95;
                    conrtol=1;
                    break;
                }
                case 61:{
                    array[i]=95;
                    conrtol=1;
                    break;
                }
                case 62:{
                    array[i]=95;
                    conrtol=1;
                    break;
                }
                case 63:{
                    array[i]=95;
                    conrtol=1;
                    break;
                }
                case 64:{
                    array[i]=95;
                    conrtol=1;
                    break;
                }
                case 91:{
                    array[i]=95;
                    break;
                }
                case 92:{
                    array[i]=95;
                    conrtol=1;
                    break;
                }
                case 93:{
                    array[i]=95;
                    conrtol=1;
                    break;
                }
                case 94:{
                    array[i]=95;
                    conrtol=1;
                    break;
                }
                case 96:{
                    array[i]=95;
                    conrtol=1;
                    break;
                }
                case 123:{
                    array[i]=95;
                    conrtol=1;
                    break;
                }
                case 124:{
                    array[i]=95;
                    conrtol=1;
                    break;
                }
                case 125:{
                    array[i]=95;
                    conrtol=1;
                    break;
                }
                case 126:{
                    array[i]=95;
                    conrtol=1;
                    break;
                }
            }
        }
    }
    return conrtol;
}
void OzelKarakterDegistir(int length,struct File* files){
    FILE * filetxt =fopen("hru_ceng_scanning.txt","a");
    FILE * filedat=fopen("hru_ceng_scanning.dat","a");
    char oldFullName[256];
    char newFullName[256];
    char tempFileName[256];
    for(int i =0;i<length;i++){
        strcpy(oldFullName,(files+i)->filePath);
        strcat(oldFullName,"\\");
        strcat(oldFullName,(files+i)->fileName);
        strcpy(tempFileName,(files+i)->fileName);
        printf("oldFullName-> %s\n",oldFullName);
        if(OzelKarakterKontrol((files+i)->fileName)!=1){
            continue;
        }
        strcpy(newFullName,(files+i)->filePath);
        strcat(newFullName,"\\");
        strcat(newFullName,(files+i)->fileName);
        printf("newFullName-> %s\n",newFullName);
        if(rename(oldFullName,newFullName)!=0){
            printf("\n\n\t\t\t %s isimli dosya yeniden adlandirilamadi!",tempFileName);
            continue;
        }
        printf("%s isimli dosya %s olarak yeniden adlandirildi.\n",tempFileName,(files+i)->fileName);
        fprintf(filetxt,"%s isimli dosya %s olarak yeniden adlandırıldı\n",tempFileName,(files+i)->fileName);
        fprintf(filedat,"%s isimli dosya %s olarak yeniden adlandırıldı\n",tempFileName,(files+i)->fileName);
    }
    fclose(filetxt);
    fclose(filedat);
    return;
}
void UzantiyaGoreOzelKarakterDegistir(int length,struct File* files,char * extension){
    FILE * filetxt =fopen("hru_ceng_scanning.txt","a");
    FILE * filedat=fopen("hru_ceng_scanning.dat","a");
    char oldFullName[256];
    char newFullName[256];
    char tempFileName[256];
    for(int i =0;i<length;i++){
        if(strcmp(extension,(files+i)->fileExt)==0){
            strcpy(oldFullName,(files+i)->filePath);
            strcat(oldFullName,"\\");
            strcat(oldFullName,(files+i)->fileName);
            strcpy(tempFileName,(files+i)->fileName);
            printf("oldFullName-> %s\n",oldFullName);
            if(OzelKarakterKontrol((files+i)->fileName)!=1){
                continue;
            }
            strcpy(newFullName,(files+i)->filePath);
            strcat(newFullName,"\\");
            strcat(newFullName,(files+i)->fileName);
            printf("newFullName-> %s\n",newFullName);
            if(rename(oldFullName,newFullName)!=0){
                printf("\n\n\t\t\t %s isimli dosya yeniden adlandirilamadi!",tempFileName);
                continue;
            }
            printf("%s isimli dosya %s olarak yeniden adlandirildi.\n",tempFileName,(files+i)->fileName);
            fprintf(filetxt,"%s isimli dosya %s olarak yeniden adlandırıldı\n",tempFileName,(files+i)->fileName);
            fprintf(filedat,"%s isimli dosya %s olarak yeniden adlandırıldı\n",tempFileName,(files+i)->fileName);
        }
    }
    fclose(filetxt);
    fclose(filedat);
    return;
}
void UzantiyaGoreIsimleriDegistir(int length,struct File*files){
    FILE * filetxt =fopen("hru_ceng_scanning.txt","a");
    FILE * filedat=fopen("hru_ceng_scanning.dat","a");
    char oldFullName[256];
    char newFullName[256];
    char tempFileName[256];
    char extension[10],newName[50];
    printf("Ornek uzanti-> .pdf\nUzantiyi giriniz-> ");
    scanf("%s",extension);
    printf("Dosyalarin yeni ismini giriniz-> ");
    scanf("%s",newName);
    for(int i=0;i<length;i++){
        if(strcmp(extension,(files+i)->fileExt)==0){
            strcpy(oldFullName,(files+i)->filePath);
            strcat(oldFullName,"\\");
            strcat(oldFullName,(files+i)->fileName);
            strcpy(tempFileName,(files+i)->fileName);
            strcpy(newFullName,(files+i)->filePath);
            strcat(newFullName,"\\");
            strcat(newFullName,newName);
            if(rename(oldFullName,newFullName)!=0){
                printf("\n\n\t\t\t %s isimli dosya yeniden adlandirilamadi!",tempFileName);
                continue;
            }
            printf("%s isimli dosya %s olarak yeniden adlandirildi.\n",tempFileName,newName);
        fprintf(filetxt,"%s isimli dosya %s olarak yeniden adlandırıldı\n",tempFileName,newName);
        fprintf(filedat,"%s isimli dosya %s olarak yeniden adlandırıldı\n",tempFileName,newName);
        }
    }
    fclose(filetxt);
    fclose(filedat);
}
void IsmiDegistir(int length,struct File* files){
    FILE * filetxt =fopen("hru_ceng_scanning.txt","a");
    FILE * filedat=fopen("hru_ceng_scanning.dat","a");
    char oldFullName[256];
    char newFullName[256];
    char newName[50];
    char oldName[50];
    printf("Ornek isim->file.txt\nDegistirilicek dosyanin ismini giriniz-> ");
    scanf("%s",oldName);
    printf("Yeni ismi giriniz-> ");
    scanf("%s",newName);
    for(int i =0;i<length;i++){
        if(strcmp((files+i)->fileName,oldName)==0){
            strcpy(oldFullName,(files+i)->filePath);
            strcat(oldFullName,"\\");
            strcat(oldFullName,(files+i)->fileName);
            strcpy(newFullName,(files+i)->filePath);
            strcat(newFullName,"\\");
            strcat(newFullName,newName);
            if(rename(oldFullName,newFullName)!=0){
                printf("\n\n\t\t\t %s isimli dosya yeniden adlandirilamadi!",oldName);
                continue;
            }
        printf("%s isimli dosya %s olarak yeniden adlandirildi.\n",oldName,newName);
        fprintf(filetxt,"%s isimli dosya %s olarak yeniden adlandırıldı\n",oldName,newName);
        fprintf(filedat,"%s isimli dosya %s olarak yeniden adlandırıldı\n",oldName,newName);
        }
    }
    fclose(filetxt);
    fclose(filedat);
}
void Sirala(int size,struct File* files){
    char *temp;
    for(int i=1;i<size;i++)
    {
        for(int j=i+1;j<size;j++)
        {
            if(strcmp((files+i)->fileExt,(files+j)->fileExt)>=0)
            {
                //fileNamelerin yerleri degistiriliyor
                strcpy(temp,(files+i)->fileName);
                strcpy((files+i)->fileName,(files+j)->fileName);
                strcpy((files+j)->fileName,temp);
                //fileExtlerin yerleri degistiriliyor
                strcpy(temp,(files+i)->fileExt);
                strcpy((files+i)->fileExt,(files+j)->fileExt);
                strcpy((files+j)->fileExt,temp);
                //filePathlerin yerleri degistiriliyor
                strcpy(temp,(files+i)->filePath);
                strcpy((files+i)->filePath,(files+j)->filePath);
                strcpy((files+j)->filePath,temp);
            }
            if(strcmp((files+i)->fileName,(files+j)->fileName)>0&&strcmp((files+i)->fileExt,(files+j)->fileExt)==0)
            {
                //fileNamelerin yerleri degistiriliyor
                strcpy(temp,(files+i)->fileName);
                strcpy((files+i)->fileName,(files+j)->fileName);
                strcpy((files+j)->fileName,temp);
                //fileExtlerin yerleri degistiriliyor
                strcpy(temp,(files+i)->fileExt);
                strcpy((files+i)->fileExt,(files+j)->fileExt);
                strcpy((files+j)->fileExt,temp);
                //filePathlerin yerleri degistiriliyor
                strcpy(temp,(files+i)->filePath);
                strcpy((files+i)->filePath,(files+j)->filePath);
                strcpy((files+j)->filePath,temp);
            }
        }
    }
    return;
}
int GetLengthJustFolders(const char* filePath){
    int sayac=0;
   DIR* dir = opendir(filePath);
    if(dir==NULL)
    {
        if("Klasor acılamıyor!\n");
        return 0;
    }
   struct dirent* entity = readdir(dir);
    while(entity!=NULL)
    {
        if(entity->d_type==DT_DIR&&strcmp(entity->d_name,".")!=0&&strcmp(entity->d_name,"..")!=0)
        {
            sayac++;
        }
        entity=readdir(dir);
    }
    closedir(dir);
    return sayac;
}
int GetLength(const char* filePath){
    int sayac=0;
   DIR* dir = opendir(filePath);
    if(dir==NULL){
        printf("Dosya acilamiyor!\n");
        return 0;
    }
    struct dirent* entity=readdir(dir);
    while(entity!=NULL){
        if(strcmp(entity->d_name,".")!=0&&strcmp(entity->d_name,"..")!=0)
        {
            sayac++;
        }
        entity=readdir(dir);
    }
    closedir(dir);
    return sayac;
}
int GetAllSize(const char* filePath){
    int size;
    size = GetLength(filePath);
    TOTALSIZE+=size;
    DIR* dir = opendir(filePath);
    if(dir==NULL){
        return 0;
    }
    struct dirent* entity=readdir(dir);
    while(entity!=NULL){
        if(entity->d_type==DT_DIR&&strcmp(entity->d_name,".")!=0&&strcmp(entity->d_name,"..")!=0){
            char path[256]={0};
            strcat(path,filePath);
            strcat(path,"\\");
            strcat(path,entity->d_name);
            GetAllSize(path);
        }
        entity=readdir(dir);
    }
    closedir(dir);
    return TOTALSIZE;
}
void SearchFilesInFile(const char* filePath){
    system("cls");
    int secim,length,i =0;
    length=GetLengthJustFolders(filePath);
    struct File *files=(struct File*) malloc(length*sizeof(struct File));
    DIR* dir=opendir(filePath);
    if(dir==NULL){
        if("Klasor acılamıyor!\n");
        return;
    }
     struct dirent* entity = readdir(dir);
    printf("Suanki konum-> %s\n\n",filePath);
    while(entity!=NULL&&length>=i){
        if(entity->d_type==DT_DIR&&strcmp(entity->d_name,".")!=0&&strcmp(entity->d_name,"..")!=0)
        {
            printf("%d-Dosya-> %s\n",i+1,entity->d_name);
            strcpy((files+i)->fileName,filePath);
            strcat((files+i)->fileName,"\\");
            strcat((files+i)->fileName,entity->d_name);
            i++;
        }
        entity=readdir(dir);
    }
    closedir(dir);
    printf("\n\n**********************************\n\n");
    printf("(1)-Yukaridaki dosyalardan birine gir\n\n(2)-Suanki konumu tara\n\n(3)-Suanki konumu komple tara\n\n");
    printf("(4)-Masaustu konumuna don\n\n(5)-Geri git\n\n(6)Basa don\n\n Seciminiz:  ");
    scanf("%d",&secim);
    switch(secim){
        case 1:{
            printf("\nGirilicek dosyayi secin: ");
            scanf("%d",&secim);
            char path[256];
            strcpy(path,(files+(secim-1))->fileName);
            free(files);
            SearchFilesInFile(path);
            return;
        }
        case 2:{
            printf("%s konumunun dosyalarina bakiliyor!\n\n",filePath);
            free(files);
            ShowCurrentFile(filePath);
            return;
        }
        case 3:{
            printf("%s konumunun dosyalarina bakiliyor!\n\n",filePath);
            free(files);
            ShowAllFiles(filePath);
            return;
        }
        case 4:{
            free(files);
            SearchFilesInFile(DESKTOPPATH);
            return;
        }
        case 5:{
            free(files);
            return;
        }
    }
}
void ShowCurrentFile(const char* filePath){
    system("cls");
    FILE * filetxt =fopen("hru_ceng_scanning.txt","a");
    FILE * filedat=fopen("hru_ceng_scanning.dat","a");
    int secim;
    if(filedat==NULL||filetxt==NULL)
    {
        printf("Dosya acma hatası!\n");
        return;
    }
    int length =GetLength(filePath);
    struct File *files = (struct File*) malloc(length*sizeof(struct File));
    int i=0;
    char* ext;
    DIR* dir = opendir(filePath);
    if(dir==NULL){
        printf("Klasor acilamiyor!\n");
        return;
    }
    struct dirent* entity=readdir(dir);
    while(entity!=NULL){
        if(strcmp(entity->d_name,".")!=0&&strcmp(entity->d_name,"..")!=0)
        {
            strcpy((files+i)->fileName,entity->d_name);
            strcpy((files+i)->filePath,filePath);
            if(entity->d_type==DT_DIR)
            {
                char * folder={".folder"};
                strcpy((files+i)->fileExt,folder);
            }
            else{
                ext = strrchr(entity->d_name,46);
            }
            strcpy((files+i)->fileExt,ext);
            i++;
        }
    entity=readdir(dir);
    }
    closedir(dir);
    Sirala(length,files);
    for( i=0;i<length;i++)
    {
        printf("Dosyanin icindekiler-> %s\n",(files+i)->fileName);
    }
    fprintf(filetxt,"%s dizini  %s tarihinde taranmistir.\n",filePath,TIMENOW);
    fprintf(filedat,"%s dizini %s tarihinde taranmistir.\n",filePath,TIMENOW);
    fclose(filedat);
    fclose(filetxt);
        printf("\nIsim degistirme islemleri yapmak ister misiniz?\n1-Evet\n2-Hayir\nSeciminiz-> ");
    scanf("%d",&secim);
    switch(secim){
        case 1:{
            ChangeNames(length,files);
            return;
        }
        case 2: return;
    }
}
void ShowAllFiles(const char* filePath){
    system("cls");
    FILE * filetxt =fopen("hru_ceng_scanning.txt","a");
    FILE * filedat=fopen("hru_ceng_scanning.dat","a");
    int length,secim;
    length = GetAllSize(filePath);
    struct File * files = (struct File*) malloc(length*sizeof(struct File));
    ShowAll(filePath,files);
    Sirala(length,files);
    for(int i=0;i<length;i++){
        printf("Dosyanin icindekiler -> %s\n",(files+i)->fileName);
    }
    fprintf(filetxt,"%s dizini   %s tarihinde tamamen taranmistir.\n",filePath,TIMENOW);
    fprintf(filedat,"%s dizini   %s tarihinde tamamen taranmistir.\n",filePath,TIMENOW);
    fclose(filedat);
    fclose(filetxt);
    printf("\nIsim degistirme islemleri yapmak ister misiniz?\n1-Evet\n2-Hayir\nSeciminiz-> ");
    scanf("%d",&secim);
    switch(secim){
        case 1:{
        ChangeNames(length,files);
        free(files);
        return;
        }
        case 2: {
            free(files);
             return;
        }
    }
}
void ShowAll(const char* filePath,struct File* files){
    char * ext;
    DIR* dir = opendir(filePath);
    if(dir==NULL){
        return;
    }
    struct dirent* entity=readdir(dir);
    while(entity!=NULL){
        if(entity->d_type==DT_DIR&&strcmp(entity->d_name,".")!=0&&strcmp(entity->d_name,"..")!=0){
            char path[100]={0};
            strcat(path,filePath);
            strcat(path,"\\");
            strcat(path,entity->d_name);
            ShowAll(path,files);
        }
        if(strcmp(entity->d_name,".")!=0&&strcmp(entity->d_name,"..")!=0)
            {
                strcpy((files+SHOWALLCOUNTER)->filePath,filePath);
                strcpy((files+SHOWALLCOUNTER)->fileName,entity->d_name);
                if(entity->d_type==DT_DIR)
                {
                    char * folder={".folder"};
                    strcpy((files+SHOWALLCOUNTER)->fileExt,folder);
                }
                else{
                    ext = strrchr(entity->d_name,46);
                    strcpy((files+SHOWALLCOUNTER)->fileExt,ext);
                }
             SHOWALLCOUNTER++;
            }
        entity=readdir(dir);
    }
    closedir(dir);
}
void ChangeNames(int length,struct File* files){
    int secim;
    while(1){
        printf("\n\n\n\t\t\t\t ***ISIM DEGISTIRME ISLEMLERI***\n\n");
        printf("1-Turkce karakterleri degistir.\n2-Ozel karakterleri cikar.\n3- Turkce karakterleri degistir ve ozel karakterleri cikar.\n");
        printf("4- Uzantiya gore turkce karakterleri degistir\n5-Uzantiya gore ozel karakterleri degistir\n");
        printf("6-Uzantiya gore turkce ve ozel karakterleri degistir.\n7- Uzantiya gore isimleri degistir.\n8-Isme gore ismi degistir.\n");
        printf("9-Basa don\nSeciminiz-> ");
        scanf("%d",&secim);
        switch(secim){
            case 1:{
                TurkceKarakterDegistir(length,files);
                break;
            }
            case 2:{
                OzelKarakterDegistir(length,files);
                break;
            }
            case 3:{
               TurkceKarakterDegistir(length,files);
               OzelKarakterDegistir(length,files);
                break;
            }
            case 4:{
                char extension[10];
                printf("Ornek -> .pdf\nUzantiyi giriniz->");
                scanf("%s",extension);
                UzantiyaGoreTurkceKarakterDegistir(length,files,extension);
                break;
            }
            case 5:{
                char extension[10];
                printf("Ornek -> .pdf\nUzantiyi giriniz-> ");
                scanf("%s",extension);
                UzantiyaGoreOzelKarakterDegistir(length,files,extension);
                break;
            }
            case 6:{
                char  extension[10];
                printf("Ornek -> .pdf\nUzantiyi giriniz-> ");
                scanf("%s",extension);
                UzantiyaGoreOzelKarakterDegistir(length,files,extension);
                UzantiyaGoreTurkceKarakterDegistir(length,files,extension);
                break;
            }
            case 7:{
                UzantiyaGoreIsimleriDegistir(length,files);
                break;
            }
            case 8:{
                IsmiDegistir(length,files); // Taranan dosyalara göre degistiriyor
                break;
            }
            case 9:{
                return;
            }
        } // switch end
    } //while end
} //Func end
