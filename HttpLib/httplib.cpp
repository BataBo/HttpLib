#include "pch.h"

#include "SupportHeader.h"
#include "MimeTypes.h"
#include <time.h>

void SetDefHeaders(HttpRequest* req);
std::string defheaders = "";
std::string* GetHeaders(HINTERNET hreq);
int GetStatusCode(HINTERNET hreq);
std::string* Seperate(std::string url);
std::string GetResponseBody(HINTERNET hreq);
std::string GetCookie(std::string cookieHeader);

HttpResponse get(std::string url, std::string headers[], int headercount, bool proxyuse, std::string proxyAddress, int ProxyPort, ProxyType type, bool KeepAlive, bool AutoRedirect, bool AllowCookies, bool PermenantHeaders, int ConnectionTimeout, int ReadWriteTimeout);
HttpResponse head(std::string url, std::string headers[], int headercount, bool proxyuse, std::string proxyAddress, int ProxyPort, ProxyType type, bool KeepAlive, bool AutoRedirect, bool AllowCookies, bool PermenantHeaders, int ConnectionTimeout, int ReadWriteTimeout);
HttpResponse post(std::string url, std::string headers[], int headercount, bool proxyuse, std::string proxyAddress, int ProxyPort, ProxyType type, bool KeepAlive, bool AutoRedirect, bool AllowCookies,std::string body,std::string contenttype, bool PermenantHeaders, int ConnectionTimeout, int ReadWriteTimeout);
HttpResponse post_multipart(std::string url, std::string headers[], int headercount, bool proxyuse, std::string proxyAddress, int ProxyPort, ProxyType type, bool KeepAlive, bool AutoRedirect, bool AllowCookies, MultiPartContent content, bool PermenantHeaders, int ConnectionTimeout, int ReadWriteTimeout);
HttpResponse put(std::string url, std::string headers[], int headercount, bool proxyuse, std::string proxyAddress, int ProxyPort, ProxyType type, bool KeepAlive, bool AutoRedirect, bool AllowCookies, std::string body, std::string contenttype, bool PermenantHeaders, int ConnectionTimeout, int ReadWriteTimeout);
HttpResponse put_multipart(std::string url, std::string headers[], int headercount, bool proxyuse, std::string proxyAddress, int ProxyPort, ProxyType type, bool KeepAlive, bool AutoRedirect, bool AllowCookies, MultiPartContent content, bool PermenantHeaders, int ConnectionTimeout, int ReadWriteTimeout);
HttpResponse patch(std::string url, std::string headers[], int headercount, bool proxyuse, std::string proxyAddress, int ProxyPort, ProxyType type, bool KeepAlive, bool AutoRedirect, bool AllowCookies, std::string body, std::string contenttype, bool PermenantHeaders, int ConnectionTimeout, int ReadWriteTimeout);
HttpResponse patch_multipart(std::string url, std::string headers[], int headercount, bool proxyuse, std::string proxyAddress, int ProxyPort, ProxyType type, bool KeepAlive, bool AutoRedirect, bool AllowCookies, MultiPartContent content, bool PermenantHeaders, int ConnectionTimeout, int ReadWriteTimeout);

std::string ReadBinaryFile(std::string path) {
	std::ifstream fs;
	fs.open(path, std::ios::binary);
	fs.seekg(0, std::ios::end);
	size_t i = fs.tellg();
	char* buf = new char[i];
	fs.seekg(0, std::ios::beg);
	fs.read(buf, i);
	fs.close();
	std::string s;
	s.assign(buf, i);
	delete[] buf;

	return s;
}
std::wstring s2ws(const std::string& s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}

void HttpRequest::AddHeader(std::string header, std::string value) {
	std::string h = header + ":" + value;
	headers[headerCount] = h;
	headerCount++;
}

void HttpRequest::AddHeader(std::string value) {
	headers[headerCount] = value;
	headerCount++;
}

void HttpRequest::AddProxy(std::string proxy, ProxyType pt) {
	if (proxy.find(":") != std::string::npos) {
		std::string adr = "";
		char c = NULL;
		int i = 0;
		while (c != ':') {
			c = proxy[i];
			i++;
			adr += c;
		}
		adr = adr.substr(0,adr.size()-1);
		std::string port = "";
		for (int j = i; j < proxy.size(); j++) {
			port += proxy[j];
		}
		int p = stoi(port);
		ProxyUse = true;
		proxyAddress = adr;
		proxyPort = p;
		proxytype = pt;
	}
	else {
		throw "Proxy submited in the wrong format";
	}
}

void HttpRequest::ClearAllHeaders() {

	for (int i = 0; i < headerCount; i++) {
		headers[i] = "";
	}
	headerCount = 0;
}

HttpResponse HttpRequest::Start(HttpMethod method, std::string url) {
	SetDefHeaders(this);
	switch (method)
	{
	case GET:
	{
		HttpResponse response = get(url, this->headers, this->headerCount, this->ProxyUse, this->proxyAddress, this->proxyPort, this->proxytype, this->KeepAlive, this->AllowAutoRedirect, this->AllowCookies, this->PermanentHeaders, this->ConnectionTimout, this->ReadWriteTimeout);
		if (!this->PermanentHeaders)this->ClearAllHeaders();
		return response; }
	case HEAD: {
		HttpResponse response = head(url, this->headers, this->headerCount, this->ProxyUse, this->proxyAddress, this->proxyPort, this->proxytype, this->KeepAlive, this->AllowAutoRedirect, this->AllowCookies, this->PermanentHeaders, this->ConnectionTimout, this->ReadWriteTimeout);
		if (!this->PermanentHeaders)this->ClearAllHeaders();
		return response; }
	case POST:
		throw "No body found";
	case PUT:
		throw "No body found";
	case PATCH:
		throw "No body found";
	}
}

