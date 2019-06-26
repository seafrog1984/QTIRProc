#ifndef CLIENT__H__
#define CLIENT__H__
#pragma once


#include "../incl/protocol.h"
#include "../utils/socket/Socket.h"
#include "../utils/string/string_utils.h"


#define  PIC_SIZE  320*240
#define  MAX_DATA  1024*10

using namespace std;

class _client_t
{
public:
	_client_t() : bConnection(false) {}
	~_client_t()
	{
		close();
	}

	void close()
	{
		if (bConnection)
			sock.Close();

		bConnection = false;
	}

	bool get_conn() { return bConnection; }

	std::string get_msg() { return sMsg; }

	std::string get_auth() { return sAuth; }

	//初始化
	bool init(const std::string &host, short port);

	//获取授权
	int login_auth(string user, string passwd, std::string &permissions, int flag = 1);
	//验证授权
	int check_auth(int flag = 1);
	//注册卡信息
	int set_cardid(std::map<std::string, std::string> &mapCardInfo, const std::string &user);
	//注册扫描ID
	int set_scanid(const std::string &card_id, std::string &scan_id, const std::string &user);
	//获取扫描ID
	int get_scanid(const std::string &card_id, std::string &params, std::map <std::string, std::string> &mapScanID);
	//发送图片
	bool send_png(const std::string &scan_id, unsigned short *data, int len, std::vector<std::string> &vecPngID);
	//发送个人信息
	bool send_info(std::map<std::string, std::string> &mapUserInfo);
	//获取个人信息
	int get_info(const std::string &scan_id, std::map<std::string, std::string> &mapUserInfo);
	//获取图片
	bool get_png(const std::string &scan_id, const std::string &png_id, unsigned short *pic, int pic_len);
	//发送结果图片
	bool send_result_dat(const std::string &scan_id, unsigned short *png, int png_len, const std::string &info, const std::string &user);
	bool send_result_bmp(const std::string &scan_id, std::string &png, const std::string &info, const std::string &user);
	//获取结果列表
	int get_result_list(const std::string &scan_id, std::string &pic_id);
	//获取结果
	bool get_result(const std::string &scan_id, std::string &pic_id, unsigned short *pic, int pic_len, std::string &pic_s, std::string &info);

	//获取用户列表
	bool get_users(string &user, string &passwd, std::map<std::string, std::string> &mapUserInfo, int &level);
	//新增用户
	bool new_user(string &user, string &passwd, int &level, int &permissions, string &loginuser, string &loginpasswd);
	//删除用户
	bool del_user(string &user, string &loginuser, string &loginpasswd);
	//修改用户
	bool update_user(string &user, string &passwd, int &permissions, string &loginuser, string &loginpasswd);

private:
	//设置请求头
	void set_req_head(req_head_t &head, const int &cmd);
	//接收数据
	bool recv_data(char *buf, int buf_len);
	//接受应答
	int recv_resp(char *buf);

	//验证授权
	std::string get_local_auth(std::string &mac, std::string &ip, std::string &device);

	bool bConnection; //连接状态
	CSocket sock;     //连接句柄

	std::string sAuth;
	std::string sMsg;
};


typedef _client_t client_t;


#endif