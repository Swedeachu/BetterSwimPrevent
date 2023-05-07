#include "Helpers/stdafx.h"

#include <sstream>
#include <algorithm>

#include "openssl\md5.h"
#include "openssl\evp.h"

#pragma comment(lib, "crypt32.lib")

#include "UtilMD5.h"

namespace Hash
{

	// returns the MD5 hash value of a file path as a wide string
	std::wstring MD5Checksum(const std::wstring& wideFilePath)
	{
		// Open the file
		FILE* fileStream;
		errno_t err = _wfopen_s(&fileStream, wideFilePath.c_str(), L"rb");
		if (err != 0 || !fileStream)
		{
			std::cerr << "Error: Unable to open file." << std::endl;
			return L"";
		}

		// Initialize the OpenSSL library and create the MD5 context
		OpenSSL_add_all_digests();
		const EVP_MD* md5_md = EVP_get_digestbyname("MD5");
		EVP_MD_CTX* md5Context = EVP_MD_CTX_new();
		EVP_DigestInit_ex(md5Context, md5_md, NULL);

		// Read the file and update the MD5 context
		const size_t bufferSize = 1024;
		std::vector<char> buffer(bufferSize);
		size_t bytesRead;
		while ((bytesRead = fread(buffer.data(), 1, bufferSize, fileStream)) > 0)
		{
			EVP_DigestUpdate(md5Context, buffer.data(), bytesRead);
		}

		// Finalize the MD5 context and store the result
		unsigned char md5Result[EVP_MAX_MD_SIZE];
		unsigned int md5ResultLength;
		EVP_DigestFinal_ex(md5Context, md5Result, &md5ResultLength);

		// Clean up and free resources
		EVP_MD_CTX_free(md5Context);
		EVP_cleanup();
		fclose(fileStream);

		// Convert the MD5 result to a wide string
		std::wstringstream md5StringStream;
		md5StringStream << std::hex << std::setfill(L'0');
		for (unsigned int i = 0; i < md5ResultLength; ++i)
		{
			md5StringStream << std::setw(2) << static_cast<unsigned>(md5Result[i]);
		}

		return md5StringStream.str();
	}

	// returns the cheat associated with the hash, 0 if it is not a cheat
	std::wstring MatchMD5(const std::wstring& hash)
	{
		// Convert the input hash to lowercase
		std::wstring lowercaseHash = hash;
		std::transform(lowercaseHash.begin(), lowercaseHash.end(), lowercaseHash.begin(), [](wchar_t c)
		{
			return std::tolower(c);
		});

		auto it = mappedHashes.find(lowercaseHash);
		if (it != mappedHashes.end())
		{
			return it->second;
		}
		return L"0";
	}

