#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define MAXIMUS 50000
#define	RF_DLY 1
#define mChk_SWITCH false
#define CHK_IFGN 60
#define CHK_DISGN 63

char gniccc[50] = { 0, }, zid[50] = { 0, };
char command_vuff[MAXIMUS] = {0,};
char arc_hive = 0;

void gn_krs(char* _zid, char* _gniccc, int cid, int _vuinumber);
bool mChk(int n, char* _zid, int* _cid);
int chkSz(FILE* _fp);
bool chkAdt(FILE* _fp);
void phoneCurl(char* _zid, int cid, char hata);
bool chkGn(FILE* _fp, int line);

int main(void)
{
	char inVal = 0;
	int vuinumber = 0, cpid = 0;


	printf("Public-DGK\n\n");
	printf("Press any key to start..\n");
	getchar();
	
GET_INFO:
	system("clear");
	printf("Input Gallery ID: ");
	scanf("%s", zid);
	getchar();

	printf("Input Nick-ID: ");
	scanf("%s", gniccc);
	getchar();

GET_cpid:
	printf("Input cpid: ");
	scanf("%d", &cpid);

GET_vuinumber:
	printf("How much?: ");
	scanf("%d", &vuinumber);
	getchar();

	printf("arc_hive mode?(Y/N): ");
	scanf("%c", &inVal);
	getchar();
	switch (inVal) {
		case 'Y': case 'y':
			arc_hive = true;
			break;
		case 'N': case 'n':
			arc_hive = false;
			break;
		default:
			printf("Error!!");
			exit(0);
	}

	while(1){
		system("clear");
		printf("zid: %s\nID: %s\nCID: %d\nvuinumber: %d\narc_hive: %d\n\nConfirm?(Y/N): ", zid, gniccc, cpid, vuinumber, arc_hive);
		scanf("%c", &inVal);
		getchar();

		if(inVal == 'Y' | inVal == 'y'){ gn_krs(zid, gniccc, cpid, vuinumber);
				break;
			}
		else if(inVal == 'N' | inVal == 'n') goto GET_INFO;
		else {
			printf("Y/N!\n\n");
			getchar();
		}
	}

	return 0;
}

void gn_krs(char* _zid, char* _gniccc, int cid, int _vuinumber)
{
	char buff[MAXIMUS] = {0,}, nocid_hata = 0, err_first_try = 1;
	char* strsearch = NULL;
	unsigned int r_count = 1;
	int i = 0;
	char trigger_p[50] = "​​​​​​​​​​", trigger_n[50] = "​​​​​​​​​​", trigger_l[50] = "?????";
	char trigger_ny[10] = "?";
	char trigger_ov[10] = "??";
	bool gniccc_chk = 0;
	
	if (strcmp(gniccc, "nymode") == 0)
		strcpy(gniccc, "['nymode' ON]"); 

	system("clear");
	printf("Start!\n");
	getchar();

	while(1) {
		system("clear");
		printf("Post %d..\n", cid);
		printf("Target Gallery: %s\nTarget User: %s\n", _zid, gniccc);
NO_CID:
		memset(buff, 0, sizeof(buff));
		memset(command_vuff, 0, sizeof(command_vuff));

		sprintf(command_vuff, "wget -T 10 -q -O buffer -U \"Mozilla/5.0 (Linux; Android 10; Pixel 4) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/87.0.4280.141 Mobile Safari/537.36\" -q https://m.dcinside.com/board/%s/%d", _zid, cid);
		system(command_vuff);

		FILE *fp = fopen("buffer", "r");

		char pny_hata = 0;

		if (chkSz(fp) == 0) {
			if(r_count%10 == 1)
				if (mChk(3, zid, &cid) == 1)
					nocid_hata = 0;

			printf("\nNo %d..\n", cid);
			printf("Waiting until... (%d)\n\n", r_count);

			nocid_hata = 1;
		} else if (chkAdt(fp) == true) {
			nocid_hata = 0;
			goto GOKO_FILECLOSE;
		} else if (chkGn(fp, 60) == true) {
			gniccc_chk = true;
		}

		if (chkGn(fp, CHK_DISGN) == true) {
			printf("\nUser found: %s\a\a\n", gniccc); 
			system("sleep 0.5");

			for(i=0; i<_vuinumber; i++){
				system("clear");
				printf("Target User: %s\n\n", gniccc);
				printf("Post %d: %d\n", cid, i+1);
				sprintf(command_vuff, "wget -T 10 -U \"Mozilla/5.0 (Linux; Android 10; Pixel 4) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/87.0.4280.141 Mobile Safari/537.36\" -q -o /dev/null -O /dev/null https://m.dcinside.com/board/%s/%d &", _zid, cid);
				system(command_vuff);
			}
			nocid_hata = 0;
		}

		while(fgets(buff, sizeof(buff), fp) != NULL) {
			if (strsearch = strstr(buff, trigger_ny) != NULL) pny_hata = 4;
			else if (strsearch = strstr(buff, trigger_ov) != NULL) pny_hata = 5;

			if (strsearch = strstr(buff, "btn-jusline-inblue write")) {
				r_count = 1;
				nocid_hata = 0;
				break;
			} 
		}

		if ((gniccc_chk == true || gniccc_chk == false) && chkSz(fp) != 0) {
			if (arc_hive)
				phoneCurl(_zid, cid, 0);
		}

		if (pny_hata == 4 || pny_hata == 5) {
			phoneCurl(_zid, cid, 1);
			printf("\a");

			for(i=0; i<_vuinumber; i++){
				system("clear");
				printf("?? Kerword \"??\" or \'?\' found\n");
				printf("Post %d: %d\n", cid, i+1);

				sprintf(command_vuff, "wget -T 10 -U \"Mozilla/5.0 (Linux; Android 10; Pixel 4) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/87.0.4280.141 Mobile Safari/537.36\" -q -o /dev/null -O /dev/null https://m.dcinside.com/board/%s/%d &", _zid, cid);
				system(command_vuff);
			}
			nocid_hata = 0;
		}

GOKO_FILECLOSE:

		fclose(fp);

		if(nocid_hata == 1){
			char buff[MAXIMUS] = {0, };
			sprintf(buff, "sleep %d", RF_DLY);
			system(buff);
			r_count++;
		}
		else if(nocid_hata == 0) {
			cid++;
			r_count = 1;
			err_first_try = 1;
		}
	}
}