HttpResponse HttpRequest::Start(HttpMethod method, std::string url, std::string content, std::string content_type) {
	SetDefHeaders(this);
	switch (method)
	{
	case GET:
		throw "Get request cannot have a body";
	case HEAD:
		throw "Head request cannot have a body";
	case POST: {
		HttpResponse response = post(url, this->headers, this->headerCount, this->ProxyUse, this->proxyAddress, this->proxyPort, this->proxytype, this->KeepAlive, this->AllowAutoRedirect, this->AllowCookies, content, content_type, this->PermanentHeaders, this->ConnectionTimout, this->ReadWriteTimeout);
		if (!this->PermanentHeaders)this->ClearAllHeaders();
		return response;
	}
	case PUT: {
		HttpResponse response = put(url, this->headers, this->headerCount, this->ProxyUse, this->proxyAddress, this->proxyPort, this->proxytype, this->KeepAlive, this->AllowAutoRedirect, this->AllowCookies, content, content_type, this->PermanentHeaders, this->ConnectionTimout, this->ReadWriteTimeout);
		if (!this->PermanentHeaders)this->ClearAllHeaders();
	return response; 
	}
	case PATCH:
	{
		HttpResponse response = patch(url, this->headers, this->headerCount, this->ProxyUse, this->proxyAddress, this->proxyPort, this->proxytype, this->KeepAlive, this->AllowAutoRedirect, this->AllowCookies, content, content_type, this->PermanentHeaders, this->ConnectionTimout, this->ReadWriteTimeout);
		if (!this->PermanentHeaders)this->ClearAllHeaders();
		return response;
	}

	}
}

HttpResponse HttpRequest::Start(HttpMethod method, std::string url, MultiPartContent content) {
	SetDefHeaders(this);
	switch (method)
	{
	case GET:
		throw "Get request cannot use multipartcontent";
	case HEAD:
		throw "Head request cannot use multipartcontent";
	case POST: {
		HttpResponse response = post_multipart(url, this->headers, this->headerCount, this->ProxyUse, this->proxyAddress, this->proxyPort, this->proxytype, this->KeepAlive, this->AllowAutoRedirect, this->AllowCookies, content, this->PermanentHeaders, this->ConnectionTimout, this->ReadWriteTimeout);
		if (!this->PermanentHeaders)this->ClearAllHeaders();
		return response;
	} 
	case PUT: {
		HttpResponse response = put_multipart(url, this->headers, this->headerCount, this->ProxyUse, this->proxyAddress, this->proxyPort, this->proxytype, this->KeepAlive, this->AllowAutoRedirect, this->AllowCookies, content, this->PermanentHeaders, this->ConnectionTimout, this->ReadWriteTimeout);
		if (!this->PermanentHeaders)this->ClearAllHeaders();
		return response;
	}
	case PATCH: {
		HttpResponse response = patch_multipart(url, this->headers, this->headerCount, this->ProxyUse, this->proxyAddress, this->proxyPort, this->proxytype, this->KeepAlive, this->AllowAutoRedirect, this->AllowCookies, content, this->PermanentHeaders, this->ConnectionTimout, this->ReadWriteTimeout);
		if (!this->PermanentHeaders)this->ClearAllHeaders();
		return response;
	}
	}
}

