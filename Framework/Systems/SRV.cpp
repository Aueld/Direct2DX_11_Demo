#include "Framework.h"
#include "SRV.h"

SRV::SRV()
{
}

SRV::~SRV()
{
	for (auto it = srvs.begin(); it != srvs.end(); ++it)
		SAFE_RELEASE(it->second);

	srvs.clear();

	cout << "SRVManager : srvs.clear complete" << endl;
}

void SRV::AddSRV(string path, ID3D11ShaderResourceView* srv)
{
	if (srv == nullptr || path == "")
	{
		cout << "SRVManager : Add Error" << endl;
		return;
	}

	srvs.insert(make_pair(path, srv));
}

ID3D11ShaderResourceView* SRV::GetSRV(string name)
{
	if (srvs.find(name) != srvs.end())
		return srvs.find(name)->second;
	else
	{
		cout << "SRVManager : Can't find name" << endl;
		return nullptr;
	}
}
