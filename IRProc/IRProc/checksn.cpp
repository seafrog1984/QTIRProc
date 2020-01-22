#include "stdafx.h"
#include "checksn.h"
#include<QMessageBox>
#include <QString>



void getMac(char *mac)
{
	PIP_ADAPTER_INFO pAdapterInfo;
	PIP_ADAPTER_INFO pAdapter = NULL;
	DWORD dwRetVal = 0;

	pAdapterInfo = (IP_ADAPTER_INFO*)malloc(sizeof(IP_ADAPTER_INFO));
	ULONG ulOutBufLen = sizeof(IP_ADAPTER_INFO);

	if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) != ERROR_SUCCESS)
	{
		free(pAdapterInfo);
		pAdapterInfo = (IP_ADAPTER_INFO*)malloc(ulOutBufLen);
	}

	if ((dwRetVal = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen)) == NO_ERROR)
	{
		pAdapter = pAdapterInfo;
		while (pAdapter)
		{
			if (
				strstr(pAdapter->Description, "PCI") > 0 // pAdapter->Description中包含"PCI"为：物理网卡
				|| pAdapter->Type == 71 // pAdapter->Type是71为：无线网卡
				)
			{
				//	AfxMessageBox(_T( pAdapter->AdapterName));
				//	AfxMessageBox(_T(pAdapter->Description));
				sprintf(mac, "%04d-%04d-%04d-%04d-%04d-%04d",
					pAdapter->Address[0],
					pAdapter->Address[1],
					pAdapter->Address[2],
					pAdapter->Address[3],
					pAdapter->Address[4],
					pAdapter->Address[5]
					);
			}
			pAdapter = pAdapter->Next;
		}
	}
	else
	{
		printf("Callto GetAdaptersInfo failed.\n");
	}

}

int CheckSN(char *mac, char *sn)
{
	// TODO:  在此添加控件通知处理程序代码
	char scode[200];
	int tmp;

	for (int i = 0; i < 6; i++)
	{
		tmp = atoi(mac + i * 5);
		if (i == 5)
		{
			sprintf(scode + i * 5, "%04d", tmp ^ 2018 & 304);
		}
		else
		{
			sprintf(scode + i * 5, "%04d-", tmp ^ 2018 & 304);
		}

	}
	//	AfxMessageBox(_T(scode));

	char sn_mac[200];
	strncpy(sn_mac, sn, 29);
	sn_mac[29] = '\0';

	if (strcmp(sn_mac, scode) == 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}

	//	char scode[200];
	//	int tmp;
	//
	//	for (int i = 0; i < 6; i++)
	//	{
	//		tmp = atoi(mac + i * 5);
	//		if (i==5)
	//		{
	//			sprintf(scode+i*5, "%04d", tmp ^ 2018 & 304);
	//		}
	//		else
	//		{
	//			sprintf(scode+i * 5, "%04d-", tmp ^ 2018 & 304);
	//		}
	//
	//	}
	////	AfxMessageBox(_T(scode));
	//
	//	
	//	if (strcmp(sn, scode) == 0)
	//	{
	//		return 1;
	//	}
	//	else
	//	{
	//		return 0;
	//	}
}

int CheckTime(char *sn)
{
	int time_left = 0;
	char year[5] = { 0 };
	char month[5] = { 0 };
	char day[5] = { 0 };

	strncpy(year, sn + 30, 4);
	strncpy(month, sn + 35, 4);
	strncpy(day, sn + 40, 4);


	//int n_year = atoi(year) ;
	//int n_month = atoi(month) ;
	//int n_day = atoi(day) ;

	int n_year = atoi(year) ^ (2018 + 1024);
	int n_month = atoi(month) ^ (2018 + 1024);
	int n_day = atoi(day) ^ (2018 + 1024);

	CTime time;

	time = CTime::GetCurrentTime();

	int current_year = time.GetYear();
	int current_month = time.GetMonth();
	int current_day = time.GetDay();


	if (current_year < n_year)
	{
		time_left = 365 * (n_year - current_year) + 30 * n_month + n_day;
	}
	else if (current_year>n_year)
		time_left = 0;
	else
	{
		if (current_month<n_month)
		{
			time_left = 30 * (n_month - current_month) + n_day;
		}
		else if (current_month>n_month)
			time_left = 0;
		else
		{
			if (current_day <= n_day)
			{
				time_left = n_day - current_day;

			}
			else
				time_left = 0;
		}
	}

	if (time_left > 0 && time_left < 15)
	{
		QString msg =QString::fromLocal8Bit( "授权时间剩余");
		msg.append(QString::number(time_left));
		msg.append( QString::fromLocal8Bit("天！"));
		QMessageBox::information(NULL, QString::fromLocal8Bit("注意！！"), msg);
	}

	return time_left;

}