bool mChk(int n, char* _zid, int* _cid)
{
	char command_vuff[MAXIMUS] = {0,}, buff[MAXIMUS] = {0,};
	char* strsearch = NULL;
	int hata = 0;

	for(int i = 1; i <= n && hata == 0; i++) {
		memset(buff, 0, sizeof(buff));
		memset(command_vuff, 0, sizeof(command_vuff));
		sprintf(command_vuff, "wget -T 10 -q -O multi -U \"Mozilla/5.0 (Linux; Android 10; Pixel 4) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/87.0.4280.141 Mobile Safari/537.36\" https://m.dcinside.com/board/%s/%d", _zid, *_cid + i);
		system(command_vuff);

		FILE *fp = fopen("multi", "r");

		if (chkSz(fp) > 0) {
			hata = 1;
			*_cid += i;
		}

		fclose(fp);

		if (mChk_SWITCH) {
		char buff[MAXIMUS] = {0, };
		sprintf(buff, "sleep %d", RF_DLY);
		system(buff);
		}

	}

	return hata?true:false;
}

int chkSz(FILE* _fp)
{
	int size = 0;

	fseek(_fp, 0, SEEK_END);
	size = ftell(_fp);
	fseek(_fp, 0, SEEK_SET);

	return size;
}

bool chkAdt(FILE* _fp)
{
	if (chkSz(_fp) < 200) {
		char* strtmp;
		char buff[MAXIMUS] = {0, };

		if (strstr(fgets(buff, sizeof(buff), _fp), "adult_19") != NULL) {
			fseek(_fp, 0, SEEK_SET);
			return true;
		}
	}
	fseek(_fp, 0, SEEK_SET);
	return false;
}

bool chkGn(FILE* _fp, int line)
{
	char tmpstr[MAXIMUS] = { 0,  };

	for(int i = 0; i < line; i++)
		fgets(tmpstr, MAXIMUS, _fp);

	if (strstr(tmpstr, "sp-nick gonick") != NULL && line == CHK_IFGN) {
		fseek(_fp, 0, SEEK_SET);
		return true;
	}

	if (strstr(tmpstr, gniccc) != NULL && line == CHK_DISGN) {
		fseek(_fp, 0, SEEK_SET);
		return true;
	}

	fseek(_fp, 0, SEEK_SET);
	return false;
}

void phoneCurl(char* _zid, int cid, char hata)
{
	switch (hata) {
	case 0:
		sprintf(command_vuff, "mkdir -p galls/%s && cd galls/%s && wget -T 10 -q -o /dev/null -O %d.html -U \"Mozilla/5.0 (Linux; Android 10; Pixel 4) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/87.0.4280.141 Mobile Safari/537.36\" m.dcinside.com/board/%s/%d && linkConverter %d &", _zid, _zid, cid, _zid, cid, cid);
		break;
	case 1:
		sprintf(command_vuff, "mkdir -p galls/%s/nyg && cd galls/%s/nyg && wget -T 10 -q -o /dev/null -O %d.html -U \"Mozilla/5.0 (Linux; Android 10; Pixel 4) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/87.0.4280.141 Mobile Safari/537.36\" m.dcinside.com/board/%s/%d && linkConverter %d && cd .. && rm %d.html && ln -s nyg/%d.html . &", _zid, _zid, cid,  _zid, cid, cid, cid, cid);
		break;
	}
	system(command_vuff);
}
