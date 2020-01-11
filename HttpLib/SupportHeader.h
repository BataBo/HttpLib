#pragma once

#include "pch.h"
#include <string>
#include <winInet.h>
#pragma comment (lib, "wininet.lib")
#include <iostream>
#include <fstream>
#include <time.h>

enum ProxyType { http, socks4, socks5 };

enum HttpMethod { GET, HEAD, POST,PUT,PATCH };

_declspec(dllexport) std::string ReadBinaryFile(std::string path);
struct _declspec(dllexport) HttpContent {
       std::string _name;
	   std::string _value;
	   std::string _filename = "";
};

struct StringContent : HttpContent {
	StringContent(std::string name, std::string value) {
		_name = name;
		_value = value;
	}
};

struct FileContent : HttpContent {
	FileContent(std::string name, std::string filename, std::string path) {
		_name = name;
		_filename = filename;
		_value = ReadBinaryFile(path);
	}
};

struct _declspec(dllexport) MultiPartContent 
{
    HttpContent contents[1024];
	std::string seperator;
public:
	MultiPartContent(std::initializer_list<HttpContent> h);

};

struct _declspec(dllexport) HttpResponse
{
	int ReturnCode;
	std::string Url;
	std::string headers[1024];
	std::string cookies[1024];
	std::string responseBody;
	void SaveToFile();
};

 struct _declspec(dllexport) HttpRequest
{
private:std::string headers[1024];
	int headerCount = 0;
	bool ProxyUse = false;
	std::string proxyAddress;
	int proxyPort;
	ProxyType proxytype;

public:bool KeepAlive = false;
	  bool PermanentHeaders = true;
	bool AllowAutoRedirect = true;
	bool AllowCookies = true;
	std::string AcceptEncoding;
	std::string Authorization;
	int ConnectionTimout = -2;
	int ReadWriteTimeout = -2;
	std::string Cookies;
	std::string Referer;
	std::string UserAgent;
	void AddHeader(std::string header, std::string value);
	void AddHeader(std::string value);
	void AddProxy(std::string proxy, ProxyType pt);
	void ClearAllHeaders();
	HttpResponse Start(HttpMethod method,std::string url);
	HttpResponse Start(HttpMethod method, std::string url,std::string content,std::string content_type);
	HttpResponse Start(HttpMethod method, std::string url, MultiPartContent content);
};

 struct Random {
	 Random() {
		 std::srand(time(NULL));
	 }
	 int GenerateInt(int min, int max) {
		 int number = std::rand();
		 return min + (number % max);
	 }
	 char GenerateChar() {
		 int number = std::rand();
		 return (char)32 + (number % 95);
	 }
 };

 _declspec(dllexport)std::string ChromeUserAgent();
 _declspec(dllexport)std::string FirefoxUserAgent();
 _declspec(dllexport)std::string IEUserAgent();
 _declspec(dllexport)std::string OperaMiniUserAgent();
 _declspec(dllexport)std::string OperaUserAgent();
 _declspec(dllexport)std::string RandomUserAgent();

