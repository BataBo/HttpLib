//This cpp file is c++ adaptation of c# code that you can find in library Leaf.Xnet,full credit for the logic goes to grandsilence
#include "pch.h"
#include "SupportHeader.h"


int firefoxversion[9] = { 64,63,62,60,58,52,51,46,45 };
Random random;

std::string RandomWindowsVersion() {
	std::string text = "Windows NT ";
	int randomizer = random.GenerateInt(0, 100);
	if (randomizer > 0 && randomizer < 46)text += "10.0";
	if (randomizer > 45 && randomizer < 81)text += "6.1";
	if (randomizer > 80 && randomizer < 96)text += "6.3";
	if (randomizer > 95)text += "6.2";
	int randomizer2 = random.GenerateInt(0, 100);
	if (randomizer2 < 66) {
		int randomizer3 = random.GenerateInt(0, 100);
		if (randomizer3 < 51)text += "; WOW64"; else text += "; Win64; x64";
	}
	return text;
}

std::string ChromeUserAgent() {
	return "Mozilla/5.0 (" + RandomWindowsVersion() + ") AppleWebKit/537.36 (KHTML, like Gecko) Chrome/" + std::to_string(random.GenerateInt(62, 70)) + ".0." + std::to_string(random.GenerateInt(2100, 3538)) + "." + std::to_string(random.GenerateInt(0, 170)) + " Safari/537.36";
}
std::string FirefoxUserAgent() {
	std::string a = std::to_string(firefoxversion[random.GenerateInt(0, 9)]);
	return "Mozilla/5.0 (" + RandomWindowsVersion() + "; rv:" + a + ".0) Gecko/20100101 Firefox/" + a + ".0";
}
std::string IEUserAgent() {
	std::string text = RandomWindowsVersion();
	std::string text2, text3, text4, text5;
	if (text.find("NT 5.1") != std::string::npos)
	{
		text2 = "9.0";
		text3 = "5.0";
		text4 = "5.0";
		text5 = ".NET CLR 2.0.50727; .NET CLR 3.5.30729";
	}
	else if (text.find("NT 6.0") != std::string::npos)
	{
		text2 = "9.0";
		text3 = "5.0";
		text4 = "5.0";
		text5 = ".NET CLR 2.0.50727; Media Center PC 5.0; .NET CLR 3.5.30729";
	}
	else {
		int num = random.GenerateInt(0, 3);
		if (num != 0)
		{
			if (num != 1)
			{
				text2 = "11.0";
				text4 = "7.0";
				text3 = "5.0";
			}
			else
			{
				text2 = "10.6";
				text4 = "6.0";
				text3 = "5.0";
			}
		}
		else
		{
			text2 = "10.0";
			text4 = "6.0";
			text3 = "5.0";
		}
		text5 = ".NET CLR 2.0.50727; .NET CLR 3.5.30729; .NET CLR 3.0.30729; Media Center PC 6.0; .NET4.0C; .NET4.0E";
	}
	return "Mozilla/" + text3 + " (compatible; MSIE " + text2 + "; " + text + "; Trident/" + text4 + "; " + text5 + ")";
}
std::string OperaMiniUserAgent() {
	int num = random.GenerateInt(0, 3);
	std::string text;
	std::string text2;
	std::string text3;
	std::string text4;
	if (num != 0)
	{
		if (num != 1)
		{
			text = "Android";
			text2 = "7.5.54678";
			text3 = "12.02";
			text4 = "2.10.289";
		}
		else
		{
			text = "J2ME/MIDP";
			text2 = "7.1.23511";
			text3 = "12.00";
			text4 = "2.10.181";
		}
	}
	else
	{
		text = "iOS";
		text2 = "7.0.73345";
		text3 = "11.62";
		text4 = "2.10.229";
	}
	return "Opera/9.80 (" + text + "; Opera Mini/" + text2 + "/28.2555; U; ru) Presto/" + text4 + " Version/" + text3;
}
std::string OperaUserAgent() {
	std::string text;
	std::string text2;
	switch (random.GenerateInt(0, 4))
	{
	case 0:
		text = "12.16";
		text2 = "2.12.388";
		break;
	case 1:
		text = "12.14";
		text2 = "2.12.388";
		break;
	case 2:
		text = "12.02";
		text2 = "2.10.289";
		break;
	default:
		text = "12.00";
		text2 = "2.10.181";
		break;
	}
	return "Opera/9.80 (" + RandomWindowsVersion() + "); U) Presto/" + text2 + " Version/" + text;
}
std::string RandomUserAgent() {
	int num = random.GenerateInt(0, 100);
	if (num >= 1 && num <= 70)
	{
		return ChromeUserAgent();
	}
	if (num > 70 && num <= 85)
	{
		return FirefoxUserAgent();
	}
	if (num > 85 && num <= 91)
	{
		return IEUserAgent();
	}
	if (num > 91 && num <= 96)
	{
		return OperaUserAgent();
	}
	return OperaMiniUserAgent();
}