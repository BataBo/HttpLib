#pragma once

#include "pch.h"
#include <string>
#include <winInet.h>
#pragma comment (lib, "wininet.lib")

enum ProxyType { http, socks4, socks5 };

enum HttpMethod { GET, HEAD, POST };

struct _declspec(dllexport) HttpResponse
{
	int ReturnCode;
	std::string Url;
	std::string headers[1024];
	std::string cookies[1024];
	std::string responseBody;
};

 struct _declspec(dllexport) HttpRequest
{
	std::string headers[1024];
	int headerCount = 0;
	bool ProxyUse = false;
	std::string proxyAddress;
	int proxyPort;
	ProxyType proxytype;
	bool KeepAlive = false;
	bool AllowAutoRedirect = true;
	bool AllowCookies = true;
	void AddHeader(std::string header, std::string value);
	void AddHeader(std::string value);
	void AddProxy(std::string proxy, ProxyType pt);
	void ClearAllHeaders();
	HttpResponse Start(HttpMethod method,std::string url);
	HttpResponse Start(HttpMethod method, std::string url,std::string content,std::string content_type);
};

