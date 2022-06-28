#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#define MAXLENGHT 50000
#include <memory.h>

void imgConverter(char* img);
char* imgSrc (char* img); // for dccon imgSrc
char* makeLoad(char* img);

int main(int argc, char* argv[2])
{
	if (argc < 2) {
		puts("Post ID가 입력되지 않았어요!!");
		exit(1);
	}
	int pid = atoi(argv[1]);
	char tmp[MAXLENGHT] = {0, };

	sprintf(tmp, "%d.html", pid);
	FILE* fp_div = fopen(tmp, "r");

	if (fp_div == NULL) {
		puts("해당하는 Post ID 파일이 없어요!!");
		exit(1);
	}

	sprintf(tmp, "%d.tmp", pid);
	FILE* fp = fopen(tmp, "w+");
	// memset(tmp, 0, sizeof(tmp));

	sprintf(tmp, "%d.html.tmp", pid);
	FILE* fp_new = fopen(tmp, "w+");

	/* 초기 ftell
	printf("ftell(fp): %ld\nftell(fp_new): %ld\n", ftell(fp), ftell(fp_new));
	fseek(fp, 0, SEEK_END); fseek(fp_new, 0, SEEK_END);
	printf("ftell(fp): %ld\nftell(fp_new): %ld\n", ftell(fp), ftell(fp_new));
	*/

	// printf("fp의 주소값: %p\n", fp);
	char* ss = NULL;
	while(fgets(tmp, MAXLENGHT, fp_div) != NULL) { // "<div"를 찾고 그 앞에 개행문자를 추가하는 코드
		if ((ss = strstr(tmp, "<div")) != NULL) {
			for (ss = tmp; (ss = strstr(ss, "<div")) != NULL; ss += strlen("<div")){
				for(int i = (MAXLENGHT-1); i > (ss-tmp); i--) tmp[i] = tmp[i-1];
				// puts(tmp); // dbg
				// system("sleep 0.5"); // dbg
				*(ss) = '\n';	
			} fputs(tmp, fp);
		} else fputs(tmp, fp);
		memset(tmp, 0, sizeof(tmp));
	}
	fclose(fp_div);
	fseek(fp, 0, SEEK_SET);
		
	char img[MAXLENGHT] = { 0, }, imgLink, dccon;
	while(fgets(tmp, MAXLENGHT, fp) != NULL) { // imgLink를 찾아내고 변환하는 코드
		imgLink = false;
		dccon = false;
		/*
		printf("fgets..\nftell(fp): %ld\nftell(fp_new): %ldn", ftell(fp), ftell(fp_new));
		printf("%s\n", tmp);

		printf("strstr before ftell(fp): %ld\n", ftell(fp));
		*/
		// if ((ss = strstr(tmp, "<img src=https://nstatic.dcinside.com/dc/m/img/gallview_loading_ori.gif")) != NULL)
		if ((ss = strstr(tmp, "https://dcimg8")) != NULL) {
			imgLink = true;
			int i;
			for (i = 0; ss[i] != '\"'; i++) {
				img[i] = ss[i];
			}
			img[i] = '\0';

			imgConverter(img);
		} else if ((ss = strstr(tmp, "https://dcimg6")) != NULL) {
			imgLink = true;
			int i;
			for (i = 0; ss[i] != '\"'; i++) {
				img[i] = ss[i];
			}
			img[i] = '\0';

			imgConverter(img);
		} else if ((ss = strstr(tmp, "https://dcimg7")) != NULL) {
			imgLink = true;
			int i;
			for (i = 0; ss[i] != '\"'; i++) {
				img[i] = ss[i];
			}
			img[i] = '\0';

			imgConverter(img);
		} 
		
		else if ((ss = strstr(tmp, "https://dcimg5")) != NULL) {
			dccon = true;
			int i;
			for (i = 0; ss[i] != '\"'; i++) {
				img[i] = ss[i];
			}
			img[i] = '\0';
		} // for imgSrc
		
		// 이미지 링크 주소 변환 완료.. 링크는 img에 저장됨.
		
		if (imgLink == true) {
			fputs(makeLoad(img), fp_new);
			fputc('\n', fp_new);
			// fputs(tmp, fp_new);
		} else if (dccon == true) {
			fputs(imgSrc(img), fp_new);
			fputc('\n', fp_new);
		} else fputs(tmp, fp_new);
	}

	// printf("fp의 주소값: %p\n", fp);

	fclose(fp);
	fclose(fp_new);

	{ // remove & rename
		char cmd[100] = {0, };
		sprintf(cmd, "%d.tmp", pid);
		remove(cmd);

		char cmd2[100] = {0, };
		sprintf(cmd, "%d.html.tmp", pid);
		sprintf(cmd2, "%d.html", pid);
		rename(cmd, cmd2);
	}

	// printf("remove return: %d\nrename return: %d\n", remove("1024.tmp"), rename("1024.html.tmp", "1024.html"));

	return 0;
}

void imgConverter(char* img)
{
	char* strings[2];
	strings[0] = "image"; 
	strings[1] = "com/viewimagePop.php";

	// "image" insert, move forward 1
	for(int i = 8; *(strings[0]+i-8) != '\0'; i++){
		img[i] = *(strings[0]+(i-8));
	} for(int i = 13; img[i] != '\0'; i++){		
		img[i] = img[i+1];
	}
	
	// Space bigger
	for(int i = (MAXLENGHT-1); i>30; i--){
		img[i] = img[i-1];
	}
	
	// "com/viewimagePop.php" insert
	for(int i = 23; *(strings[1]+i-23) != '\0'; i++){
		img[i] = *(strings[1]+(i-23));
	}

	// getting rid of amp;
	char* tmp = strstr(img, "amp;");
	for(int i = 0; i<4; i++){
		// printf("\n★ %s\n", tmp);
		// *tmp = ' ';
		for(int i = 0; tmp[i] != '\0'; i++){
			tmp[i] = tmp[i+1];
		}
		/*
		 * 주의: *(strstr(img, "amp;")+i) = ' ';
		 * 이런식으로 strstr의 반환값을 참조하여 값을 대입하려고 하면
		 * Segmentation Fault (core dump) 에러가 발생함.
		 *
		 * 아! strstr을 사용해서 그런 게 아니라, amp;에서 a를 ' '로 바꾸고 strstr(img, "amp;"); 
		 * 이지랄을 하니까 당연히 (null)이 뜨지 멍청아..!!
		 */
	}
}

char* imgSrc (char* img)
{
	static char html[MAXLENGHT] = {0,};
	memset(html, 0, sizeof(html));

	// sprintf(html, "<a href=\"%s\"><img src=\"/imgs/LoadImage.png\"></a>", img);
	sprintf(html, "<img src=\"%s\"><br><br>", img);

	return html;
}

char* makeLoad(char* img)
{
	static char html[MAXLENGHT] = {0,};
	memset(html, 0, sizeof(html));

	// sprintf(html, "<a href=\"%s\"><img src=\"/imgs/LoadImage.png\"></a>", img);
	sprintf(html, "<a href=\"%s\"><img src=\"/imgs/LoadImage.png\"></a><br><br>", img);

	return html;
}
