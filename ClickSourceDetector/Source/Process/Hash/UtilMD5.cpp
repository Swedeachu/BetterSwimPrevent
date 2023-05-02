#include "Helpers/stdafx.h"

#include <sstream>

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
		auto it = mappedHashes.find(hash);
		if (it != mappedHashes.end())
		{
			return it->second;
		}
		return L"0";
	}

	// the set of illegal hash's for programs
	std::unordered_map<std::wstring, std::wstring> mappedHashes = {
	{L"4ca3c23042c6e7baa2171fe2c2243cd6", L"Horion 1.12"},
	{L"09A637F677110149C6E43223EF77DAA7", L"Ascendancy.exe"},
	{L"689AEAD0BDF3F68F3A5F1273CCE7ED04", L"Fake svhost"},
	{L"9E6DE7C7EBD1A00C2F7DDEC78BA9403A", L"Fate Injector.exe"},
	//{L"8E20218DFEF733FFD8FA284D4E1BAF78", L"Old Onix Injector.exe"},
	//{L"E4759B2E6AFF7E4D18A02F8E1FE2223D", L"Onix Launcher.exe"},
	//{L"B91AB28DB07F131095929EBF7A0E6B8A", L"Onix.dll"},
	//{L"CACAAFEC50116B8120027954BAD9AFDD", L"Onix Injector.exe"},
	{L"4D7BE1A6DAD181D0E42AFA7AC7D8A872", L"Helix.dll"},
	{L"689C0AD4003F1FEAB8EC45B281489460", L"Helix Alpha.exe"},
	{L"0EC153E4EABB1AA5DFE11FD16ED1EA23", L"Helix PvP Injector.exe"},
	{L"EB44D5A1914C9C32EF9994F9C4F6188D", L"Vene Clicker.jar"},
	{L"B77FCB0221E5EDF634FF3B7A9CB75937", L"Nitro 1.17.10"},
	{L"E93E3856353AA90A2E93D6E2D626AFC1", L"Eternium 1.17.10"},
	{L"F8508B90A8DEA540F5BD2D497932C71A", L"Xodus.exe"},
	{L"7872E9B626775F63747FCDF6650F647E", L"Ambrosial"},
	{L"C33B0FFC80E108ECFF0C3832A06A8EC6", L"Nitro 1.17.2"},
	{L"EF279B341F62120E1F5ABCFF85B24541", L"Horion DLL 1.17.2"},
	{L"18CB627B78DD3C81F67F4002B92EECBA", L"YaamiClient.dll 1.17.2"},
	{L"021F85ACA44ED090DF93C241CB37DE1A", L"Anarchy_Client 1.17.2"},
	{L"29212477DF769B364B5769F0A5506FE0", L"auto_updating_hooks 1.17.2/1.17.0"},
	{L"70B08DD86F749C1D26296BD8060224FB", L"Nitro 1.17.0"},
	{L"CFF9E3E67D55BFECADF0D3D5AE14B35C", L"Ambrosial_And_Zephyr 1.17.0"},
	{L"42384237C8CA34C94C9753F6484397AB", L"Anarchy_Client 1.17.0"},
	{L"0F84EFDB27BB7CDF6A27A4E0EC5CE19B", L"Nitro 1.16.221"},
	{L"58BDB1920EBA082553A7F8F7D62CF9CF", L"Loki 1.16.221"},
	{L"655DBF3A33CAE2EE1C0BC68B91C399EB", L"Moon_V0.3 1.16.221"},
	{L"D82D15F63342377D9A2C9CA5CD479A2B", L"Strike 1.16.221"},
	{L"D7259DDE5D8F36FF2246391FFFACF60F", L"Eternium 1.16.221/ 1.16.220"},
	{L"8C10C62384D6D5DC33EDC28C127E6D8D", L"Eclipse 1.16.221"},
	{L"AD5CE67AF55E471417E929AFDBD9AE9F", L"Autism 1.16.221"},
	{L"3ADB1338EDCA48C8A1DCA71CF2D70B53", L"Wave 1.16.221"},
	{L"8A184F8A9C3D3A9BFD26D1676643DD98", L"Nitro 1.16.220"},
	{L"2AFEEC5FBB445DED2FAC959DF9AE6B6F", L"Eclipse_Client"},
	{L"8506318AC79494C9F7047BF76ADF7E33", L"Quadrix.dll 1.16.220"},
	{L"11B45786CF2A41009BCD53EE8BCB2E4F", L"Nitro 1.16.210"},
	{L"348EE5AFB324BB1E8297ECEF396CBB84", L"Disguised Process Hacker Nitr0  1.16.210"},
	{L"894135DF60D9370CDDC17D316EB40DCC", L"Disguised Reasource Hacker Nitr0  1.16.210"},
	{L"4C6703D30F207377329FE92E5597C82A", L"Disguised Spotify Nitr0  1.16.210"},
	{L"555125301AE860C13A1EADB8732AB447", L"Disguised Minecraft Launcher Nitr0  1.16.210"},
	{L"235AA72B4FB4B2306ECE386B744A24C8", L"Coffe 1.16.210, Coffee 1.16.201"},
	{L"DA8D630DE50A275D0319E48DF50D7BCC", L"Eternium 1.16.210"},
	{L"E420CD70AAFA0CA42C6242E0F5EBBE8F", L"Nitro 1.16.201"},
	{L"1F52A54D68CC984CD4F2FFD7A1E675C5", L"Fadeaway 1.16.201"},
	{L"BA2B6A24AB8DB28CFE9D27F590B60616", L"BadManPublic 1.16.201"},
	{L"0A414304FA0DDE93C6C7276A339566B0", L"V3CTER 1.16.201"},
	{L"F9A70C5D744ADA0C4FFAB4C83600BFAC", L"RixCheets 1.16.201"},
	{L"1987AAB4325441558D798D5D1DD31E60", L"PacketClient 1.16.201"},
	{L"B3D680A7DB274ADE853BD64980553693", L"HorionDLL 1.16.201"},
	{L"200B18C738DD5465F6C638428D6FA8DE", L"HorionDebugInjector 1.16.201"},
	{L"81D6E71A75E590237041E38851756DBA", L"Lakeside 1.16.100"},
	{L"E9CB88B026CE2F528017636222AF1251", L"Nitro 1.16.100"},
	{L"D6CF5B18F80300BE621EDDD41F1425B0", L"Explorer 1.16.100, Distinct 1.16.100"},
	{L"7ECFC8CD7455DD9998F7DAD88F2A8A9D", L"OP AutoClicker.exe"},
	{L"9350C5CF61A4D3D68177FAD54ACFD87C", L"XMouseButtonControl.exe"},
	{L"8FD172F85EE2111BB2C796F255DFCA77", L"7Clicker.jar"},
	{L"77652D318A4E61F0A6D76028B75493A3", L"mouseclicker.exe"},
	{L"6862F65BE14FD3CE88086EC79777DB6E", L"GS Auto Clicker.exe"},
	{L"27D5A6C9274255CF89B38F28480EDCF0", L"Polr Clicker.exe"},
	{L"64701AE13E815DFF419E73CF9D66F924", L"TheFastestMouseClicker.exe"},
	{L"4F3CE2D1C1C63AD7D1F563A9FFD5B7BD", L"TheFastestMouseGroupClicker.exe"},
	{L"2CE992ABD25F22DE9A9B737BC608CF89", L"Cheat Engine.exe"},
	{L"84B67974390A284A9B50E1FB1848B8BF", L"Clumsy.exe"},
	{L"A68EE5AFB324BO9E8297ECEF396CBP84", L"Ascendancy 1.17.10"},
	{L"20B98DD86NI49C1D26296BOD8060224F", L"Ascendancy 1.17.2"},
	{L"E16V3856393AD90A2E93D6E2D62NAFC1", L"Ascendancy 1.17.2 Lite"},
	{L"9S1OE1B6267S5F63749FCDF6650V647E", L"Ascendancy 1.17.0"},
	{L"1A05B925F9CB6AACE8E20226305F6D87", L"Packet Client Injector.exe"},
	{L"C6CD45133F9D90973C57C568DFEE0D51", L"Packet Client DLL"},
	{L"01BD9957DD4B0E580E6A0FA271667C9F", L"Packet Client Old DLL"},
	{L"98B2109BCF941B4FBBAD01C45F9D4B88", L"Nitr0 lite"},
	{L"867aee91cd59fe555b607d28fea4e9af", L"treor"},
	{L"596b0f4684d45de83c204967c06e48a3", L"Ambrosial"},
	{L"b365af317ae730a67c936f21432b9c71", L"Process Hacker 2.0"},
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
	};

} // Hash
