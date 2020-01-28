#include <winsock2.h>
#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <tchar.h>

WSADATA     wsaData;
SOCKET      sock0;
SOCKET      sockw;
struct      sockaddr_in addr;
struct      sockaddr_in client;
#define PORT_NUM 8000


// IPアドレス取得
int getAddrHost(void) {
	int i;
	HOSTENT *lpHost; 			//  ホスト情報を格納する構造体
	IN_ADDR inaddr; 			// IPアドレスを格納する構造体
	char szBuf[256], szIP[16];	// ホスト名/IPアドレスを格納する配列

	// エラー処理
	if (WSAStartup(MAKEWORD(1, 1), &wsaData) != 0) {
		printf("WSAStartup Error\n");
		return -1;
	}

	// ローカルマシンのホスト名を取得する
	gethostname(szBuf, (int)sizeof(szBuf));

	// ホスト情報を取得
	lpHost = gethostbyname(szBuf);
	// IPアドレスを取得
	for (i = 0; lpHost->h_addr_list[i]; i++) {
		memcpy(&inaddr, lpHost->h_addr_list[i], 4);
	}
	strcpy(szIP, inet_ntoa(inaddr));
	printf("build server: http://%s:%d\n", szIP,PORT_NUM);	

	WSACleanup();

	return 0;
}

char *get_name(char *name) {
		
	// nameの検索
	for (int i = 0;	name[i] ;i++) {
		
	}

	// 文字列の格納

	// return
}

// ルート関数
char *route(char *path) {

	// 宣言
	unsigned char html[1024];
	memset(html, 0, 1024);	

	// ルーティング
	if (!strcmp(path, "/page1")) {
		strcpy(html,
			"<!DOCTYPE html>\n"
			"<html lang = \"ja\">\n"
			"<head>\n"
			"<meta charset = \"utf-8\">\n"
			"</head>\n"
			"<body>\n"
			"<h1>Page1</h1>\n"
			"<a href=\"/page2\">->page2</a>\n"
			"</body>"
			"</html>");
	}
	else if (!strcmp(path, "/page2")) {
		strcpy(html,
			"<!DOCTYPE html>\n"
			"<html lang = \"ja\">\n"
			"<head>\n"
			"<meta charset = \"utf-8\">\n"
			"</head>\n"
			"<body>\n"
			"<h1>Page2</h1>\n"
			"<a href=\"/page1\">->page1</a>\n"
			"</body>"
			"</html>");
	}
	else if (!strcmp(path, "/form")) {
		strcpy(html,
			"<!DOCTYPE html>\n"
			"<html lang = \"ja\">\n"
			"<head>\n"
			"<meta charset = \"utf-8\">\n"
			"</head>\n"
			"<body>\n"
			"<h1>Page2</h1>\n"
			"<form action=\"/page1\"method=\"POST\"><input type=\"text\"name=\"input_text\"><input type=\"submit\"></form>\n"
			"</body>"
			"</html>");
	}
	else {
		strcpy(html,
			"<!DOCTYPE html>\n"
			"<html lang = \"ja\">\n"
			"<head>\n"
			"<meta charset = \"utf-8\">\n"
			"</head>\n"
			"<body>\n"
			"<h1>404- Not Found</h1>\n"
			"</body>"
			"</html>");
	}

	return html;
}

int main()
{
	int len;
	int n;
	int sockaddr_in_size = sizeof(struct sockaddr_in);
	int recv_len = 0;
	unsigned char buf[1024];
	unsigned char path[1024];

	//IPアドレスの表示
	if (getAddrHost() != 0) {
		printf("get IP address failed");
		getch();
		return -1;
	}

	// winsock2の初期化
	if (WSAStartup(MAKEWORD(2, 0), &wsaData))
	{
		puts("reset winsock failed");
		getch();
		return -1;
	}

	// ソケットの作成
	sock0 = socket(AF_INET, SOCK_STREAM, 0);
	if (sock0 == INVALID_SOCKET)
	{
		printf("socket : %d\n", WSAGetLastError());
		getch();
		return -1;
	}

	// ソケットの設定
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT_NUM);
	addr.sin_addr.S_un.S_addr = INADDR_ANY;

	// ソケットをバインド
	if (bind(sock0, (struct sockaddr *)&addr, sizeof(addr)) != 0)
	{
		printf("bind : %d\n", WSAGetLastError());
		getch();
		return -1;
	}

	// TCPクライアントからの接続要求を待てる状態にする
	if (listen(sock0, 5) != 0)
	{
		printf("listen : %d\n", WSAGetLastError());
		getch();
		return -1;
	}


	// サーバ起動
	while (1)
	{

		len = sizeof(client);
		sockw = accept(sock0, (struct sockaddr *)&client, &len);
		if (sockw == INVALID_SOCKET)
		{
			printf("accept : %d\n", WSAGetLastError());
			break;
		}

		//バッファ初期化
		memset(path, 0, 1024);

		//接続
		recv_len = recvfrom(sockw, buf, 1024, 0, (struct sockaddr *)&client, &sockaddr_in_size);
		buf[recv_len - 1] = 0;
		if (buf[0] == '\0')
			strcpy(buf, NULL);


		// 通信表示
		printf("%s \n", buf);

		// method
		for (int i = 0; i < strlen(buf); i++) {
			printf("%d\n", i);
			if (buf[i] == 'G' && buf[i + 1] == 'E' && buf[i + 2] == 'T' && buf[i + 3] == ' ') {
				for (int j = 0; buf[i + 4 + j] != ' '; j++) {
					path[j] = buf[i + 4 + j];
				}
				break;
			}
			else if (buf[i] == 'P' && buf[i + 1] == 'O' && buf[i + 2] == 'S' && buf[i + 3] == 'T' && buf[i + 4] == ' ') {
				for (int j = 0; buf[i + 4 + j] != ' '; j++) {
					path[j] = buf[i + 4 + j];
				}
				break;
			}
		}
		printf("request: %s \n", path);

		// HTTP
		unsigned char *header =
			"HTTP/1.0 200 OK\n"
			"Content-type: text/html\n"
			"\n";

		send(sockw, header, strlen(header), 0);

		char *output_html = route(path);

		// 応答（HTMLを返答）
		if (send(sockw, output_html, strlen(output_html), 0) < 1)
		{
			printf("send : %d\n", WSAGetLastError());
			break;
		}

		// ソケットのクローズ
		closesocket(sockw);
	}

	// winsock2 の終了処理
	closesocket(sock0);
	WSACleanup();
	return 0;
}
