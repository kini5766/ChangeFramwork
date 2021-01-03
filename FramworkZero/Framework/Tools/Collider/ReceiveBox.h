#pragma once

struct BoxReceveDesc
{
	const wstring& Tag;
	const void* Data;
};

class ReceiveBox
{
public:
	ReceiveBox(ColliderBox* collider);
	~ReceiveBox();

public:
	void Update();

public:
	void ReleaseColliderBox() { SafeRelease(collider); }
	ColliderBox* GetCollider() { return collider; }
	vector<BoxReceveDesc>& GetReceived() { return received; }
	
	void AddReceiveTag(wstring value) { receiveTags.push_back(value); }
	void ResetChecker() { receiveDescs.clear(); }

private:
	bool CheckTag(const BoxMessageDesc* value);
	void ReceiveBoxMessage(const BoxMessageDesc* value);

private:
	ColliderBox* collider;
	vector<BoxReceveDesc> received;
	vector<wstring> receiveTags;

private:
	struct ReceiveMessageDesc
	{
		int Id;
		float Time;
	};
	list<ReceiveMessageDesc> receiveDescs;
};