	// the set of illegal hash's for programs
	std::unordered_map<std::wstring, std::wstring> mappedHashes = {
	{L"4ca3c23042c6e7baa2171fe2c2243cd6", L"horion 1.12"},
	{L"09a637f677110149c6e43223ef77daa7", L"ascendancy.exe"},
	{L"689aead0bdf3f68f3a5f1273cce7ed04", L"fake svhost"},
	{L"9e6de7c7ebd1a00c2f7ddec78ba9403a", L"fate injector.exe"},
	//{L"8e20218dfef733ffd8fa284d4e1baf78", L"old onix injector.exe"},
	//{L"e4759b2e6aff7e4d18a02f8e1fe2223d", L"onix launcher.exe"},
	//{L"b91ab28db07f131095929ebf7a0e6b8a", L"onix.dlL"},
	//{L"cacaafec50116b8120027954bad9afdd", L"onix injector.exe"},
	{L"4d7be1a6dad181d0e42afa7ac7d8a872", L"helix.dlL"},
	{L"689c0ad4003f1feab8ec45b281489460", L"helix alpha.exe"},
	{L"0ec153e4eabb1aa5dfe11fd16ed1ea23", L"helix pvp injector.exe"},
	{L"eb44d5a1914c9c32ef9994f9c4f6188d", L"vene clicker.jar"},
	{L"b77fcb0221e5edf634ff3b7a9cb75937", L"nitro 1.17.10"},
	{L"e93e3856353aa90a2e93d6e2d626afc1", L"eternium 1.17.10"},
	{L"f8508b90a8dea540f5bd2d497932c71a", L"xodus.exe"},
	{L"7872e9b626775f63747fcdf6650f647e", L"ambrosiaL"},
	{L"c33b0ffc80e108ecff0c3832a06a8ec6", L"nitro 1.17.2"},
	{L"ef279b341f62120e1f5abcff85b24541", L"horion dll 1.17.2"},
	{L"18cb627b78dd3c81f67f4002b92eecba", L"yaamiclient.dll 1.17.2"},
	{L"021f85aca44ed090df93c241cb37de1a", L"anarchy_client 1.17.2"},
	{L"29212477df769b364b5769f0a5506fe0", L"auto_updating_hooks 1.17.2/1.17.0"},
	{L"70b08dd86f749c1d26296bd8060224fb", L"nitro 1.17.0"},
	{L"cff9e3e67d55bfecadf0d3d5ae14b35c", L"ambrosial_and_zephyr 1.17.0"},
	{L"42384237c8ca34c94c9753f6484397ab", L"anarchy_client 1.17.0"},
	{L"0f84efdb27bb7cdf6a27a4e0ec5ce19b", L"nitro 1.16.221"},
	{L"58bdb1920eba082553a7f8f7d62cf9cf", L"loki 1.16.221"},
	{L"655dbf3a33cae2ee1c0bc68b91c399eb", L"moon_v0.3 1.16.221"},
	{L"d82d15f63342377d9a2c9ca5cd479a2b", L"strike 1.16.221"},
	{L"d7259dde5d8f36ff2246391fffacf60f", L"eternium 1.16.221/ 1.16.220"},
	{L"8c10c62384d6d5dc33edc28c127e6d8d", L"eclipse 1.16.221"},
	{L"ad5ce67af55e471417e929afdbd9ae9f", L"autism 1.16.221"},
	{L"3adb1338edca48c8a1dca71cf2d70b53", L"wave 1.16.221"},
	{L"8a184f8a9c3d3a9bfd26d1676643dd98", L"nitro 1.16.220"},
	{L"2afeec5fbb445ded2fac959df9ae6b6f", L"eclipse_client"},
	{L"8506318ac79494c9f7047bf76adf7e33", L"quadrix.dll 1.16.220"},
	{L"11b45786cf2a41009bcd53ee8bcb2e4f", L"nitro 1.16.210"},
	{L"348ee5afb324bb1e8297ecef396cbb84", L"disguised process hacker nitr0  1.16.210"},
	{L"894135df60d9370cddc17d316eb40dcc", L"disguised reasource hacker nitr0  1.16.210"},
	{L"4c6703d30f207377329fe92e5597c82a", L"disguised spotify nitr0  1.16.210"},
	{L"555125301ae860c13a1eadb8732ab447", L"disguised minecraft launcher nitr0  1.16.210"},
	{L"235aa72b4fb4b2306ece386b744a24c8", L"coffe 1.16.210, coffee 1.16.201"},
	{L"da8d630de50a275d0319e48df50d7bcc", L"eternium 1.16.210"},
	{L"e420cd70aafa0ca42c6242e0f5ebbe8f", L"nitro 1.16.201"},
	{L"1f52a54d68cc984cd4f2ffd7a1e675c5", L"fadeaway 1.16.201"},
	{L"ba2b6a24ab8db28cfe9d27f590b60616", L"badmanpublic 1.16.201"},
	{L"0a414304fa0dde93c6c7276a339566b0", L"v3cter 1.16.201"},
	{L"f9a70c5d744ada0c4ffab4c83600bfac", L"rixcheets 1.16.201"},
	{L"1987aab4325441558d798d5d1dd31e60", L"packetclient 1.16.201"},
	{L"b3d680a7db274ade853bd64980553693", L"horiondll 1.16.201"},
	{L"200b18c738dd5465f6c638428d6fa8de", L"horiondebuginjector 1.16.201"},
	{L"81d6e71a75e590237041e38851756dba", L"lakeside 1.16.100"},
	{L"e9cb88b026ce2f528017636222af1251", L"nitro 1.16.100"},
	{L"d6cf5b18f80300be621eddd41f1425b0", L"explorer 1.16.100, distinct 1.16.100"},
	{L"7ecfc8cd7455dd9998f7dad88f2a8a9d", L"op autoclicker.exe"},
	{L"9350c5cf61a4d3d68177fad54acfd87c", L"xmousebuttoncontrol.exe"},
	{L"8fd172f85ee2111bb2c796f255dfca77", L"7clicker.jar"},
	{L"77652d318a4e61f0a6d76028b75493a3", L"mouseclicker.exe"},
	{L"6862f65be14fd3ce88086ec79777db6e", L"gs auto clicker.exe"},
	{L"27d5a6c9274255cf89b38f28480edcf0", L"polr clicker.exe"},
	{L"64701ae13e815dff419e73cf9d66f924", L"thefastestmouseclicker.exe"},
	{L"4f3ce2d1c1c63ad7d1f563a9ffd5b7bd", L"thefastestmousegroupclicker.exe"},
	{L"2ce992abd25f22de9a9b737bc608cf89", L"cheat engine.exe"},
	{L"f921416197c2ae407d53ba5712c3930a", L"cheat engine 7.5.exe"},
	{L"edeef697cbf212b5ecfcd9c1d9a8803d", L"cheat engine 7.5.exe"},
	{L"5be6a65f186cf219fa25bdd261616300", L"cheat engine 7.5.exe"},
	{L"910de25bd63b5da521fc0b598920c4ec", L"cheat engine 7.5.exe"},
	{L"84b67974390a284a9b50e1fb1848b8bf", L"clumsy.exe"},
	{L"a68ee5afb324bo9e8297ecef396cbp84", L"ascendancy 1.17.10"},
	{L"20b98dd86ni49c1d26296bod8060224f", L"ascendancy 1.17.2"},
	{L"e16v3856393ad90a2e93d6e2d62nafc1", L"ascendancy 1.17.2 lite"},
	{L"9s1oe1b6267s5f63749fcdf6650v647e", L"ascendancy 1.17.0"},
	{L"1a05b925f9cb6aace8e20226305f6d87", L"packet client injector.exe"},
	{L"c6cd45133f9d90973c57c568dfee0d51", L"packet client dlL"},
	{L"01bd9957dd4b0e580e6a0fa271667c9f", L"packet client old dlL"},
	{L"98b2109bcf941b4fbbad01c45f9d4b88", L"nitr0 lite"},
	{L"867aee91cd59fe555b607d28fea4e9af", L"treor"},
	{L"596b0f4684d45de83c204967c06e48a3", L"ambrosiaL"},
	{L"b365af317ae730a67c936f21432b9c71", L"process hacker 2.0"},
	{L"ae0d569dd692dd8374b4e211f2f4c1bd", L"nitro 1.18"},
	{L"7a9bb20aaf6bf6971c8d40a4fe3352e6", L"nitro"},
	{L"1289463d7d8df7d9176068f59c2ef2d0", L"nitro"},
	{L"401ed0badfe8bc5b1c846716214acb72", L"nitro lite"},
	{L"a5a8a0c225982cc2e48250c77476d4bc", L"nitro"},
	{L"9f68552aa09d850984788fa6697e240b", L"nitro"},
	{L"34b4737f23d1545a345ffbbdff43c0eb", L"nitro"},
	{L"bf0d96e4046134b854b5467fa365ef78", L"nitro"},
	{L"d02140e2d65107f79ac52eea83698535", L"nitro"},
	{L"45947390ad0391be18fc5e503f9cd66e", L"nitro"},
	{L"cef0ec6f3332ef60a87ac4003f4dbe6d", L"nitro"},
	{L"10af2d94f7710876142f60244911addc", L"nitro"},
	{L"96cc083b1cb50e9d914f8f8785c4ab35", L"nitro"},
	{L"5d90bf948c41ac2c4789bda9d948323f", L"cnd"},
	};

} // Hash