HttpResponse get(std::string url, std::string headers[], int headercount, bool proxyuse, std::string proxyAddress, int ProxyPort, ProxyType type,bool KeepAlive,bool AutoRedirect,bool AllowCookies,bool PermenantHeaders,int ConnectionTimeout,int ReadWriteTimeout) {
	
	std::string* all = Seperate(url);
	std::string domain = all[0];
	std::string path = all[1];
	DWORD flags = NULL;
	int port = 80;
	if (url.find("https://") != std::string::npos) {
		flags |= INTERNET_FLAG_SECURE;
		flags |= INTERNET_FLAG_IGNORE_CERT_DATE_INVALID;
		flags |= INTERNET_FLAG_IGNORE_REDIRECT_TO_HTTP;
		port = 443;
	}
	if (KeepAlive) {
		flags |= INTERNET_FLAG_KEEP_CONNECTION;
	}
	if (!AutoRedirect) {
		flags |= INTERNET_FLAG_NO_AUTO_REDIRECT;
	}
	if (!AllowCookies) {
		flags |= INTERNET_FLAG_NO_COOKIES;
	}


	HINTERNET io;
	if (proxyuse) {
		if (type == http) {
			std::string proxyBody = proxyAddress + ":" + std::to_string(ProxyPort);
			io = InternetOpen(NULL, INTERNET_OPEN_TYPE_PROXY, s2ws(proxyBody).c_str(), NULL, 0);
		}
		else {
			std::string proxyBody = "socks=" + proxyAddress + ":" + std::to_string(ProxyPort);
			io = InternetOpen(NULL, CERN_PROXY_INTERNET_ACCESS, s2ws(proxyBody).c_str(),NULL,0);
		}
	}
	else {
		io = InternetOpen(NULL, INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
	}
	
	HINTERNET ic = InternetConnect(io, s2ws(domain).c_str(), port, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
	if(ConnectionTimeout > -2)
	InternetSetOption(ic, INTERNET_OPTION_CONNECT_TIMEOUT,&ConnectionTimeout,sizeof(int));
	if (ReadWriteTimeout > -2) {
		InternetSetOption(ic, INTERNET_OPTION_RECEIVE_TIMEOUT, &ReadWriteTimeout, sizeof(int));
		InternetSetOption(ic, INTERNET_OPTION_SEND_TIMEOUT, &ReadWriteTimeout, sizeof(int));
	}
	HINTERNET hreq = HttpOpenRequest(ic, L"GET", s2ws(path).c_str(), L"HTTP/1.1", NULL, NULL, flags, 0);
	std::string ReqHeaders = "";
	for (int i = 0; i < 1024;i++) {
		if (headers[i] == "")break;
		ReqHeaders += headers[i] + "\r\n";
	}
	ReqHeaders += defheaders;
	HttpAddRequestHeaders(hreq,s2ws(ReqHeaders).c_str(),-1L,NULL);
	HttpSendRequest(hreq, NULL, 0, NULL, 0);
	HttpResponse response;
	response.Url = url;
	response.ReturnCode = GetStatusCode(hreq);
	if (response.ReturnCode == 0)throw GetLastError();
	std::string* ResponseHeaders = GetHeaders(hreq);

	for (int i = 1; i < 1024; i++) {
		if (ResponseHeaders[i - 1] == "")break;
		response.headers[i - 1] = ResponseHeaders[i];
	}
	int j = 0;
	response.responseBody = GetResponseBody(hreq);
	for (int i = 0; i < 1024; i++) {
		if (response.headers[i] == "")break;
		if (response.headers[i].find("Set-Cookie:") != std::string::npos) {
			std::string cookieforheader = GetCookie(response.headers[i]);
			response.cookies[j] = cookieforheader;
			j++;
		}
	}



	return response;
}

HttpResponse head(std::string url, std::string headers[], int headercount, bool proxyuse, std::string proxyAddress, int ProxyPort, ProxyType type, bool KeepAlive, bool AutoRedirect, bool AllowCookies,bool PermenantHeaders, int ConnectionTimeout, int ReadWriteTimeout) {
	std::string* all = Seperate(url);
	std::string domain = all[0];
	std::string path = all[1];
	DWORD flags = NULL;
	int port = 80;
	if (url.find("https://") != std::string::npos) {
		flags |= INTERNET_FLAG_SECURE;
		flags |= INTERNET_FLAG_IGNORE_CERT_DATE_INVALID;
		flags |= INTERNET_FLAG_IGNORE_REDIRECT_TO_HTTP;
		port = 443;
	}
	if (KeepAlive) {
		flags |= INTERNET_FLAG_KEEP_CONNECTION;
	}
	if (!AutoRedirect) {
		flags |= INTERNET_FLAG_NO_AUTO_REDIRECT;
	}
	if (!AllowCookies) {
		flags |= INTERNET_FLAG_NO_COOKIES;
	}


	HINTERNET io;
	if (proxyuse) {
		if (type == http) {
			std::string proxyBody = proxyAddress + ":" + std::to_string(ProxyPort);
			io = InternetOpen(NULL, INTERNET_OPEN_TYPE_PROXY, s2ws(proxyBody).c_str(), NULL, 0);
		}
		else {
			std::string proxyBody = "socks=" + proxyAddress + ":" + std::to_string(ProxyPort);
			io = InternetOpen(NULL, CERN_PROXY_INTERNET_ACCESS, s2ws(proxyBody).c_str(), NULL, 0);
		}
	}
	else {
		io = InternetOpen(NULL, INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
	}

	HINTERNET ic = InternetConnect(io, s2ws(domain).c_str(), port, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
	if (ConnectionTimeout > -2)
		InternetSetOption(ic, INTERNET_OPTION_CONNECT_TIMEOUT, &ConnectionTimeout, sizeof(int));
	if (ReadWriteTimeout > -2) {
		InternetSetOption(ic, INTERNET_OPTION_RECEIVE_TIMEOUT, &ReadWriteTimeout, sizeof(int));
		InternetSetOption(ic, INTERNET_OPTION_SEND_TIMEOUT, &ReadWriteTimeout, sizeof(int));
	}
	HINTERNET hreq = HttpOpenRequest(ic, L"HEAD", s2ws(path).c_str(), L"HTTP/1.1", NULL, NULL, flags, 0);
	std::string ReqHeaders = "";
	for (int i = 0; i < 1024; i++) {
		if (headers[i] == "")break;
		ReqHeaders += headers[i] + "\r\n";
	}
	ReqHeaders += defheaders;
	HttpAddRequestHeaders(hreq, s2ws(ReqHeaders).c_str(), -1L, NULL);
	HttpSendRequest(hreq, NULL, 0, NULL, 0);
	HttpResponse response;
	response.Url = url;
	response.ReturnCode = GetStatusCode(hreq);
	if (response.ReturnCode == 0)throw GetLastError();
	std::string* ResponseHeaders = GetHeaders(hreq);

	for (int i = 1; i < 1024; i++) {
		if (ResponseHeaders[i - 1] == "")break;
		response.headers[i - 1] = ResponseHeaders[i];
	}
	int j = 0;
	for (int i = 0; i < 1024; i++) {
		if (response.headers[i] == "")break;
		if (response.headers[i].find("Set-Cookie:") != std::string::npos) {
			std::string cookieforheader = GetCookie(response.headers[i]);
			response.cookies[j] = cookieforheader;
			j++;
		}
	}
	return response;
}

HttpResponse post(std::string url, std::string headers[], int headercount, bool proxyuse, std::string proxyAddress, int ProxyPort, ProxyType type, bool KeepAlive, bool AutoRedirect, bool AllowCookies, std::string body, std::string contenttype, bool PermenantHeaders, int ConnectionTimeout, int ReadWriteTimeout) {
	std::string* all = Seperate(url);
	std::string domain = all[0];
	std::string path = all[1];
	DWORD flags = NULL;
	int port = 80;
	if (url.find("https://") != std::string::npos) {
		flags |= INTERNET_FLAG_SECURE;
		flags |= INTERNET_FLAG_IGNORE_CERT_DATE_INVALID;
		flags |= INTERNET_FLAG_IGNORE_REDIRECT_TO_HTTP;
		port = 443;
	}
	if (KeepAlive) {
		flags |= INTERNET_FLAG_KEEP_CONNECTION;
	}
	if (!AutoRedirect) {
		flags |= INTERNET_FLAG_NO_AUTO_REDIRECT;
	}
	if (!AllowCookies) {
		flags |= INTERNET_FLAG_NO_COOKIES;
	}


	HINTERNET io;
	if (proxyuse) {
		if (type == http) {
			std::string proxyBody = proxyAddress + ":" + std::to_string(ProxyPort);
			io = InternetOpen(NULL, INTERNET_OPEN_TYPE_PROXY, s2ws(proxyBody).c_str(), NULL, 0);
		}
		else {
			std::string proxyBody = "socks=" + proxyAddress + ":" + std::to_string(ProxyPort);
			io = InternetOpen(NULL, CERN_PROXY_INTERNET_ACCESS, s2ws(proxyBody).c_str(), NULL, 0);
		}
	}
	else {
		io = InternetOpen(NULL, INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
	}

	HINTERNET ic = InternetConnect(io, s2ws(domain).c_str(), port, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
	if (ConnectionTimeout > -2)
		InternetSetOption(ic, INTERNET_OPTION_CONNECT_TIMEOUT, &ConnectionTimeout, sizeof(int));
	if (ReadWriteTimeout > -2) {
		InternetSetOption(ic, INTERNET_OPTION_RECEIVE_TIMEOUT, &ReadWriteTimeout, sizeof(int));
		InternetSetOption(ic, INTERNET_OPTION_SEND_TIMEOUT, &ReadWriteTimeout, sizeof(int));
	}
	HINTERNET hreq = HttpOpenRequest(ic, L"POST", s2ws(path).c_str(), L"HTTP/1.1", NULL, NULL, flags, 0);
	std::string ReqHeaders = "";
	for (int i = 0; i < 1024; i++) {
		if (headers[i] == "")break;
		ReqHeaders += headers[i] + "\r\n";
	}
	ReqHeaders += "Content-Type: " + contenttype + "\r\n";
	ReqHeaders += defheaders;
	HttpAddRequestHeaders(hreq, s2ws(ReqHeaders).c_str(), -1L, NULL);
	char* data = new char[body.size()];
	for (int i = 0; i < body.size(); i++) {
		data[i] = body[i];
	}
	HttpSendRequest(hreq, NULL, 0,data , body.size());
	HttpResponse response;
	response.Url = url;
	response.ReturnCode = GetStatusCode(hreq);
	if (response.ReturnCode == 0)throw GetLastError();
	std::string* ResponseHeaders = GetHeaders(hreq);

	for (int i = 1; i < 1024; i++) {
		if (ResponseHeaders[i - 1] == "")break;
		response.headers[i - 1] = ResponseHeaders[i];
	}
	int j = 0;
	response.responseBody = GetResponseBody(hreq);
	for (int i = 0; i < 1024; i++) {
		if (response.headers[i] == "")break;
		if (response.headers[i].find("Set-Cookie:") != std::string::npos) {
			std::string cookieforheader = GetCookie(response.headers[i]);
			response.cookies[j] = cookieforheader;
			j++;
		}
	}
	return response;
}

HttpResponse post_multipart(std::string url, std::string headers[], int headercount, bool proxyuse, std::string proxyAddress, int ProxyPort, ProxyType type, bool KeepAlive, bool AutoRedirect, bool AllowCookies, MultiPartContent content, bool PermenantHeaders, int ConnectionTimeout, int ReadWriteTimeout) {
	std::string* all = Seperate(url);
	std::string domain = all[0];
	std::string path = all[1];
	DWORD flags = NULL;
	int port = 80;
	if (url.find("https://") != std::string::npos) {
		flags |= INTERNET_FLAG_SECURE;
		flags |= INTERNET_FLAG_IGNORE_CERT_DATE_INVALID;
		flags |= INTERNET_FLAG_IGNORE_REDIRECT_TO_HTTP;
		port = 443;
	}
	if (KeepAlive) {
		flags |= INTERNET_FLAG_KEEP_CONNECTION;
	}
	if (!AutoRedirect) {
		flags |= INTERNET_FLAG_NO_AUTO_REDIRECT;
	}
	if (!AllowCookies) {
		flags |= INTERNET_FLAG_NO_COOKIES;
	}
	HINTERNET io;
	if (proxyuse) {
		if (type == http) {
			std::string proxyBody = proxyAddress + ":" + std::to_string(ProxyPort);
			io = InternetOpen(NULL, INTERNET_OPEN_TYPE_PROXY, s2ws(proxyBody).c_str(), NULL, 0);
		}
		else {
			std::string proxyBody = "socks=" + proxyAddress + ":" + std::to_string(ProxyPort);
			io = InternetOpen(NULL, CERN_PROXY_INTERNET_ACCESS, s2ws(proxyBody).c_str(), NULL, 0);
		}
	}
	else {
		io = InternetOpen(NULL, INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
	}
	HINTERNET ic = InternetConnect(io, s2ws(domain).c_str(), port, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
	if (ConnectionTimeout > -2)
		InternetSetOption(ic, INTERNET_OPTION_CONNECT_TIMEOUT, &ConnectionTimeout, sizeof(int));
	if (ReadWriteTimeout > -2) {
		InternetSetOption(ic, INTERNET_OPTION_RECEIVE_TIMEOUT, &ReadWriteTimeout, sizeof(int));
		InternetSetOption(ic, INTERNET_OPTION_SEND_TIMEOUT, &ReadWriteTimeout, sizeof(int));
	}
	HINTERNET hreq = HttpOpenRequest(ic, L"POST", s2ws(path).c_str(), L"HTTP/1.1", NULL, NULL, flags, 0);
	std::string ReqHeaders = "";
	for (int i = 0; i < 1024; i++) {
		if (headers[i] == "")break;
		ReqHeaders += headers[i] + "\r\n";
	}
	ReqHeaders += "Content-Type: multipart/form-data; boundary=" + content.seperator + "\r\n";
	ReqHeaders += defheaders;
	HttpAddRequestHeaders(hreq, s2ws(ReqHeaders).c_str(), -1L, NULL);

	std::string body = "";
	for (int i = 0; i < 1024; i++) {
		if (content.contents[i]._name == "")break;
		if (content.contents[i]._filename != "") {
			body += "--" + content.seperator;
			body += "\r\nContent-Disposition: form-data; name=\"" + content.contents[i]._name + "\"; filename=\"" + content.contents[i]._filename + "\"\r\nContent-Type: " + GetMimeType(content.contents[i]._filename) + "\r\n\r\n";
			body += content.contents[i]._value + "\r\n";
			continue;
		}
		else {
			body += "--" + content.seperator;
			body += "\r\nContent-Disposition: form-data; name=\"" + content.contents[i]._name + "\"\r\n\r\n";
			body += content.contents[i]._value + "\r\n";
			continue;
		}
	}
	body += "--" + content.seperator + "--";
	char* data = new char[body.size()];
	for (int i = 0; i < body.size(); i++) {
		data[i] = body[i];
	}
	HttpSendRequest(hreq, NULL, 0, data, body.size());
	HttpResponse response;
	response.Url = url;
	response.ReturnCode = GetStatusCode(hreq);
	if (response.ReturnCode == 0)throw GetLastError();
	std::string* ResponseHeaders = GetHeaders(hreq);

	for (int i = 1; i < 1024; i++) {
		if (ResponseHeaders[i - 1] == "")break;
		response.headers[i - 1] = ResponseHeaders[i];
	}
	int j = 0;
	response.responseBody = GetResponseBody(hreq);
	for (int i = 0; i < 1024; i++) {
		if (response.headers[i] == "")break;
		if (response.headers[i].find("Set-Cookie:") != std::string::npos) {
			std::string cookieforheader = GetCookie(response.headers[i]);
			response.cookies[j] = cookieforheader;
			j++;
		}
	}
	return response;
}

HttpResponse put(std::string url, std::string headers[], int headercount, bool proxyuse, std::string proxyAddress, int ProxyPort, ProxyType type, bool KeepAlive, bool AutoRedirect, bool AllowCookies, std::string body, std::string contenttype, bool PermenantHeaders, int ConnectionTimeout, int ReadWriteTimeout)
{
	std::string* all = Seperate(url);
	std::string domain = all[0];
	std::string path = all[1];
	DWORD flags = NULL;
	int port = 80;
	if (url.find("https://") != std::string::npos) {
		flags |= INTERNET_FLAG_SECURE;
		flags |= INTERNET_FLAG_IGNORE_CERT_DATE_INVALID;
		flags |= INTERNET_FLAG_IGNORE_REDIRECT_TO_HTTP;
		port = 443;
	}
	if (KeepAlive) {
		flags |= INTERNET_FLAG_KEEP_CONNECTION;
	}
	if (!AutoRedirect) {
		flags |= INTERNET_FLAG_NO_AUTO_REDIRECT;
	}
	if (!AllowCookies) {
		flags |= INTERNET_FLAG_NO_COOKIES;
	}


	HINTERNET io;
	if (proxyuse) {
		if (type == http) {
			std::string proxyBody = proxyAddress + ":" + std::to_string(ProxyPort);
			io = InternetOpen(NULL, INTERNET_OPEN_TYPE_PROXY, s2ws(proxyBody).c_str(), NULL, 0);
		}
		else {
			std::string proxyBody = "socks=" + proxyAddress + ":" + std::to_string(ProxyPort);
			io = InternetOpen(NULL, CERN_PROXY_INTERNET_ACCESS, s2ws(proxyBody).c_str(), NULL, 0);
		}
	}
	else {
		io = InternetOpen(NULL, INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
	}

	HINTERNET ic = InternetConnect(io, s2ws(domain).c_str(), port, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
	if (ConnectionTimeout > -2)
		InternetSetOption(ic, INTERNET_OPTION_CONNECT_TIMEOUT, &ConnectionTimeout, sizeof(int));
	if (ReadWriteTimeout > -2) {
		InternetSetOption(ic, INTERNET_OPTION_RECEIVE_TIMEOUT, &ReadWriteTimeout, sizeof(int));
		InternetSetOption(ic, INTERNET_OPTION_SEND_TIMEOUT, &ReadWriteTimeout, sizeof(int));
	}
	HINTERNET hreq = HttpOpenRequest(ic, L"PUT", s2ws(path).c_str(), L"HTTP/1.1", NULL, NULL, flags, 0);
	std::string ReqHeaders = "";
	for (int i = 0; i < 1024; i++) {
		if (headers[i] == "")break;
		ReqHeaders += headers[i] + "\r\n";
	}
	ReqHeaders += "Content-Type: " + contenttype + "\r\n";
	ReqHeaders += defheaders;

	HttpAddRequestHeaders(hreq, s2ws(ReqHeaders).c_str(), -1L, NULL);
	char* data = new char[body.size()];
	for (int i = 0; i < body.size(); i++) {
		data[i] = body[i];
	}
	HttpSendRequest(hreq, NULL, 0, data, body.size());
	HttpResponse response;
	response.Url = url;
	response.ReturnCode = GetStatusCode(hreq);
	if (response.ReturnCode == 0)throw GetLastError();
	std::string* ResponseHeaders = GetHeaders(hreq);

	for (int i = 1; i < 1024; i++) {
		if (ResponseHeaders[i - 1] == "")break;
		response.headers[i - 1] = ResponseHeaders[i];
	}
	int j = 0;
	response.responseBody = GetResponseBody(hreq);
	for (int i = 0; i < 1024; i++) {
		if (response.headers[i] == "")break;
		if (response.headers[i].find("Set-Cookie:") != std::string::npos) {
			std::string cookieforheader = GetCookie(response.headers[i]);
			response.cookies[j] = cookieforheader;
			j++;
		}
	}
	return response;
}

HttpResponse put_multipart(std::string url, std::string headers[], int headercount, bool proxyuse, std::string proxyAddress, int ProxyPort, ProxyType type, bool KeepAlive, bool AutoRedirect, bool AllowCookies, MultiPartContent content, bool PermenantHeaders, int ConnectionTimeout, int ReadWriteTimeout) {
	std::string* all = Seperate(url);
	std::string domain = all[0];
	std::string path = all[1];
	DWORD flags = NULL;
	int port = 80;
	if (url.find("https://") != std::string::npos) {
		flags |= INTERNET_FLAG_SECURE;
		flags |= INTERNET_FLAG_IGNORE_CERT_DATE_INVALID;
		flags |= INTERNET_FLAG_IGNORE_REDIRECT_TO_HTTP;
		port = 443;
	}
	if (KeepAlive) {
		flags |= INTERNET_FLAG_KEEP_CONNECTION;
	}
	if (!AutoRedirect) {
		flags |= INTERNET_FLAG_NO_AUTO_REDIRECT;
	}
	if (!AllowCookies) {
		flags |= INTERNET_FLAG_NO_COOKIES;
	}
	HINTERNET io;
	if (proxyuse) {
		if (type == http) {
			std::string proxyBody = proxyAddress + ":" + std::to_string(ProxyPort);
			io = InternetOpen(NULL, INTERNET_OPEN_TYPE_PROXY, s2ws(proxyBody).c_str(), NULL, 0);
		}
		else {
			std::string proxyBody = "socks=" + proxyAddress + ":" + std::to_string(ProxyPort);
			io = InternetOpen(NULL, CERN_PROXY_INTERNET_ACCESS, s2ws(proxyBody).c_str(), NULL, 0);
		}
	}
	else {
		io = InternetOpen(NULL, INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
	}
	HINTERNET ic = InternetConnect(io, s2ws(domain).c_str(), port, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
	if (ConnectionTimeout > -2)
		InternetSetOption(ic, INTERNET_OPTION_CONNECT_TIMEOUT, &ConnectionTimeout, sizeof(int));
	if (ReadWriteTimeout > -2) {
		InternetSetOption(ic, INTERNET_OPTION_RECEIVE_TIMEOUT, &ReadWriteTimeout, sizeof(int));
		InternetSetOption(ic, INTERNET_OPTION_SEND_TIMEOUT, &ReadWriteTimeout, sizeof(int));
	}
	HINTERNET hreq = HttpOpenRequest(ic, L"PUT", s2ws(path).c_str(), L"HTTP/1.1", NULL, NULL, flags, 0);
	std::string ReqHeaders = "";
	for (int i = 0; i < 1024; i++) {
		if (headers[i] == "")break;
		ReqHeaders += headers[i] + "\r\n";
	}
	ReqHeaders += "Content-Type: multipart/form-data; boundary=" + content.seperator + "\r\n";
	ReqHeaders += defheaders;
	HttpAddRequestHeaders(hreq, s2ws(ReqHeaders).c_str(), -1L, NULL);

	std::string body = "";
	for (int i = 0; i < 1024; i++) {
		if (content.contents[i]._name == "")break;
		if (content.contents[i]._filename != "") {
			body += "--" + content.seperator;
			body += "\r\nContent-Disposition: form-data; name=\"" + content.contents[i]._name + "\"; filename=\"" + content.contents[i]._filename + "\"\r\nContent-Type: " + GetMimeType(content.contents[i]._filename) + "\r\n\r\n";
			body += content.contents[i]._value + "\r\n";
			continue;
		}
		else {
			body += "--" + content.seperator;
			body += "\r\nContent-Disposition: form-data; name=\"" + content.contents[i]._name + "\"\r\n\r\n";
			body += content.contents[i]._value + "\r\n";
			continue;
		}
	}
	body += "--" + content.seperator + "--";
	char* data = new char[body.size()];
	for (int i = 0; i < body.size(); i++) {
		data[i] = body[i];
	}
	HttpSendRequest(hreq, NULL, 0, data, body.size());
	HttpResponse response;
	response.Url = url;
	response.ReturnCode = GetStatusCode(hreq);
	if (response.ReturnCode == 0)throw GetLastError();
	std::string* ResponseHeaders = GetHeaders(hreq);

	for (int i = 1; i < 1024; i++) {
		if (ResponseHeaders[i - 1] == "")break;
		response.headers[i - 1] = ResponseHeaders[i];
	}
	int j = 0;
	response.responseBody = GetResponseBody(hreq);
	for (int i = 0; i < 1024; i++) {
		if (response.headers[i] == "")break;
		if (response.headers[i].find("Set-Cookie:") != std::string::npos) {
			std::string cookieforheader = GetCookie(response.headers[i]);
			response.cookies[j] = cookieforheader;
			j++;
		}
	}
	return response;
}

HttpResponse patch(std::string url, std::string headers[], int headercount, bool proxyuse, std::string proxyAddress, int ProxyPort, ProxyType type, bool KeepAlive, bool AutoRedirect, bool AllowCookies, std::string body, std::string contenttype, bool PermenantHeaders, int ConnectionTimeout, int ReadWriteTimeout)
{
	std::string* all = Seperate(url);
	std::string domain = all[0];
	std::string path = all[1];
	DWORD flags = NULL;
	int port = 80;
	if (url.find("https://") != std::string::npos) {
		flags |= INTERNET_FLAG_SECURE;
		flags |= INTERNET_FLAG_IGNORE_CERT_DATE_INVALID;
		flags |= INTERNET_FLAG_IGNORE_REDIRECT_TO_HTTP;
		port = 443;
	}
	if (KeepAlive) {
		flags |= INTERNET_FLAG_KEEP_CONNECTION;
	}
	if (!AutoRedirect) {
		flags |= INTERNET_FLAG_NO_AUTO_REDIRECT;
	}
	if (!AllowCookies) {
		flags |= INTERNET_FLAG_NO_COOKIES;
	}


	HINTERNET io;
	if (proxyuse) {
		if (type == http) {
			std::string proxyBody = proxyAddress + ":" + std::to_string(ProxyPort);
			io = InternetOpen(NULL, INTERNET_OPEN_TYPE_PROXY, s2ws(proxyBody).c_str(), NULL, 0);
		}
		else {
			std::string proxyBody = "socks=" + proxyAddress + ":" + std::to_string(ProxyPort);
			io = InternetOpen(NULL, CERN_PROXY_INTERNET_ACCESS, s2ws(proxyBody).c_str(), NULL, 0);
		}
	}
	else {
		io = InternetOpen(NULL, INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
	}

	HINTERNET ic = InternetConnect(io, s2ws(domain).c_str(), port, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
	if (ConnectionTimeout > -2)
		InternetSetOption(ic, INTERNET_OPTION_CONNECT_TIMEOUT, &ConnectionTimeout, sizeof(int));
	if (ReadWriteTimeout > -2) {
		InternetSetOption(ic, INTERNET_OPTION_RECEIVE_TIMEOUT, &ReadWriteTimeout, sizeof(int));
		InternetSetOption(ic, INTERNET_OPTION_SEND_TIMEOUT, &ReadWriteTimeout, sizeof(int));
	}
	HINTERNET hreq = HttpOpenRequest(ic, L"PATCH", s2ws(path).c_str(), L"HTTP/1.1", NULL, NULL, flags, 0);
	std::string ReqHeaders = "";
	for (int i = 0; i < 1024; i++) {
		if (headers[i] == "")break;
		ReqHeaders += headers[i] + "\r\n";
	}
	ReqHeaders += "Content-Type: " + contenttype + "\r\n";
	ReqHeaders += defheaders;

	HttpAddRequestHeaders(hreq, s2ws(ReqHeaders).c_str(), -1L, NULL);
	char* data = new char[body.size()];
	for (int i = 0; i < body.size(); i++) {
		data[i] = body[i];
	}
	HttpSendRequest(hreq, NULL, 0, data, body.size());
	HttpResponse response;
	response.Url = url;
	response.ReturnCode = GetStatusCode(hreq);
	if (response.ReturnCode == 0)throw GetLastError();
	std::string* ResponseHeaders = GetHeaders(hreq);

	for (int i = 1; i < 1024; i++) {
		if (ResponseHeaders[i - 1] == "")break;
		response.headers[i - 1] = ResponseHeaders[i];
	}
	int j = 0;
	response.responseBody = GetResponseBody(hreq);
	for (int i = 0; i < 1024; i++) {
		if (response.headers[i] == "")break;
		if (response.headers[i].find("Set-Cookie:") != std::string::npos) {
			std::string cookieforheader = GetCookie(response.headers[i]);
			response.cookies[j] = cookieforheader;
			j++;
		}
	}
	return response;
}

HttpResponse patch_multipart(std::string url, std::string headers[], int headercount, bool proxyuse, std::string proxyAddress, int ProxyPort, ProxyType type, bool KeepAlive, bool AutoRedirect, bool AllowCookies, MultiPartContent content, bool PermenantHeaders, int ConnectionTimeout, int ReadWriteTimeout) {
	std::string* all = Seperate(url);
	std::string domain = all[0];
	std::string path = all[1];
	DWORD flags = NULL;
	int port = 80;
	if (url.find("https://") != std::string::npos) {
		flags |= INTERNET_FLAG_SECURE;
		flags |= INTERNET_FLAG_IGNORE_CERT_DATE_INVALID;
		flags |= INTERNET_FLAG_IGNORE_REDIRECT_TO_HTTP;
		port = 443;
	}
	if (KeepAlive) {
		flags |= INTERNET_FLAG_KEEP_CONNECTION;
	}
	if (!AutoRedirect) {
		flags |= INTERNET_FLAG_NO_AUTO_REDIRECT;
	}
	if (!AllowCookies) {
		flags |= INTERNET_FLAG_NO_COOKIES;
	}
	HINTERNET io;
	if (proxyuse) {
		if (type == http) {
			std::string proxyBody = proxyAddress + ":" + std::to_string(ProxyPort);
			io = InternetOpen(NULL, INTERNET_OPEN_TYPE_PROXY, s2ws(proxyBody).c_str(), NULL, 0);
		}
		else {
			std::string proxyBody = "socks=" + proxyAddress + ":" + std::to_string(ProxyPort);
			io = InternetOpen(NULL, CERN_PROXY_INTERNET_ACCESS, s2ws(proxyBody).c_str(), NULL, 0);
		}
	}
	else {
		io = InternetOpen(NULL, INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
	}
	HINTERNET ic = InternetConnect(io, s2ws(domain).c_str(), port, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
	if (ConnectionTimeout > -2)
		InternetSetOption(ic, INTERNET_OPTION_CONNECT_TIMEOUT, &ConnectionTimeout, sizeof(int));
	if (ReadWriteTimeout > -2) {
		InternetSetOption(ic, INTERNET_OPTION_RECEIVE_TIMEOUT, &ReadWriteTimeout, sizeof(int));
		InternetSetOption(ic, INTERNET_OPTION_SEND_TIMEOUT, &ReadWriteTimeout, sizeof(int));
	}
	HINTERNET hreq = HttpOpenRequest(ic, L"PATCH", s2ws(path).c_str(), L"HTTP/1.1", NULL, NULL, flags, 0);
	std::string ReqHeaders = "";
	for (int i = 0; i < 1024; i++) {
		if (headers[i] == "")break;
		ReqHeaders += headers[i] + "\r\n";
	}
	ReqHeaders += "Content-Type: multipart/form-data; boundary=" + content.seperator + "\r\n";
	ReqHeaders += defheaders;
	HttpAddRequestHeaders(hreq, s2ws(ReqHeaders).c_str(), -1L, NULL);

	std::string body = "";
	for (int i = 0; i < 1024; i++) {
		if (content.contents[i]._name == "")break;
		if (content.contents[i]._filename != "") {
			body += "--" + content.seperator;
			body += "\r\nContent-Disposition: form-data; name=\"" + content.contents[i]._name + "\"; filename=\"" + content.contents[i]._filename + "\"\r\nContent-Type: " + GetMimeType(content.contents[i]._filename) + "\r\n\r\n";
			body += content.contents[i]._value + "\r\n";
			continue;
		}
		else {
			body += "--" + content.seperator;
			body += "\r\nContent-Disposition: form-data; name=\"" + content.contents[i]._name + "\"\r\n\r\n";
			body += content.contents[i]._value + "\r\n";
			continue;
		}
	}
	body += "--" + content.seperator + "--";
	char* data = new char[body.size()];
	for (int i = 0; i < body.size(); i++) {
		data[i] = body[i];
	}
	HttpSendRequest(hreq, NULL, 0, data, body.size());
	HttpResponse response;
	response.Url = url;
	response.ReturnCode = GetStatusCode(hreq);
	if (response.ReturnCode == 0)throw GetLastError();
	std::string* ResponseHeaders = GetHeaders(hreq);

	for (int i = 1; i < 1024; i++) {
		if (ResponseHeaders[i - 1] == "")break;
		response.headers[i - 1] = ResponseHeaders[i];
	}
	int j = 0;
	response.responseBody = GetResponseBody(hreq);
	for (int i = 0; i < 1024; i++) {
		if (response.headers[i] == "")break;
		if (response.headers[i].find("Set-Cookie:") != std::string::npos) {
			std::string cookieforheader = GetCookie(response.headers[i]);
			response.cookies[j] = cookieforheader;
			j++;
		}
	}
	return response;
}

std::string* GetHeaders(HINTERNET hreq) {
	DWORD dwInfoLevel = HTTP_QUERY_RAW_HEADERS_CRLF;
	DWORD dwInfoBufferLength = 10;
	BYTE* pInfoBuffer = (BYTE*)malloc(dwInfoBufferLength + 1);
	while (!HttpQueryInfo(hreq, dwInfoLevel, pInfoBuffer, &dwInfoBufferLength, NULL))
	{
		DWORD dwError = GetLastError();
		if (dwError == ERROR_INSUFFICIENT_BUFFER)
		{
			free(pInfoBuffer);
			pInfoBuffer = (BYTE*)malloc(dwInfoBufferLength + 1);
		}
		else
		{
			throw GetLastError();
			break;
		}
	}
	pInfoBuffer[dwInfoBufferLength] = '\0';
	char* ccc = (char*)pInfoBuffer;
	std::string *headerss = new std::string[1024];
	int b = 0;
	int k = 0;
	int h = 0;
	while (b != 2) {
		if (ccc[k] == '\0') {
			b++;
		}
		else {
			b = 0;
			if (ccc[k] == '\n') {
				h++;
				k++;
				continue;
			}
			headerss[h] += ccc[k];
		}
		k++;
	}

	for (int i = 0; i < h; i++) {
		for (int l = 0; l < headerss[i].size(); l++) {
			if (headerss[i][l] == '\r') {
				headerss[i][l] = '\0';
			}
		}
	}
	return headerss;
}

int GetStatusCode(HINTERNET hreq) {
	DWORD statusCode = 0;
	DWORD length = sizeof(DWORD);
	HttpQueryInfo(hreq, HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER, &statusCode, &length, NULL);
	
	return (int)statusCode;
}

std::string* Seperate(std::string url) {
	int index;
	if (url.find("http://") != std::string::npos) {
		index = 7;
	}
	else if (url.find("https://") != std::string::npos) {
		index = 8;
	}
	else {
		index = 0;
	}

	std::string domain = "";
	char c = NULL;
	while (c != '/') {
		try {
			if (index > url.size() - 1)break;
			c = url[index];
			index++;
			domain += c;
		}
		catch (const std::exception & e) {
			break;
		}
	}
	if(domain[domain.size()-1] == '/')
	domain = domain.substr(0, domain.size() - 1);
	std::string path = "";
	for (int i = index; i < url.size(); i++) {
		c = url[i];
		path += c;
	}

	std::string *all = new std::string[2];
	all[0] = domain;
	all[1] = path;
	return all;
}

std::string GetResponseBody(HINTERNET hreq) {
	DWORD blocksize = 4096;
	DWORD received = 0;
	std::string temp;
	std::string block(blocksize, 0);
	while (InternetReadFile(hreq, &block[0], blocksize, &received) && received)
	{
		block.resize(received);
		temp += block;
	}
	return temp;
}

std::string GetCookie(std::string cookieHeader) {
	std::string a = "";
	int i = 12;
	char c = NULL;
	while (c != ';') {
		c = cookieHeader[i];
		a += c;
		i++;
	}
	a = a.substr(0, a.size() - 1);
	return a;
}

MultiPartContent::MultiPartContent(std::initializer_list<HttpContent> h) {
	int i = 0;
	for (HttpContent content:h) {
		contents[i] = content;
		i++;
	}
	Random random;
	for (i = 0; i < 32; i++) {
		seperator += random.GenerateChar();
	}
}

void SetDefHeaders(HttpRequest* req) {
	HttpRequest request = *req;
	if (request.AcceptEncoding != "")defheaders += "Accept-Encoding: " + request.AcceptEncoding + "\r\n";
	if (request.Authorization != "")defheaders += "Authorization: " + request.Authorization + "\r\n";
	if (request.Cookies != "")defheaders += "Cookies: " + request.Cookies + "\r\n";
	if (request.Referer != "")defheaders += "Referer: " + request.Referer + "\r\n";
	if (request.UserAgent != "")defheaders += "User-Agent: " + request.UserAgent + "\r\n";
}
