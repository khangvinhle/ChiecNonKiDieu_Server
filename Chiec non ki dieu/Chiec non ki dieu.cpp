// server_socket_chiecnonkydieu.cpp : Defines the entry point for the console application.
//
//algorithlm, ctime, cstdlib.h, random

#include "stdafx.h"
#include "Chiec non ki dieu.h"
#include "afxSock.h"
#include "string"
#include "iostream"
#include "ios"
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <random>
#include <windows.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define N_CLIENT 2


// The one and only application object

CWinApp theApp;

using namespace std;

struct Thongtin_ochu
{
	char ochu[30];
	char goiy[30];
	int solandoan;
};


struct client_info
{
	char nickname[20];
	int score;
	int stt; //so thu tu doan
	int n_luot;
};

struct TTDoan
{
	int diem;
	char kq[20];
};

struct OChu
{
	char goiy[30];
	int sokytu;
	int thutudoan;
	int n_thamgia;
	int soluot;
};



int main()
{
	int nRetCode = 0;

	HMODULE hModule = ::GetModuleHandle(nullptr);

	if (hModule != nullptr)
	{
		// initialize MFC and print and error on failure
		if (!AfxWinInit(hModule, nullptr, ::GetCommandLine(), 0))
		{
			// TODO: change error code to suit your needs
			wprintf(L"Fatal Error: MFC initialization failed\n");
			nRetCode = 1;
		}
		else
		{
			// TODO: code your application's behavior here.
			CSocket server, *client;
			client = new CSocket[N_CLIENT];

			/*List thong tin ve client*/
			client_info list_client[N_CLIENT];

			/*Du lieu o chu*/
			Thongtin_ochu list_ochu[3] = { "DHCP","Ten mo hinh mang",3,
				"LinkLayer","Tang trong mo hinh OSI",7,
				"Application Layer","Ten mo hinh mang",8 };

			/*Random o chu*/
			srand(unsigned(time(0)));
			int sttOchu = rand() % 3;
			Thongtin_ochu OCHU = list_ochu[sttOchu];
			/************************************/


			AfxSocketInit();
			server.Create(1234);

			//lang nghe ket noi
			server.Listen();
			cout << "Dang cho mot ket noi..." << endl;


			/*Dang ky nickname khoi tao diem*/
			for (int i = 0; i < N_CLIENT; i++)
			{
				if (server.Accept(client[i]))
				{
					cout << "Da nhan ket noi tu client." << endl;
					cout << "Dang cho client " << i << " dang ky nickname" << endl;

					bool is_trung = 1;
					char nickname[20];

					/*Kiem Tra Trung*/
					while (is_trung == 1)
					{
						//Nhan nickname tu client
						int len = client[i].Receive(nickname, 100, 0);
						nickname[len] = '\0';
						for (int i = 0; i < N_CLIENT; i++)
						{
							if (strcmp(list_client[i].nickname, nickname) == 0)
							{
								is_trung = 1;
								break;
							}
							else
								is_trung = 0;
						}
						client[i].Send(&is_trung, sizeof(bool), 0);
					}
					strcpy(list_client[i].nickname, nickname);
					list_client[i].score = 0;
					list_client[i].n_luot = OCHU.solandoan;
					cout << list_client[i].nickname << " dang ky thanh cong" << endl;
				}
			}
			/////////////////////////////////////////////////
			/*Ket thuc dang ky nickname*/
			cout << "-----------------------------------------------------------" << endl;


		BD:
			/*Random so thu tu nguoi choi*/
			int *MangSTT = new int[N_CLIENT];
			for (int i = 0; i < N_CLIENT; i++)
			{
				MangSTT[i] = i + 1;
			}
			srand(unsigned(time(0)));
			random_shuffle(&MangSTT[0], &MangSTT[N_CLIENT]);
			for (int i = 0; i < N_CLIENT; i++)
			{
				list_client[i].stt = MangSTT[i];
			}
			/*Ket thuc random so thu tu nguoi choi*/


			/*Tao mang so thu tu de gui cho client*/
			int stt = 1;
			int index = 0;
			int list_stt[N_CLIENT];
			while (index < N_CLIENT)
			{
				for (int i = 0; i < N_CLIENT; i++)
				{
					if (list_client[i].stt == stt)
					{
						list_stt[index] = i;
						break;
					}
				}
				index++;
				stt++;
			}
			/*************************************************************/

			/*Gui cho client so nguoi doan o chu, thu tu doan cua nguoi choi, so ky tu trong o chu, goi y*/
			OChu TTGuiNguoiChoi;
			strcpy(TTGuiNguoiChoi.goiy, OCHU.goiy);
			TTGuiNguoiChoi.sokytu = strlen(OCHU.ochu);
			TTGuiNguoiChoi.n_thamgia = N_CLIENT;
			TTGuiNguoiChoi.soluot = OCHU.solandoan;
			for (int i = 0; i < N_CLIENT; i++)
			{
				TTGuiNguoiChoi.thutudoan = list_client[list_stt[i]].stt;
				client[list_stt[i]].Send(&TTGuiNguoiChoi, sizeof(OChu), 0);

			}
			/***************************************/

			/*Tao mat na*/
			int len = TTGuiNguoiChoi.sokytu;
			char Mask[25];
			for (int i = 0; i < len; i++)
			{
				if (OCHU.ochu[i] == ' ')
					Mask[i] = ' ';
				else
					Mask[i] = '*';
			}
			Mask[len] = '\0';
			/*************************************/

			/*Vong lap GAME*/

			bool ketthuc = false;
			int n_luot = OCHU.solandoan;
			while ((strcmp(Mask, OCHU.ochu) != 0) && (!ketthuc) && n_luot > 0) //Dieu kien ket thuc game 
			{
				n_luot--;
				for (int i = 0; i < N_CLIENT; i++)
				{
					if (list_client[list_stt[i]].n_luot > 0)
					{
						char inform[50] = "Den Luot Ban Doan O Chu: \n";
						strcat(inform, Mask);
						client[list_stt[i]].Send(inform, strlen(inform) + 1, 0);
						cout << "Da gui cho client " << list_client[list_stt[i]].nickname << endl;

						TTDoan doan;
						client[list_stt[i]].Receive(&doan, sizeof(TTDoan), 0);

						/*Neu diem nguoi choi == 0*/
						if (doan.diem == 0)
						{
							n_luot++;
							char nickname0[20];
							strcpy(nickname0, list_client[list_stt[i]].nickname);
							char thongbao0[40];
							strcpy(thongbao0, "O");
							strcat(thongbao0, nickname0);
							strcat(thongbao0, " mat luot!\0");
							for (int j = 0; j < N_CLIENT; j++)
							{
								if (list_client[j].n_luot > 0)
									client[j].Send(thongbao0, strlen(thongbao0) + 1, 0);
							}
							Sleep(2000);
						}
						/***********************/

						/*Neu doan nguyen o chu ma dung*/
						else if (strlen(doan.kq) >= 2)
						{
							if (strcmp(doan.kq, OCHU.ochu) == 0)
							{

								char tbcm[25] = "You are right!";
								client[list_stt[i]].Send(tbcm, strlen(tbcm), 0);

								list_client[list_stt[i]].score += doan.diem * 3;
								char nicknameF[20];
								strcpy(nicknameF, list_client[list_stt[i]].nickname);
								char thongbaoF[40];
								strcpy(thongbaoF, "F");
								strcat(thongbaoF, nicknameF);
								strcat(thongbaoF, " tra loi dung o chu!\0");
								for (int k = 0; k < N_CLIENT; k++)
									if (list_client[k].n_luot > 0)
										client[k].Send(thongbaoF, strlen(thongbaoF) + 1, 0);
								ketthuc = true;
								Sleep(2000);
								break;
							}

							/*Neu doan o chu ma sai*/
							else
							{
								list_client[list_stt[i]].n_luot--;
								char tlsai[30] = "Sorry!@@";
								client[list_stt[i]].Send(tlsai, strlen(tlsai) + 1, 0);
							}

						}
						/***************************************/


						/*Neu client doan 1 chu*/
						else
						{
							int flag = 0;
							//Kiem tra mat na xem co trung ky tu khong?
							for (int v = 0; v < strlen(Mask); v++)
							{
								if (Mask[v] == doan.kq[0])
								{
									flag = 1;
									break;
								}
							}
							if (flag == 1)
							{
								char tbtontai[50] = "That character is repeated in my word";
								client[list_stt[i]].Send(tbtontai, sizeof(tbtontai), 1);
								list_client[list_stt[i]].n_luot--;
							}

							else
							{
								list_client[list_stt[i]].n_luot--;
								int flagchu = 0;
								for (int l = 0; l < strlen(OCHU.ochu); l++)
								{
									if (OCHU.ochu[l] == doan.kq[0])
									{
										flagchu = 1;
										Mask[l] = doan.kq[0];
									}
								}
								if (flagchu == 1)
								{
									list_client[list_stt[i]].score += doan.diem;
									char tbcm[25] = "Congratulations!";
									client[list_stt[i]].Send(tbcm, strlen(tbcm), 0);
								}
								else
								{
									char tl1sai[45] = "That character doesn't exit in my word";
									client[list_stt[i]].Send(tl1sai, strlen(tl1sai), 0);
								}
								// Neu o chu da duoc doan, gui thong bao cho tat ca cac client 
								if (strcmp(Mask, OCHU.ochu) == 0)
								{
									char endgame[5];
									strcpy(endgame, "E");
									strcat(endgame, "\0");
									for (int u = 0; u < N_CLIENT; u++)
									{
										if (list_client[u].n_luot > 0)
											client[u].Send(&endgame, sizeof(endgame), 0);
									}
									Sleep(2000);
									break;
								}
							}
						}/*Ket thuc gui thong bao khi client doan 1 chu*/
					} /*Dieu kien khi client van con luot choi*/
				} /*Ket thuc 1 luot choi*/
			}
			/*******************************Ket thuc game*******************************/

			char result[500] = "";
			for (int p = 0; p < N_CLIENT; p++)
			{
				int diemnguyen = list_client[p].score;
				char diem[10];
				itoa(diemnguyen, diem, 10);
				char tbdiem[50] = "Diem cua ";
				strcat(tbdiem, list_client[p].nickname);
				strcat(tbdiem, " la: ");
				strcat(tbdiem, diem);
				strcat(result, tbdiem);
				int lenresult = strlen(result);
				result[lenresult] = '\n';
			}
			cout << "------------------------------------------------------" << endl;
			cout << "KET QUA TONG KET" << endl;
			cout << "------------------------------------------------------" << endl;
			cout << result << endl;
			for (int g = 0; g < N_CLIENT; g++)
			{
				client[g].Send(result, strlen(result) + 1, 0);
			}

			Sleep(1000);

			/*BAT DAU MOT O CHU MOI*/
			char newword[60] = "MOT LUOT QUAY MOI SE BAT DAU, DANG DOI RANDOM O CHU.....";
			cout << "-----------------------------------------------------------------------" << endl;
			for (int x = 0; x < N_CLIENT; x++)
			{
				client[x].Send(newword, strlen(newword), 0);
				cout << "Da gui cho client " << list_client[x].nickname << " bat dau luot choi" << endl;
			}
			cout << "-----------------------------------------------------------------------" << endl;


			srand(unsigned(time(0)));
			sttOchu = rand() % 3;
			OCHU = list_ochu[sttOchu];

			goto BD;


			system("pause");
		}
	}
	else
	{
		// TODO: change error code to suit your needs
		wprintf(L"Fatal Error: GetModuleHandle failed\n");
		nRetCode = 1;
	}
	return nRetCode;
}





